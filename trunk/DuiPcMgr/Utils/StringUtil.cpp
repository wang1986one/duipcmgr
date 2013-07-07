#include "stdafx.h"
#include "StringUtil.h"
#include <Windows.h>
#include <assert.h>

CStringW Utils::Utf8ToUnicode(LPCSTR pUtf8)
{
	if (pUtf8==NULL)
		return L"";

	UINT nLength=strlen(pUtf8);
	int nChar = MultiByteToWideChar(CP_UTF8, 0, pUtf8, nLength, NULL, 0);
	CStringW tempBuffer;

	nChar = MultiByteToWideChar(CP_UTF8, 0, pUtf8, nLength, (LPWSTR)tempBuffer.GetBufferSetLength(nChar), nChar);
	if(nChar == 0)
	{
		return L"";
	}

	return tempBuffer;
}


CStringA Utils::UnicodeToUtf8(LPCWSTR pUnicode)
{
	if (pUnicode==NULL)
		return "";

	UINT nLength=wcslen(pUnicode);
	int nChar = WideCharToMultiByte(CP_UTF8, 0, pUnicode, nLength, 0, 0, 0, 0);

	CStringA tempBuffer;
	
	nChar = WideCharToMultiByte(CP_UTF8, 0, pUnicode, nLength, (LPSTR)tempBuffer.GetBufferSetLength(nChar), nChar, 0, 0);
	if( nChar == 0 )
	{
		return "";
	}

	return tempBuffer;
}

CStringW Utils::MbcsToUnicode(LPCSTR pMbcs)
{
	if(pMbcs==NULL)
		return L"";

	int codepage = AreFileApisANSI() ? CP_ACP : CP_OEMCP;
	UINT nLength=strlen(pMbcs);
	int nChar = MultiByteToWideChar(codepage, 0, pMbcs, nLength, NULL,0);

	CStringW tempBuffer;

	nChar = MultiByteToWideChar(codepage, 0, pMbcs, nLength, (LPWSTR)tempBuffer.GetBufferSetLength(nChar), nChar);
	if(nChar == 0)
	{
		return L"";
	}

	return tempBuffer;
}

CStringA	Utils::UnicodeToMbcs(LPCWSTR pUnicode)
{
	if (pUnicode==NULL)
		return "";

	int codepage = AreFileApisANSI() ? CP_ACP : CP_OEMCP;
	UINT nLength=wcslen(pUnicode);
	int nChar = WideCharToMultiByte(codepage, 0, pUnicode, nLength, 0, 0, 0, 0);

	CStringA tempBuffer;

	nChar = WideCharToMultiByte(codepage, 0, pUnicode, nLength, tempBuffer.GetBufferSetLength(nChar), nChar, 0, 0);
	if( nChar == 0 )
	{
		return "";
	}

	return tempBuffer;
}

CStringA Utils::MbcsToUtf8(LPCSTR pMbcs)
{
   if (pMbcs==NULL)
	   return "";
   return	UnicodeToUtf8(MbcsToUnicode(pMbcs));
}

CStringA Utils::Utf8ToMbcs(LPCSTR pUtf8)
{
	if(pUtf8==NULL)
		return "";
	return	UnicodeToMbcs(Utf8ToUnicode(pUtf8));
}
#ifdef _UNICODE

CStringW Utils::Utf8ToTCHAR( LPCSTR pUtf8 )
{
	if(pUtf8==NULL)
		return _T("");

	return Utf8ToUnicode(pUtf8);
}

CStringA Utils::TCHARToUtf8( LPCWSTR pUnicode )
{
	if(pUnicode==NULL)
		return _T("");

	return UnicodeToUtf8(pUnicode);
}

#else
CStringA Utils::Utf8ToTCHAR( LPCSTR pUtf8 )
{
	if(pUtf8==NULL)
		return _T("");

	return Utf8ToMbcs(pUtf8);
}

CStringA Utils::TCHARToUtf8( LPCSTR pMbcs )
{
	if(pMbcs==NULL)
		return _T("");

	return MbcsToUtf8(pMbcs);
}

#endif

