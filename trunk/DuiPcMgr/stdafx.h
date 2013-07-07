#pragma once

//////////////////////////////////////////////////////////////////////////
// ���뻷����VS 2008 SP1 ����
// ���л�����XP SP3 ����
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#ifndef _WIN32_IE
#define _WIN32_IE _WIN32_IE_IE60SP2
#endif

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_DEPRECATE

#include <windows.h>
#include <atlstr.h>
#include <tchar.h>
#include <string>

//////////////////////////////////////////////////////////////////////////
// VLD�ڴ�й©���
#include "VLD/vld.h"

//////////////////////////////////////////////////////////////////////////
// GUI��
#include "DuiLib/UIlib.h"
using namespace DuiLib;

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_ud.lib")
#   else
#       pragma comment(lib, "DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_u.lib")
#   else
#       pragma comment(lib, "DuiLib.lib")
#   endif
#endif

//////////////////////////////////////////////////////////////////////////
// ZThread ���߳̿�
//#include "ZThread/ZThread.h"		// ZThread Header
#include "ZThread/Runnable.h"		// ����ִ�нӿ�
#include "ZThread/Cancelable.h"		// ����ȡ���ӿ�
#include "ZThread/Thread.h"			// �߳���
#include "ZThread/ThreadedExecutor.h"	// ����ִ������һ������һ���߳�
#include "ZThread/CountedPtr.h"		// ���ü���
#include "ZThread/Guard.h"				// ������
#include "ZThread/Singleton.h"
#include "ZThread/Condition.h"

#ifdef _DEBUG
#pragma comment(lib, "ZThreadLib_d.lib")
#else
#pragma comment(lib, "ZThreadLib.lib")
#endif

extern ZThread::ThreadedExecutor g_ThreadExecutor;

//////////////////////////////////////////////////////////////////////////
// Utils��
#include "Utils/Utils.h"

#ifdef _DEBUG 
void _Trace(LPCTSTR lpszFmt, ...);
#define _TRACE _Trace  
#else  
#define _TRACE  
#endif 

