#include "StdAfx.h"
#include <process.h>
#include "Utils/Utils.h"
#include "ThreadUtil.h"

static unsigned int WINAPI _MyThreadProc(LPVOID lpParam);

Utils::ThreadLock::ThreadLock(void)
{
	::InitializeCriticalSection(&mCs);
}

Utils::ThreadLock::~ThreadLock( void )
{
	::DeleteCriticalSection(&mCs);
}

void Utils::ThreadLock::Lock( void )
{
	::EnterCriticalSection(&mCs);
}

void Utils::ThreadLock::Unlock( void )
{
	::LeaveCriticalSection(&mCs);
}

bool Utils::ThreadLock::isLocked( void ) const
{
	return mCs.RecursionCount>0;
}

Utils::AutoLocker::AutoLocker( ILocker *pLocker )
: m_pLocker(pLocker)
{
	if (m_pLocker)
		m_pLocker->Lock();
}

Utils::AutoLocker::~AutoLocker( void )
{
	if (m_pLocker)
		m_pLocker->Unlock();
}

static unsigned int WINAPI _MyThreadProc(LPVOID lpParam)
{
	DWORD dwParam[2] = {((DWORD*)lpParam)[0], ((DWORD*)lpParam)[1]};
	delete[] lpParam;lpParam = NULL;
	__try
	{
		return ( (LPTHREAD_START_ROUTINE)(dwParam[0]) )
			( (LPVOID)dwParam[1] );
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		//²¶×½ËùÓÐµÄÒì³£
		return GetExceptionCode();
	}
}

HANDLE Utils::CreateThread( LPTHREAD_START lpStartAddress, LPVOID lpParameter /*= NULL*/, LPDWORD pthreadId /*= NULL*/, DWORD dwCreationFlags /*= 0*/ )
{
	HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0,lpStartAddress, lpParameter, dwCreationFlags, (unsigned int*)pthreadId);
	if (!hThread)
	{
		TRACE(_T("Utils::BeginThreadÊ§°Ü£¡"));
	}
	return hThread;
}

HANDLE Utils::CreateWorkThread( LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter /*= NULL*/ , LPDWORD pthreadId /*= NULL*/, DWORD dwCreationFlags /*= 0*/ )
{
	HANDLE hThread = ::CreateThread(NULL,0,lpStartAddress,lpParameter,dwCreationFlags,pthreadId);
	if (!hThread)
	{
		TRACE(_T("Utils::CreateThreadÊ§°Ü£¡"));
	}
	return hThread;
}

BOOL Utils::CreateThreadPool( LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter /*= NULL*/ )
{
	if (::QueueUserWorkItem(lpStartAddress, lpParameter, WT_EXECUTEDEFAULT))
	{
		return TRUE;
	}

	TRACE(_T("Utils::CreateThreadPoolÊ§°Ü£¡0x%0x"),lpStartAddress);
	return FALSE;
}

Utils::Monitor::Monitor( BOOL isManual /*= FALSE*/ )
{
	this->m_pEvent = ::CreateEvent(NULL ,FALSE ,isManual ,NULL) ;
}

Utils::Monitor::~Monitor()
{
	//ReleaseEvent()
	CloseHandle(this->m_pEvent) ;
}

void Utils::Monitor::Set()
{
	::SetEvent(this->m_pEvent) ;
}

void Utils::Monitor::Reset()
{
	::ResetEvent(this->m_pEvent) ;
}

void Utils::Monitor::Pulse()
{
	::PulseEvent(this->m_pEvent) ;
}

DWORD Utils::Monitor::Wait( long timeout )
{
	return WaitForSingleObject(this->m_pEvent ,timeout) ;
}

