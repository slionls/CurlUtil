#include "tools.h"
#include <sstream>
#include <iostream>
#include <Windows.h>
#include <locale.h>
#include <Shobjidl.h>

using namespace std;

//����strKey����õ���strKey��ֵ
string GetValue( string& strContent, string strKey, string strSeparator )
{
	string strRes;
	string str = strContent;
	string strOneItem;
	size_t curPos = 0;
	size_t oldPos = 0;
	oldPos = curPos;

	curPos = str.find(strSeparator.c_str(),oldPos);
	strOneItem=str.substr(0,curPos-oldPos);
	while (strOneItem != "")
	{		
		string strLeft,strRight;
		StringHelp( strOneItem,strLeft,strRight );
		if(strLeft.compare(strKey) == 0)
		{
			strRes = strRight;
			break;
		}
		oldPos = curPos+strSeparator.length();
		curPos=str.find(strSeparator.c_str(),oldPos);

		//���һ�������ˣ�����û����Ҫ���ҵģ��������Ҫ�����Ϣ�ַ���
		if (curPos > str.length() || curPos < 0)
		{
			strOneItem = str.substr(oldPos);
			StringHelp(strOneItem,strLeft,strRight);
			//�޸�Ϊ��ѯ�����ؿ�-2013-01-28
			if (strLeft.compare(strKey) == 0)
			{
				strRes = strRight;
			}
			else
			{
				strRes = "";
			}
			break;
		}
		strOneItem = str.substr(oldPos,curPos-oldPos);
	};
	return strRes;
}

//���ַ����ԡ�=���ָ������2����
void StringHelp(const string& strInput,string& strLeft,string& strRight)
{
	strLeft = strRight = "";

	int iPosEq = strInput.find('=');

	if( iPosEq == -1 )
	{
		return;
	}
	strLeft		= strInput.substr(0,iPosEq);
	strRight	= strInput.substr(iPosEq+1,strInput.length()-iPosEq-1);
}

std::wstring StringToWstring(const std::string str)
{
// 	setlocale(LC_ALL, "chs"); 
// 	size_t _Dsize = str.size() + 1;
// 	wchar_t *_Dest = new wchar_t[_Dsize];
// 	wmemset(_Dest, 0, _Dsize);
// 	mbstowcs(_Dest,str.c_str(),_Dsize);
// 	std::wstring result = _Dest;
// 	delete []_Dest;
// 	_Dest = NULL;
// 	setlocale(LC_ALL, "C");

	return L"";
}

std::string WstringToString(const std::wstring str)
{
// 	std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
// 	setlocale(LC_ALL, "chs");
// 	size_t _Dsize = 2 * str.size() + 1;
// 	char *_Dest = new char[_Dsize];
// 	memset(_Dest,0,_Dsize);
// 	wcstombs(_Dest,str.c_str(),_Dsize);
// 	std::string result = _Dest;
// 	delete []_Dest;
// 	_Dest = NULL;
// 	setlocale(LC_ALL, curLocale.c_str());

	return "";
}

std::string StringToUTF8(std::string oldstr)
{
	std::string newstr;
	newstr.empty();
	DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, oldstr.c_str(), -1, NULL, 0);  
	wchar_t *pwText;  
	pwText = new wchar_t[dwNum];  
	if (!pwText)  
	{  
		delete []pwText;  
	}
	else
	{
		MultiByteToWideChar(CP_ACP, 0, oldstr.c_str(), -1, pwText, dwNum);  
		dwNum = WideCharToMultiByte(CP_UTF8,0,pwText,wcslen(pwText),NULL,0,NULL,NULL);
		char* utf =new char[dwNum+1];
		WideCharToMultiByte(CP_UTF8,0,pwText,-1,utf,dwNum,NULL,NULL);
		utf[dwNum]='\0';
		newstr= utf;
		//------�ͷſռ�
		delete []utf;
		delete []pwText;
	}
	return newstr;
}

