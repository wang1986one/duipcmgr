// yykingking@126.com
//
#include "stdafx.h"
#include "ProcessMgr.h"

#include <tlhelp32.h>
#include <ShellAPI.h>
#include <Psapi.h>

#pragma comment(lib, "psapi.lib")

namespace Utils
{
	typedef LONG (WINAPI *PFNTQUERYINFORMATIONPROCESS)(HANDLE,UINT,PVOID,ULONG,PULONG);
	PFNTQUERYINFORMATIONPROCESS	NtQueryInformationProcess = NULL;

	VOID CProcessMgr::GetProcessNameByPID( DWORD dwProcessID,CString& strName )
	{
		HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (INVALID_HANDLE_VALUE != hSnapshot)
		{
			PROCESSENTRY32 pe = { sizeof(pe) };
			BOOL fOk;
			for (fOk = Process32First(hSnapshot, &pe); fOk; fOk = ::Process32Next(hSnapshot, &pe))
			{
				if (pe.th32ProcessID==dwProcessID)
				{
					strName=pe.szExeFile;
					::CloseHandle(hSnapshot);
					return;
				} 
			}
		}

		::CloseHandle(hSnapshot);
	}

	void CProcessMgr::UpdateProcessList( ProcessEntryList& ProcessEntry )
	{
		ProcessEntry.clear();

		PROCESSENTRY32 processEntry;
		HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		processEntry.dwSize = sizeof(PROCESSENTRY32);

		while (::Process32Next(hProcessSnap, &processEntry) != FALSE)
		{
			PROCESSENTRY	psEntry;
			psEntry.ProcessID=processEntry.th32ProcessID;
			psEntry.strExeName=processEntry.szExeFile;
			psEntry.ParentProcessID =processEntry.th32ParentProcessID;
			ProcessEntry.push_back(psEntry);
		}
		::CloseHandle(hProcessSnap);
	}

	DWORD CProcessMgr::GetProcessIDByName( LPCTSTR lpszProcessName )
	{
		DWORD dwProcessID = -1;
		PROCESSENTRY32 processEntry;
		HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		processEntry.dwSize = sizeof(PROCESSENTRY32);

		while (::Process32Next(hProcessSnap, &processEntry) != FALSE)
		{
			CString strProcessName(processEntry.szExeFile);
			if ( strProcessName.CompareNoCase(lpszProcessName) == 0 )
			{
				dwProcessID = processEntry.th32ProcessID;
				break;
			}
		}

		return dwProcessID;
	}

	typedef	BOOL (WINAPI *PFQueryFullProcessImageNameW)(
		_In_     HANDLE hProcess,
		_In_     DWORD dwFlags,
		_Out_    LPWSTR lpExeName,
		_Inout_  PDWORD lpdwSize
		);

	typedef	BOOL (WINAPI *PFQueryFullProcessImageNameA)(
		_In_     HANDLE hProcess,
		_In_     DWORD dwFlags,
		_Out_    LPSTR lpExeName,
		_Inout_  PDWORD lpdwSize
		);

	VOID CProcessMgr::GetProcessFullPath( DWORD dwProcessID,CString& strFullPath )
	{
		strFullPath=_T("");

		DWORD dwCount;
		HANDLE hProcess=GetProcessHandleByPID(dwProcessID);
		if (hProcess==INVALID_HANDLE_VALUE)
			return;
		if ( Utils::AppGetSystemInfo()->IsVistaOrLater() )
		{
			// VistaAbove
			dwCount=MAX_PATH;
			HMODULE hModule=::LoadLibrary(_T("kernel32.dll"));
#ifdef _UNICODE
			PFQueryFullProcessImageNameW PFQueryFullProcessImageName=(PFQueryFullProcessImageNameW)GetProcAddress(hModule, "QueryFullProcessImageNameW");
			if (PFQueryFullProcessImageName!=NULL)
			{
				PFQueryFullProcessImageName(hProcess,0,strFullPath.GetBufferSetLength(MAX_PATH),&dwCount);
				strFullPath.ReleaseBuffer(dwCount);
			}
#else
			PFQueryFullProcessImageNameA PFQueryFullProcessImageName=(PFQueryFullProcessImageNameA)GetProcAddress(hModule, "QueryFullProcessImageNameA");
			if (PFQueryFullProcessImageName!=NULL)
			{
				PFQueryFullProcessImageName(hProcess,0,strFullPath.GetBufferSetLength(MAX_PATH),&dwCount);
				strFullPath.ReleaseBuffer(dwCount);
			}
#endif // _UNICODE

		}
		else
		{	// XP
			dwCount=GetModuleFileNameEx(hProcess,NULL,strFullPath.GetBufferSetLength(MAX_PATH),MAX_PATH);
			strFullPath.ReleaseBuffer(dwCount);
		}
	}

	HANDLE CProcessMgr::GetProcessHandleByPID( DWORD dwProcessID )
	{
		return ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
	}

	BOOL CProcessMgr::ExecuteExe( LPCTSTR lpExecutePath, LPCTSTR lpParam, DWORD* pdwExitCode,BOOL bRequiredAdmin /*=FALSE */, BOOL bWaitExitCode /*= FALSE*/ )
	{
		SHELLEXECUTEINFO ShExecInfo = {0}; 
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO); 
		ShExecInfo.lpFile = lpExecutePath; 
		ShExecInfo.lpParameters = lpParam;
		ShExecInfo.lpDirectory = NULL; 
		ShExecInfo.nShow = SW_SHOW; 
		ShExecInfo.hInstApp = NULL; 
		ShExecInfo.hwnd = NULL;

		if (bWaitExitCode)
			ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS; 
		else
			ShExecInfo.fMask = NULL; 

		if (bRequiredAdmin)
			ShExecInfo.lpVerb = _T("runas"); 
		else
			ShExecInfo.lpVerb = _T("open"); 

		if (!::ShellExecuteEx(&ShExecInfo))
		{
			return FALSE;
		}

		if (bWaitExitCode)
		{
			::WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
			::GetExitCodeProcess(ShExecInfo.hProcess, pdwExitCode);
			::CloseHandle( ShExecInfo.hProcess );
			return TRUE;
		}
		else
			return TRUE;
	}
}

