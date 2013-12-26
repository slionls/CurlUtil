#include "HtmlHelper.h"
#include <iostream>
using namespace std;

#include <fstream>

void log(LPCTSTR msg)
{
	wfstream fp;
	locale loc("");
	fp.imbue(loc);
	fp.open(L"c:\\html.txt",ios::app);

	fp << msg;
	fp.close();
	
}

LPCTSTR CHtmlHelper::SkipSpace(LPCTSTR lpszString)
{
	LPCTSTR pszFind = lpszString;
	if( pszFind )
	{
		//跳这下面这些字符
		while( *pszFind == _T(' ')||*pszFind == _T('\r')||*pszFind == _T('\n')||*pszFind == _T('\t') )
		{
			pszFind++;
		}
	}
	return pszFind;
}


LPCTSTR CHtmlHelper::SkipLetter(LPCTSTR lpszString,TCHAR cbLetter)
{
	LPCTSTR pszFind = lpszString;
	if( pszFind )
	{
		//抵达
		while( *pszFind != _T('\0') && *pszFind == cbLetter )
		{
			pszFind++;
		}
	}
	return pszFind;
}

LPCTSTR CHtmlHelper::ArriveLetter(LPCTSTR lpszString,TCHAR cbLetter)
{
	LPCTSTR pszFind = lpszString;
	if( pszFind )
	{
		//抵达
		while( *pszFind != _T('\0') && *pszFind != cbLetter )
		{
			pszFind++;
		}
	}
	return pszFind;
}

UINT CHtmlHelper::CalculateStringLen(LPCTSTR lpszString,TCHAR* lpszEnd)
{
	UINT nResult = 0;

	if( lpszString )
	{
		LPCTSTR pszFind = ::_tcsstr(lpszString,lpszEnd);

		if(pszFind)
			nResult = (pszFind - lpszString);
	}

	return nResult;
}


UINT CHtmlHelper::CalculateRangeLen(LPCTSTR lpszString,LPCTSTR lpszBegin,LPCTSTR lpszEnd)
{
	UINT nResult = 0;

	LPCTSTR pszFind = lpszString;
	if( pszFind && lpszBegin && lpszEnd)
	{
		UINT nBeginLen = ::lstrlen(lpszBegin);
		UINT nEndLen = ::lstrlen(lpszEnd);
		// 起始字符串必须相同
		if( ::_tcsnicmp(pszFind,lpszBegin,nBeginLen) == 0 )
		{
			//+=nEndLen
			nResult += nBeginLen;
			pszFind += nBeginLen;

			while( *pszFind != _T('\0'))
			{
				if( ::_tcsnicmp(pszFind,lpszEnd,nEndLen) == 0 )
				{
					//+=nEndLen
					nResult += nEndLen;


					break;
				}
				else
				{
					//++
					nResult++;
					pszFind++;
				}

			}
		}
	}

	return nResult;
}

UINT CHtmlHelper::CalculateTagStringLen(LPCTSTR lpszString,TCHAR cbBegin,TCHAR cbEnd)
{
	UINT nResult = 0;

	LPCTSTR pszFind = lpszString;
	if( pszFind && *pszFind == cbBegin ) // 起始字符必须相同
	{
		while( *pszFind != _T('\0') )
		{
			//++
			nResult++;

			if( *pszFind == cbEnd)
			{
				break;		
			}

			//++
			pszFind++;
		}
	}

	return nResult;
}

UINT CHtmlHelper::CalculateTagAllStringLen(LPCTSTR lpszString, LPCTSTR lpszTag)
{
	UINT nResult = 0;

	if( lpszTag != NULL && lpszString != NULL)
	{
		LPCTSTR pszFind = ::_tcschr(lpszString,_T('>'));
		
		if (pszFind != NULL 
			&& pszFind - lpszString > 1
			&& *(pszFind-1) == _T('/'))
		{
			//"/>"
			nResult = (pszFind - lpszString + 1);
			return nResult;
		}	

		while (pszFind != NULL)
		{
			pszFind = ::_tcsstr(pszFind,lpszTag);	
			if (pszFind != NULL 
				&& pszFind - lpszString > 2 
				&& *(pszFind-1) == _T('/') 
				&& *(pszFind-2) == _T('<'))
			{
				//"</tag>"
				nResult = (pszFind - lpszString - 2);
				return nResult;
			}
			
			if (pszFind != NULL)
			{
				pszFind += _tcslen(lpszTag);
			}
		}
	}

	return nResult;
}

