// CurlUtil.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "CurlUtil.h"
#include "curl/curl.h"
#include "pthread/pthread.h"
#include "util/tools.h"

namespace curlutil
{

CCurlUtil::CCurlUtil()
{

}

CCurlUtil::~CCurlUtil(void)
{

}

//下载完成
void CCurlUtil::FinishDownload(DownloadFileInfo *pFileInfo)
{
	printf("CCurlUtil::FinishDownload\n");
}

//下载中
void CCurlUtil::Downloading(DownloadFileInfo *pFileInfo)
{
	printf("CCurlUtil::Downloading\n");
}

}//namespace curlutil