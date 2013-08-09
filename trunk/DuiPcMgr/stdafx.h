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
// ��־��
#define LOG4CPLUS_STATIC
#include "Utils/Log4cplus.h"
#pragma comment(lib,"ws2_32.lib")
using namespace log4cplus;
using namespace log4cplus::helpers;
extern Utils::CLogger g_logger;
#if _MSC_VER >= 1600
#ifdef _DEBUG
#       pragma comment(lib, "log4cplusS_UD_VS2012.lib")
#else
#       pragma comment(lib, "log4cplusS_U_VS2012.lib")
#endif
#else
#ifdef _DEBUG
#       pragma comment(lib, "log4cplusUSD.lib")
#else
#       pragma comment(lib, "log4cplusUS.lib")
#endif
#endif

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
// Utils��
#include "Utils/Utils.h"

