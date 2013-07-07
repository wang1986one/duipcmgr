#pragma once

//////////////////////////////////////////////////////////////////////////
// 编译环境：VS 2008 SP1 以上
// 运行环境：XP SP3 以上
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
// VLD内存泄漏检测
#include "VLD/vld.h"

//////////////////////////////////////////////////////////////////////////
// GUI库
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
// ZThread 多线程库
//#include "ZThread/ZThread.h"		// ZThread Header
#include "ZThread/Runnable.h"		// 任务执行接口
#include "ZThread/Cancelable.h"		// 任务取消接口
#include "ZThread/Thread.h"			// 线程类
#include "ZThread/ThreadedExecutor.h"	// 任务执行器，一个任务一个线程
#include "ZThread/CountedPtr.h"		// 引用计数
#include "ZThread/Guard.h"				// 保护锁
#include "ZThread/Singleton.h"
#include "ZThread/Condition.h"

#ifdef _DEBUG
#pragma comment(lib, "ZThreadLib_d.lib")
#else
#pragma comment(lib, "ZThreadLib.lib")
#endif

extern ZThread::ThreadedExecutor g_ThreadExecutor;

//////////////////////////////////////////////////////////////////////////
// Utils库
#include "Utils/Utils.h"

#ifdef _DEBUG 
void _Trace(LPCTSTR lpszFmt, ...);
#define _TRACE _Trace  
#else  
#define _TRACE  
#endif 

