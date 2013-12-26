#pragma once
#include <windows.h>
#include <tchar.h>
#include "Macro.h"
class CURLUTIL_API CHtmlHelper
{
public:
	//
	static	LPCTSTR SkipSpace(LPCTSTR lpszString);
	static	LPCTSTR SkipLetter(LPCTSTR lpszString,TCHAR cbLetter);
	static	LPCTSTR ArriveLetter(LPCTSTR lpszString,TCHAR cbLetter);
	static	UINT CalculateStringLen(LPCTSTR lpszString,TCHAR* lpszEnd);
	static	UINT CalculateRangeLen(LPCTSTR lpszString,LPCTSTR lpszBegin,LPCTSTR lpszEnd);
	static	UINT CalculateTagStringLen(LPCTSTR lpszString,TCHAR cbBegin,TCHAR cbEnd);
	static	UINT CalculateTagAllStringLen(LPCTSTR lpszString, LPCTSTR lpszTag);
	static	UINT CalculateScriptStringLen(LPCTSTR lpszString,TCHAR cbEnd);
	static  void TrimString(LPCTSTR lpszString,LPCTSTR lpszClean);
	static  void TrimChar(LPCTSTR lpszString,LPCTSTR lpszCharSet);
	static  void CombineChar(LPCTSTR lpszString,TCHAR szChar);
public:
	struct tagHtmlTagAttribute
	{
		LPTSTR s_pszKey;
		LPTSTR s_pszValue;
		struct tagHtmlTagAttribute * s_pstNext;
	};
	struct tagHtmlNode
	{
		LPTSTR s_pszTag;
		LPTSTR s_pszContent;
		tagHtmlTagAttribute * s_pstTagAttribute;
		struct tagHtmlNode * s_pstNext;
	};
public:
	CHtmlHelper(void);
	virtual ~CHtmlHelper(void);
protected:
	//
	tagHtmlNode * AllocHtmlNode(UINT nTagLen,UINT nContentLen);
	void FreeHtmlNode(tagHtmlNode * lpstNode);
	void LinkHtmlNode(tagHtmlNode * lpstNode);
	void CleanupHtmlNode();
	//
	tagHtmlTagAttribute * AllocHtmlTagAttribute(UINT nKeyLen,UINT nValueLen);
	void FreeHtmlTagAttribute(tagHtmlTagAttribute * lpstTagAttribute);
	void AttachHtmlTagAttribute(tagHtmlNode * lpstNode,tagHtmlTagAttribute * lpstTagAttribute);
	void CleanupHtmlTagAttribute(tagHtmlNode * lpstNode);
public:
	enum {CHARSET_UTF8,CHARSET_UNICODE,CHARSET_MULTIBYTE}TextCharset;
public:
	//
	void AutoTakeSnapshot(PBYTE lpszString,UINT nStringLen);
	void TakeSnapshot(PBYTE lpszString,UINT nStringLen,UINT nFromCharset );
	void DeleteSnapshot();
	//
	//virtual void Parse();
	virtual void Parse2();
protected:
	//
	void SplitTagAttribute(tagHtmlNode * lpstNode);

protected:
	//
	LPTSTR  m_pszSnapshotBuffer;
	UINT m_nSnapshotBufferLen;
	UINT m_nSnapshotStringLen;
	//
	tagHtmlNode * m_pstHead;
	tagHtmlNode * m_pstPrev;

};

