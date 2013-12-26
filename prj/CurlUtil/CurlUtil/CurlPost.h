#pragma once

#include "stdafx.h"

struct tagPostData
{
	//��ø���Content-Length��Ϣ��Ϊ�����ݵĳ���
	char cHead[MAX_HTTP_HEAD_ALL_CNT][MAX_HTTP_HEAD_ONCE_LEN];//ͷ��Ϣ
	int nHeadCnt;//ͷ��Ŀ
	char cURL[MAX_URL];//url
	int nTimeOut;//��ʱʱ�䣬��λΪ��
	char *cPostData;//post����
	int nPostDataLen;//���ݳ���
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
	//����ͷ��Ϣ
	strcpy_s(postData.cHead[0],"Accept-Encoding: gzip, deflate");
	strcpy_s(postData.cHead[1],"User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; CIBA)");
	strcpy_s(postData.cHead[2],"Connection: Keep-Alive");
	//������Ҫ�������ݵĳ���
	strcpy_s(postData.cHead[3],"Content-Length: 6");
	//����url
	strcpy_s(postData.cURL,"http://127.0.0.1:21427/");
	//ͷ��Ϣ��Ŀ
	postData.nHeadCnt = 4;
	//���ó�ʱʱ��
	postData.nTimeOut = 100;
	//�������ݳ���
	postData.nPostDataLen = 6;
	//��������
	postData.cPostData = "123456";

	CCurlPost::RequestPost(&postData);
*/
	static bool RequestPost(tagPostData *pData,bool bNeedResponse = false,char **csResponseData = NULL,long nResponseDataMaxLen = 0);
};

