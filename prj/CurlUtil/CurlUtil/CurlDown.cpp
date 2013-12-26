
#include "CurlDown.h"
#include <tchar.h>
#include <queue>
#include "util/tools.h"
//
using std::queue;

static queue<DownloadFileInfo*>* s_pFinishQueue = NULL;//已下载队列

static CCurlDown *s_pDownloadIntance = NULL;//实例句柄
static bool s_bNeedQuit = false;//下载线程是否退出

static pthread_t	s_ReadThread;//读线程句柄

static pthread_mutex_t   s_FinishQueueMutex;//已下载队列互斥量

static pthread_mutex_t	s_ReadSleepMutex;
static pthread_cond_t	s_ReadSleepCondition;//读线程睡眠唤醒条件变量

CCurlDown::CCurlDown(void)
{
	m_pDownloadProxy = NULL;

	CURL *curl = NULL;
	CURLcode code;
	code = curl_global_init(CURL_GLOBAL_DEFAULT);
	if (code != CURLE_OK)
	{
		printf( "Failed to global init default [%d]\n", code );
	} 
}

CCurlDown::~CCurlDown(void)
{
	s_bNeedQuit = true;
	//通知读线程退出
	pthread_cond_signal(&s_ReadSleepCondition);
}

CCurlDown* CCurlDown::GetInstance()
{
	if (s_pDownloadIntance == NULL)
	{
		s_pDownloadIntance = new CCurlDown;
	}

	return s_pDownloadIntance;
}

long GetLocalFileLength(FILE *file)
{
	long len = 0;
	if (file != NULL)
	{
		fseek(file,0,SEEK_END);  
		len = ftell(file);
	}
	return len;
}

long GetLocalFileLength(string strPath)
{
	long len = 0;
	FILE *file = NULL;
	fopen_s(&file,strPath.c_str(),"ab+");
	if (file != NULL)
	{
		len = GetLocalFileLength(file);
	}
	fclose(file);

	return len;
}

long GetDownloadFileLength(const char *csURL)
{
	CURL *curl = NULL;
	CURLcode code;
	curl = curl_easy_init();
	long ret = 0;

	code = curl_easy_setopt(curl, CURLOPT_URL, csURL);
	if (code != CURLE_OK)
	{
		printf("Failed CURLOPT_URL\n");
		return ret;
	}
	code = curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
	if (code != CURLE_OK)
	{
		printf( "Failed to get length '%s'\n", csURL);
		return ret;
	}

	code = curl_easy_perform(curl);
	if (code != CURLE_OK)
	{
		printf( "Failed to get length '%s'\n", csURL);
	}
	else
	{
		double lenght = 0;
		curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &lenght);
		ret = (long)lenght;
	}
	curl_easy_cleanup(curl);

	return ret;
}

size_t WriteToFile(void *buffer, size_t size, size_t nmemb, DownloadFileInfo *pFileInfo)
{
	if(NULL != pFileInfo->pDownloadInfo->pFile)
	{
		fwrite(buffer, size, nmemb, pFileInfo->pDownloadInfo->pFile);
	}

	pFileInfo->pDownloadInfo->nAlreadyCnt += nmemb;
	pFileInfo->pDownloadInfo->csDataBlock = (char*)buffer;
	pFileInfo->pDownloadInfo->nDataLen = nmemb;

	CCurlDown::GetInstance()->Downloading(pFileInfo);

	return size * nmemb;
}

/* 从http头部获取文件size*/
size_t GetContentLength(void *ptr, size_t size, size_t nmemb, DownloadFileInfo *pFileInfo) 
{
	long len = 0;
	int nRet = _snscanf_s((char*)ptr, size * nmemb, "Content-Length: %ld\n", &len);
 	if (nRet) 
	{
		//由于断点续传，初始化已下载的字节
		pFileInfo->pDownloadInfo->nAlreadyCnt = pFileInfo->pDownloadInfo->nTotalCnt - len;
	}

	strcat_s(pFileInfo->pDownloadInfo->csHttpHead,(char*)ptr);
	return size * nmemb;
}