UINT CHtmlHelper::CalculateScriptStringLen(LPCTSTR lpszString,TCHAR cbEnd)
{
	UINT nResult = 0;

	LPCTSTR pszFind = lpszString;

	if( pszFind )
	{
		UINT nMarkIndex = 0;
		TCHAR szMark[1024] = {_T('\0')}; //max 1024

		while( *pszFind != _T('\0') )
		{
			//	if( szMark[nMarkIndex] != _T('\'') && szMark[nMarkIndex] != _T('\"') )
			if( szMark[nMarkIndex] != _T('(') && szMark[nMarkIndex] != _T('\'') && szMark[nMarkIndex] != _T('\"') )
				//	if( szMark[0] == _T('\0') )
			{
				//note
				if( *pszFind == _T('/') && *(pszFind+1) == _T('/') )
				{
					UINT nLen = CHtmlHelper::CalculateStringLen(pszFind,_T("\n"));

					//+=nLen+1
					nResult += nLen+1;
					pszFind += nLen+1;
				}
				else if( *pszFind == _T('/') && *(pszFind+1) == _T('*')  )
				{
					UINT nLen =  CHtmlHelper::CalculateRangeLen(pszFind,_T("/*"),_T("*/"));

					//+=nLen
					nResult += nLen;
					pszFind += nLen;
				}

			}

			if( *pszFind == _T('\\') && 
				( *(pszFind+1) == _T('\\') || 
				*(pszFind+1) == _T('(') || *(pszFind+1) == _T(')') || 
				*(pszFind+1) == _T('[') || *(pszFind+1) == _T(']') || 
				*(pszFind+1) == _T('{') || *(pszFind+1) == _T('}') || 
				*(pszFind+1) == _T('\'') || 
				*(pszFind+1) == _T('\"') ) )
			{
				//算转意字符
				//+=2
				nResult+=2;
				pszFind+=2;

			}
			else if( *pszFind == _T('{') || *pszFind == _T('(') || *pszFind == _T('[') ||  (*pszFind == _T('\'') || *pszFind == _T('\"')) )
			{
				if( szMark[nMarkIndex] != _T('\'') && szMark[nMarkIndex] != _T('\"') )
				{
					if( nMarkIndex < 1024 )
					{
						if( nMarkIndex == 0 && szMark[nMarkIndex] == _T('\0')  )
							szMark[nMarkIndex] = *pszFind;
						else
							szMark[++nMarkIndex] = *pszFind;
					}
				}
				else if( szMark[nMarkIndex] == *pszFind )
				{
					if( nMarkIndex >0  )
						szMark[nMarkIndex--] = _T('\0');
					else
						szMark[nMarkIndex] = _T('\0');
				}
				//++
				nResult++;
				pszFind++;

			}
			else if( *pszFind == _T('}') ) 
			{
				if( szMark[nMarkIndex] == _T('{') )
				{
					if( nMarkIndex >0  )
						szMark[nMarkIndex--] = _T('\0');
					else
						szMark[nMarkIndex] = _T('\0');
				}
				//++
				nResult++;
				pszFind++;
			}
			else if( *pszFind == _T(')') )
			{
				if( szMark[nMarkIndex] == _T('(') )
				{
					if( nMarkIndex >0  )
						szMark[nMarkIndex--] = _T('\0');
					else
						szMark[nMarkIndex] = _T('\0');
				}
				//++
				nResult++;
				pszFind++;
			}
			else if( *pszFind == _T(']') ) 
			{
				if( szMark[nMarkIndex] == _T('[') )
				{
					if( nMarkIndex >0  )
						szMark[nMarkIndex--] = _T('\0');
					else
						szMark[nMarkIndex] = _T('\0');
				}
				//++
				nResult++;
				pszFind++;
			}
			else if( *pszFind == cbEnd && szMark[0] == _T('\0') )  //nMarkIndex == 0 &&
			{
				break;
			}
			else
			{
				//++
				nResult++;
				pszFind++;
			}
		}
	}

	return nResult;
}

