// CurlUtil.cpp : ���� DLL Ӧ�ó���ĵ���������
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

//�������
void CCurlUtil::FinishDownload(DownloadFileInfo *pFileInfo)
{
	printf("CCurlUtil::FinishDownload\n");
}

//������
void CCurlUtil::Downloading(DownloadFileInfo *pFileInfo)
{
	printf("CCurlUtil::Downloading\n");
}

}//namespace curlutil