bool CurlInitToFile(CURL *&curl, DownloadFileInfo *pFileInfo,bool bFromBreak)
{
	CURLcode code;
	string error;
	curl = curl_easy_init();
	if (curl == NULL)
	{
		printf( "Failed to create CURL connection\n");
		return false;
	}
	code = curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error);
	if (code != CURLE_OK)
	{
		printf( "Failed to set error buffer [%d]\n", code );
		return false;
	}
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	code = curl_easy_setopt(curl, CURLOPT_URL, pFileInfo->csUrl);
	if (code != CURLE_OK)
	{
		printf("Failed to set URL [%s]\n", error);
		return false;
	}
	code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	if (code != CURLE_OK)
	{
		printf( "Failed to set redirect option [%s]\n", error );
		return false;
	}
	code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteToFile);
	if (code != CURLE_OK)
	{
		printf( "Failed to set writer [%s]\n", error);
		return false;
	}
	code = curl_easy_setopt(curl, CURLOPT_WRITEDATA, pFileInfo);
	if (code != CURLE_OK)
	{
		printf( "Failed to set write data [%s]\n", error );
		return false;
	}
	code = curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, GetContentLength);
	if (code != CURLE_OK)
	{
		printf( "Failed to get Content-Length [%s]\n", error );
		return false;
	}
	code = curl_easy_setopt(curl, CURLOPT_HEADERDATA, pFileInfo);
	if (code != CURLE_OK)
	{
		printf( "Failed to set write head data [%s]\n", error );
		return false;
	}

	if (bFromBreak)
	{
		long lLen = GetLocalFileLength(pFileInfo->pDownloadInfo->pFile);
		curl_easy_setopt(curl, CURLOPT_RESUME_FROM, lLen);
		if (code != CURLE_OK)
		{
			printf( "Failed to interruption resuming [%s]\n", error );
			return false;
		}
	}

	return true;
}

//使用curl下载
bool GetURLDataBycurl(DownloadFileInfo *pFileInfo)
{              
	if(strlen(pFileInfo->csFilePath) == 0)
	{
		printf( "Fail:strFilePath is empty\n");
		return false;
	}

	string strPath = pFileInfo->csFilePath;
	size_t n = strPath.find_last_of("\\");
	if( n != string::npos )
	{
		string strDir = strPath.substr(0,n);
		//创建文件夹
		EnsureDirExist(strDir);
	}

	bool bFromBreak = false;//是否断点续传
	long lDownloadFileLen = GetDownloadFileLength(pFileInfo->csUrl);
	long lLocalFileLength = GetLocalFileLength(pFileInfo->csFilePath);

	//设置总长度
	pFileInfo->pDownloadInfo->nTotalCnt = lDownloadFileLen;

	if (!pFileInfo->bSupportFromBreak || lLocalFileLength == 0 || lDownloadFileLen == 0 || lDownloadFileLen < lLocalFileLength)
	{
		//文件不存在或文件内容不正确则重写文件
		fopen_s(&pFileInfo->pDownloadInfo->pFile,pFileInfo->csFilePath, "wb+");
		if(NULL == pFileInfo->pDownloadInfo->pFile)
		{
			printf( "Failed to Open File\n");
			return false;
		}
	}
	else
	{
		//追加方式打开文件，断点续传
		fopen_s(&pFileInfo->pDownloadInfo->pFile,pFileInfo->csFilePath, "ab+");
		if(NULL == pFileInfo->pDownloadInfo->pFile)
		{
			printf( "Failed to Open File\n");
			return false;
		}
		bFromBreak = true;
	}
	
	CURL *curl = NULL;
	CURLcode code;
	string error;

	if ( !CurlInitToFile(curl,pFileInfo,bFromBreak) )
	{
		printf( "Failed to global init default\n" );
		return false;
	}

	code = curl_easy_perform(curl);
	if (code != CURLE_OK)
	{
		printf( "Failed to get '%s' [%s]\n", pFileInfo->csUrl, error);
		return false;
	}

	if(NULL != pFileInfo->pDownloadInfo->pFile)
	{
		fclose(pFileInfo->pDownloadInfo->pFile);
	}

	curl_easy_cleanup(curl);
	return true;
}

