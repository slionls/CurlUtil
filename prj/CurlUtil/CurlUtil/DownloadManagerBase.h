#pragma once
#include "CurlDown.h"

#include <map>
using std::map;

/*
	//CDownloadManagerBase 对CrulDown进行了封装AddDownloadTask函数为添加下载任务
	//FinishDownload，Downloading为下载中回调
	//可继承此类实现FinishDownload，Downloading两个方法，实现下载回调
eg:
	//CCurlUtil继承CDownloadManagerBase类
	CCurlUtil cutil;

	DownloadTask *pTask = new DownloadTask;
	//下载url
	strcpy_s(pTask->csUrl,"http://flash.7k7k.com/cms/cms10/20131128/0941542526/ALLbattalioncommander2.swf");
	//本地保存路径
	strcpy_s(pTask->csFilePath,"c:/1234.swf");
	//是否需要回调
	pTask->bNeedCallback = true;
	//是否断点续传
	pTask->bSupportFromBreak = false;
	cutil.AddDownloadTask(pTask);
*/
class CURLUTIL_API CDownloadManagerBase : public IDownloadProxy
{
public:
	CDownloadManagerBase(void);
	~CDownloadManagerBase(void);

	//添加下载任务，设置bAsync为是否异步下载
	void AddDownloadTask(DownloadTask *pDownloadTask,bool bAsync = true);

	//下载完成回调
	virtual void FinishDownload(DownloadFileInfo *pFileInfo) = 0;
	
	//下载中回调
	virtual void Downloading(DownloadFileInfo *pFileInfo) = 0;
};

