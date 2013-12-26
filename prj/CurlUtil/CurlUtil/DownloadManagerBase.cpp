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
		//�����ļ���
		EnsureDirExist(strDir);
	}

	if (bAsync)
	{
		//�첽����
		CCurlDown::GetInstance()->DownloadFileAsync(pDownloadTask);
	}
	else
	{
		//ͬ������
		CCurlDown::GetInstance()->DownloadFileSyncy(pDownloadTask);
	}
}