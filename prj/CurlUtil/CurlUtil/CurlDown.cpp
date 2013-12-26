
#include "CurlDown.h"
#include <tchar.h>
#include <queue>
#include "util/tools.h"
//
using std::queue;

static queue<DownloadFileInfo*>* s_pFinishQueue = NULL;//�����ض���

static CCurlDown *s_pDownloadIntance = NULL;//ʵ�����
static bool s_bNeedQuit = false;//�����߳��Ƿ��˳�

static pthread_t	s_ReadThread;//���߳̾��

static pthread_mutex_t   s_FinishQueueMutex;//�����ض��л�����

static pthread_mutex_t	s_ReadSleepMutex;
static pthread_cond_t	s_ReadSleepCondition;//���߳�˯�߻�����������

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
	//֪ͨ���߳��˳�
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

/* ��httpͷ����ȡ�ļ�size*/
size_t GetContentLength(void *ptr, size_t size, size_t nmemb, DownloadFileInfo *pFileInfo) 
{
	long len = 0;
	int nRet = _snscanf_s((char*)ptr, size * nmemb, "Content-Length: %ld\n", &len);
 	if (nRet) 
	{
		//���ڶϵ���������ʼ�������ص��ֽ�
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

//ʹ��curl����
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
		//�����ļ���
		EnsureDirExist(strDir);
	}

	bool bFromBreak = false;//�Ƿ�ϵ�����
	long lDownloadFileLen = GetDownloadFileLength(pFileInfo->csUrl);
	long lLocalFileLength = GetLocalFileLength(pFileInfo->csFilePath);

	//�����ܳ���
	pFileInfo->pDownloadInfo->nTotalCnt = lDownloadFileLen;

	if (!pFileInfo->bSupportFromBreak || lLocalFileLength == 0 || lDownloadFileLen == 0 || lDownloadFileLen < lLocalFileLength)
	{
		//�ļ������ڻ��ļ����ݲ���ȷ����д�ļ�
		fopen_s(&pFileInfo->pDownloadInfo->pFile,pFileInfo->csFilePath, "wb+");
		if(NULL == pFileInfo->pDownloadInfo->pFile)
		{
			printf( "Failed to Open File\n");
			return false;
		}
	}
	else
	{
		//׷�ӷ�ʽ���ļ����ϵ�����
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

	//��ʼ����
	if (GetURLDataBycurl(pFileInfo))
	{
		//����������ѹ����ɶ���
		pthread_mutex_lock(&s_FinishQueueMutex);
		s_pFinishQueue->push(pFileInfo);
		pthread_mutex_unlock(&s_FinishQueueMutex); 

		//֪ͨ���߳�
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
				//��ʱ˯�ߣ��ȴ����ļ�ѹ����к���
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
		
		//���߳�
		pthread_create(&s_ReadThread, NULL, ReadFinishFileThread, NULL);

 		s_bNeedQuit = false;
	}
	
	DownloadFileInfo *pFileInfo = new DownloadFileInfo;
	pFileInfo->SetTask(pDownLoadTask);

	//�����߳�
	pthread_t hDownloadThread;
	pthread_create(&hDownloadThread, NULL, DownloadFileThread, pFileInfo);
}

//ͬ�������ļ�
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