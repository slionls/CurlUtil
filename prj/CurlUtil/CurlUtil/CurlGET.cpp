#include "CurlGET.h"

#include "curl/curl.h"
#include "pthread/pthread.h"

CCurlGET::CCurlGET(void)
{
	m_pmapCache = new map<string,string>;
}

CCurlGET::~CCurlGET(void)
{
	if (m_pmapCache != NULL)
	{
		m_pmapCache->clear();
		delete m_pmapCache;
		m_pmapCache = NULL;
	}
	
}

size_t WriteToData(void *buffer, size_t size, size_t nmemb, string *pData)
{

	pData->append((char*)buffer,nmemb);

	return size * nmemb;
}

bool CCurlGET::RequestGET(tagGetData *pData,char **csResponseData,long nResponseDataMaxLen,long *nResLen)
{
	//assert(m_pmapCache != NULL);
	if (m_pmapCache == NULL)
	{
		return false;
	}
	if (!m_pmapCache->empty())
	{
		string strUrl = pData->cURL;
		string strContent;
		map<string,string>::iterator it = m_pmapCache->find(strUrl);
		if (it != m_pmapCache->end())
		{
			strContent = it->second;
			long nLen = 0;
			if (strContent.empty())
			{
				memset(*csResponseData,0,nResponseDataMaxLen);
				nLen = 0;
				m_pmapCache->erase(strUrl);
			}
			else if (strContent.length() > nResponseDataMaxLen-1)
			{
				memcpy(*csResponseData,strContent.c_str(),nResponseDataMaxLen-1);
				(*csResponseData)[nResponseDataMaxLen-1] = '\0';

				strContent = strContent.substr(nResponseDataMaxLen-1);
				(*m_pmapCache)[strUrl] = strContent;

				nLen = nResponseDataMaxLen-1;
			}
			else
			{
				memcpy(*csResponseData,strContent.c_str(),strContent.length());
				(*csResponseData)[strContent.length()] = '\0';

				nLen = strContent.length();
				strContent = "";
				(*m_pmapCache)[strUrl] = strContent;
			}

			if (nResLen != NULL)
			{
				*nResLen = nLen;
			}

			return true;
		}
	}

	CURL *curl = NULL;
	CURLcode CUrlRes;
	struct curl_slist *chunk = NULL;
	bool ret = false;
	string strResponseData;

	curl = curl_easy_init();
	if (curl == NULL)
	{
		printf("error CCurlGET::SendData curl_easy_init");
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
			printf("error CCurlGET::RequestGET CURLOPT_HTTPHEADER\n");
			break;
		}
		CUrlRes = curl_easy_setopt(curl, CURLOPT_TIMEOUT, pData->nTimeOut);
		if (CUrlRes != CURLE_OK)
		{
			printf("error CCurlGET::RequestGET CURLOPT_TIMEOUT\n");
			break;
		}
		CUrlRes = curl_easy_setopt(curl, CURLOPT_URL, pData->cURL);
		if (CUrlRes != CURLE_OK)
		{
			printf("error CCurlGET::RequestGET CURLOPT_URL\n");
			break;
		}
		CUrlRes = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteToData);
		if (CUrlRes != CURLE_OK)
		{
			printf("error CCurlGET::RequestGET CURLOPT_WRITEFUNCTION\n");
			break;
		}
		CUrlRes = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strResponseData);
		if (CUrlRes != CURLE_OK)
		{
			printf("error CCurlGET::RequestGET CURLOPT_WRITEDATA\n");
			break;
		}

		CUrlRes = curl_easy_perform(curl);
		if(CUrlRes == CURLE_OK)
		{
			printf("success CCurlGET::RequestGET curl_easy_perform\n");

			if (csResponseData != NULL)
			{
				string strUrl = pData->cURL;
				long nLen = 0;
				if (strResponseData.empty())
				{
					memset(*csResponseData,0,nResponseDataMaxLen);
					nLen = 0;
				}
				else if (strResponseData.length() > nResponseDataMaxLen-1)
				{
					memcpy(*csResponseData,strResponseData.c_str(),nResponseDataMaxLen-1);
					(*csResponseData)[nResponseDataMaxLen-1] = '\0';

					strResponseData = strResponseData.substr(nResponseDataMaxLen-1);
					(*m_pmapCache)[strUrl] = strResponseData;

					nLen = nResponseDataMaxLen-1;
				}
				else
				{
					memcpy(*csResponseData,strResponseData.c_str(),strResponseData.length());
					(*csResponseData)[strResponseData.length()] = '\0';

					strResponseData = "";
					(*m_pmapCache)[strUrl] = strResponseData;

					nLen = strResponseData.length();
				}

				if (nResLen != NULL)
				{
					*nResLen = nLen;
				}
			}

			ret = true;
		}
		else
		{
			printf("error CCurlGET::RequestGET curl_easy_perform [%d]\n",CUrlRes);
			if (csResponseData != NULL)
			{
				(*csResponseData) = NULL;
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