void CHtmlHelper::TrimString(LPCTSTR lpszString,LPCTSTR lpszClean)
{
	if( lpszString )
	{
		UINT nStringLen = ::lstrlen(lpszString);
		UINT nCleanLen = ::lstrlen(lpszClean);
		LPCTSTR lpszFind = lpszString;
		while( *lpszFind != _T('\0') )
		{
			if( 0 == ::_tcsnicmp(lpszFind,lpszClean,nCleanLen) )
			{
				LPTSTR lpszMove = (LPTSTR)lpszFind;
				//::memmove(lpszFind,lpszFind+nCleanLen,nStringLen-nCleanLen);
				while( *lpszMove = *(lpszMove+nCleanLen) )
				{
					lpszMove++;
				}
			}
			else
			{
				lpszFind++;
			}
		}
	}
}

void CHtmlHelper::TrimChar(LPCTSTR lpszString,LPCTSTR lpszCharSet)
{
	if( lpszString )
	{
		UINT nStringLen = ::lstrlen(lpszString);
		UINT nCleanSetLen = ::lstrlen(lpszCharSet);
		LPTSTR lpszFind = (LPTSTR)lpszString;
		while( *lpszFind != _T('\0') )
		{
			bool bIsFind = false;
			TCHAR szFind = *lpszFind;
			for (UINT i=0; i<nCleanSetLen && *lpszFind != _T('\0'); ++i)
			{
				if( szFind == lpszCharSet[i] )
				{
					bIsFind = true;
					LPTSTR lpszMove = lpszFind;
					
					while( *lpszMove != _T('\0') )
					{
						int offset = 1;
						int j = 0;
						while (j <nCleanSetLen)
						{
							if (*(lpszMove+offset) == lpszCharSet[j])
							{
								++offset;
								j=0;
							}
							else
							{
								++j;
							}							
						}
						if(_T('\0') == (*lpszFind = *(lpszMove+offset)))
						{
							break;
						}
						lpszFind++;
						lpszMove+=offset;
					}
				}
			}
			
			if( !bIsFind )
			{
				lpszFind++;
			}
		}
	}
}

void CHtmlHelper::CombineChar(LPCTSTR lpszString,TCHAR szChar)
{
	if(lpszString != NULL)
	{
		LPTSTR lpszFind = (LPTSTR)lpszString;
		
		int nIndex = 0;
		while (lpszFind[nIndex] != _T('\0'))
		{
			int nOffset = 1;
			if (lpszFind[nIndex] == szChar)
			{
				while (lpszFind[nIndex+nOffset] == szChar)
				{
					++nOffset;
				}

				int i = 0;
				while( nOffset > 1 && _T('\0') == (lpszFind[nIndex+i+1] = lpszFind[nIndex+nOffset+i]) )
				{
					i++;
				}
			}
			if (lpszFind[nIndex] == _T('\0'))
			{
				break;
			}
			
			++nIndex;
		}
	}
}

CHtmlHelper::CHtmlHelper()
{
	m_pszSnapshotBuffer = NULL;
	m_nSnapshotBufferLen = 0;
	m_nSnapshotStringLen = 0;
	m_pstHead = NULL;
	m_pstPrev = NULL;
}
CHtmlHelper::~CHtmlHelper()
{
	DeleteSnapshot();
	CleanupHtmlNode();
}

CHtmlHelper::tagHtmlNode * CHtmlHelper::AllocHtmlNode(UINT nTagLen,UINT nContentLen)
{
	CHtmlHelper::tagHtmlNode * pstResult = new CHtmlHelper::tagHtmlNode;
	if( pstResult )
	{
		::ZeroMemory(pstResult,sizeof(CHtmlHelper::tagHtmlNode));
		if( nTagLen >0 )
		{
			pstResult->s_pszTag = new TCHAR[nTagLen+1];
			::ZeroMemory((LPVOID)pstResult->s_pszTag,(nTagLen+1)*sizeof(TCHAR));
		}
		if( nContentLen >0 )
		{
			pstResult->s_pszContent = new TCHAR[nContentLen+1];
			::ZeroMemory((LPVOID)pstResult->s_pszContent,(nContentLen+1)*sizeof(TCHAR));
		}
	}

	return pstResult;
}


void CHtmlHelper::FreeHtmlNode(CHtmlHelper::tagHtmlNode * lpstNode)
{
	if( lpstNode )
	{
		if( lpstNode->s_pszTag)
			delete []lpstNode->s_pszTag;
		if(lpstNode->s_pszContent)
			delete []lpstNode->s_pszContent;

		delete lpstNode;
	}
}

