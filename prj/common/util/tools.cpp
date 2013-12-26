#include "tools.h"
#include <sstream>
#include <iostream>
#include <Windows.h>
#include <locale.h>
#include <Shobjidl.h>

using namespace std;

//根据strKey，获得等于strKey的值
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

		//最后一个查找了，后面没有了要查找的，但有这个要查的信息字符串
		if (curPos > str.length() || curPos < 0)
		{
			strOneItem = str.substr(oldPos);
			StringHelp(strOneItem,strLeft,strRight);
			//修改为查询到返回空-2013-01-28
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

//将字符串以“=”分割成左右2部分
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
		//------释放空间
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

	//获得临时变量的大小
	int i = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i+1];
	MultiByteToWideChar(CP_ACP, 0, str, -1, strSrc, i);

	//获得临时变量的大小
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

	//获得临时变量的大小
	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i+1];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

	//获得临时变量的大小
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
		//------释放空间
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
功能：unsigned long类型转化为string类型
*/
string ulong2str( unsigned long d )
{
	ostringstream oss;
	oss << d;
	return oss.str();
}

/*
功能：long类型转化为string类型
*/
string long2str( long d )
{
	ostringstream oss;
	oss << d;
	return oss.str();
}

/*
功能：unsigned int类型转化为string类型
*/
string uint2str( unsigned int d )
{
	ostringstream oss;
	oss << d;
	return oss.str();
}

/*
功能：int类型转化为string类型
*/
string int2str( int d )
{
	ostringstream oss;
	oss << d;
	//MessageBoxA(NULL,ret.c_str(),NULL,NULL);
	return oss.str();
}

/*
功能:string类型转化为unsigned long类型
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
功能:string类型转化为long类型
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
功能:string类型转化为unsigned int类型
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
功能:string类型转化为int类型
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
	if (CreateDirectoryA(strPath.c_str(),NULL) != 0) //创建成功
	{
		return true;
	}
	DWORD dwErrorCode = GetLastError();
	if (dwErrorCode == ERROR_ALREADY_EXISTS) //已经存在
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
		if (CreateDirectoryA(strTry.c_str(),NULL) == 0) //失败
		{
			DWORD dw = GetLastError();
			if (dw != ERROR_ALREADY_EXISTS) //不是由于存在
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
	//通过GetLogicalDriveStrings()函数获取所有驱动器字符串信息长度。

	TCHAR* DStr = new TCHAR[DSLength];//用获取的长度在堆区创建一个c风格的字符串数组
	GetLogicalDriveStrings(DSLength, (LPTSTR)DStr);
	//通过GetLogicalDriveStrings将字符串信息复制到堆区数组中,其中保存了所有驱动器的信息。
	int si=0;
	for (int i = 0; i < DSLength/4; ++i)
		//DStr内部保存的数据是A:\NULLB:\NULLC:\NULL，这样的信息，所以DSLength/4可以获得具体大循环范围
	{
		TCHAR dir[4]={DStr[si],':','\\','\0'};
		int DType = GetDriveType(DStr+i*4);
		//GetDriveType函数，可以获取驱动器类型，参数为驱动器的根目录
		if (DType == DRIVE_FIXED)//硬盘
		{
			vDisk.push_back(dir);
			//lsdebug
		}
		else if (DType == DRIVE_CDROM)//光驱
		{
		}
		else if (DType == DRIVE_REMOVABLE)//可移动式磁盘
		{
		}
		else if (DType == DRIVE_REMOTE)//网络磁盘
		{
		}
		else if (DType == DRIVE_RAMDISK)//虚拟RAM磁盘
		{
		}
		else if (DType == DRIVE_UNKNOWN)//未知设备
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
		//GetDiskFreeSpaceEx函数，可以获取驱动器磁盘的空间状态,函数返回的是个BOOL类型数据
		if (fResult)//通过返回的BOOL数据判断驱动器是否在工作状态
		{
			if( i64LargestFreeBytes < i64FreeBytes )
			{
				i64LargestFreeBytes = i64FreeBytes;
				strDisk = *it1;
			}
		}
		else
		{
			//设备未准备好
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