#ifndef __WinThread_H__
#define __WinThread_H__
#pragma once
#include <assert.h>
#include <atlbase.h>

class IWinRunnable
{
public:
    virtual DWORD   STDMETHODCALLTYPE Run() = 0;
};

class CWinThread: public CHandle
{
public:

    HRESULT Create(IWinRunnable* piRunnable);
    HRESULT CreateNoCRT(IWinRunnable* piRunnable);

    DWORD   WaitExit(DWORD dwMilliseconds);
    BOOL    IsExit();
    //void    Terminate(DWORD dwExitCode = 1);

    DWORD   Suspend();
    DWORD   Resume();


    BOOL    SetPriority(int nPriority);
    int     GetPriority();


    DWORD   GetExitCode(DWORD dwDefaultCode = 0);

private:

    // for CRT thread API
    static UINT __stdcall RunThreadFunc(void* pParam);

    // for Win32 thread API
    static DWORD WINAPI RunThreadFuncNoCRT(LPVOID pParam);
};

#endif // __WinThread_H__