std::string GBToUTF8(const char* str)
{
	std::string result;
	WCHAR *strSrc;
	CHAR *szRes;

	//�����ʱ�����Ĵ�С
	int i = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i+1];
	MultiByteToWideChar(CP_ACP, 0, str, -1, strSrc, i);

	//�����ʱ�����Ĵ�С
	i = WideCharToMultiByte(CP_UTF8, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new CHAR[i+1];
	int j=WideCharToMultiByte(CP_UTF8, 0, strSrc, -1, szRes, i, NULL, NULL);

	result = szRes;
	delete []strSrc;
	delete []szRes;

	return result;
}

std::string UTF8ToGB(const char* str)
{
	std::string result;
	WCHAR *strSrc;
	CHAR *szRes;

	//�����ʱ�����Ĵ�С
	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i+1];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

	//�����ʱ�����Ĵ�С
	i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new CHAR[i+1];
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

	result = szRes;
	delete []strSrc;
	delete []szRes;

	return result;
}

std::string UTF8ToString(std::string oldstr)
{
	std::string newstr;
	newstr.empty();
	DWORD dwNum = MultiByteToWideChar (CP_UTF8, 0, oldstr.c_str(), -1, NULL, 0);  
	wchar_t *pwText;  
	pwText = new wchar_t[dwNum];  
	if (!pwText)  
	{  
		delete []pwText;  
	}
	else
	{
		MultiByteToWideChar(CP_UTF8, 0, oldstr.c_str(), -1, pwText, dwNum);  
		dwNum = WideCharToMultiByte(CP_ACP,0,pwText,wcslen(pwText),NULL,0,NULL,NULL);
		char* utf =new char[dwNum+1];
		WideCharToMultiByte(CP_ACP,0,pwText,-1,utf,dwNum,NULL,NULL);
		utf[dwNum]='\0';
		newstr= utf;
		//------�ͷſռ�
		delete []utf;
		delete []pwText;
	}
	return newstr;
}

int HexPairValue(const char * code) 
{
	int value = 0;
	const char * pch = code;
	for (;;) 
	{
		int digit = *pch++;

		if (digit >= '0' && digit <= '9') 
		{
			value += digit - '0';
		}
		else if (digit >= 'A' && digit <= 'F') 
		{
			value += digit - 'A' + 10;
		}
		else if (digit >= 'a' && digit <= 'f') 
		{
			value += digit - 'a' + 10;
		}
		else 
		{
			return -1;
		}
		if (pch == code + 2)
			return value;
		value <<= 4;
	}
}

int UrlDecode(const char *source, char *dest)
{
	char * start = dest;

	while (*source) 
	{
		switch (*source)
		{
		case '+':
			*(dest++) = ' ';
			break;
		case '%':
			if (source[1] && source[2]) 
			{
				int value = HexPairValue(source + 1);

				if (value >= 0)
				{
					*(dest++) = value;
					source += 2;
				}
				else 
				{
					*dest++ = '?';
				}
			}
			else 
			{
				*dest++ = '?';
			}
			break;
		default:
			*dest++ = *source;
		}
		source++;
	}

	*dest = 0;

	return dest - start;
}

string UrlDecodeString(string strSrc) 
{
	size_t nLength = strSrc.length();

	for (const char * pChar = strSrc.c_str(); *pChar; pChar++) 
	{
		if (*pChar == '%')
		{
			nLength += 2;
		}
	}

	nLength += 10;
	char * pBuffer = new char [nLength];

	UrlDecode(strSrc.c_str(), pBuffer);

	string strResult = UTF8ToGB(pBuffer);

	delete pBuffer;

	return strResult;
}

/*
���ܣ�unsigned long����ת��Ϊstring����
*/
string ulong2str( unsigned long d )
{
	ostringstream oss;
	oss << d;
	return oss.str();
}