bool Utils::StringToInt(const char* strValue, int & iValue)
{
	iValue = 0;
	bool br = true;
	if (!strValue || !*strValue)
	{
		return false;
	}
	while (isspace((int)*strValue))
	{
		strValue ++;
	}
	if (*strValue == '+' || *strValue == '-')
	{
		br = (*strValue++ == '+');
	}
	bool bOK = true;
	if (strValue[0] == '0' && (strValue[1]|0x20)=='x')
	{
		strValue += 2;
		for (;;)
		{
			TCHAR ch = *strValue;
			int iValue2 = 0;
			if (ch >= '0' && ch <= '9')	iValue2 = iValue*16 + ch -'0';
			else if (ch>='a' && ch<='f') iValue2 = iValue*16 + ch -'a'+10;
			else if (ch>='A' && ch<='F') iValue2 = iValue*16 + ch -'A'+10;
			else break;
			if (iValue2 < 0 || iValue >= 134217728)	bOK = false;
			iValue = iValue2;
			++strValue;
		}
	}
	else
	{
		while (*strValue >= '0' && *strValue <= '9')
		{
			int iValue2 = iValue * 10 + *strValue++ -'0';
			if (iValue2 < 0 || iValue > 214748364) bOK = false;
			iValue = iValue2;
		}
	}
	if (!br) iValue = -iValue;
	while (*strValue && isspace((BYTE)*strValue)) ++strValue;
	return bOK && strValue[0] == 0;
}

bool Utils::StringToDWord(const char* strValue, unsigned int & dwValue)
{
	dwValue = 0;
	if (!strValue || !*strValue)
	{
		return false;
	}
	while (isspace((int)*strValue))
	{
		strValue ++;
	}
	if (*strValue == '+')
	{
		strValue ++;
	}
	bool bOK = true;
	if (strValue[0] == '0' && (strValue[1]|0x20)=='x')
	{
		strValue += 2;
		for (;;)
		{
			TCHAR ch = *strValue;
			unsigned int dwValue2 = 0;
			if (ch >= '0' && ch <= '9')	dwValue2 = dwValue*16 + ch -'0';
			else if (ch>='a' && ch<='f') dwValue2 = dwValue*16 + ch -'a'+10;
			else if (ch>='A' && ch<='F') dwValue2 = dwValue*16 + ch -'A'+10;
			else break;
			if (dwValue2 < dwValue || dwValue >= 268435456) bOK = false;
			dwValue = dwValue2;
			++strValue;
		}
	}
	else
	{
		while (*strValue >= '0' && *strValue <= '9')
		{
			unsigned int dwValue2 = dwValue * 10 + *strValue++ -'0';
			if (dwValue2 < dwValue || dwValue > 429496729) bOK = false;
			dwValue = dwValue2;
		}
	}
	while (*strValue && isspace((BYTE)*strValue)) ++strValue;
	return bOK && strValue[0] == 0;
}

bool Utils::StringToInt64(const char* strValue, long long & i64Value)
{
	i64Value = 0;
	bool br = true;
	if (!strValue || !*strValue)
	{
		return false;
	}
	while (isspace((int)*strValue))
	{
		strValue ++;
	}
	if (*strValue == '+' || *strValue == '-')
	{
		br = (*strValue++ == '+');
	}
	bool bOK = true;
	if (strValue[0] == '0' && (strValue[1]|0x20)=='x')
	{
		strValue += 2;
		for (;;)
		{
			TCHAR ch = *strValue;
			long long i64Value2 = 0;
			if (ch >= '0' && ch <= '9')	i64Value2 = i64Value*16 + ch -'0';
			else if (ch>='a' && ch<='f') i64Value2 = i64Value*16 + ch -'a'+10;
			else if (ch>='A' && ch<='F') i64Value2 = i64Value*16 + ch -'A'+10;
			else break;
			if (i64Value2 < 0 || i64Value >= 576460752303423488ULL) bOK = false;
			i64Value = i64Value2;
			++strValue;
		}
	}
	else
	{
		while (*strValue >= '0' && *strValue <= '9')
		{
			long long i64Value2 = i64Value * 10 + *strValue++ -'0';
			if (i64Value2 < 0 || i64Value > 922337203685477580ULL) bOK = false;

			i64Value = i64Value2;
		}
	}
	if (!br) i64Value = -i64Value;
	while (*strValue && isspace((BYTE)*strValue)) ++strValue;
	return bOK && strValue[0] == 0;
}

