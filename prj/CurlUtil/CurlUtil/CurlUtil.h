#pragma once

#include "Macro.h"
#include <string>
using std::string;

#include "DownloadManagerBase.h"
#include "CurlGET.h"

namespace curlutil
{

class CURLUTIL_API CCurlUtil : public CDownloadManagerBase
{
public:
	CCurlUtil(void);
	~CCurlUtil(void);

	//下载完成
	virtual void FinishDownload(DownloadFileInfo *pFileInfo);
	//下载中
	virtual void Downloading(DownloadFileInfo *pFileInfo);
};

}