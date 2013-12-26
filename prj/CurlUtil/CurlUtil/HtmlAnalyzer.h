#pragma once
#include "HtmlHelper.h"
#include "Macro.h"

const LPCTSTR HTMLANALYZER_HTTP_PREFIX						= _T("http://");
const LPCTSTR HTMLANALYZER_HTTPS_PREFIX						= _T("https://");

#define HTMLANALYZER_HTTP_PREFIX_LEN						7
#define HTMLANALYZER_HTTPS_PREFIX_LEN						8

const LPCTSTR HTMLANALYZER_HERF_KEY							=_T("href");
const LPCTSTR HTMLANALYZER_SRC_KEY							=_T("src");


class CURLUTIL_API CHtmlAnalyzer :public CHtmlHelper
{
public:
	//验证URL是否在域内
	static BOOL VerifyURLInField(LPCTSTR lpszURL, LPCTSTR lpszWebSiteField);
	//提取HOST地址
	static UINT ExtractHost(LPCTSTR lpszURL,LPTSTR lpszHost,UINT nHostMaxLen);
	//
	static BOOL IsURL(LPCTSTR lpszURL);
public:
	CHtmlAnalyzer(void);
	~CHtmlAnalyzer(void);
public:

	virtual BOOL Parse(PBYTE lpszString,UINT nStringLen,UINT nFromCharset);
	//
	LPCTSTR GetFirstLink();
	LPCTSTR GetNextLink();
	LPCTSTR GetFirstContent();
	LPCTSTR GetNextContent();
	//"aaabbbb",search "bbbb",but return "aaabbbb"
	LPCTSTR	SearchText(LPCTSTR lpszText);

	const tagHtmlNode* GetFirstNode(LPCTSTR *lpszLink, LPCTSTR *lpszContent);
	const tagHtmlNode* GetNextNode(LPCTSTR *lpszLink, LPCTSTR *lpszContent);
protected:
	//
	tagHtmlNode * m_pstCur;

};