/*
���ܣ�long����ת��Ϊstring����
*/
string long2str( long d )
{
	ostringstream oss;
	oss << d;
	return oss.str();
}

/*
���ܣ�unsigned int����ת��Ϊstring����
*/
string uint2str( unsigned int d )
{
	ostringstream oss;
	oss << d;
	return oss.str();
}

/*
���ܣ�int����ת��Ϊstring����
*/
string int2str( int d )
{
	ostringstream oss;
	oss << d;
	//MessageBoxA(NULL,ret.c_str(),NULL,NULL);
	return oss.str();
}

/*
����:string����ת��Ϊunsigned long����
*/
unsigned long str2ulong( const string& s )
{
	if(s ==  "")
	{
		return 0;
	}

	istringstream iss(s);
	unsigned long i;
	iss >> i;
	return i;
}

/*
����:string����ת��Ϊlong����
*/
long str2long( const string& s )
{
	if(s ==  "")
	{
		return 0;
	}

	istringstream iss(s);
	long i;
	iss >> i;
	return i;
}

double str2double (const string &s)
{
	if (s == "")
		return 0.00;

	istringstream iss (s);
	double d;
	iss >> d;

	return d;
}

/*
����:string����ת��Ϊunsigned int����
*/
unsigned int str2uint( const string& s )
{
	if(s ==  "")
	{
		return 0;
	}

	istringstream iss(s);
	unsigned int i;
	iss >> i;
	return i;
}

/*
����:string����ת��Ϊint����
*/
int str2int( const string& s )
{
	if(s ==  "")
	{
		return 0;
	}

	istringstream iss(s);
	int i;
	iss >> i;
	return i;
}

bool EnsureDirExist(string strPath)
{
	if (CreateDirectoryA(strPath.c_str(),NULL) != 0) //�����ɹ�
	{
		return true;
	}
	DWORD dwErrorCode = GetLastError();
	if (dwErrorCode == ERROR_ALREADY_EXISTS) //�Ѿ�����
	{
		return true;
	}
	vector<string> vStr;
	SplitString(strPath,vStr);

	if (vStr.empty())
	{
		return false;
	}
	string strTry;
	vector<string>::iterator it ;
	for (it = vStr.begin();it != vStr.end();++it)
	{
		strTry += *it;
		if (CreateDirectoryA(strTry.c_str(),NULL) == 0) //ʧ��
		{
			DWORD dw = GetLastError();
			if (dw != ERROR_ALREADY_EXISTS) //�������ڴ���
			{
				return false;
			}
		}
		strTry += "\\";
	}
	return true;
}

void SplitString (string strSrc, vector<string> &vStr)
{
	if (!strSrc.length ())
		return;

	int nCutAt;
	while((nCutAt = strSrc.find_first_of('\\')) != strSrc.npos )
	{
		if(nCutAt > 0)
		{
			string strTmp = strSrc.substr(0,nCutAt);
			vStr.push_back(strTmp);
		}
		strSrc = strSrc.substr(nCutAt+1);
	}

	if(strSrc.length() > 0)
	{
		vStr.push_back(strSrc);
	}
}

