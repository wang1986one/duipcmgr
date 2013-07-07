#include "StdAfx.h"
#include "ThreadPool.h"

namespace Utils
{
	namespace ThreadPool
	{
		ThreadPool::ThreadPool( DWORD dwNum /*= -1*/ )
			: _lThreadNum(0)
			, _lRunningNum(0)
		{
			SYSTEM_INFO info;
			::GetSystemInfo(&info);
			nBestThreadCount=info.dwNumberOfProcessors*2;

			InitializeCriticalSection(&_csThreadVector);
			InitializeCriticalSection(&_csWorkQueue);

			_EventComplete = CreateEvent(0, false, false, NULL);
			_EventEnd = CreateEvent(0, true, false, NULL);
			_SemaphoreCall = CreateSemaphore(0, 0, 0x7FFFFFFF, NULL);
			_SemaphoreDel =  CreateSemaphore(0, 0, 0x7FFFFFFF, NULL);

			assert(_SemaphoreCall != INVALID_HANDLE_VALUE);
			assert(_EventComplete != INVALID_HANDLE_VALUE);
			assert(_EventEnd != INVALID_HANDLE_VALUE);
			assert(_SemaphoreDel != INVALID_HANDLE_VALUE);

			if (dwNum==-1)
			{
				AdjustSize(nBestThreadCount);
			}
			else
			{
				AdjustSize(dwNum <= 0 ? 4 : dwNum);
			}
		}

		ThreadPool::~ThreadPool()
		{
			DeleteCriticalSection(&_csWorkQueue);

			CloseHandle(_EventEnd);
			CloseHandle(_EventComplete);
			CloseHandle(_SemaphoreCall);
			CloseHandle(_SemaphoreDel);

			std::vector<ThreadItem*>::iterator iter;
			for(iter = _ThreadVector.begin(); iter != _ThreadVector.end(); iter++)
			{
				if(*iter)
					delete *iter;
			}

			DeleteCriticalSection(&_csThreadVector);
		}

		int ThreadPool::AdjustSize( int iNum )
		{
			if(iNum > 0)
			{
				ThreadItem *pNew;
				EnterCriticalSection(&_csThreadVector);
				for(int _i=0; _i<iNum; _i++)
				{
					_ThreadVector.push_back(pNew = new ThreadItem(this)); 
					assert(pNew);
					pNew->_Handle = ::CreateThread(NULL, 0, DefaultJobProc, pNew, 0, NULL);
					assert(pNew->_Handle);
				}
				LeaveCriticalSection(&_csThreadVector);
			}
			else
			{
				iNum *= -1;
				ReleaseSemaphore(_SemaphoreDel,  iNum > _lThreadNum ? _lThreadNum : iNum, NULL);
			}
			return (int)_lThreadNum;
		}

		void ThreadPool::Call( void (*pFunc)(LPVOID), LPVOID pParam /*= NULL*/ )
		{
			assert(pFunc);

			EnterCriticalSection(&_csWorkQueue);
			_JobQueue.push(new JobItem(pFunc, pParam));
			LeaveCriticalSection(&_csWorkQueue);

			ReleaseSemaphore(_SemaphoreCall, 1, NULL);
		}

		void ThreadPool::Call( ThreadJob * p, LPVOID pParam /*= NULL*/ )
		{
			Call(CallProc, new CallProcPara(p, pParam));
		}

		bool ThreadPool::EndAndWait( DWORD dwWaitTime /*= INFINITE*/ )
		{
			SetEvent(_EventEnd);
			return WaitForSingleObject(_EventComplete, dwWaitTime) == WAIT_OBJECT_0;
		}

		void ThreadPool::End()
		{
			SetEvent(_EventEnd);
		}

		DWORD ThreadPool::Size()
		{
			return (DWORD)_lThreadNum;
		}

		DWORD ThreadPool::GetRunningSize()
		{
			return (DWORD)_lRunningNum;
		}

		bool ThreadPool::IsRunning()
		{
			return _lRunningNum > 0;
		}

		DWORD WINAPI ThreadPool::DefaultJobProc( LPVOID lpParameter /*= NULL*/ )
		{
			ThreadItem *pThread = static_cast<ThreadItem*>(lpParameter);
			assert(pThread);

			ThreadPool *pThreadPoolObj = pThread->_pThis;
			assert(pThreadPoolObj);

			InterlockedIncrement(&pThreadPoolObj->_lThreadNum);

			HANDLE hWaitHandle[3];
			hWaitHandle[0] = pThreadPoolObj->_SemaphoreCall;
			hWaitHandle[1] = pThreadPoolObj->_SemaphoreDel;
			hWaitHandle[2] = pThreadPoolObj->_EventEnd;

			JobItem *pJob;
			bool fHasJob;

			for(;;)
			{
				DWORD wr = WaitForMultipleObjects(3, hWaitHandle, false, INFINITE);

				//响应删除线程信号
				if(wr == WAIT_OBJECT_0 + 1)  
					break;

				//从队列里取得用户作业
				EnterCriticalSection(&pThreadPoolObj->_csWorkQueue);
				if(fHasJob = !pThreadPoolObj->_JobQueue.empty())
				{
					pJob = pThreadPoolObj->_JobQueue.front();
					pThreadPoolObj->_JobQueue.pop();
					assert(pJob);
				}
				LeaveCriticalSection(&pThreadPoolObj->_csWorkQueue);

				//受到结束线程信号 确定是否结束线程(结束线程信号 && 是否还有工作)
				if(wr == WAIT_OBJECT_0 + 2 && !fHasJob)  
					break;

				if(fHasJob && pJob)
				{
					InterlockedIncrement(&pThreadPoolObj->_lRunningNum);
					pThread->_dwLastBeginTime = GetTickCount();
					pThread->_dwCount++;
					pThread->_fIsRunning = true;
					pJob->_pFunc(pJob->_pParam); //运行用户作业
					delete pJob; 
					pThread->_fIsRunning = false;
					InterlockedDecrement(&pThreadPoolObj->_lRunningNum);
				}
			}

			//删除自身结构
			EnterCriticalSection(&pThreadPoolObj->_csThreadVector);
			pThreadPoolObj->_ThreadVector.erase(find(pThreadPoolObj->_ThreadVector.begin(), pThreadPoolObj->_ThreadVector.end(), pThread));
			LeaveCriticalSection(&pThreadPoolObj->_csThreadVector);

			delete pThread;

			InterlockedDecrement(&pThreadPoolObj->_lThreadNum);

			if(!pThreadPoolObj->_lThreadNum)  //所有线程结束
				SetEvent(pThreadPoolObj->_EventComplete);

			return 0;
		}

		void ThreadPool::CallProc( LPVOID pParam )
		{
			CallProcPara *cp = static_cast<CallProcPara *>(pParam);
			assert(cp);
			if(cp)
			{
				cp->_pObj->DoJob(cp->_pParam);
				delete cp;
			}
		}
	}
}