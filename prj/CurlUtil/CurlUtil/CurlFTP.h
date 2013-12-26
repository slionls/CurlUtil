#pragma once
#include "Macro.h"

struct tagFtpUpload
{
	char csFilePath[MAX_PATH];//需要上传文件的本地路径
	char csSaveUrl[MAX_FILE_NAME];//ftp服务器上保存的文件路径包括文件名
	char csUserName[MAX_FTP_USERNAME];//用户名
	char csPassWord[MAX_FTP_PASSWORD];//密码

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
	char csFtpURL[MAX_URL];//下载ftp地址
	char csSavePath[MAX_FILE_NAME];//本地保存文件路径包括文件名
	char csUserName[MAX_FTP_USERNAME];//用户名
	char csPassWord[MAX_FTP_PASSWORD];//密码

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
	//设置下载ftp地址
	strcpy_s(ftpDownload.csFtpURL,"ftp://192.168.1.86:2121/21224.jpg");
	//设置保存到本地地址
	strcpy_s(ftpDownload.csSavePath,"c:\\21224.jpg");
	//密码，如没有可不设置
	strcpy_s(ftpDownload.csPassWord,"3290");
	//用户名，如没有可不设置
	strcpy_s(ftpDownload.csUserName,"lion");
	CCurlFTP::Download(&ftpDownload);
*/
	static bool Download(tagFtpDownload *pFtpDownload);

/*
eg.
	tagFtpUpload ftpUpload;
	//设置上传ftp地址，目录会自动创建
	strcpy_s(ftpUpload.csSaveUrl,"ftp://192.168.1.86:21/up/1/2/21224.jpg");
	//本地上传文件
	strcpy_s(ftpUpload.csFilePath,"c:\\21224.jpg");
	//密码，如没有可不设置
	strcpy_s(ftpUpload.csPassWord,"3290");
	//用户名，如没有可不设置
	strcpy_s(ftpUpload.csUserName,"lion");
	CCurlFTP::Upload(&ftpUpload);
*/
	static bool Upload(tagFtpUpload *pFtpUpload);
	
};

