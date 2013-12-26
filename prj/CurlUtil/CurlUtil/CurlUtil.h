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

	//�������
	virtual void FinishDownload(DownloadFileInfo *pFileInfo);
	//������
	virtual void Downloading(DownloadFileInfo *pFileInfo);
};

}