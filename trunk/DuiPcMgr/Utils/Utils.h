#ifndef Utils_h__
#define Utils_h__
#pragma once

#define WIN32_LEAN_AND_MEAN	
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NON_CONFORMING_SWPRINTFS
#define _ATL_NO_COM_SUPPORT
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <WindowsX.h>
#include <objbase.h>
#include <atlstr.h>
#include <tchar.h>
#include <string>
#include <vector>

#define  THREADSAFE
#define  SQLITE_SECURE_DELETE
#define  SQLITE_SOUNDEX
#define  SQLITE_ENABLE_COLUMN_METADATA

#define LOINT32(i)				((int)((__int64)(i) & 0xffffffff))
#define HIINT32(i)				((int)((__int64)(i) >> 32))
#define SAFE_CLOSE_HANDLE(h) if(h){CloseHandle(h);h=NULL;}
#define SAFE_CLOSE_INTERNETHANDLE(h) if(h){InternetCloseHandle(h);h=NULL;}
#define SIZE_OF_ARRAY(x) sizeof(x)/sizeof(x[0])
#define SAFE_DELETE(x) if(x){delete x;x=NULL;}
#define SAFE_DELETE_ARRAY(x) if(x){delete []x;x=NULL;}

typedef int int32;
typedef unsigned int uint32;
typedef __int64 int64;
typedef unsigned __int64 uint64;

#include "StringUtil.h"
#include "System/SystemInfo.h"

#ifdef _DEBUG
	#ifndef TRACE
		#define TRACE Utils__Trace
	#endif
#else
	#ifndef TRACE
		#define TRACE
	#	endif
#endif

namespace Utils
{
	typedef std::vector<CString> VString;

	class IDataStream
	{
	public:
		virtual ~IDataStream() {};
		virtual BOOL Create();
		virtual BOOL	 SetLength(UINT iLength);
		virtual BOOL Read(LPVOID lpBuffer,DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead);
		virtual BOOL Write(UINT iPosition, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten);
		virtual BOOL Write(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten);
		virtual VOID Close();
		virtual UINT GetLength();
	};

	class NonCopyable {

		//! Restrict the copy constructor
		NonCopyable(const NonCopyable&);

		//! Restrict the assignment operator
		const NonCopyable& operator=(const NonCopyable&);

	protected:

		//! Create a NonCopyable object
		NonCopyable() { }

		//! Destroy a NonCopyable object
		~NonCopyable() { }

	}; /* NonCopyable */
}

extern void Utils__Trace( LPCTSTR pstrFormat, ... );

inline int _szttoi(LPCTSTR _Str)
{
	return ::StrToInt(_Str);
}

extern CString GetLastErrorString(DWORD dwLastError);

//////////////////////////////////////////////////////////////////////////
// 全局对象
extern Utils::CSystemInfo* Utils::AppGetSystemInfo();	// 读取系统信息
class CWinOSVer;
#endif // Utils_h__

