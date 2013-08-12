#include "StdAfx.h"
#include "Utils.h"

extern void Utils__Trace( LPCTSTR pstrFormat, ... )
{
	va_list arg;
	va_start(arg , pstrFormat);
	static CString strLog;
	wvsprintf(strLog.GetBufferSetLength(_vsctprintf(pstrFormat, arg)) ,  pstrFormat , arg);
	va_end(arg);

	if (strLog.GetLength()>4096)
	{
		strLog.Truncate(4095);
	}
	strLog.AppendChar(_T('\n'));
	OutputDebugString(strLog);
}

extern CString GetLastErrorString( DWORD dwLastError )
{
	LPVOID lpMsgBuf;
	::FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		dwLastError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		NULL
		);
	CString strTemp  = lpMsgBuf  ? (LPCTSTR)lpMsgBuf : _T( " 没有获得有效信息。" );
	::LocalFree(lpMsgBuf);
	return strTemp;
}

