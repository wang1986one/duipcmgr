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

	//<By张方雪 2013-5-28>这个函数能够把程序pin到任务栏.
	BOOL	WINAPI	Pin2Taskbar(LPCTSTR lpFullPath, bool bPin=true);

}

#endif