//
void CHtmlHelper::LinkHtmlNode(CHtmlHelper::tagHtmlNode * lpstNode)
{
	if( m_pstHead == NULL )
	{
		m_pstHead = lpstNode;
		m_pstPrev = lpstNode;
	}
	else
	{
		m_pstPrev->s_pstNext = lpstNode;
		m_pstPrev = lpstNode;
	}

#ifdef _DEBUG

	wstring LogMsg;
	LogMsg = _T("Tag:");
	//::OutputDebugString(_T("Tag:"));
	if( lpstNode->s_pszTag )
	{		
		LogMsg += lpstNode->s_pszTag;
		//::OutputDebugString(lpstNode->s_pszTag);
	}
	LogMsg += _T("\nContent:");
	//::OutputDebugString(_T("\r\nContent:"));
	if( lpstNode->s_pszContent )
	{
		LogMsg += lpstNode->s_pszContent;
		//::OutputDebugString(lpstNode->s_pszContent);
	}
	//::OutputDebugString(_T("\r\n"));
	LogMsg += _T("\n");
	log(LogMsg.c_str());
#endif //_DEBUG
}
void CHtmlHelper::CleanupHtmlNode()
{
	while( m_pstHead )
	{
		m_pstPrev = m_pstHead;
		m_pstHead = m_pstHead->s_pstNext;
		//first
		CleanupHtmlTagAttribute(m_pstPrev);
		//second
		FreeHtmlNode(m_pstPrev);
	}
	m_pstHead = NULL;
	m_pstPrev = NULL;
}

CHtmlHelper::tagHtmlTagAttribute * CHtmlHelper::AllocHtmlTagAttribute(UINT nKeyLen,UINT nValueLen)
{
	CHtmlHelper::tagHtmlTagAttribute * pstResult = new CHtmlHelper::tagHtmlTagAttribute;
	if( pstResult )
	{
		::ZeroMemory(pstResult,sizeof(CHtmlHelper::tagHtmlTagAttribute));
		if( nKeyLen >0 )
		{
			pstResult->s_pszKey = new TCHAR[nKeyLen+1];
			::ZeroMemory((LPVOID)pstResult->s_pszKey,(nKeyLen+1)*sizeof(TCHAR));
		}
		if( nValueLen >0 )
		{
			pstResult->s_pszValue = new TCHAR[nValueLen+1];
			::ZeroMemory((LPVOID)pstResult->s_pszValue,(nValueLen+1)*sizeof(TCHAR));
		}
	}

	return pstResult;
}

void CHtmlHelper::FreeHtmlTagAttribute(CHtmlHelper::tagHtmlTagAttribute * lpstTagAttribute)
{
	if( lpstTagAttribute )
	{
		if( lpstTagAttribute->s_pszKey)
			delete []lpstTagAttribute->s_pszKey;
		if(lpstTagAttribute->s_pszValue)
			delete []lpstTagAttribute->s_pszValue;

		delete lpstTagAttribute;
	}
}

//
void CHtmlHelper::AttachHtmlTagAttribute(tagHtmlNode * lpstNode,CHtmlHelper::tagHtmlTagAttribute * lpstTagAttribute)
{
	lpstTagAttribute->s_pstNext = lpstNode->s_pstTagAttribute;
	lpstNode->s_pstTagAttribute = lpstTagAttribute;

#ifdef _DEBUG

	wstring LogMsg;
	LogMsg = _T("\t");
	//OutputDebugString(_T("\tKey:"));
	if( lpstTagAttribute->s_pszKey )
	{
		LogMsg += lpstTagAttribute->s_pszKey;
		//::OutputDebugString(lpstTagAttribute->s_pszKey);
	}
	LogMsg += _T("=");
	//OutputDebugString(_T("\n\tValue:"));
	if( lpstTagAttribute->s_pszValue )
	{
		LogMsg += lpstTagAttribute->s_pszValue;
		//::OutputDebugString(_T("--"));
		//::OutputDebugString(lpstTagAttribute->s_pszValue);
		//::OutputDebugString(_T("--\r\n"));
		//::OutputDebugString(_T("\r\n"));
	}
	//OutputDebugString(_T("\n"));
	LogMsg += _T("\n");
	log(LogMsg.c_str());
#endif //_DEBUG
}

