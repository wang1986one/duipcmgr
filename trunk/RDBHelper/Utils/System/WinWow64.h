#ifndef WINWOW64_H
#define WINWOW64_H
#pragma once

#include "Utils/Path/WinPath.h"
#include "Kernel32Mod.h"
#include "SystemInfo.h"
namespace Utils
{

	class CWinWow64
	{
	public:
		CWinWow64();

		BOOL    IsWow64() const;

		BOOL Wow64FsRedirection(LPCWSTR lpszWow64Path, CString& strRealPath, BOOL bNormalized = FALSE) const;
		BOOL Wow64FsReverseRedirection(LPCWSTR lpszRealPath, CString& strWow64Path, BOOL bNormalized = FALSE) const;

		BOOL    Wow64IsX64Path(LPCWSTR lpszRealPath, BOOL bNormalized = FALSE) const;

		BOOL    IsExemptSubDir(LPCWSTR lpszSubDir) const;

		const CWinPath& GetPathSystem32();
		const CWinPath& GetPathProgramFiles();
		const CWinPath& GetPathSysWow64();
		const CWinPath& GetPathProgramFilesX86();

	protected:

		BOOL m_bIsWow64;
		BOOL m_bRedirectionIsValid;

		// real path
		CWinPath m_PathSystem32;
		CWinPath m_PathProgramFiles;

		// wow64 path
		CWinPath m_PathSysWow64;
		CWinPath m_PathProgramFilesX86;
	};

	class CWinWow64FsRedirectionGuard
	{
	public:
		CWinWow64FsRedirectionGuard();
		~CWinWow64FsRedirectionGuard();

		BOOL Enable();
		BOOL Disable();
		BOOL Revert();

	protected:
		BOOL  m_bChanged;
		PVOID m_OldValue;
	};

}
#endif//WINWOW64_H