#pragma once

#include "stdafx.h"
#include <string>
#include "curl/curl.h"
#include "pthread/pthread.h"
#include "Macro.h"

using std::string;

struct FileInfo
{
	int nID;//�ļ�id
	int nType;//�ļ�����

	FileInfo()
	{
		nID = -1;
		nType = -1;
	}
};

struct DownloadInfo
{
	FILE *pFile;//д�ļ���ַ
	long nTotalCnt;//���ļ���С
	long nAlreadyCnt;//�Ѿ����صĴ�С
	char csHttpHead[MAX_HTTP_HEAD_ALL_LEN];//ͷ
	char *csDataBlock;//��ǰ���ص����ݿ�
	int nDataLen;//��ǰ���ص����ݿ鳤��

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
	char csUrl[MAX_URL];//���ص�ַ
	char csFilePath[MAX_PATH];//�����ļ�����·��
	bool bNeedCallback;//�Ƿ���Ҫ�ص�,Ĭ��true
	bool bSupportFromBreak;//�Ƿ�֧�ֶϵ�����,Ĭ��false
	FileInfo *pFileInfo;//�ļ���Ϣ�����ڻص�ʱ�����ļ����粻��Ҫ����Ϊ��
	void *pUserInfo;//�Զ�����Ϣ

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
	char csUrl[MAX_URL];//���ص�ַ
	char csFilePath[MAX_PATH];//�����ļ�����·��
	bool bNeedCallback;//�Ƿ���Ҫ�ص�,Ĭ��true
	bool bSupportFromBreak;//�Ƿ�֧�ֶϵ�����,Ĭ��false
	FileInfo *pFileInfo;//�ļ���Ϣ
	DownloadInfo *pDownloadInfo;//������Ϣ
	void *pUserInfo;//�Զ�����Ϣ

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
	//�������
	virtual void FinishDownload(DownloadFileInfo *pFileInfo) = 0;
	//������
	virtual void Downloading(DownloadFileInfo *pFileInfo) = 0;
};

class CURLUTIL_API CCurlDown
{
public:
	static CCurlDown *GetInstance();

	//�첽�����ļ�
	void DownloadFileAsync(DownloadTask *pDownLoadTask);
	//ͬ�������ļ�
	void DownloadFileSyncy(DownloadTask *pDownLoadTask);

	//���ûص�����
	void SetDownloadProxy(IDownloadProxy *pDownloadDelegate){m_pDownloadProxy = pDownloadDelegate;}
	//��ȡ����
	IDownloadProxy *GetDownloadProxy(){return m_pDownloadProxy;}
	
	//�������
	void FinishDownload(DownloadFileInfo *pFileInfo);
	//������
	void Downloading(DownloadFileInfo *pFileInfo);

private:
	CCurlDown(void);
	~CCurlDown(void);

	IDownloadProxy *m_pDownloadProxy;
};