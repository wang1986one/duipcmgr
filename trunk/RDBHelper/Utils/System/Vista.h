#ifndef _VISTA_INCLUDE_
#define _VISTA_INCLUDE_
#pragma once
#include <ShlDisp.h>
namespace Utils
{

	BOOL	WINAPI	IsVistaLater();
	BOOL	WINAPI	IsOpenUAC();
	BOOL	WINAPI	AllowMeesageForVistaAbove(UINT uMessage, BOOL bAllow=TRUE);
	BOOL	WINAPI	EnablePrivilege( LPCTSTR szPrivName );

	//<By�ŷ�ѩ 2013-5-28>��������ܹ��ѳ���pin��������.
	BOOL	WINAPI	Pin2Taskbar(LPCTSTR lpFullPath, bool bPin=true);

}

#endif