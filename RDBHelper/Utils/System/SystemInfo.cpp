#include "StdAfx.h"
#include "SystemInfo.h"
#include <ShlObj.h>

#pragma comment(lib,"ShFolder.Lib")

namespace Utils
{

	CSystemInfo* AppGetSystemInfo()
	{
		static CSystemInfo	state;
		return &state;
	}

	CSystemInfo::CSystemInfo(void)
	{
		InitModule( (HMODULE)&__ImageBase );
	}

	CSystemInfo::~CSystemInfo(void)
	{
	}

	VOID CSystemInfo::InitModule( HMODULE hMod )
	{
		m_hMod = hMod;

		OSVERSIONINFO	osver = {0};
		osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx(&osver);
		m_dwMajorVer	        = osver.dwMajorVersion;
		m_dwMinVer			= osver.dwMinorVersion;
		m_dwBuildNumber	= osver.dwBuildNumber;
		m_dwPlatformId		= osver.dwPlatformId;

		GetIsWow64();
		GetIsWow64Process();
		GetModulePath();
		GetModuleDir();
		GetProgramDir();
		GetSystem32Dir();
		GetWindowsDir();
		GetProgramDir();
		GetCommonAppDataDir();
		GetAppDataDir();
	}

	LPCTSTR CSystemInfo::GetModulePath()
	{
		static	TCHAR	szPath[MAX_PATH] = {0};
		if ( szPath[0] == 0 )
		{
			GetModuleFileName(m_hMod,szPath, MAX_PATH);
		}
		return szPath;
	}

	LPCTSTR CSystemInfo::GetModuleDir()
	{
		static	TCHAR	szPath[MAX_PATH] = {0};
		if ( szPath[0] == 0 )
		{
			GetModuleFileName(m_hMod,szPath, MAX_PATH);
			PathRemoveFileSpec(szPath);
		}
		return szPath;
	}

	LPCTSTR CSystemInfo::GetWindowsDir()
	{
		static	TCHAR	szPath[MAX_PATH] = {0};
		if ( szPath[0] == 0 )
		{
			GetSystemWindowsDirectory(szPath, MAX_PATH);
			//	GetWindowsDirectory(szPath, MAX_PATH);
		}
		return szPath;
	}

	LPCTSTR CSystemInfo::GetSystem32Dir()
	{
		static	TCHAR	szPath[MAX_PATH] = {0};
		if ( szPath[0] == 0 )
		{
			GetSystemDirectory(szPath, MAX_PATH);
		}
		return szPath;
	}

	LPCTSTR CSystemInfo::GetProgramDir()
	{
		static TCHAR szDir[MAX_PATH] = {0};

		if ( szDir[0] == 0 )
		{
			DWORD dwType = REG_SZ;
			DWORD dwSize = sizeof(szDir);
			if( ERROR_SUCCESS == SHGetValue( HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion"), 
				_T("ProgramFilesDir"), &dwType, szDir, &dwSize ) && dwSize > 1 )
			{
				FixPathLastSpec(szDir);
				return szDir ;
			}

			if( 0 != GetEnvironmentVariable( _T("ProgramFiles"), szDir, dwSize ) )
			{
				FixPathLastSpec(szDir);
				return szDir ;
			}	
		}
		return szDir;
	}

	BOOL CSystemInfo::FixPathLastSpec( LPTSTR lpPath )
	{
		if ( lpPath == NULL )
			return FALSE;

		size_t	nLen = _tcslen(lpPath);
		if ( nLen > 1 )
		{
			if ( lpPath[nLen-1] == _T('\\') )
				lpPath[nLen-1] = 0;

			return TRUE;
		}
		return FALSE;
	}

	LPCTSTR CSystemInfo::GetTempDir()
	{
		static	TCHAR	szPath[MAX_PATH] = {0};
		if ( szPath[0] == 0 )
		{
			GetTempPath(MAX_PATH,szPath);
			FixPathLastSpec(szPath);
		}
		return szPath;
	}

	LPCTSTR CSystemInfo::GetCommonAppDataDir()
	{
		static	TCHAR	szPath[MAX_PATH] = {0};
		if ( szPath[0] == 0 )
		{
			SHGetSpecialFolderPath( NULL, szPath, CSIDL_COMMON_APPDATA, FALSE );
			FixPathLastSpec(szPath);
		}
		return szPath;
	}

	LPCTSTR CSystemInfo::GetAppDataDir()
	{
		static	TCHAR	szPath[MAX_PATH] = {0};
		if ( szPath[0] == 0 )
		{
			SHGetSpecialFolderPath( NULL, szPath, CSIDL_APPDATA, FALSE );
			FixPathLastSpec(szPath);
		}
		return szPath;
	}

	LPCTSTR CSystemInfo::GetRundll32Path()
	{
		static	TCHAR	szPath[MAX_PATH] = {0};
		if ( szPath[0] == 0 )
		{
			LPCTSTR	lpWinPath = GetSystem32Dir();
			_sntprintf_s( szPath, MAX_PATH, _T("%s\\Rundll32.exe"), lpWinPath);
		}
		return szPath;
	}


