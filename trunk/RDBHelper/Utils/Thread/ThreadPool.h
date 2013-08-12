#pragma once
#include <cassert>
#include <vector>
#include <queue>
#include <windows.h>

/*

Use:
1):
void threadfunc(void *p)
{
//...
}
ThreadPool tp;
for(i=0; i<100; i++)
tp.Call(threadfunc);
ThreadPool tp(20);//20Ϊ��ʼ�̳߳ع�ģ
tp.Call(threadfunc, lpPara);
tp.AdjustSize(50);//����50
tp.AdjustSize(-30);//����30

2):
class MyThreadJob : public ThreadJob //�̶߳����ThreadJob��չ
{
public:
virtual void DoJob(void *p)//�Զ�����麯��
{
//....
}
};
MyThreadJob mt[10];
ThreadPool tp;
for(i=0; i<100 i++)
tp.Call(mt + i);//tp.Call(mt + i, para);

*/



namespace Utils
{
	namespace ThreadPool
	{

		class ThreadJob  //��������
		{
		public:
			//���̳߳ص��õ��麯��
			virtual void DoJob(LPVOID pParam) = 0;
		};

		class ThreadPool
		{

		public:
			// ����߳��� dwNumberOfProcessors*2
			UINT nBestThreadCount;

			//dwNum �̳߳ع�ģ -1 ��������߳���
			ThreadPool(DWORD dwNum = -1);

			~ThreadPool();

			//�����̳߳ع�ģ
			int AdjustSize(int iNum);

			//�����̳߳�
			void Call(void (*pFunc)(LPVOID), LPVOID pParam = NULL);

			//�����̳߳�
			void Call(ThreadJob * p, LPVOID pParam = NULL);

			//�����̳߳�, ��ͬ���ȴ�
			bool EndAndWait(DWORD dwWaitTime = INFINITE);

			//�����̳߳�
			void End();

			DWORD Size();

			DWORD GetRunningSize();

			bool IsRunning();

		protected:

			//�����߳�
			static DWORD WINAPI DefaultJobProc(LPVOID lpParameter = NULL);

			//�����û������麯��
			static void CallProc(LPVOID pParam);

			//�û�����ṹ
			struct CallProcPara  
			{
				ThreadJob* _pObj;//�û����� 
				LPVOID _pParam;//�û�����
				CallProcPara(ThreadJob* p, LPVOID pParam): _pObj(p), _pParam(pParam) { }
			};

			//�û������ṹ
			struct JobItem 
			{
				void (*_pFunc)(LPVOID);//����
				LPVOID _pParam; //����
				JobItem(void (*pFunc)(LPVOID) = NULL, LPVOID pParam = NULL) : _pFunc(pFunc), _pParam(pParam) { };
			};

			//�̳߳��е��߳̽ṹ
			struct ThreadItem
			{
				HANDLE _Handle; //�߳̾��
				ThreadPool *_pThis;  //�̳߳ص�ָ��
				DWORD _dwLastBeginTime; //���һ�����п�ʼʱ��
				DWORD _dwCount; //���д���
				bool _fIsRunning;
				ThreadItem(ThreadPool *pthis): _pThis(pthis), _Handle(NULL), _dwLastBeginTime(0), _dwCount(0), _fIsRunning(false) {}
				~ThreadItem()
				{
					if(_Handle)
					{
						CloseHandle(_Handle);
						_Handle = NULL;
					}
				}
			};

			std::queue<JobItem *> _JobQueue;  //��������
			std::vector<ThreadItem *>  _ThreadVector; //�߳�����

			CRITICAL_SECTION _csThreadVector, _csWorkQueue; //���������ٽ�, �߳������ٽ�

			HANDLE _EventEnd, _EventComplete, _SemaphoreCall, _SemaphoreDel;//����֪ͨ, ����¼�, �����źţ� ɾ���߳��ź�
			long _lThreadNum, _lRunningNum; //�߳���, ���е��߳���
		};
	}
}