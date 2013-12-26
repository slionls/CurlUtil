#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <Windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <shellapi.h>

std::string	GetValue( std::string& strContent, std::string strKey, std::string strSeparator = "&");
void StringHelp(const std::string& strInput,std::string& strLeft,std::string& strRight);
std::wstring StringToWstring(const std::string str);
std::string WstringToString(const std::wstring str);
std::string StringToUTF8(std::string oldstr);
std::string UTF8ToString(std::string oldstr);
std::string GBToUTF8(const char* str);
std::string UTF8ToGB(const char* str);
int HexPairValue(const char * code);
int UrlDecode(const char *source, char *dest);
std::string UrlDecodeString(std::string strSrc);
//string����ת��Ϊunsigned long����
unsigned long str2ulong( const std::string& s );
//string����ת��Ϊlong����
long str2long( const std::string& s );

double str2double (const std::string &s);

//string����ת��Ϊunsigned int����
unsigned int str2uint( const std::string& s );
//string����ת��Ϊint����
int str2int( const std::string& s );
//unsigned long����ת��Ϊstring����
std::string ulong2str( unsigned long d );
//long����ת��Ϊstring����
std::string long2str( long d );
//unsigned int����ת��Ϊstring����
std::string int2str( int d );

bool EnsureDirExist(std::string strPath);
void SplitString (std::string strSrc, std::vector<std::string> &vStr);

//******************************************************************************//
//																				//
//	���ұ�������Ӳ��    														//
//																				//
//  ����:																		//
//		vDisk	:	�ҵ���Ӳ���̷�											//
//																				//
//		true	:	�����ɹ�													//
//		false	:	����ʧ��													//
//******************************************************************************//
bool GetDiskNames(std::vector <std::wstring> &vDisk);

//��ȡʣ��ռ������̷�
bool GetLargestFreeBytesDisk(std::wstring &strDisk);

void ShowInTaskbar(HWND hWnd, BOOL bshow);

SHELLEXECUTEINFO RunApp(LPCTSTR strFileName, LPCTSTR strDir, LPCTSTR strCmdLine = _T(""), UINT nShowMode = SW_SHOWNORMAL, DWORD dwMilliseconds = 0);

#endif