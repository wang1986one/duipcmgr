#include "stdafx.h"
#include "Kernel32Mod.h"

namespace Utils
{
	void* CWinModule_kernel32::GetProcAddr( LPCSTR lpProcName )
	{
		HMODULE hKernel32 = GetModHandle();
		if (!hKernel32)
			return NULL;

		return ::GetProcAddress(hKernel32, lpProcName);
	}

	HMODULE CWinModule_kernel32::GetModHandle()
	{
		if (m_hKernel32)
			return m_hKernel32;

		m_hKernel32 = ::GetModuleHandle(L"kernel32");
		return m_hKernel32;
	}

	BOOLEAN WINAPI CWinModule_kernel32::Wow64EnableWow64FsRedirection( BOOLEAN Wow64FsEnableRedirection )
	{
		if (!m_pfnWow64EnableWow64FsRedirection)
		{
			m_pfnWow64EnableWow64FsRedirection = (PFN_Wow64EnableWow64FsRedirection)GetProcAddr("Wow64EnableWow64FsRedirection");
			if (NULL == m_pfnWow64EnableWow64FsRedirection)
				return FALSE;
		}

		return m_pfnWow64EnableWow64FsRedirection(Wow64FsEnableRedirection);
	}

	BOOL WINAPI CWinModule_kernel32::Wow64RevertWow64FsRedirection( PVOID OldValue )
	{
		if (!m_pfnWow64RevertWow64FsRedirection)
		{
			m_pfnWow64RevertWow64FsRedirection = (PFN_Wow64RevertWow64FsRedirection)GetProcAddr("Wow64RevertWow64FsRedirection");
			if (NULL == m_pfnWow64RevertWow64FsRedirection)
				return FALSE;
		}

		return m_pfnWow64RevertWow64FsRedirection(OldValue);
	}

	BOOL WINAPI CWinModule_kernel32::Wow64DisableWow64FsRedirection( PVOID* OldValue )
	{
		if (!m_pfnWow64DisableWow64FsRedirection)
		{
			m_pfnWow64DisableWow64FsRedirection = (PFN_Wow64DisableWow64FsRedirection)GetProcAddr("Wow64DisableWow64FsRedirection");
			if (NULL == m_pfnWow64DisableWow64FsRedirection)
				return FALSE;
		}

		return m_pfnWow64DisableWow64FsRedirection(OldValue);
	}

	BOOL WINAPI CWinModule_kernel32::IsWow64Process( HANDLE hProcess, BOOL* pbIsWow64Process )
	{
		if (!m_pfnIsWow64Process)
		{
			m_pfnIsWow64Process = (PFN_IsWow64Process)GetProcAddr("IsWow64Process");
			if (NULL == m_pfnIsWow64Process)
				return FALSE;
		}

		return m_pfnIsWow64Process(hProcess, pbIsWow64Process);
	}

}

