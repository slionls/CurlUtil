#include "CurlPost.h"
#include "curl/curl.h"
#include <string>
using namespace std;

CCurlPost::CCurlPost(void)
{
}


CCurlPost::~CCurlPost(void)
{
}

size_t PostData(void *buffer, size_t size, size_t nmemb, string *pData)
{
	pData->append((char*)buffer,nmemb);

	return size * nmemb;
}

bool CCurlPost::RequestPost(tagPostData *pData, bool bNeedResponse, char **csResponseData,long nResponseDataMaxLen)
{
	CURL *curl = NULL;
	CURLcode CUrlRes;
	struct curl_slist *chunk = NULL;
	bool ret = false;
	string strResponseData;

	curl = curl_easy_init();
	if (curl == NULL)
	{
		printf("error CCurlPost::RequestPost curl_easy_init");
		return ret;
	}

	for (int i = 0; i < pData->nHeadCnt; i++)
	{
		char temBuf[1024] = {0};
		memcpy(temBuf,pData->cHead[i],strlen(pData->cHead[i]));
		chunk = curl_slist_append(chunk, temBuf);
	}
	// 	chunk = curl_slist_append(chunk, "Accept-Encoding: gzip, deflate");  
	// 	chunk = curl_slist_append(chunk, "User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; CIBA)");
	// 	chunk = curl_slist_append(chunk, "Connection: Keep-Alive");  

	do 
	{
		CUrlRes = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
		if (CUrlRes != CURLE_OK)
		{
			printf("error CCurlPost::RequestPost CURLOPT_HTTPHEADER\n");
			break;
		}
		CUrlRes = curl_easy_setopt(curl, CURLOPT_TIMEOUT, pData->nTimeOut);
		if (CUrlRes != CURLE_OK)
		{
			printf("error CCurlPost::RequestPost CURLOPT_TIMEOUT\n");
			break;
		}
		CUrlRes = curl_easy_setopt(curl, CURLOPT_URL, pData->cURL);
		if (CUrlRes != CURLE_OK)
		{
			printf("error CCurlPost::RequestPost CURLOPT_URL\n");
			break;
		}
		CUrlRes = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, PostData);
		if (CUrlRes != CURLE_OK)
		{
			printf("error CCurlPost::RequestPost CURLOPT_WRITEFUNCTION\n");
			break;
		}
		CUrlRes = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strResponseData);
		if (CUrlRes != CURLE_OK)
		{
			printf("error CCurlPost::RequestPost CURLOPT_WRITEDATA\n");
			break;
		}
		CUrlRes = curl_easy_setopt(curl, CURLOPT_POST, 1);
		if (CUrlRes != CURLE_OK)
		{
			printf("error CCurlPost::RequestPost CURLOPT_WRITEDATA\n");
			break;
		}
		CUrlRes = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, pData->cPostData);
		if (CUrlRes != CURLE_OK)
		{
			printf("error CCurlPost::RequestPost CURLOPT_WRITEDATA\n");
			break;
		}
		CUrlRes = curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, pData->nPostDataLen);
		if (CUrlRes != CURLE_OK)
		{
			printf("error CCurlPost::RequestPost CURLOPT_POSTFIELDSIZE\n");
			break;
		}

		CUrlRes = curl_easy_perform(curl);
		if(CUrlRes == CURLE_OK)
		{
			printf("success CCurlPost::RequestPost curl_easy_perform\n");

			if (bNeedResponse && csResponseData != NULL)
			{
				memcpy(*csResponseData,strResponseData.c_str(),nResponseDataMaxLen-1);
				(*csResponseData)[nResponseDataMaxLen-1] = '\0';
			}
			ret = true;
		}
		else
		{
			printf("error CCurlPost::RequestPost curl_easy_perform [%d]\n",CUrlRes);
			if (bNeedResponse && csResponseData != NULL)
			{
				*csResponseData = NULL;
			}

			ret = false;
			break;
		}
	} 
	while (false);

	curl_slist_free_all(chunk);

	curl_easy_cleanup(curl);
	return ret;
}