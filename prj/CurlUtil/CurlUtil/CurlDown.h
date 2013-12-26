#pragma once

#include "stdafx.h"
#include <string>
#include "curl/curl.h"
#include "pthread/pthread.h"
#include "Macro.h"

using std::string;

struct FileInfo
{
	int nID;//文件id
	int nType;//文件类型

	FileInfo()
	{
		nID = -1;
		nType = -1;
	}
};

struct DownloadInfo
{
	FILE *pFile;//写文件地址
	long nTotalCnt;//总文件大小
	long nAlreadyCnt;//已经下载的大小
	char csHttpHead[MAX_HTTP_HEAD_ALL_LEN];//头
	char *csDataBlock;//当前下载的数据块
	int nDataLen;//当前下载的数据块长度

	DownloadInfo()
	{
		pFile = NULL;
		nTotalCnt = 0;
		nAlreadyCnt = 0;
		csDataBlock = NULL;
		nDataLen = 0;
		memset(csHttpHead,0,MAX_HTTP_HEAD_ALL_LEN);
	}
};

struct DownloadTask
{
	char csUrl[MAX_URL];//下载地址
	char csFilePath[MAX_PATH];//下载文件保存路径
	bool bNeedCallback;//是否需要回调,默认true
	bool bSupportFromBreak;//是否支持断点续传,默认false
	FileInfo *pFileInfo;//文件信息，用于回调时区分文件，如不需要可以为空
	void *pUserInfo;//自定义信息

	DownloadTask()
	{
		memset(csUrl,0,MAX_URL);
		memset(csFilePath,0,MAX_PATH);
		bNeedCallback = true;
		bSupportFromBreak = false;
		pFileInfo = NULL;
		pUserInfo = NULL;
	}
};

struct DownloadFileInfo  
{
	char csUrl[MAX_URL];//下载地址
	char csFilePath[MAX_PATH];//下载文件保存路径
	bool bNeedCallback;//是否需要回调,默认true
	bool bSupportFromBreak;//是否支持断点续传,默认false
	FileInfo *pFileInfo;//文件信息
	DownloadInfo *pDownloadInfo;//下载信息
	void *pUserInfo;//自定义信息

	DownloadFileInfo()
	{
		memset(csUrl,0,MAX_URL);
		memset(csFilePath,0,MAX_PATH);
		bNeedCallback = true;
		bSupportFromBreak = false;
		pFileInfo = NULL;
		pDownloadInfo = NULL;
		pUserInfo = NULL;
	}

	~DownloadFileInfo()
	{
		if (pDownloadInfo != NULL)
		{
			delete pDownloadInfo;
			pDownloadInfo = NULL;
		}
	}

	void SetTask(DownloadTask *pDownLoadTask)
	{
		memcpy(csUrl,pDownLoadTask->csUrl,strlen(pDownLoadTask->csUrl));
		memcpy(csFilePath,pDownLoadTask->csFilePath,strlen(pDownLoadTask->csFilePath));
		bNeedCallback = pDownLoadTask->bNeedCallback;
		bSupportFromBreak = pDownLoadTask->bSupportFromBreak;
		pFileInfo = pDownLoadTask->pFileInfo;
		pDownloadInfo = new DownloadInfo;
		pUserInfo = pDownLoadTask->pUserInfo;
	}
};

class CURLUTIL_API IDownloadProxy
{
public:
	//下载完成
	virtual void FinishDownload(DownloadFileInfo *pFileInfo) = 0;
	//下载中
	virtual void Downloading(DownloadFileInfo *pFileInfo) = 0;
};

class CURLUTIL_API CCurlDown
{
public:
	static CCurlDown *GetInstance();

	//异步下载文件
	void DownloadFileAsync(DownloadTask *pDownLoadTask);
	//同步下载文件
	void DownloadFileSyncy(DownloadTask *pDownLoadTask);

	//设置回调代理
	void SetDownloadProxy(IDownloadProxy *pDownloadDelegate){m_pDownloadProxy = pDownloadDelegate;}
	//获取代理
	IDownloadProxy *GetDownloadProxy(){return m_pDownloadProxy;}
	
	//下载完成
	void FinishDownload(DownloadFileInfo *pFileInfo);
	//下载中
	void Downloading(DownloadFileInfo *pFileInfo);

private:
	CCurlDown(void);
	~CCurlDown(void);

	IDownloadProxy *m_pDownloadProxy;
};