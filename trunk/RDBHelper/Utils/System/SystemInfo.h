#pragma once

namespace Utils
{

	class CSystemInfo
	{
	public:
		CSystemInfo(void);
		~CSystemInfo(void);

	public:
		LPCTSTR		GetModulePath();		// Ӧ�ó���ȫ·��
		LPCTSTR		GetModuleDir();		//	Ӧ�ó���·��
		LPCTSTR		GetTempDir();			//	ϵͳ��ʱ�ļ���
		LPCTSTR		GetWindowsDir();		// ϵͳ�ļ���
		LPCTSTR		GetSystem32Dir();	// System32�ļ���
		LPCTSTR		GetProgramDir();		// Ĭ��Ӧ�ó���װ·��
		LPCTSTR		GetCommonAppDataDir();	// Ĭ��Ӧ�ó�������·��
		LPCTSTR		GetAppDataDir();
		LPCTSTR		GetUserStartupDir();
		LPCTSTR		GetAllStartupDir();

		LPCTSTR		GetUserinitPath();
		LPCTSTR		GetIExplorerPath();
		LPCTSTR		GetExplorerPath();
		LPCTSTR		GetSysExplorerPath();	// system32Ŀ¼��explorer.exe��·��
		LPCTSTR		GetRundll32Path();
		LPCTSTR		GetRegSrvPath();

		LPCTSTR		GetWinlogonPath();

		LPCTSTR		GetUserStartMenuProgDir();		// ��ʼ�˵�������Ŀ¼
		LPCTSTR		GetCommonStartMenuProgDir();	

		LPCTSTR		GetUserDesktopDir();	// ����
		LPCTSTR		GetCommonDesktopDir();

		LPCTSTR		GetQuickLanchDir();		// ��������

		LPCTSTR		GetUserStartMenuRootDir();		// �����˵���Ŀ¼
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
		BOOL		IsWin5x();	// �ǲ���5.Xϵ�е�

		BOOL		IsWin2kOrLater();	// Win2K������
		BOOL		IsXPOrLater();		//	XP������
		BOOL		IsVistaOrLater();	// Vista������
		BOOL		IsWin7OrLater();	// Win7������

		BOOL		IsWOW64();			// ����ϵͳ�Ƿ�Ϊ64λ
		BOOL		IsWow64Process();	// ��ǰ�����Ƿ�Ϊ64λ

		BOOL		FixPathLastSpec(LPTSTR lpPath);	// �����һλ '\' ȥ��
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