void CHtmlHelper::CleanupHtmlTagAttribute(tagHtmlNode * lpstNode)
{
	CHtmlHelper::tagHtmlTagAttribute * pstHead = lpstNode->s_pstTagAttribute;
	CHtmlHelper::tagHtmlTagAttribute * pstPrev = NULL;
	while( pstHead )
	{
		pstPrev = pstHead;
		pstHead = pstHead->s_pstNext;
		FreeHtmlTagAttribute(pstPrev);
	}
	pstHead = NULL;
	pstPrev = NULL;
}

//
void CHtmlHelper::AutoTakeSnapshot(PBYTE lpszString,UINT nStringLen)
{
	if( lpszString && nStringLen > 0)
	{
		if( nStringLen >= 2 )
		{
			if( lpszString[0] == 0xFE && lpszString[1] == 0xFF ) // skip 0xFE,0xFF
			{
				TakeSnapshot(lpszString+2,nStringLen-2,CHtmlHelper::CHARSET_UNICODE);
			}
			else if( lpszString[0] == 0xEF && lpszString[1] == 0xBB && lpszString[2] == 0xBF )// skip 0xEF,0xBB,0xBF
			{
				TakeSnapshot(lpszString+3,nStringLen-3,CHtmlHelper::CHARSET_UTF8);
			}
			else
			{
				TakeSnapshot(lpszString,nStringLen,CHtmlHelper::CHARSET_MULTIBYTE);
			}
		}
		else
		{
			TakeSnapshot(lpszString,nStringLen,CHtmlHelper::CHARSET_MULTIBYTE);
		}
	}
}

void CHtmlHelper::TakeSnapshot(PBYTE lpszString,UINT nStringLen,UINT nFromCharset )
{
	//delete old snapshot
	DeleteSnapshot();

	if( lpszString && nStringLen > 0 )
	{

		//transform to TCHAR

		if( CHtmlHelper::CHARSET_UTF8 == nFromCharset )
		{

#ifdef _UNICODE 

			m_nSnapshotBufferLen = nStringLen;
			m_pszSnapshotBuffer = new TCHAR[m_nSnapshotBufferLen];

			::memset((LPVOID)m_pszSnapshotBuffer,0,m_nSnapshotBufferLen*sizeof(TCHAR));
			m_nSnapshotStringLen = ::MultiByteToWideChar(CP_UTF8,0,(LPCSTR)lpszString,nStringLen,m_pszSnapshotBuffer,m_nSnapshotBufferLen);
#else
			::OutputDebugString(_T("no support"));

#endif //_UNICODE

		}
		else if( CHtmlHelper::CHARSET_UNICODE == nFromCharset )
		{

#ifdef _UNICODE 

			m_nSnapshotBufferLen = nStringLen;
			m_pszSnapshotBuffer = new TCHAR[m_nSnapshotBufferLen];

			::memset((LPVOID)m_pszSnapshotBuffer,0,m_nSnapshotBufferLen*sizeof(TCHAR));
			::memcpy((LPVOID)m_pszSnapshotBuffer,lpszString,nStringLen);

#else

			m_nSnapshotBufferLen = nStringLen/2+1;
			m_pszSnapshotBuffer = new TCHAR[m_nSnapshotBufferLen];

			::memset((LPVOID)m_pszSnapshotBuffer,0,m_nSnapshotBufferLen*sizeof(TCHAR));
			m_nSnapshotStringLen = ::WideCharToMultiByte(CP_ACP,0,(LPWSTR)lpszString,nStringLen,(LPSTR)m_pszSnapshotBuffer,m_nSnapshotBufferLen,NULL,NULL);

#endif //_UNICODE

		}
		else
		{

#ifdef _UNICODE 

			m_nSnapshotBufferLen = nStringLen;
			m_pszSnapshotBuffer = new TCHAR[m_nSnapshotBufferLen];

			::memset(m_pszSnapshotBuffer,0,m_nSnapshotBufferLen*sizeof(TCHAR));
			m_nSnapshotStringLen = ::MultiByteToWideChar(CP_ACP,0,(LPCSTR)lpszString,nStringLen,m_pszSnapshotBuffer,m_nSnapshotBufferLen);
#else

			m_nSnapshotBufferLen = nStringLen;
			m_pszSnapshotBuffer = new TCHAR[m_nSnapshotBufferLen];

			::memset((LPVOID)m_pszSnapshotBuffer,0,m_nSnapshotBufferLen*sizeof(TCHAR));
			::memcpy((LPVOID)m_pszSnapshotBuffer,lpszString,nStringLen);

#endif //_UNICODE

		}

	}
}

