#include "HtmlAnalyzer.h"
#include <Shlwapi.h>


CHtmlAnalyzer::CHtmlAnalyzer(void)
{
	m_pstCur = NULL;
}

CHtmlAnalyzer::~CHtmlAnalyzer(void)
{

}

//验证URL是否在域内
BOOL CHtmlAnalyzer::VerifyURLInField(LPCTSTR lpszURL, LPCTSTR lpszWebSiteField)
{
	BOOL bResult = FALSE;

	if( lpszURL && lpszWebSiteField)
	{
		UINT nHostLen = 0;
		UINT nWebSiteFieldLen = ::_tcslen(lpszWebSiteField);

		//
		UINT nCount = 1;
		if( CHtmlAnalyzer::IsURL(lpszURL) )
			nCount = 3;
		//
		while( lpszURL[nHostLen] != _T('\0') )
		{
			if( lpszURL[nHostLen] == _T('/') )
				nCount--;
			if( nCount == 0 )	
				break;
			//
			nHostLen++;
		}

		bResult = ( 0 == ::_tcsnicmp(lpszURL+(nHostLen-nWebSiteFieldLen),lpszWebSiteField,nWebSiteFieldLen) );
	}

	return bResult;
}
//提取HOST地址
UINT CHtmlAnalyzer::ExtractHost(LPCTSTR lpszURL,LPTSTR lpszHost,UINT nHostMaxLen)
{
	UINT nResult = 0;

	if( lpszURL && lpszHost && nHostMaxLen > 0  )
	{
		//
		UINT nCount = 1;
		if( CHtmlAnalyzer::IsURL(lpszURL) )
			nCount = 3;
		//
		while( nResult < nHostMaxLen && lpszURL[nResult] != _T('\0') )
		{
			if( lpszURL[nResult] == _T('/') )
				nCount--;
			if( nCount > 0 )	
				lpszHost[nResult] = lpszURL[nResult];
			else
				break;
			//
			nResult++;
		}
	}

	return nResult;
}
BOOL CHtmlAnalyzer::IsURL(LPCTSTR lpszURL)
{
	BOOL bResult = FALSE;

	if( lpszURL )
	{
		if( (0 == ::_tcsnicmp(lpszURL,_T("http://"),7)) || 
			(0 == ::_tcsnicmp(lpszURL,_T("https://"),8)))
		{
			bResult = TRUE;
		}
	}

	return bResult;
}

BOOL CHtmlAnalyzer::Parse(PBYTE lpszString,UINT nStringLen,UINT nFromCharset)
{
	BOOL bResult = FALSE;
	if( lpszString && nStringLen >0  )
	{
		switch( nFromCharset )
		{
		case CHtmlAnalyzer::CHARSET_UTF8:
			{
				if( lpszString[0] == 0xEF && lpszString[1] == 0xBB && lpszString[2] == 0xBF )
					CHtmlHelper::TakeSnapshot(lpszString + 3,nStringLen -3,CHtmlAnalyzer::CHARSET_UTF8); // skip 0xEF,0xBB,0xBF
				else
					CHtmlHelper::TakeSnapshot(lpszString,nStringLen,CHtmlAnalyzer::CHARSET_UTF8); // skip 0xEF,0xBB,0xBF
			}
			break;
		case CHtmlAnalyzer::CHARSET_UNICODE:
			{
				if( lpszString[0] == 0xFE && lpszString[1] == 0xFF ) // skip 0xFE,0xFF
					CHtmlHelper::TakeSnapshot(lpszString + 2,nStringLen -2,CHtmlAnalyzer::CHARSET_UNICODE);// skip 0xFE,0xFF
				else
					CHtmlHelper::TakeSnapshot(lpszString,nStringLen,CHtmlAnalyzer::CHARSET_UNICODE);// skip 0xFE,0xFF
			}
			break;
		case CHtmlAnalyzer::CHARSET_MULTIBYTE:
		default:
			{
				CHtmlHelper::TakeSnapshot(lpszString,nStringLen,CHtmlAnalyzer::CHARSET_MULTIBYTE);
			}
			break;
		}

		CHtmlHelper::Parse2();

		bResult = TRUE;
	}

	return bResult;
}

