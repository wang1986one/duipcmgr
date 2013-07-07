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
ThreadPool tp(20);//20为初始线程池规模
tp.Call(threadfunc, lpPara);
tp.AdjustSize(50);//增加50
tp.AdjustSize(-30);//减少30

2):
class MyThreadJob : public ThreadJob //线程对象从ThreadJob扩展
{
public:
virtual void DoJob(void *p)//自定义的虚函数
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

		class ThreadJob  //工作基类
		{
		public:
			//供线程池调用的虚函数
			virtual void DoJob(LPVOID pParam) = 0;
		};

		class ThreadPool
		{

		public:
			// 最佳线程数 dwNumberOfProcessors*2
			UINT nBestThreadCount;

			//dwNum 线程池规模 -1 计算最佳线程数
			ThreadPool(DWORD dwNum = -1);

			~ThreadPool();

			//调整线程池规模
			int AdjustSize(int iNum);

			//调用线程池
			void Call(void (*pFunc)(LPVOID), LPVOID pParam = NULL);

			//调用线程池
			void Call(ThreadJob * p, LPVOID pParam = NULL);

			//结束线程池, 并同步等待
			bool EndAndWait(DWORD dwWaitTime = INFINITE);

			//结束线程池
			void End();

			DWORD Size();

			DWORD GetRunningSize();

			bool IsRunning();

		protected:

			//工作线程
			static DWORD WINAPI DefaultJobProc(LPVOID lpParameter = NULL);

			//调用用户对象虚函数
			static void CallProc(LPVOID pParam);

			//用户对象结构
			struct CallProcPara  
			{
				ThreadJob* _pObj;//用户对象 
				LPVOID _pParam;//用户参数
				CallProcPara(ThreadJob* p, LPVOID pParam): _pObj(p), _pParam(pParam) { }
			};

			//用户函数结构
			struct JobItem 
			{
				void (*_pFunc)(LPVOID);//函数
				LPVOID _pParam; //参数
				JobItem(void (*pFunc)(LPVOID) = NULL, LPVOID pParam = NULL) : _pFunc(pFunc), _pParam(pParam) { };
			};

			//线程池中的线程结构
			struct ThreadItem
			{
				HANDLE _Handle; //线程句柄
				ThreadPool *_pThis;  //线程池的指针
				DWORD _dwLastBeginTime; //最后一次运行开始时间
				DWORD _dwCount; //运行次数
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

			std::queue<JobItem *> _JobQueue;  //工作队列
			std::vector<ThreadItem *>  _ThreadVector; //线程数据

			CRITICAL_SECTION _csThreadVector, _csWorkQueue; //工作队列临界, 线程数据临界

			HANDLE _EventEnd, _EventComplete, _SemaphoreCall, _SemaphoreDel;//结束通知, 完成事件, 工作信号， 删除线程信号
			long _lThreadNum, _lRunningNum; //线程数, 运行的线程数
		};
	}
}