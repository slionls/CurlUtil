#pragma once
#include "stdafx.h"
#include <map>
#include <string>
using std::map;
using namespace std;

struct tagGetData
{
	char cHead[MAX_HTTP_HEAD_ALL_CNT][MAX_HTTP_HEAD_ONCE_LEN];//头信息
	int nHeadCnt;//头数目
	char cURL[MAX_URL];//url
	int nTimeOut;//超时时间，单位为秒

	tagGetData()
	{
		for (int i=0; i<MAX_HTTP_HEAD_ALL_CNT; ++i)
		{
			memset(cHead[i],0,MAX_HTTP_HEAD_ONCE_LEN);
			
		}
		nHeadCnt = 0;
		memset(cURL,0,MAX_URL);
		nTimeOut = 120;
	}
};

class QKCURLDOWN_API CCurlGET
{
public:
	CCurlGET(void);
	~CCurlGET(void);

/*
eg.
	tagGetData getData;
	//设置头信息
	strcpy_s(getData.cHead[0],"Accept-Encoding: gzip, deflate");
	strcpy_s(getData.cHead[1],"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; CIBA)");
	strcpy_s(getData.cHead[2],"Connection: Keep-Alive");
	//设置url
	strcpy_s(getData.cURL,"ip138.com");
	//头信息数目
	getData.nHeadCnt = 3;
	//超时时间
	getData.nTimeOut = 100;

	CCurlGET curlGet;
	curlGet.RequestGET(&getData);
*/
	bool RequestGET(tagGetData *pData, char **csResponseData = NULL,long nResponseDataMaxLen = 0,long *nResLen = NULL);

private:
	map<string,string> *m_pmapCache;
};

