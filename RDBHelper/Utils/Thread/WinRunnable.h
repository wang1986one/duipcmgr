/**
* @file    winrunnable.h
* @brief   ...
* @author  bbcallen
* @date    2009-03-27  15:54
*/

#ifndef WINRUNNABLE_H
#define WINRUNNABLE_H

#include <atlsync.h>
#include <atlbase.h>
#include "winthread.h"

/**
* @brief abstract class for thread
*/
class AWinRunnable: public IWinRunnable
{
public:
    AWinRunnable();
    virtual ~AWinRunnable();

    void    CloseThread();
    HRESULT StartRunning();

    DWORD   WaitExit(DWORD dwMilliseconds);
    BOOL    IsExit();
    DWORD   GetExitCode(DWORD dwDefaultCode = 0);

protected:

    // override this function to implement thread routine
    virtual DWORD STDMETHODCALLTYPE Run() = 0;

protected:
    CWinThread  m_hThread;  
};

#endif//WINRUNNABLE_H

