#include "CurlFTP.h"
#include "curl/curl.h"

CCurlFTP::CCurlFTP(void)
{
}


CCurlFTP::~CCurlFTP(void)
{
}

bool CCurlFTP::Upload(tagFtpUpload *pFtpUpload)
{
	bool ret = false;

	if (_strnicmp(pFtpUpload->csSaveUrl,"ftp://",6) != 0)
	{
		//在url前加上ftp协议
		char csServerPath[MAX_URL] = "ftp://\0";
		strcat_s(csServerPath,pFtpUpload->csSaveUrl);
		strcpy_s(pFtpUpload->csSaveUrl,csServerPath);
	}

	FILE* fp = NULL;
	fopen_s(&fp,pFtpUpload->csFilePath, "rb");
	if(NULL == fp)
	{
		printf("error CCurlFTP::Upload fopen (%s)\n", pFtpUpload->csFilePath);
		return false;
	}

	fseek(fp, 0l, SEEK_END);
	int nFileSize = (int)ftell(fp);

	if(nFileSize <= 0)
	{
		printf("error CCurlFTP::Upload file len (%d)!\n", nFileSize);
	}

	fseek(fp, 0l, SEEK_SET);

	do 
	{
		CURL *curl = NULL;
		CURLcode CUrlRes;

		curl = curl_easy_init();
		if(curl == NULL)
		{
			printf("error CCurlFTP::Upload curl_easy_init\n");
			break;
		}
		//curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, TRUE);   //这个参数可以在FTP过程中显示FTP指令，如果你想看到的话。

		if (strlen(pFtpUpload->csUserName) != 0 
			&& strlen(pFtpUpload->csPassWord) != 0)
		{
			char temUser[256] = {0};
			sprintf_s(temUser,"%s:%s",pFtpUpload->csUserName,pFtpUpload->csPassWord);
			CUrlRes = curl_easy_setopt(curl, CURLOPT_USERPWD, temUser);
			if (CUrlRes != CURLE_OK)
			{
				printf("error CCurlFTP::Upload CURLOPT_USERPWD\n");
				break;
			}
		}

		curl_easy_setopt(curl, CURLOPT_URL, pFtpUpload->csSaveUrl);
		if (CUrlRes != CURLE_OK)
		{
			printf("error CCurlFTP::Upload CURLOPT_USERPWD\n");
			break;
		}
		curl_easy_setopt(curl, CURLOPT_PUT, 1);
		if (CUrlRes != CURLE_OK)
		{
			printf("error CCurlFTP::Upload CURLOPT_PUT\n");
			break;
		}
		curl_easy_setopt(curl, CURLOPT_INFILE, fp);
		if (CUrlRes != CURLE_OK)
		{
			printf("error CCurlFTP::Upload CURLOPT_INFILE\n");
			break;
		}
		curl_easy_setopt(curl, CURLOPT_INFILESIZE, (curl_off_t)(size_t)nFileSize);
		if (CUrlRes != CURLE_OK)
		{
			printf("error CCurlFTP::Upload CURLOPT_INFILESIZE\n");
			break;
		}
		curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1);
		if (CUrlRes != CURLE_OK)
		{
			printf("error CCurlFTP::Upload CURLOPT_FTP_CREATE_MISSING_DIRS\n");
			break;
		}


		CUrlRes = curl_easy_perform(curl);
		if(CUrlRes == CURLE_OK)
		{
			printf("success CCurlFTP::Upload curl_easy_perform\n");
			ret = true;
		}
		else
		{
			printf("error CCurlFTP::Upload curl_easy_perform [%d]\n",CUrlRes);
			ret = false;
		}
	} 
	while (false);

	fclose(fp);

	return ret;
}

size_t FtpDownloadData(void *buffer, size_t size, size_t nmemb, FILE *pfile)
{
	if (pfile != NULL)
	{
		fwrite(buffer,size,nmemb,pfile);
	}

	return size * nmemb;
}

bool CCurlFTP::Download(tagFtpDownload *pFtpDownload)
{
	bool ret = false;

	FILE* fp = NULL;
	fopen_s(&fp,pFtpDownload->csSavePath, "wb+");
	if(NULL == fp)
	{
		printf("error CCurlFTP::Download fopen (%s)\n", pFtpDownload->csSavePath);
		return false;
	}

	do 
	{
		CURL *curl = NULL;
		CURLcode CUrlRes;

		curl = curl_easy_init();
		if(curl == NULL)
		{
			printf("error CCurlFTP::Download curl_easy_init\n");
			break;
		}

		//这个参数可以在FTP过程中显示FTP指令，如果你想看到的话。
		//curl_easy_setopt(m_pCurl, CURLOPT_VERBOSE, TRUE);   
		
		if (strlen(pFtpDownload->csUserName) != 0 
			&& strlen(pFtpDownload->csPassWord) != 0)
		{
			char temUser[256] = {0};
			sprintf_s(temUser,"%s:%s",pFtpDownload->csUserName,pFtpDownload->csPassWord);
			CUrlRes = curl_easy_setopt(curl, CURLOPT_USERPWD, temUser);
			if (CUrlRes != CURLE_OK)
			{
				printf("error CCurlFTP::Download CURLOPT_USERPWD\n");
				break;
			}
		}

		curl_easy_setopt(curl, CURLOPT_URL, pFtpDownload->csFtpURL);
		if (CUrlRes != CURLE_OK)
		{
			printf("error CCurlFTP::Download CURLOPT_USERPWD\n");
			break;
		}
		CUrlRes = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, FtpDownloadData);
		if (CUrlRes != CURLE_OK)
		{
			printf("error CCurlFTP::Download CURLOPT_WRITEFUNCTION\n");
			break;
		}
		CUrlRes = curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		if (CUrlRes != CURLE_OK)
		{
			printf("error CCurlFTP::Download CURLOPT_WRITEDATA\n");
			break;
		}

		CUrlRes = curl_easy_perform(curl);
		if(CUrlRes == CURLE_OK)
		{
			printf("success CCurlFTP::Download curl_easy_perform\n");
			ret = true;
		}
		else
		{
			printf("error CCurlFTP::Download curl_easy_perform [%d]\n",CUrlRes);
			ret = false;
		}
	} 
	while (false);

	fclose(fp);
	return true;
}
