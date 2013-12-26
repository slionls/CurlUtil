#pragma once
#include "stdafx.h"

#ifdef CURLUTIL_EXPORTS
#define CURLUTIL_API __declspec(dllexport)
#else
#define CURLUTIL_API __declspec(dllimport)
#endif

#define MAX_FILE_NAME 128
#define MAX_URL 256
#define MAX_HTTP_HEAD_ALL_LEN 1024
#define MAX_HTTP_HEAD_ALL_CNT 64
#define MAX_HTTP_HEAD_ONCE_LEN 256

#define MAX_FTP_USERNAME 128
#define MAX_FTP_PASSWORD 128
