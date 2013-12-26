#pragma once
#include "Macro.h"

struct tagFtpUpload
{
	char csFilePath[MAX_PATH];//��Ҫ�ϴ��ļ��ı���·��
	char csSaveUrl[MAX_FILE_NAME];//ftp�������ϱ�����ļ�·�������ļ���
	char csUserName[MAX_FTP_USERNAME];//�û���
	char csPassWord[MAX_FTP_PASSWORD];//����

	tagFtpUpload()
	{
		memset(csFilePath,0,MAX_PATH);
		memset(csSaveUrl,0,MAX_FILE_NAME);
		memset(csUserName,0,MAX_FTP_USERNAME);
		memset(csPassWord,0,MAX_FTP_PASSWORD);
	}
};

struct tagFtpDownload
{
	char csFtpURL[MAX_URL];//����ftp��ַ
	char csSavePath[MAX_FILE_NAME];//���ر����ļ�·�������ļ���
	char csUserName[MAX_FTP_USERNAME];//�û���
	char csPassWord[MAX_FTP_PASSWORD];//����

	tagFtpDownload()
	{
		memset(csFtpURL,0,MAX_URL);
		memset(csSavePath,0,MAX_FILE_NAME);
		memset(csUserName,0,MAX_FTP_USERNAME);
		memset(csPassWord,0,MAX_FTP_PASSWORD);
	}
};

class CURLUTIL_API CCurlFTP
{
public:
	CCurlFTP(void);
	~CCurlFTP(void);
/*
eg.
	tagFtpDownload ftpDownload;
	//��������ftp��ַ
	strcpy_s(ftpDownload.csFtpURL,"ftp://192.168.1.86:2121/21224.jpg");
	//���ñ��浽���ص�ַ
	strcpy_s(ftpDownload.csSavePath,"c:\\21224.jpg");
	//���룬��û�пɲ�����
	strcpy_s(ftpDownload.csPassWord,"3290");
	//�û�������û�пɲ�����
	strcpy_s(ftpDownload.csUserName,"lion");
	CCurlFTP::Download(&ftpDownload);
*/
	static bool Download(tagFtpDownload *pFtpDownload);

/*
eg.
	tagFtpUpload ftpUpload;
	//�����ϴ�ftp��ַ��Ŀ¼���Զ�����
	strcpy_s(ftpUpload.csSaveUrl,"ftp://192.168.1.86:21/up/1/2/21224.jpg");
	//�����ϴ��ļ�
	strcpy_s(ftpUpload.csFilePath,"c:\\21224.jpg");
	//���룬��û�пɲ�����
	strcpy_s(ftpUpload.csPassWord,"3290");
	//�û�������û�пɲ�����
	strcpy_s(ftpUpload.csUserName,"lion");
	CCurlFTP::Upload(&ftpUpload);
*/
	static bool Upload(tagFtpUpload *pFtpUpload);
	
};

