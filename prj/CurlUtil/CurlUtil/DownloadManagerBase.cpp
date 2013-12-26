#include "DownloadManagerBase.h"
#include "util/tools.h"

static CDownloadManagerBase* s_pDownloadManager = NULL;

CDownloadManagerBase::CDownloadManagerBase(void)
{
	CCurlDown::GetInstance()->SetDownloadProxy(this);
}

CDownloadManagerBase::~CDownloadManagerBase(void)
{
}

void CDownloadManagerBase::AddDownloadTask(DownloadTask *pDownloadTask,bool bAsync)
{
	string strPath = pDownloadTask->csFilePath;
	size_t n = strPath.find_last_of("\\");
	if( n != string::npos )
	{
		string strDir = strPath.substr(0,n);
		//创建文件夹
		EnsureDirExist(strDir);
	}

	if (bAsync)
	{
		//异步下载
		CCurlDown::GetInstance()->DownloadFileAsync(pDownloadTask);
	}
	else
	{
		//同步下载
		CCurlDown::GetInstance()->DownloadFileSyncy(pDownloadTask);
	}
}