//
LPCTSTR CHtmlAnalyzer::GetFirstLink()
{
	LPCTSTR pszResult = NULL;

	m_pstCur = m_pstHead;

	while( m_pstCur && !pszResult )
	{
		if( 0 != ::_tcsnicmp(m_pstCur->s_pszTag,_T("<script"),7) &&
			0 != ::_tcsnicmp(m_pstCur->s_pszTag,_T("<link"),5) &&
			0 != ::_tcsnicmp(m_pstCur->s_pszTag,_T("<style"),6) )
		{
			tagHtmlTagAttribute * pstAttributeCur = m_pstCur->s_pstTagAttribute;
			while( pstAttributeCur )
			{
				if( 0 == ::_tcsnicmp(pstAttributeCur->s_pszKey,_T("href"),4) ||
					0 == ::_tcsnicmp(pstAttributeCur->s_pszKey,_T("src"),3) )
				{
					//return
					pszResult = pstAttributeCur->s_pszValue;
					break ;
				}
				else
				{
					pstAttributeCur = pstAttributeCur->s_pstNext;
				}
			}
		}
		m_pstCur = m_pstCur->s_pstNext;
	}

	return pszResult;
}
LPCTSTR CHtmlAnalyzer::GetNextLink()
{
	LPCTSTR pszResult = NULL;

	while( m_pstCur && !pszResult )
	{
		if( 0 != ::_tcsnicmp(m_pstCur->s_pszTag,_T("<script"),7) &&
			0 != ::_tcsnicmp(m_pstCur->s_pszTag,_T("<link"),5) &&
			0 != ::_tcsnicmp(m_pstCur->s_pszTag,_T("<style"),6) )
		{
			tagHtmlTagAttribute * pstAttributeCur = m_pstCur->s_pstTagAttribute;
			while( pstAttributeCur )
			{
				if( 0 == ::_tcsnicmp(pstAttributeCur->s_pszKey,_T("href"),4) ||
					0 == ::_tcsnicmp(pstAttributeCur->s_pszKey,_T("src"),3) )
				{
					//return
					pszResult = pstAttributeCur->s_pszValue;
					break ;
				}
				else
				{
					pstAttributeCur = pstAttributeCur->s_pstNext;
				}
			}
		}

		m_pstCur = m_pstCur->s_pstNext;
	}

	return pszResult;
}

LPCTSTR CHtmlAnalyzer::GetFirstContent()
{
	LPCTSTR pszResult = NULL;

	m_pstCur = m_pstHead;

	while( m_pstCur && !pszResult )
	{
		if( 0 != ::_tcsnicmp(m_pstCur->s_pszTag,_T("<script"),7) &&
			0 != ::_tcsnicmp(m_pstCur->s_pszTag,_T("<style"),6) )
		{

			if( m_pstCur->s_pszContent )
				pszResult = m_pstCur->s_pszContent;
		}

		m_pstCur = m_pstCur->s_pstNext;
	}

	return pszResult;
}

LPCTSTR CHtmlAnalyzer::GetNextContent()
{
	LPCTSTR pszResult = NULL;

	while( m_pstCur && !pszResult )
	{
		if( 0 != ::_tcsnicmp(m_pstCur->s_pszTag,_T("<script"),7) &&
			0 != ::_tcsnicmp(m_pstCur->s_pszTag,_T("<style"),6) )
		{

			if( m_pstCur->s_pszContent )
				pszResult = m_pstCur->s_pszContent;
		}

		m_pstCur = m_pstCur->s_pstNext;
	}

	return pszResult;
}
//
LPCTSTR CHtmlAnalyzer::SearchText(LPCTSTR lpszText)
{
	LPCTSTR pszResult = NULL;

	tagHtmlNode *pstCur = m_pstHead;

	while( pstCur && !pszResult)
	{
		if( 0 != ::_tcsnicmp(pstCur->s_pszTag,_T("<script"),7) &&
			0 != ::_tcsnicmp(pstCur->s_pszTag,_T("<style"),6) )
		{
			if( pstCur->s_pszContent )
			{
				if( (NULL != ::StrStrI(pstCur->s_pszContent,lpszText)) )
					pszResult = pstCur->s_pszContent;
			}
		}

		pstCur = pstCur->s_pstNext;
	}

	return pszResult;
}


