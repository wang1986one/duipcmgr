#include "stdafx.h"

void _Trace(LPCTSTR lpszFmt, ...)
{
	va_list args;
	va_start(args, lpszFmt);
	int len = _vsctprintf(lpszFmt, args)+1;
	TCHAR *lpszBuf = (TCHAR*)_alloca(len*sizeof(TCHAR));//ջ�з���, ����Ҫ�ͷ�
	_vstprintf_s(lpszBuf, len, lpszFmt, args);
	va_end(args);
	OutputDebugString(lpszBuf);
}

