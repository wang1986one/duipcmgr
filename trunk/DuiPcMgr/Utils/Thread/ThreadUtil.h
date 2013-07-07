#pragma once

typedef UINT (WINAPI *PTHREAD_START)(LPVOID lpThreadParameter);
typedef PTHREAD_START LPTHREAD_START;

namespace Utils
{
	//���߳�ͬ����
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

		//��ǰ�Ƿ���ס
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

		// ���Ϊauto����SetEvent��event obj��Ϊ���źţ���һ���ȴ��߳�release��
		//event obj�Զ���Ϊ���ź�
		//�����manual����release���еȴ��̣߳���û�к����Զ�����
		void Set();

		//�ֶ���event obj��Ϊ���ź�
		void Reset();
		// ���Ϊauto����PulseEvent��event obj��Ϊ���źţ���һ���ȴ��߳�release��
		//event obj�Զ���Ϊ���ź�
		//�����manual��PulseEvent��event obj��Ϊ���źţ���release���еȴ��̣߳�
		//Ȼ��event obj�Զ���Ϊ���ź�
		void Pulse();
		// �ȴ�ֱ����ʱ
		DWORD Wait(long timeout);
	};

	//�����߳�(�����߳��׳����쳣)
	//�����߳̾��
	HANDLE CreateThread(LPTHREAD_START lpStartAddress,
		LPVOID lpParameter = NULL,
		LPDWORD pthreadId = NULL,
		DWORD dwCreationFlags = 0);

	// ::CreateThread��װ
	HANDLE CreateWorkThread(LPTHREAD_START_ROUTINE lpStartAddress,
		LPVOID lpParameter = NULL,
		LPDWORD pthreadId = NULL,
		DWORD dwCreationFlags = 0);

	//�����̳߳ط�ʽ�����߳�(�����߳��׳����쳣)
	BOOL CreateThreadPool(LPTHREAD_START_ROUTINE lpStartAddress, 
		LPVOID lpParameter = NULL);
}