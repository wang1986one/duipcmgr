#pragma once

typedef UINT (WINAPI *PTHREAD_START)(LPVOID lpThreadParameter);
typedef PTHREAD_START LPTHREAD_START;

namespace Utils
{
	//多线程同步锁
	class AutoLocker;

	class ILocker
	{
	public:
		virtual ~ILocker() {};

		virtual void Lock(void) =0;
		virtual void Unlock(void) =0;
		virtual bool isLocked(void) const =0;
	};

	class ThreadLock : public ILocker
	{
	public:
		ThreadLock(void);

		~ThreadLock(void);

		virtual void Lock(void);

		virtual void Unlock(void);

		//当前是否被锁住
		virtual bool isLocked(void) const;

	protected:
		CRITICAL_SECTION mCs;
	};

	class AutoLocker
	{
	public:
		AutoLocker(ILocker *pLocker);
		~AutoLocker(void);

	protected:
		ILocker *m_pLocker;
	};

	class Monitor
	{
	private:
		HANDLE m_pEvent ;
	public:
		Monitor(BOOL isManual = FALSE);
		~Monitor(); 

		// 如果为auto，则SetEvent将event obj设为有信号，当一个等待线程release后，
		//event obj自动设为无信号
		//如果是manual，则release所有等待线程，且没有后面自动重设
		void Set();

		//手动将event obj设为无信号
		void Reset();
		// 如果为auto，则PulseEvent将event obj设为有信号，当一个等待线程release后，
		//event obj自动设为无信号
		//如果是manual，PulseEvent将event obj设为有信号，且release所有等待线程，
		//然后将event obj自动设为无信号
		void Pulse();
		// 等待直到超时
		DWORD Wait(long timeout);
	};

	//创建线程(忽略线程抛出的异常)
	//返回线程句柄
	HANDLE CreateThread(LPTHREAD_START lpStartAddress,
		LPVOID lpParameter = NULL,
		LPDWORD pthreadId = NULL,
		DWORD dwCreationFlags = 0);

	// ::CreateThread封装
	HANDLE CreateWorkThread(LPTHREAD_START_ROUTINE lpStartAddress,
		LPVOID lpParameter = NULL,
		LPDWORD pthreadId = NULL,
		DWORD dwCreationFlags = 0);

	//采用线程池方式创建线程(忽略线程抛出的异常)
	BOOL CreateThreadPool(LPTHREAD_START_ROUTINE lpStartAddress, 
		LPVOID lpParameter = NULL);
}