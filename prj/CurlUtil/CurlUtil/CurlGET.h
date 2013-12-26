#pragma once
#include "stdafx.h"
#include <map>
#include <string>
using std::map;
using namespace std;

struct tagGetData
{
	char cHead[MAX_HTTP_HEAD_ALL_CNT][MAX_HTTP_HEAD_ONCE_LEN];//ͷ��Ϣ
	int nHeadCnt;//ͷ��Ŀ
	char cURL[MAX_URL];//url
	int nTimeOut;//��ʱʱ�䣬��λΪ��

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
	//����ͷ��Ϣ
	strcpy_s(getData.cHead[0],"Accept-Encoding: gzip, deflate");
	strcpy_s(getData.cHead[1],"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; CIBA)");
	strcpy_s(getData.cHead[2],"Connection: Keep-Alive");
	//����url
	strcpy_s(getData.cURL,"ip138.com");
	//ͷ��Ϣ��Ŀ
	getData.nHeadCnt = 3;
	//��ʱʱ��
	getData.nTimeOut = 100;

	CCurlGET curlGet;
	curlGet.RequestGET(&getData);
*/
	bool RequestGET(tagGetData *pData, char **csResponseData = NULL,long nResponseDataMaxLen = 0,long *nResLen = NULL);

private:
	map<string,string> *m_pmapCache;
};

