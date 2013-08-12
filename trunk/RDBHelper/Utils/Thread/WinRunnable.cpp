#include "stdafx.h"
#include "WinRunnable.h"

DWORD AWinRunnable::GetExitCode( DWORD dwDefaultCode )
{
	return m_hThread.GetExitCode(dwDefaultCode);
}

BOOL AWinRunnable::IsExit()
{
	return m_hThread.IsExit();
}

DWORD AWinRunnable::WaitExit( DWORD dwMilliseconds )
{
	return m_hThread.WaitExit(dwMilliseconds);
}

HRESULT AWinRunnable::StartRunning()
{
	if (m_hThread && !m_hThread.IsExit())
	{
		return AtlHresultFromWin32(ERROR_ALREADY_INITIALIZED);
	}

	m_hThread.Close();

	return m_hThread.Create(this);
}

void AWinRunnable::CloseThread()
{
	m_hThread.Close();
}

AWinRunnable::AWinRunnable()
{

}

AWinRunnable::~AWinRunnable()
{

}