bool GetDiskNames(vector<wstring>& vDisk)
{
	int DSLength = GetLogicalDriveStrings(0, NULL);
	//ͨ��GetLogicalDriveStrings()������ȡ�����������ַ�����Ϣ���ȡ�

	TCHAR* DStr = new TCHAR[DSLength];//�û�ȡ�ĳ����ڶ�������һ��c�����ַ�������
	GetLogicalDriveStrings(DSLength, (LPTSTR)DStr);
	//ͨ��GetLogicalDriveStrings���ַ�����Ϣ���Ƶ�����������,���б�������������������Ϣ��
	int si=0;
	for (int i = 0; i < DSLength/4; ++i)
		//DStr�ڲ������������A:\NULLB:\NULLC:\NULL����������Ϣ������DSLength/4���Ի�þ����ѭ����Χ
	{
		TCHAR dir[4]={DStr[si],':','\\','\0'};
		int DType = GetDriveType(DStr+i*4);
		//GetDriveType���������Ի�ȡ���������ͣ�����Ϊ�������ĸ�Ŀ¼
		if (DType == DRIVE_FIXED)//Ӳ��
		{
			vDisk.push_back(dir);
			//lsdebug
		}
		else if (DType == DRIVE_CDROM)//����
		{
		}
		else if (DType == DRIVE_REMOVABLE)//���ƶ�ʽ����
		{
		}
		else if (DType == DRIVE_REMOTE)//�������
		{
		}
		else if (DType == DRIVE_RAMDISK)//����RAM����
		{
		}
		else if (DType == DRIVE_UNKNOWN)//δ֪�豸
		{
		}
		si += 4;
	}
	delete []DStr;
	return true;
}

bool GetLargestFreeBytesDisk(wstring& strDisk)
{
	vector<wstring> vDisk;
	if( !GetDiskNames(vDisk) )
		return false;

	ULONGLONG i64LargestFreeBytes = 0;
	for( vector<wstring>::iterator it1 = vDisk.begin(); it1 != vDisk.end(); it1++ )
	{
		ULONGLONG i64FreeBytesToCaller, i64TotalBytes, i64FreeBytes;
		BOOL fResult = GetDiskFreeSpaceEx (
			it1->c_str(),
			//lsdebug
			(PULARGE_INTEGER)&i64FreeBytesToCaller,
			(PULARGE_INTEGER)&i64TotalBytes,
			(PULARGE_INTEGER)&i64FreeBytes);
		//GetDiskFreeSpaceEx���������Ի�ȡ���������̵Ŀռ�״̬,�������ص��Ǹ�BOOL��������
		if (fResult)//ͨ�����ص�BOOL�����ж��������Ƿ��ڹ���״̬
		{
			if( i64LargestFreeBytes < i64FreeBytes )
			{
				i64LargestFreeBytes = i64FreeBytes;
				strDisk = *it1;
			}
		}
		else
		{
			//�豸δ׼����
		}
	}

	if( i64LargestFreeBytes == 0 )
	{
		return false;
	}
	else
	{
		return true;
	}
}

void ShowInTaskbar(HWND hWnd, BOOL bshow)
{
	HRESULT hr;
	ITaskbarList *pTaskbarList;

	hr = CoCreateInstance(CLSID_TaskbarList,NULL,CLSCTX_INPROC_SERVER, IID_ITaskbarList,(void**)&pTaskbarList);

	if ( hr == S_OK )
	{
		hr = pTaskbarList->HrInit();

		if ( hr == S_OK )
		{
			if(bshow)
			{
				pTaskbarList->AddTab(hWnd);
			}
			else
			{
				pTaskbarList->DeleteTab(hWnd);
			}
		}

		pTaskbarList->Release();
	}
}

SHELLEXECUTEINFO RunApp(LPCTSTR strFileName, LPCTSTR strDir, LPCTSTR strCmdLine, UINT nShowMode, DWORD dwMilliseconds)
{
	SHELLEXECUTEINFO shei = {0} ;
	shei.cbSize = sizeof(shei) ;
	shei.hwnd = NULL ;
	shei.lpVerb = _T("open") ;
	shei.lpFile = strFileName;
	shei.lpDirectory  = strDir;
	shei.nShow = nShowMode ;
	shei.lpParameters = strCmdLine ;
	shei.fMask = SEE_MASK_NOCLOSEPROCESS;

	BOOL bRtn = ::ShellExecuteEx(&shei);
	HANDLE hWnd = shei.hProcess;

	if (bRtn
		&& hWnd != NULL
		&& dwMilliseconds > 0)
	{
		::WaitForSingleObject(hWnd, dwMilliseconds);
	}

	return shei;
}