bool Utils::StringToUnsignedInt64(const char* strValue, unsigned long long & ui64Value)
{
	ui64Value = 0;
	if (!strValue || !*strValue)
	{
		return false;
	}
	while (isspace((int)*strValue))
	{
		strValue ++;
	}
	if (*strValue == '+')
	{
		strValue ++;
	}
	bool bOK = true;
	if (strValue[0] == '0' && (strValue[1]|0x20)=='x')
	{
		strValue += 2;
		for (;;)
		{
			TCHAR ch = *strValue;
			unsigned long long ui64Value2 = 0;
			if (ch >= '0' && ch <= '9')	ui64Value2 = ui64Value*16 + ch -'0';
			else if (ch>='a' && ch<='f') ui64Value2 = ui64Value*16 + ch -'a'+10;
			else if (ch>='A' && ch<='F') ui64Value2 = ui64Value*16 + ch -'A'+10;
			else break;
			if (ui64Value2 < ui64Value || ui64Value >= 1152921504606846976ULL) bOK = false;
			ui64Value = ui64Value2;
			++strValue;
		}
	}
	else
	{
		while (*strValue >= '0' && *strValue <= '9')
		{
			unsigned long long ui64Value2 = ui64Value * 10 + *strValue++ -'0';
			if (ui64Value2 < ui64Value || ui64Value > 1844674407370955161ULL) bOK = false;
			ui64Value = ui64Value2;
		}
	}
	while (*strValue && isspace((BYTE)*strValue)) ++strValue;
	return bOK && strValue[0] == 0;
}


inline int Utils::UrlEncode(const char *source, char *dest, unsigned max)
{
	static const char *digits = "0123456789ABCDEF";
	unsigned char ch;
	unsigned len = 0;
	char *start = dest;

	while (len < max - 4 && *source)
	{
		ch = (unsigned char)*source;
		if (*source == '&') {
			*dest++ = '%';
			*dest++ = digits[(ch >> 4) & 0x0F];
			*dest++ = digits[ch & 0x0F];
		}
		else
		{
			*dest++ = *source;
		}
		source++;
	}
	*dest = 0;
	return start - dest;
}

char* Utils::FormUrlEncode(const char* pEncode)
{
	size_t needed_length = strlen(pEncode) * 3 + 3;
	char stackalloc[64];
	char * buf = needed_length > sizeof(stackalloc)/sizeof(*stackalloc) ?
		(char *)malloc(needed_length) : stackalloc;
	UrlEncode(pEncode, buf, needed_length);
	std::string result(buf);
	if (buf != stackalloc) {
		free(buf);
	}

	char * szBuffer = new char[result.length() + 1];
	strncpy_s(szBuffer,result.length()+1,result.c_str(),result.length());
	return szBuffer;
}

char* Utils::FormUrlDecode(const char* pDecode)
{
	std::string buffer = "";
	int len = strlen(pDecode);

	for (int i = 0; i < len; i++)
	{
		int j = i ;
		char ch = pDecode[j];
		if(ch =='+')
		{
			buffer+=' ';
		}
		else if (ch == '%')
		{
			char tmpstr[] = "0x0__";
			int chnum;
			tmpstr[3] = pDecode[j+1];
			tmpstr[4] = pDecode[j+2];
			chnum = strtol(tmpstr, NULL, 16);
			buffer += chnum;
			i += 2;
		}
		else
		{
			buffer += ch;
		}
	}

	char * szBuffer = new char[buffer.length() + 1];
	strncpy_s(szBuffer,buffer.length()+1,buffer.c_str(),buffer.length());
	return szBuffer;
}

Utils::VString Utils::SplitStr( LPCTSTR pStr, TCHAR ch/*=_T('\x20')*/ )
{
	CString tmp;
	VString vRet;
	if (!pStr)
	{
		return vRet;
	}
	LPCTSTR pos0 = pStr;
	LPCTSTR pos1 = NULL;
	while(TRUE)
	{
		pos1=_tcschr(pos0, ch);
		if (pos1!=NULL)
		{
			_tcsncpy_s(tmp.GetBufferSetLength(pos1-pos0), pos1-pos0+1 ,pos0, pos1-pos0);
			vRet.push_back(tmp);
			pos0 = ++pos1;
		}
		else
		{
			if (*pos0)
			{
				int len = _tcslen(pStr)-(pos0-pStr);
				_tcsncpy_s(tmp.GetBufferSetLength(len), len+1, pos0, len);
				vRet.push_back(tmp);
			}
			break;
		}
	}//end of While(TRUE)
	return vRet;
}