void CHtmlHelper::DeleteSnapshot()
{
	if( m_pszSnapshotBuffer )
		delete []m_pszSnapshotBuffer;

	m_pszSnapshotBuffer = NULL;
	m_nSnapshotBufferLen = 0;
	m_nSnapshotStringLen = 0;
}

void CHtmlHelper::Parse2()
{
	CleanupHtmlNode();

	LPCTSTR pszFind = CHtmlHelper::SkipSpace(m_pszSnapshotBuffer);
	while( *pszFind != _T('\0'))
	{
		if( *pszFind == _T('<') )
		{
			if( *(pszFind+1) == _T('!') )
			{
				UINT nTagLen = 0;
				if( *(pszFind+2) == _T('-') && *(pszFind+3) == _T('-') )
				{
					//<!--aasdfasdfasdfsdf-->
					nTagLen = CHtmlHelper::CalculateRangeLen(pszFind,_T("<!--"),_T("-->"));
				}
				else
				{
					//<!aasdfasdf>
					nTagLen = CHtmlHelper::CalculateTagStringLen(pszFind,_T('<'),_T('>'));
				}

				if( nTagLen >0 )
				{
					//tagHtmlNode * pstNode = AllocHtmlNode(nTagLen,0);
					//if( nTagLen > 0 )
					//	::_tcsncpy(pstNode->s_pszTag,pszFind,nTagLen);

					////link
					//LinkHtmlNode(pstNode);
					//skip
					pszFind += nTagLen;
				}
				else
				{
					break;
				}
			}
			else 
			{
				UINT nTagLen = CHtmlHelper::CalculateTagStringLen(pszFind,_T('<'),_T('>'));
				if( *(pszFind+1) == _T('/'))
				{
					/*
					//</aaa>
					tagHtmlNode * pstNode = AllocHtmlNode(nTagLen,0);
					if( nTagLen > 0 )
						::_tcsncpy_s(pstNode->s_pszTag,nTagLen+1,pszFind,nTagLen);

					//link
					LinkHtmlNode(pstNode);
					*/
					//skip
					pszFind += nTagLen;
				}
				else
				{
					LPCTSTR pszFind2 = CHtmlHelper::SkipSpace(pszFind+nTagLen);

					if( *pszFind2 == _T('<') )
					{
						TCHAR szTag[16] = {0};
						//skip '<' or 'space',get tagString,until 'space' or '>'
						::_stscanf_s(pszFind,_T("%*[<| ]%[^ |=|\"|\r|\n|\t|>]"),szTag,16);
						//is STYLE or SCRIPT
						if( ::lstrcmpi(szTag,_T("STYLE")) == 0 || ::lstrcmpi(szTag,_T("SCRIPT")) == 0 )
						{
							UINT nContentLen = CalculateTagAllStringLen(pszFind,szTag);
							pszFind = pszFind+nContentLen;

							//UINT nContentLen = CHtmlHelper::CalculateScriptStringLen(pszFind2,_T('<'));
							//pszFind = pszFind2+nContentLen;
						}
						else
						{
							tagHtmlNode * pstNode = AllocHtmlNode(nTagLen,0);
							if( nTagLen > 0 )
								::_tcsncpy_s(pstNode->s_pszTag,nTagLen+1,pszFind,nTagLen);

							//link
							LinkHtmlNode(pstNode);
							//split Tag Attribute
							SplitTagAttribute(pstNode);
							//skip
							pszFind += nTagLen;
						}
					}
					else
					{
						TCHAR szTag[16] = {0};
						//skip '<' or 'space',get tagString,until 'space' or '>'
						::_stscanf_s(pszFind,_T("%*[<| ]%[^ |=|\"|\r|\n|\t|>]"),szTag,16);
						//is STYLE or SCRIPT
						if( ::lstrcmpi(szTag,_T("STYLE")) == 0 || ::lstrcmpi(szTag,_T("SCRIPT")) == 0 )
						{
							UINT nContentLen = CalculateTagAllStringLen(pszFind,szTag);
							pszFind = pszFind+nContentLen;

							//UINT nContentLen = CHtmlHelper::CalculateScriptStringLen(pszFind2,_T('<'));
							//pszFind = pszFind2+nContentLen;
						}
						else
						{
							UINT nContentLen = CHtmlHelper::CalculateStringLen(pszFind2,_T("<"));
							tagHtmlNode * pstNode = AllocHtmlNode(nTagLen,nContentLen);

							if( nTagLen > 0 )
								::_tcsncpy_s(pstNode->s_pszTag,nTagLen+1,pszFind,nTagLen);
							if( nContentLen > 0 )
								::_tcsncpy_s(pstNode->s_pszContent,nContentLen+1,pszFind2,nContentLen);

							//合并空格
							CombineChar(pstNode->s_pszContent,_T(' '));
							//剔除转义字符
							TrimChar(pstNode->s_pszContent,_T("\r\n\t"));
							
							//link
							LinkHtmlNode(pstNode);
							//split Tag Attribute
							SplitTagAttribute(pstNode);
							//skip
							pszFind = pszFind2+nContentLen;
						}
					}
				}
			}
		}
		else
		{
			pszFind++;
		}
	}
}