	LPCTSTR CSystemInfo::GetRegSrvPath()
	{
		static	TCHAR	szPath[MAX_PATH] = {0};
		if ( szPath[0] == 0 )
		{
			LPCTSTR	lpWinPath = GetSystem32Dir();
			_sntprintf_s( szPath, MAX_PATH, _T("%s\\regsvr32.exe"), lpWinPath);
		}
		return szPath;
	}

	VOID CSystemInfo::GetIsWow64()
	{
		m_bIsWOW64=FALSE;

		typedef VOID(__stdcall* PFN_GetNativeSystemInfo)(LPSYSTEM_INFO lpSystemInfo);
		PFN_GetNativeSystemInfo pFN_GetNativeSystemInfo;

		pFN_GetNativeSystemInfo = (PFN_GetNativeSystemInfo) ::GetProcAddress(GetModuleHandle(_T("kernel32.dll")), "GetNativeSystemInfo");
		if(pFN_GetNativeSystemInfo)
		{
			SYSTEM_INFO si;
			pFN_GetNativeSystemInfo(&si);
			if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
				si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 )
			{
				m_bIsWOW64=TRUE;
				return;
			}
		}
	}

	LPCTSTR CSystemInfo::GetExplorerPath()
	{
		static	TCHAR	szPath[MAX_PATH] = {0};
		if ( szPath[0] == 0 )
		{
			LPCTSTR	lpWinPath = GetWindowsDir();
			_sntprintf_s( szPath, MAX_PATH, _T("%s\\explorer.exe"), lpWinPath);
		}
		return szPath;
	}

	LPCTSTR CSystemInfo::GetSysExplorerPath()
	{
		static	TCHAR	szPath[MAX_PATH] = {0};
		if ( szPath[0] == 0 )
		{
			LPCTSTR	lpWinPath = GetSystem32Dir();
			_sntprintf_s( szPath, MAX_PATH, _T("%s\\explorer.exe"), lpWinPath);
		}
		return szPath;
	}

	LPCTSTR CSystemInfo::GetUserStartMenuProgDir()
	{
		static	TCHAR	szPath[MAX_PATH] = {0};
		if ( szPath[0] == 0 )
		{
			SHGetSpecialFolderPath( NULL, szPath, CSIDL_PROGRAMS, FALSE );
			FixPathLastSpec(szPath);
		}
		return szPath;
	}

	LPCTSTR CSystemInfo::GetCommonStartMenuProgDir()
	{
		static	TCHAR	szPath[MAX_PATH] = {0};
		if ( szPath[0] == 0 )
		{
			SHGetSpecialFolderPath( NULL, szPath, CSIDL_COMMON_PROGRAMS, FALSE );
			FixPathLastSpec(szPath);
		}
		return szPath;
	}

	LPCTSTR CSystemInfo::GetUserDesktopDir()
	{
		static	TCHAR	szPath[MAX_PATH] = {0};
		if ( szPath[0] == 0 )
		{
			SHGetSpecialFolderPath( NULL, szPath, CSIDL_DESKTOPDIRECTORY, FALSE );
			FixPathLastSpec(szPath);
		}
		return szPath;
	}

	LPCTSTR CSystemInfo::GetCommonDesktopDir()
	{
		static	TCHAR	szPath[MAX_PATH] = {0};
		if ( szPath[0] == 0 )
		{
			SHGetSpecialFolderPath( NULL, szPath, CSIDL_COMMON_DESKTOPDIRECTORY, FALSE );
			FixPathLastSpec(szPath);
		}
		return szPath;
	}

	LPCTSTR CSystemInfo::GetQuickLanchDir()
	{
		static	TCHAR	szPath[MAX_PATH] = {0};
		if ( szPath[0] == 0 )
		{
			LPCTSTR	lpAppData = GetAppDataDir();
			if (lpAppData[0]!=0)
			{
				_sntprintf_s( szPath, MAX_PATH, _T("%s\\Microsoft\\Internet Explorer\\Quick Launch"), lpAppData);
			}
		}
		return szPath;
	}

	LPCTSTR CSystemInfo::GetUserStartMenuRootDir()
	{
		static	TCHAR	szPath[MAX_PATH] = {0};
		if ( szPath[0] == 0 )
		{
			SHGetSpecialFolderPath( NULL, szPath, CSIDL_STARTMENU, FALSE );
			FixPathLastSpec(szPath);
		}
		return szPath;
	}

	LPCTSTR CSystemInfo::GetCommonStartMenuRootDir()
	{
		static	TCHAR	szPath[MAX_PATH] = {0};
		if ( szPath[0] == 0 )
		{
			SHGetSpecialFolderPath( NULL, szPath, CSIDL_COMMON_STARTMENU, FALSE );
			FixPathLastSpec(szPath);
		}
		return szPath;
	}

	LPCTSTR CSystemInfo::GetUserStartRunDir()
	{
		static	TCHAR	szPath[MAX_PATH] = {0};
		if ( szPath[0] == 0 )
		{
			SHGetSpecialFolderPath( NULL, szPath, CSIDL_STARTUP, FALSE );
			FixPathLastSpec(szPath);
		}
		return szPath;
	}

	LPCTSTR CSystemInfo::GetCommonStartRunDir()
	{
		static	TCHAR	szPath[MAX_PATH] = {0};
		if ( szPath[0] == 0 )
		{
			SHGetSpecialFolderPath( NULL, szPath, CSIDL_COMMON_STARTUP, FALSE );
			FixPathLastSpec(szPath);
		}
		return szPath;
	}

	LPCTSTR CSystemInfo::GetUserFavoriteDir()
	{
		static	TCHAR	szPath[MAX_PATH] = {0};
		if ( szPath[0] == 0 )
		{
			SHGetSpecialFolderPath( NULL, szPath, CSIDL_FAVORITES, FALSE );
			FixPathLastSpec(szPath);
		}
		return szPath;
	}

	LPCTSTR CSystemInfo::GetCommonFavoriteDir()
	{
		static	TCHAR	szPath[MAX_PATH] = {0};
		if ( szPath[0] == 0 )
		{
			SHGetSpecialFolderPath( NULL, szPath, CSIDL_COMMON_FAVORITES, FALSE );
			FixPathLastSpec(szPath);
		}
		return szPath;
	}

	VOID CSystemInfo::GetLongPath( LPCTSTR lpFile, CString& strLongFile )
	{
		DWORD	nSize = ::GetLongPathName(lpFile, NULL, 0);

		if ( nSize > 0 )
		{
			TCHAR*	pBuffer = new TCHAR[nSize+1];
			::GetLongPathName(lpFile, pBuffer, nSize+1);
			strLongFile = pBuffer;
			delete pBuffer;
		}
		else
			strLongFile = lpFile;
	}

	LPCTSTR CSystemInfo::GetIExplorerPath()
	{
		static	TCHAR	szPath[MAX_PATH] = {0};
		if ( szPath[0] == 0 )
		{
			LPCTSTR	lpProgDir = GetProgramDir();
			if (lpProgDir[0]!=0)
			{
				_sntprintf_s( szPath, MAX_PATH, _T("%s\\Internet Explorer\\iexplore.exe"), lpProgDir);
			}
		}
		return szPath;
	}

	LPCTSTR CSystemInfo::GetUserinitPath()
	{
		static	TCHAR	szPath[MAX_PATH] = {0};
		if ( szPath[0] == 0 )
		{
			LPCTSTR	lpSysDir = GetSystem32Dir();
			if (lpSysDir[0]!=0)
			{
				_sntprintf_s( szPath, MAX_PATH, _T("%s\\Userinit.exe"), lpSysDir);
			}
		}
		return szPath;
	}

	LPCTSTR CSystemInfo::GetWinlogonPath()
	{
		static	TCHAR	szPath[MAX_PATH] = {0};
		if ( szPath[0] == 0 )
		{
			LPCTSTR	lpSysDir = GetSystem32Dir();
			if (lpSysDir[0]!=0)
			{
				_sntprintf_s( szPath, MAX_PATH, _T("%s\\winlogon.exe"), lpSysDir);
			}
		}
		return szPath;
	}

	BOOL CSystemInfo::IsWOW64()
	{
		return m_bIsWOW64;
	}

	BOOL CSystemInfo::Is2K3()
	{
		return (m_dwMajorVer==5 && m_dwMinVer==2);
	}

	BOOL CSystemInfo::IsWin7()
	{
		return m_dwMajorVer==7;
	}

	BOOL CSystemInfo::IsWin2kOrLater()
	{
		return m_dwMajorVer>=5;
	}

	BOOL CSystemInfo::IsXPOrLater()
	{
		return (m_dwMajorVer>5)||(m_dwMajorVer==5&&m_dwMinVer>0);
	}

	BOOL CSystemInfo::IsWin5x()
	{
		return m_dwMajorVer==5;
	}

	BOOL CSystemInfo::Is2K()
	{
		return (m_dwMajorVer==5 && m_dwMinVer ==0);
	}

	BOOL CSystemInfo::IsXP()
	{
		return (m_dwMajorVer==5 && m_dwMinVer ==1);
	}

	BOOL CSystemInfo::IsVista()
	{
		return m_dwMajorVer==6;
	}

	BOOL CSystemInfo::IsNT4()
	{
		return m_dwMajorVer==4;
	}

	BOOL CSystemInfo::IsVistaOrLater()
	{
		return m_dwMajorVer >= 6;
	}

	BOOL CSystemInfo::IsWin7OrLater()
	{
		return (m_dwMajorVer > 6) || (m_dwMajorVer==6 && m_dwMinVer >=1 ) ;
	}

	BOOL CSystemInfo::IsWow64Process()
	{
		return m_bIsWow64Process;
	}

	VOID CSystemInfo::GetIsWow64Process()
	{
		typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
		LPFN_ISWOW64PROCESS fnIsWow64Process;
		m_bIsWow64Process = FALSE;

		fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress( GetModuleHandle(_T("kernel32")),"IsWow64Process");
		if (NULL != fnIsWow64Process)
		{
			fnIsWow64Process(::GetModuleHandle(NULL),&m_bIsWow64Process);
		}
	}

}