static void* DownloadFileThread(void *pData)
{
	DownloadFileInfo *pFileInfo = static_cast<DownloadFileInfo*>(pData);

	//开始下载
	if (GetURLDataBycurl(pFileInfo))
	{
		//将下载内容压入完成队列
		pthread_mutex_lock(&s_FinishQueueMutex);
		s_pFinishQueue->push(pFileInfo);
		pthread_mutex_unlock(&s_FinishQueueMutex); 

		//通知读线程
		pthread_cond_signal(&s_ReadSleepCondition);
	}
	else
	{
		printf("end download error");
	}
	return 0;
}

static void* ReadFinishFileThread(void *pData)
{
	while (true)
	{
		queue<DownloadFileInfo*> *pFinishFile = s_pFinishQueue;

		pthread_mutex_lock(&s_FinishQueueMutex);
		if (pFinishFile->empty())
		{
			pthread_mutex_unlock(&s_FinishQueueMutex);
			if (s_bNeedQuit)
			{
				break;
			}
			else 
			{
				//暂时睡眠，等待有文件压入队列后唤醒
				pthread_cond_wait(&s_ReadSleepCondition, &s_ReadSleepMutex);
				continue;
			}
		}
		else
		{
			DownloadFileInfo *pFileInfo = pFinishFile->front();
			pFinishFile->pop();
			pthread_mutex_unlock(&s_FinishQueueMutex);

			CCurlDown::GetInstance()->FinishDownload(pFileInfo);
			
			if (pFileInfo != NULL)
			{
				delete pFileInfo;
				pFileInfo = NULL;
			}
		}
	}

	if (s_pFinishQueue != NULL)
	{
		delete s_pFinishQueue;
		s_pFinishQueue = NULL;

		pthread_mutex_destroy(&s_FinishQueueMutex);
		pthread_mutex_destroy(&s_ReadSleepMutex);
		pthread_cond_destroy(&s_ReadSleepCondition);
	}

	return 0;
}

void CCurlDown::FinishDownload(DownloadFileInfo *pFileInfo)
{
	if (m_pDownloadProxy != NULL)
	{
		if (pFileInfo->bNeedCallback)
		{
			m_pDownloadProxy->FinishDownload(pFileInfo);
		}
	}
}

void CCurlDown::Downloading(DownloadFileInfo *pFileInfo)
{
	if (m_pDownloadProxy != NULL)
	{
		if (pFileInfo->bNeedCallback)
		{
			m_pDownloadProxy->Downloading(pFileInfo);
		}
	}
}

void CCurlDown::DownloadFileAsync(DownloadTask *pDownLoadTask)
{
	if (s_pFinishQueue == NULL)
	{             
		s_pFinishQueue = new queue<DownloadFileInfo*>;

		pthread_mutex_init(&s_FinishQueueMutex, NULL);
		pthread_mutex_init(&s_ReadSleepMutex, NULL);
		pthread_cond_init(&s_ReadSleepCondition, NULL);
		
		//读线程
		pthread_create(&s_ReadThread, NULL, ReadFinishFileThread, NULL);

 		s_bNeedQuit = false;
	}
	
	DownloadFileInfo *pFileInfo = new DownloadFileInfo;
	pFileInfo->SetTask(pDownLoadTask);

	//下载线程
	pthread_t hDownloadThread;
	pthread_create(&hDownloadThread, NULL, DownloadFileThread, pFileInfo);
}

//同步下载文件
void CCurlDown::DownloadFileSyncy(DownloadTask *pDownLoadTask)
{
	DownloadFileInfo *pFileInfo = new DownloadFileInfo;
	pFileInfo->SetTask(pDownLoadTask);

	if (GetURLDataBycurl(pFileInfo))
	{
		FinishDownload(pFileInfo);
	}

	if (pFileInfo != NULL)
	{
		delete pFileInfo;
		pFileInfo = NULL;
	}
}