//
void CHtmlHelper::SplitTagAttribute(CHtmlHelper::tagHtmlNode * lpstNode)
{
	LPCTSTR pszFind = CHtmlHelper::ArriveLetter(lpstNode->s_pszTag,_T(' '));
	while( *pszFind != _T('\0') && *pszFind != _T('>') && *pszFind != _T('/'))
	{
		LPCTSTR pszKeyBegin = CHtmlHelper::SkipSpace(pszFind);
		LPCTSTR pszKeyEnd = pszKeyBegin;
		UINT nKeyLen = 0;
		while( *pszKeyEnd != _T(' ') && *pszKeyEnd != _T('=') && *pszKeyEnd != _T('\0') )
		{
			nKeyLen++;
			pszKeyEnd++;
		}

		TCHAR szTag[16] = {0};
		::_stscanf_s(pszKeyEnd,_T("%*[=| ]%[0-9]"),szTag,16);

		LPCTSTR pszValueBegin = NULL;
		LPCTSTR pszValueEnd = NULL;
		UINT nValueLen = 0;
		if (_tcslen(szTag) > 0)
		{
			//参数为数字的情况
			pszValueBegin = CHtmlHelper::ArriveLetter(pszKeyEnd,_T('='));
			pszValueBegin = CHtmlHelper::SkipLetter(pszValueBegin,_T('='));
			pszValueEnd = pszValueBegin;
			while( *pszValueEnd != _T(' ') && *pszValueEnd != _T('>')&& *pszValueEnd != _T('\0'))
			{
				nValueLen++;
				pszValueEnd++;
			}
		}
		else
		{
			//"aaa" or 'aaa'
			LPCTSTR pszTemKey = pszKeyEnd;
			while( *pszTemKey != _T('\0') && *pszTemKey != _T('\"') && *pszTemKey != _T('\'') )
			{
				pszTemKey++;
			}
			pszValueBegin = pszTemKey;
			//"aaa" or 'aaa'
			//pszValueBegin = CHtmlHelper::ArriveLetter(pszKeyEnd,*pszKeyEnd);
			// aaa" or aaa'
			pszValueBegin++;
			//pszValueBegin = CHtmlHelper::SkipLetter(pszValueBegin,_T('\"'));
			pszValueEnd = pszValueBegin;
			while( *pszValueEnd != *pszTemKey && *pszValueEnd != _T('\0'))
			{
				nValueLen++;
				pszValueEnd++;
			}
		}
		
		// get "key" and "value"
		CHtmlHelper::tagHtmlTagAttribute * pstAttribute = AllocHtmlTagAttribute(nKeyLen,nValueLen);

		if( nKeyLen > 0 )
			::_tcsncpy_s(pstAttribute->s_pszKey,nKeyLen+1,pszKeyBegin,nKeyLen);
		if( nValueLen > 0 )
			::_tcsncpy_s(pstAttribute->s_pszValue,nValueLen+1,pszValueBegin,nValueLen);

		//link
		AttachHtmlTagAttribute(lpstNode,pstAttribute);

		//next
		pszFind = CHtmlHelper::ArriveLetter(pszValueEnd,_T(' '));
		pszFind = CHtmlHelper::SkipSpace(pszFind);
	}
}

