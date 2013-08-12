#include "stdafx.h"
#include "WinWow64.h"
#include <Shlwapi.h>

namespace Utils
{
	BOOL CWinWow64::IsExemptSubDir( LPCWSTR lpszSubDir ) const
	{
		if (!lpszSubDir || !*lpszSubDir)
			return FALSE;

		if (0 == StrCmpNW(lpszSubDir, L"catroot\\", (int)wcslen(L"catroot\\")))
			return TRUE;

		if (0 == StrCmpNW(lpszSubDir, L"catroot2\\", (int)wcslen(L"catroot2\\")))
			return TRUE;

		if (0 == StrCmpNW(lpszSubDir, L"drivers\\etc\\", (int)wcslen(L"drivers\\etc\\")))
			return TRUE;

		if (0 == StrCmpNW(lpszSubDir, L"logfiles\\", (int)wcslen(L"logfiles\\")))
			return TRUE;

		if (0 == StrCmpNW(lpszSubDir, L"spool\\", (int)wcslen(L"spool\\")))
			return TRUE;

		if (! AppGetSystemInfo()->IsWin7OrLater() )
			return FALSE;

		if (0 == StrCmpNW(lpszSubDir, L"driversstore\\", (int)wcslen(L"driversstore\\")))
			return TRUE;

		return FALSE;
	}

	BOOL CWinWow64::Wow64IsX64Path( LPCWSTR lpszRealPath, BOOL bNormalized ) const
	{
		if (!lpszRealPath || !*lpszRealPath)
		{
			return FALSE;
		}


		CWinPath PathReal = lpszRealPath;
		if (!bNormalized)
			PathReal.ExpandNormalizedPathName();


		if (0 == StrCmpNW(PathReal.m_strPath, m_PathSysWow64.m_strPath, m_PathSysWow64.m_strPath.GetLength()))
		{
			LPCWSTR lpszSubPath = (LPCWSTR)PathReal.m_strPath + m_PathSysWow64.m_strPath.GetLength();
			if (!IsExemptSubDir(lpszSubPath))
			{
				return TRUE;
			}
		}


		if (0 == StrCmpNW(PathReal.m_strPath, m_PathProgramFilesX86.m_strPath, m_PathProgramFilesX86.m_strPath.GetLength()))
		{
			return TRUE;
		}


		return FALSE;
	}

	BOOL CWinWow64::Wow64FsReverseRedirection( LPCWSTR lpszRealPath, CString& strWow64Path, BOOL bNormalized ) const
	{
		if (!lpszRealPath || !*lpszRealPath)
		{
			strWow64Path.Empty();
			return FALSE;
		}


		CWinPath PathReal = lpszRealPath;
		if (!bNormalized)
			PathReal.ExpandNormalizedPathName();


		if (0 == StrCmpNW(PathReal.m_strPath, m_PathSysWow64.m_strPath, m_PathSysWow64.m_strPath.GetLength()))
		{
			LPCWSTR lpszSubPath = (LPCWSTR)PathReal.m_strPath + m_PathSysWow64.m_strPath.GetLength();
			if (!IsExemptSubDir(lpszSubPath))
			{
				strWow64Path = m_PathSystem32.m_strPath;
				strWow64Path.Append(lpszSubPath);
				return TRUE;
			}
		}


		if (0 == StrCmpNW(PathReal.m_strPath, m_PathProgramFilesX86.m_strPath, m_PathProgramFilesX86.m_strPath.GetLength()))
		{
			LPCWSTR lpszSubPath = (LPCWSTR)PathReal.m_strPath + m_PathProgramFilesX86.m_strPath.GetLength();
			strWow64Path = m_PathProgramFiles.m_strPath;
			strWow64Path.Append(lpszSubPath);
			return TRUE;
		}


		strWow64Path = PathReal;
		return FALSE;
	}

	BOOL CWinWow64::Wow64FsRedirection( LPCWSTR lpszWow64Path, CString& strRealPath, BOOL bNormalized ) const
	{
		if (!m_bIsWow64 || !m_bRedirectionIsValid)
		{
			strRealPath.Empty();
			return FALSE;
		}


		if (!lpszWow64Path || !*lpszWow64Path)
		{
			strRealPath.Empty();
			return FALSE;
		}


		CWinPath PathWow64 = lpszWow64Path;
		if (!bNormalized)
			PathWow64.ExpandNormalizedPathName();


		if (0 == StrCmpNW(PathWow64.m_strPath, m_PathSystem32.m_strPath, m_PathSystem32.m_strPath.GetLength()))
		{
			LPCWSTR lpszSubPath = (LPCWSTR)PathWow64.m_strPath + m_PathSystem32.m_strPath.GetLength();
			if (!IsExemptSubDir(lpszSubPath))
			{
				strRealPath = m_PathSysWow64.m_strPath;
				strRealPath.Append(lpszSubPath);
				return TRUE;
			}
		}


		if (0 == StrCmpNW(PathWow64.m_strPath, m_PathProgramFiles.m_strPath, m_PathProgramFiles.m_strPath.GetLength()))
		{
			LPCWSTR lpszSubPath = (LPCWSTR)PathWow64.m_strPath + m_PathProgramFiles.m_strPath.GetLength();
			strRealPath = m_PathProgramFilesX86.m_strPath;
			strRealPath.Append(lpszSubPath);
			return TRUE;
		}


		strRealPath = PathWow64;
		return FALSE;
	}

