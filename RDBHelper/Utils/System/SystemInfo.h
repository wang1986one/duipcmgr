#pragma once

namespace Utils
{

	class CSystemInfo
	{
	public:
		CSystemInfo(void);
		~CSystemInfo(void);

	public:
		LPCTSTR		GetModulePath();		// 应用程序全路径
		LPCTSTR		GetModuleDir();		//	应用程序路径
		LPCTSTR		GetTempDir();			//	系统临时文件夹
		LPCTSTR		GetWindowsDir();		// 系统文件夹
		LPCTSTR		GetSystem32Dir();	// System32文件夹
		LPCTSTR		GetProgramDir();		// 默认应用程序安装路径
		LPCTSTR		GetCommonAppDataDir();	// 默认应用程序数据路径
		LPCTSTR		GetAppDataDir();
		LPCTSTR		GetUserStartupDir();
		LPCTSTR		GetAllStartupDir();

		LPCTSTR		GetUserinitPath();
		LPCTSTR		GetIExplorerPath();
		LPCTSTR		GetExplorerPath();
		LPCTSTR		GetSysExplorerPath();	// system32目录下explorer.exe的路径
		LPCTSTR		GetRundll32Path();
		LPCTSTR		GetRegSrvPath();

		LPCTSTR		GetWinlogonPath();

		LPCTSTR		GetUserStartMenuProgDir();		// 开始菜单，程序目录
		LPCTSTR		GetCommonStartMenuProgDir();	

		LPCTSTR		GetUserDesktopDir();	// 桌面
		LPCTSTR		GetCommonDesktopDir();

		LPCTSTR		GetQuickLanchDir();		// 快速启动

		LPCTSTR		GetUserStartMenuRootDir();		// 启动菜单根目录
		LPCTSTR		GetCommonStartMenuRootDir();

		LPCTSTR		GetUserStartRunDir();
		LPCTSTR		GetCommonStartRunDir();

		LPCTSTR		GetUserFavoriteDir();
		LPCTSTR		GetCommonFavoriteDir();

		BOOL		IsNT4();
		BOOL		Is2K();
		BOOL		Is2K3();
		BOOL		IsXP();
		BOOL		IsVista();
		BOOL		IsWin7();	
		BOOL		IsWin5x();	// 是不是5.X系列的

		BOOL		IsWin2kOrLater();	// Win2K或以上
		BOOL		IsXPOrLater();		//	XP或以上
		BOOL		IsVistaOrLater();	// Vista或以上
		BOOL		IsWin7OrLater();	// Win7或以上

		BOOL		IsWOW64();			// 操作系统是否为64位
		BOOL		IsWow64Process();	// 当前进程是否为64位

		BOOL		FixPathLastSpec(LPTSTR lpPath);	// 把最后一位 '\' 去掉
		VOID		GetLongPath(LPCTSTR lpFile, CString& strLongFile);

	protected:

		VOID		InitModule(HMODULE hMod);
		VOID		GetIsWow64();
		VOID		GetIsWow64Process();
	protected:
		HMODULE		m_hMod;
		DWORD		m_dwMajorVer;
		DWORD		m_dwMinVer;
		DWORD		m_dwBuildNumber;
		DWORD		m_dwPlatformId;
		BOOL			m_bIsWOW64;
		BOOL			m_bIsWow64Process;
	};

	extern CSystemInfo* AppGetSystemInfo();

}

