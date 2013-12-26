// CurlTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
//
#include "../../CurlUtil/CurlUtil/CurlUtil.h"
#include "../../CurlUtil/CurlUtil/CurlGET.h"
#include "../../CurlUtil/CurlUtil/CurlPost.h"
#include "../../CurlUtil/CurlUtil/CurlFTP.h"
#include "../../CurlUtil/CurlUtil/HtmlAnalyzer.h"
int _tmain(int argc, _TCHAR* argv[])
{

// 	curlutil::CCurlUtil cutil;
// 
// 	DownloadTask *pTask = new DownloadTask;
// 	string strUrl = "http://flash.7k7k.com/cms/cms10/20131128/0941542526/ALLbattalioncommander2.swf";
// 	memcpy(pTask->csUrl,strUrl.c_str(),strUrl.length());
// 
// 	string strSavePath = "c:/1234.swf";
// 	memcpy(pTask->csFilePath,strSavePath.c_str(),strSavePath.length());
// 	pTask->bNeedCallback = true;
// 	pTask->bSupportFromBreak = false;//是否断点续传
// 	cutil.AddDownloadTask(pTask);

	tagGetData getData;
	strcpy_s(getData.cHead[0],"Accept-Encoding: gzip, deflate");
	strcpy_s(getData.cHead[1],"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; CIBA)");
	strcpy_s(getData.cHead[2],"Connection: Keep-Alive");
	strcpy_s(getData.cURL,"news.sina.com.cn");
	getData.nTimeOut = 100;
	
	char *buf;
	CCurlGET::RequestGET(&getData,buf);

// 	char *buf = new char[1024*1024];
// 	memset(buf,0,1024*1024);
// 
// 	fstream fpp;
// 	fpp.open("c:\\sina.txt",ios::in);
// 	fpp.read(buf,687294);
// 	fpp.close();

	//while (true)
	{
		int nstart = clock();
		cout << "start Parse" << endl;

		CHtmlAnalyzer html;
		html.Parse((PBYTE)buf,strlen(buf),CHtmlAnalyzer::CHARSET_MULTIBYTE);

		cout << "end Parse" << clock()-nstart << endl;
	}

// 	tagPostData postData;
// 	strcpy_s(postData.cHead[0],"Accept-Encoding: gzip, deflate");
// 	strcpy_s(postData.cHead[1],"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; CIBA)");
// 	strcpy_s(postData.cHead[2],"Connection: Keep-Alive");
// 	strcpy_s(postData.cHead[3],"Content-Length: 6");	
// 	strcpy_s(postData.cURL,"http://127.0.0.1:21427/");
// 	postData.nHeadCnt = 4;
// 	postData.nTimeOut = 100;
// 	postData.nPostDataLen = 6;
// 	postData.cPostData = "123456";
// 
// 	char *buf = NULL;
// 	CCurlPost testGet;
// 	testGet.RequestPost(&postData,buf);

	

// 	tagFtpDownload ftpDownload;
// 	strcpy_s(ftpDownload.csFtpURL,"ftp://192.168.1.86:2121/21224.jpg");
// 	strcpy_s(ftpDownload.csSavePath,"c:\\21224.jpg");
// 	strcpy_s(ftpDownload.csPassWord,"3290");
// 	strcpy_s(ftpDownload.csUserName,"lion");
// 	CCurlFTP::Download(&ftpDownload);
// 
// 	tagFtpUpload ftpUpload;
// 	strcpy_s(ftpUpload.csSaveUrl,"ftp://192.168.1.86:21/up/1/2/21224.jpg");
// 	strcpy_s(ftpUpload.csFilePath,"c:\\21224.jpg");
// 	strcpy_s(ftpUpload.csPassWord,"3290");
// 	strcpy_s(ftpUpload.csUserName,"lion");
// 	CCurlFTP::Upload(&ftpUpload);

	int i;
	cin >> i; 
	return 0;
}