const CHtmlAnalyzer::tagHtmlNode* CHtmlAnalyzer::GetFirstNode(LPCTSTR *lpszLink, LPCTSTR *lpszContent)
{
	m_pstCur = m_pstHead;

	*lpszContent = NULL;
	*lpszLink = NULL;
	tagHtmlNode *pHtmlNode = NULL;
	while( m_pstCur != NULL && *lpszLink == NULL )
	{
		if( 0 != ::_tcsnicmp(m_pstCur->s_pszTag,_T("<script"),7) &&
			0 != ::_tcsnicmp(m_pstCur->s_pszTag,_T("<style"),6) )
		{
			if( m_pstCur->s_pszContent != NULL)
			{
				if( 0 != ::_tcsnicmp(m_pstCur->s_pszTag,_T("<script"),7) &&
					0 != ::_tcsnicmp(m_pstCur->s_pszTag,_T("<link"),5) &&
					0 != ::_tcsnicmp(m_pstCur->s_pszTag,_T("<style"),6) )
				{
					tagHtmlTagAttribute * pstAttributeCur = m_pstCur->s_pstTagAttribute;
					while( pstAttributeCur )
					{
						if( 0 == ::_tcsnicmp(pstAttributeCur->s_pszKey,_T("href"),4) ||
							0 == ::_tcsnicmp(pstAttributeCur->s_pszKey,_T("src"),3) )
						{
							//return
							*lpszLink = pstAttributeCur->s_pszValue;
							*lpszContent = m_pstCur->s_pszContent;
							pHtmlNode = m_pstCur;
							break ;
						}
						else
						{
							pstAttributeCur = pstAttributeCur->s_pstNext;
						}
					}
				}
			}
		}

		m_pstCur = m_pstCur->s_pstNext;
	}

	return pHtmlNode;
}

const CHtmlAnalyzer::tagHtmlNode* CHtmlAnalyzer::GetNextNode(LPCTSTR *lpszLink, LPCTSTR *lpszContent)
{
	*lpszContent = NULL;
	*lpszLink = NULL;
	tagHtmlNode *pHtmlNode = NULL;
	while( m_pstCur != NULL && *lpszLink == NULL )
	{
		if( 0 != ::_tcsnicmp(m_pstCur->s_pszTag,_T("<script"),7) &&
			0 != ::_tcsnicmp(m_pstCur->s_pszTag,_T("<style"),6) )
		{
			if( m_pstCur->s_pszContent != NULL)
			{
				if( 0 != ::_tcsnicmp(m_pstCur->s_pszTag,_T("<script"),7) &&
					0 != ::_tcsnicmp(m_pstCur->s_pszTag,_T("<link"),5) &&
					0 != ::_tcsnicmp(m_pstCur->s_pszTag,_T("<style"),6) )
				{
					tagHtmlTagAttribute * pstAttributeCur = m_pstCur->s_pstTagAttribute;
					while( pstAttributeCur )
					{
						if( 0 == ::_tcsnicmp(pstAttributeCur->s_pszKey,_T("href"),4) ||
							0 == ::_tcsnicmp(pstAttributeCur->s_pszKey,_T("src"),3) )
						{
							//return
							*lpszLink = pstAttributeCur->s_pszValue;
							*lpszContent = m_pstCur->s_pszContent;
							pHtmlNode = m_pstCur;
							break ;
						}
						else
						{
							pstAttributeCur = pstAttributeCur->s_pstNext;
						}
					}
				}
			}
		}

		m_pstCur = m_pstCur->s_pstNext;
	}
	return pHtmlNode;
}