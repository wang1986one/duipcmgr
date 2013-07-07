#include "stdafx.h"
#include "NTHelper.h"

namespace Utils
{

	VOID CNTSystem::_Init()
	{
		if (m_pfQuery==NULL)
		{
			TCHAR	szBuffer[MAX_PATH];
			::GetSystemWindowsDirectory(szBuffer,MAX_PATH);
			_tcscat(szBuffer,NT_DLL_NAME);

			m_mod.LoadLib(szBuffer);
			m_pfQuery = (pfnZwQuerySystemInformation)m_mod.GetProcAddr("ZwQuerySystemInformation");
		}
	}

	BOOL CNTSystem::GetBasicInfo( SYSTEM_BASIC_INFORMATION& info )
	{
		_Init();
		if (m_pfQuery==NULL)
			return FALSE;

		return ( 0==m_pfQuery(SystemBasicInformation,&info,sizeof(SYSTEM_BASIC_INFORMATION),NULL) );
	}

	BOOL CNTSystem::GetTimeInfo( SYSTEM_TIME_INFORMATION& info )
	{
		_Init();
		if (m_pfQuery==NULL)
			return FALSE;

		return ( 0==m_pfQuery(SystemTimeOfDayInformation,&info,sizeof(SYSTEM_TIME_INFORMATION),NULL) );
	}

	BOOL CNTSystem::GetPerformanceInfo64( SYSTEM_PERFORMANCE_INFORMATION64& info )
	{
		_Init();
		if (m_pfQuery==NULL)
			return FALSE;

		return ( 0==m_pfQuery(SystemPerformanceInformation,&info,sizeof(SYSTEM_PERFORMANCE_INFORMATION64),NULL) );
	}

	BOOL CNTSystem::GetPerformanceInfo( SYSTEM_PERFORMANCE_INFORMATION& info )
	{
		_Init();
		if (m_pfQuery==NULL)
			return FALSE;

		return ( 0==m_pfQuery(SystemPerformanceInformation,&info,sizeof(SYSTEM_PERFORMANCE_INFORMATION),NULL) );
	}

	CNTSystem::~CNTSystem()
	{

	}

	CNTSystem::CNTSystem()
	{
		m_pfQuery = NULL;
	}

	VOID CNTProcess::_Init()
	{
		if (m_pfQuery==NULL)
		{
			TCHAR	szBuffer[MAX_PATH];
			::GetSystemWindowsDirectory(szBuffer,MAX_PATH);
			_tcscat(szBuffer,NT_DLL_NAME);

			m_mod.LoadLib(szBuffer);
			m_pfQuery = (pfnZwQueryInformationProcess)m_mod.GetProcAddr("ZwQueryInformationProcess");
		}
	}

	BOOL CNTProcess::QueryVMMemory( HANDLE hProcess,VM_COUNTERS& vmCounter )
	{
		_Init();

		if (m_pfQuery==NULL)
			return FALSE;

		return (m_pfQuery(hProcess,ProcessVmCounters, &vmCounter, sizeof(VM_COUNTERS),NULL) >= 0 );
	}

	BOOL CNTProcess::QueryIOCounter( HANDLE hProcess,IO_COUNTERS& ioConter )
	{
		_Init();

		if (m_pfQuery==NULL)
			return FALSE;

		return (m_pfQuery(hProcess,ProcessIoCounters, &ioConter, sizeof(IO_COUNTERS),NULL) >= 0 );
	}

	CNTProcess::~CNTProcess()
	{

	}

	CNTProcess::CNTProcess()
	{
		m_pfQuery = NULL;
	}
}

