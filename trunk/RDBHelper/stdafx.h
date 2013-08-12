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
extern Utils::CLogger g_Logger;
using namespace log4cplus;
using namespace log4cplus::helpers;
#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "log4cplusUSD.lib")
#   else
#       //pragma comment(lib, "DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "log4cplusUS.lib")
#   else
#       //pragma comment(lib, "DuiLib.lib")
#   endif
#endif

//////////////////////////////////////////////////////////////////////////
// GUI��
#include "../DuiLib/UIlib.h"
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
#include "Utils/System/SystemInfo.h"
#include "Utils/StringUtil.h"

