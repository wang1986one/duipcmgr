#include "stdafx.h"
#include "WinThread.h"

DWORD WINAPI CWinThread::RunThreadFuncNoCRT( LPVOID pParam )
{
	IWinRunnable* pThis = (IWinRunnable*)pParam;
	if (pThis)
		return pThis->Run();

	return 0;
}

unsigned int __stdcall CWinThread::RunThreadFunc( void* pParam )
{
	DWORD dwRet = RunThreadFuncNoCRT(pParam);

#ifndef _ATL_MIN_CRT
	_endthreadex(dwRet);
#endif//_ATL_MIN_CRT

	return DWORD(dwRet);
}

DWORD CWinThread::GetExitCode( DWORD dwDefaultCode )
{
	assert(m_h);
	DWORD dwExitCode;
	if (::GetExitCodeThread(m_h, &dwExitCode))
		return dwExitCode;

	return dwDefaultCode;
}

int CWinThread::GetPriority()
{
	assert(m_h);
	return ::GetThreadPriority(m_h);
}

BOOL CWinThread::SetPriority( int nPriority )
{
	assert(m_h);
	return ::SetThreadPriority(m_h, nPriority);
}

DWORD CWinThread::Resume()
{
	return ::ResumeThread(m_h);
}

DWORD CWinThread::Suspend()
{
	assert(m_h);
	return ::SuspendThread(m_h);
}

BOOL CWinThread::IsExit()
{
	return WAIT_TIMEOUT != WaitExit(0);
}

DWORD CWinThread::WaitExit( DWORD dwMilliseconds )
{
	return ::WaitForSingleObject(m_h, dwMilliseconds);
}

HRESULT CWinThread::CreateNoCRT( IWinRunnable* piRunnable )
{
	assert(!m_h);
	assert(piRunnable);
	if (m_h)
		return AtlHresultFromWin32(ERROR_ALREADY_INITIALIZED);

	if (!piRunnable)
		return E_POINTER;

	m_h = ::CreateThread(NULL, 0, RunThreadFuncNoCRT, piRunnable, CREATE_SUSPENDED, NULL);
	if (!m_h)
		return GetLastError() ? AtlHresultFromLastError() : E_FAIL;

	Resume();
	return S_OK;
}

HRESULT CWinThread::Create( IWinRunnable* piRunnable )
{
	assert(!m_h);
	assert(piRunnable);
	if (m_h)
		return AtlHresultFromWin32(ERROR_ALREADY_INITIALIZED);

	if (!piRunnable)
		return E_POINTER;

	// use _beginthreadex for initialization of c runtime lib
	m_h = (HANDLE)_beginthreadex(NULL, 0, RunThreadFunc, piRunnable, CREATE_SUSPENDED, NULL);
	if (!m_h)
		return GetLastError() ? AtlHresultFromLastError() : E_FAIL;

	Resume();
	return S_OK;
}

