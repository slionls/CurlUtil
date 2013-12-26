#pragma once
#include "CurlDown.h"

#include <map>
using std::map;

/*
	//CDownloadManagerBase ��CrulDown�����˷�װAddDownloadTask����Ϊ�����������
	//FinishDownload��DownloadingΪ�����лص�
	//�ɼ̳д���ʵ��FinishDownload��Downloading����������ʵ�����ػص�
eg:
	//CCurlUtil�̳�CDownloadManagerBase��
	CCurlUtil cutil;

	DownloadTask *pTask = new DownloadTask;
	//����url
	strcpy_s(pTask->csUrl,"http://flash.7k7k.com/cms/cms10/20131128/0941542526/ALLbattalioncommander2.swf");
	//���ر���·��
	strcpy_s(pTask->csFilePath,"c:/1234.swf");
	//�Ƿ���Ҫ�ص�
	pTask->bNeedCallback = true;
	//�Ƿ�ϵ�����
	pTask->bSupportFromBreak = false;
	cutil.AddDownloadTask(pTask);
*/
class CURLUTIL_API CDownloadManagerBase : public IDownloadProxy
{
public:
	CDownloadManagerBase(void);
	~CDownloadManagerBase(void);

	//���������������bAsyncΪ�Ƿ��첽����
	void AddDownloadTask(DownloadTask *pDownloadTask,bool bAsync = true);

	//������ɻص�
	virtual void FinishDownload(DownloadFileInfo *pFileInfo) = 0;
	
	//�����лص�
	virtual void Downloading(DownloadFileInfo *pFileInfo) = 0;
};

