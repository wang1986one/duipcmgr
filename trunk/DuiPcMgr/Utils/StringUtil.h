#ifndef __StringUtil_H__
#define __StringUtil_H__
#pragma once

#include <vector>

namespace Utils
{
	//不同格式字符串之间的转换函数
	CStringW		MbcsToUnicode(LPCSTR pMbcs);
	CStringA	    MbcsToUtf8(LPCSTR pMbcs);
	CStringA		UnicodeToMbcs(LPCWSTR pUnicode);
	CStringA		UnicodeToUtf8(LPCWSTR pUnicode);

	CStringA	    Utf8ToMbcs(LPCSTR pUtf8);
	CStringW		Utf8ToUnicode(LPCSTR pUtf8);
#ifdef _UNICODE
	CStringW		Utf8ToTCHAR(LPCSTR pUtf8);
	CStringA		TCHARToUtf8(LPCWSTR pUnicode);
#else
	CStringA		Utf8ToTCHAR(LPCSTR pUtf8);
	CStringA		TCHARToUtf8(LPCSTR pMbcs);
#endif // _UNICODE

	typedef std::vector<CString> VString;
	VString SplitStr(LPCTSTR pStr, TCHAR ch=_T('\x20'));

	//字符串到数字的转换函数
	bool		StringToInt(const char* pValue, int & nValue);
	bool		StringToDWord(const char* pValue, unsigned int & dwValue);
	bool		StringToInt64(const char* pValue, long long & i64Value);
	bool		StringToUnsignedInt64(const char* pValue, unsigned long long & ui64Value);


	///URL编码解码函数
	char*		FormUrlEncode(const char* pEncode);
	char*		FormUrlDecode(const char* pDecode);
	inline int	UrlEncode(const char *source, char *dest, unsigned max);
}


#endif // __StringUtil_H__

