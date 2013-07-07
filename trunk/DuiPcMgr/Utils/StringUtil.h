#ifndef __StringUtil_H__
#define __StringUtil_H__
#pragma once

#include <vector>

namespace Utils
{
	//��ͬ��ʽ�ַ���֮���ת������
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

	//�ַ��������ֵ�ת������
	bool		StringToInt(const char* pValue, int & nValue);
	bool		StringToDWord(const char* pValue, unsigned int & dwValue);
	bool		StringToInt64(const char* pValue, long long & i64Value);
	bool		StringToUnsignedInt64(const char* pValue, unsigned long long & ui64Value);


	///URL������뺯��
	char*		FormUrlEncode(const char* pEncode);
	char*		FormUrlDecode(const char* pDecode);
	inline int	UrlEncode(const char *source, char *dest, unsigned max);
}


#endif // __StringUtil_H__

