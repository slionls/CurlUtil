#pragma once

#include "stdafx.h"

struct tagPostData
{
	//最好附加Content-Length信息，为你数据的长度
	char cHead[MAX_HTTP_HEAD_ALL_CNT][MAX_HTTP_HEAD_ONCE_LEN];//头信息
	int nHeadCnt;//头数目
	char cURL[MAX_URL];//url
	int nTimeOut;//超时时间，单位为秒
	char *cPostData;//post数据
	int nPostDataLen;//数据长度
	tagPostData()
	{
		for (int i=0; i<MAX_HTTP_HEAD_ALL_CNT; ++i)
		{
			memset(cHead[i],0,MAX_HTTP_HEAD_ONCE_LEN);
		}
		nHeadCnt = 0;
		memset(cURL,0,MAX_URL);
		nTimeOut = 120;
		nPostDataLen = 0;
		cPostData = NULL;
	}
};

class QKCURLDOWN_API CCurlPost
{
public:
	CCurlPost(void);
	~CCurlPost(void);

/*
eg.
	tagPostData postData;
	//设置头信息
	strcpy_s(postData.cHead[0],"Accept-Encoding: gzip, deflate");
	strcpy_s(postData.cHead[1],"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; CIBA)");
	strcpy_s(postData.cHead[2],"Connection: Keep-Alive");
	//设置需要发送数据的长度
	strcpy_s(postData.cHead[3],"Content-Length: 6");
	//设置url
	strcpy_s(postData.cURL,"http://127.0.0.1:21427/");
	//头信息数目
	postData.nHeadCnt = 4;
	//设置超时时间
	postData.nTimeOut = 100;
	//设置数据长度
	postData.nPostDataLen = 6;
	//发送数据
	postData.cPostData = "123456";

	CCurlPost::RequestPost(&postData);
*/
	static bool RequestPost(tagPostData *pData,bool bNeedResponse = false,char **csResponseData = NULL,long nResponseDataMaxLen = 0);
};