	CWinWow64::CWinWow64()
		: m_bIsWow64(FALSE)
		, m_bRedirectionIsValid(FALSE)
	{
		if (!CWinModule_kernel32::IsWow64Process(::GetCurrentProcess(), &m_bIsWow64))
		{
			m_bIsWow64 = FALSE;
			return;
		}

		m_PathSystem32 = L"%windir%\\System32";        // c:\windows\system32
		m_PathSystem32.ExpandEnvironmentStrings();
		m_PathSystem32.ExpandNormalizedPathName();
		m_PathSystem32.AddBackslash();

		m_PathProgramFiles = L"%ProgramW6432%";            // c:\program files
		m_PathProgramFiles.ExpandEnvironmentStrings();
		m_PathProgramFiles.ExpandNormalizedPathName();
		m_PathProgramFiles.AddBackslash();

		m_PathSysWow64 = L"%windir%\\SysWow64";        // c:\windows\syswow64
		m_PathSysWow64.ExpandEnvironmentStrings();
		m_PathSysWow64.ExpandNormalizedPathName();
		m_PathSysWow64.AddBackslash();

		m_PathProgramFilesX86 = L"%ProgramFiles(x86)%";       // c:\program files (x86)
		m_PathProgramFilesX86.ExpandEnvironmentStrings();
		m_PathProgramFilesX86.ExpandNormalizedPathName();
		m_PathProgramFilesX86.AddBackslash();

		if (!m_PathSystem32.m_strPath.IsEmpty() &&
			!m_PathProgramFiles.m_strPath.IsEmpty() &&
			!m_PathSysWow64.m_strPath.IsEmpty() &&
			!m_PathProgramFilesX86.m_strPath.IsEmpty())
		{
			m_bRedirectionIsValid = TRUE;
			return;
		}
	}

	const CWinPath& CWinWow64::GetPathProgramFilesX86()
	{
		return m_PathProgramFilesX86;
	}

	const CWinPath& CWinWow64::GetPathSysWow64()
	{
		return m_PathSysWow64;
	}

	const CWinPath& CWinWow64::GetPathProgramFiles()
	{
		return m_PathProgramFiles;
	}

	const CWinPath& CWinWow64::GetPathSystem32()
	{
		return m_PathSystem32;
	}

	BOOL CWinWow64::IsWow64() const
	{
		return m_bIsWow64;
	}

	CWinWow64FsRedirectionGuard::CWinWow64FsRedirectionGuard()
		: m_bChanged(FALSE)
		, m_OldValue(NULL)
	{

	}

	CWinWow64FsRedirectionGuard::~CWinWow64FsRedirectionGuard()
	{
		Revert();
	}

	BOOL CWinWow64FsRedirectionGuard::Enable()
	{
		if (!m_bChanged)
		{
			BOOL br = Disable();
			if (br)
				return br;
		}


		BOOL br = CWinModule_kernel32::Wow64EnableWow64FsRedirection(TRUE);
		if (!br)
			return FALSE;


		m_bChanged = TRUE;
		return TRUE;
	}

	BOOL CWinWow64FsRedirectionGuard::Disable()
	{
		BOOL br = FALSE;
		if (m_bChanged)
		{
			br = CWinModule_kernel32::Wow64EnableWow64FsRedirection(FALSE);
		}
		else
		{
			br = CWinModule_kernel32::Wow64DisableWow64FsRedirection(&m_OldValue);
		}


		if (!br)
			return FALSE;


		m_bChanged = TRUE;
		return TRUE;
	}

	BOOL CWinWow64FsRedirectionGuard::Revert()
	{
		if (!m_bChanged)
			return FALSE;


		BOOL br = CWinModule_kernel32::Wow64RevertWow64FsRedirection(m_OldValue);
		if (!br)
			return FALSE;


		m_OldValue = NULL;
		m_bChanged = FALSE;
		return TRUE;
	}

}

