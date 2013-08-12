#ifndef __NTHelper_H__
#define __NTHelper_H__
#pragma once
#include "WinModule.h"

namespace Utils
{

#define NT_DLL_NAME		L"\\system32\\ntdll.dll"

	typedef struct _IO_COUNTERS {
		ULONGLONG  ReadOperationCount;
		ULONGLONG  WriteOperationCount;
		ULONGLONG  OtherOperationCount;
		ULONGLONG ReadTransferCount;
		ULONGLONG WriteTransferCount;
		ULONGLONG OtherTransferCount;
	} IO_COUNTERS;
	typedef IO_COUNTERS *PIO_COUNTERS;

	//
	// Process Virtual Memory Counters
	//  NtQueryInformationProcess using ProcessVmCounters
	//

	typedef struct _VM_COUNTERS {
		SIZE_T PeakVirtualSize;
		SIZE_T VirtualSize;
		ULONG PageFaultCount;
		SIZE_T PeakWorkingSetSize;
		SIZE_T WorkingSetSize;
		SIZE_T QuotaPeakPagedPoolUsage;
		SIZE_T QuotaPagedPoolUsage;
		SIZE_T QuotaPeakNonPagedPoolUsage;
		SIZE_T QuotaNonPagedPoolUsage;
		SIZE_T PagefileUsage;
		SIZE_T PeakPagefileUsage;
	} VM_COUNTERS;
	typedef VM_COUNTERS *PVM_COUNTERS;

	typedef struct _VM_COUNTERS_EX {
		SIZE_T PeakVirtualSize;
		SIZE_T VirtualSize;
		ULONG PageFaultCount;
		SIZE_T PeakWorkingSetSize;
		SIZE_T WorkingSetSize;
		SIZE_T QuotaPeakPagedPoolUsage;
		SIZE_T QuotaPagedPoolUsage;
		SIZE_T QuotaPeakNonPagedPoolUsage;
		SIZE_T QuotaNonPagedPoolUsage;
		SIZE_T PagefileUsage;
		SIZE_T PeakPagefileUsage;
		SIZE_T PrivateUsage;
	} VM_COUNTERS_EX;
	typedef VM_COUNTERS_EX *PVM_COUNTERS_EX;


	typedef enum _PROCESSINFOCLASS {
		ProcessBasicInformation,
		ProcessQuotaLimits,
		ProcessIoCounters,
		ProcessVmCounters,
		ProcessTimes,
		ProcessBasePriority,
		ProcessRaisePriority,
		ProcessDebugPort,
		ProcessExceptionPort,
		ProcessAccessToken,
		ProcessLdtInformation,
		ProcessLdtSize,
		ProcessDefaultHardErrorMode,
		ProcessIoPortHandlers,          // Note: this is kernel mode only
		ProcessPooledUsageAndLimits,
		ProcessWorkingSetWatch,
		ProcessUserModeIOPL,
		ProcessEnableAlignmentFaultFixup,
		ProcessPriorityClass,
		ProcessWx86Information,
		ProcessHandleCount,
		ProcessAffinityMask,
		ProcessPriorityBoost,
		ProcessDeviceMap,
		ProcessSessionInformation,
		ProcessForegroundInformation,
		ProcessWow64Information,
		ProcessImageFileName,
		ProcessLUIDDeviceMapsEnabled,
		ProcessBreakOnTermination,
		ProcessDebugObjectHandle,
		ProcessDebugFlags,
		ProcessHandleTracing,
		ProcessIoPriority,
		ProcessExecuteFlags,
		ProcessResourceManagement,
		ProcessCookie,
		ProcessImageInformation,
		MaxProcessInfoClass             // MaxProcessInfoClass should always be the last enum
	} PROCESSINFOCLASS;


	typedef LONG ( WINAPI *pfnZwQueryInformationProcess)(
		HANDLE ProcessHandle,
		PROCESSINFOCLASS ProcessInformationClass,
		PVOID ProcessInformation,
		ULONG ProcessInformationLength,
		PULONG ReturnLength
		);


	typedef struct 
	{ 
		DWORD dwUnknown1; 
		ULONG uKeMaximumIncrement; 
		ULONG uPageSize; 
		ULONG uMmNumberOfPhysicalPages; 
		ULONG uMmLowestPhysicalPage; 
		ULONG uMmHighestPhysicalPage; 
		ULONG uAllocationGranularity; 
		PVOID pLowestUserAddress; 
		PVOID pMmHighestUserAddress; 
		ULONG uKeActiveProcessors; 
		BYTE bKeNumberProcessors; 
		BYTE bUnknown2; 
		WORD wUnknown3; 
	} SYSTEM_BASIC_INFORMATION;

	typedef struct _SYSTEM_PERFORMANCE_INFORMATION
	{
		LARGE_INTEGER  IdleTime;                    //CPU����ʱ�䣻
		LARGE_INTEGER  ReadTransferCount;           //I/O��������Ŀ��
		LARGE_INTEGER  WriteTransferCount;          //I/Oд������Ŀ��
		LARGE_INTEGER  OtherTransferCount;          //I/O����������Ŀ��
		ULONG          ReadOperationCount;          //I/O��������Ŀ��
		ULONG          WriteOperationCount;         //I/Oд������Ŀ��
		ULONG          OtherOperationCount;         //I/O��������������Ŀ��
		ULONG          AvailablePages;              //�ɻ�õ�ҳ��Ŀ��
		ULONG          TotalCommittedPages;         //�ܹ��ύҳ��Ŀ��
		ULONG          TotalCommitLimit;            //���ύҳ��Ŀ��
		ULONG          PeakCommitment;              //ҳ�ύ��ֵ��
		ULONG          PageFaults;                  //ҳ������Ŀ��
		ULONG          WriteCopyFaults;             //Copy-On-Write������Ŀ��
		ULONG          TransitionFaults;            //��ҳ������Ŀ��
		ULONG          Reserved1;
		ULONG          DemandZeroFaults;            //����0��������
		ULONG          PagesRead;                   //��ҳ��Ŀ��
		ULONG          PageReadIos;                 //��ҳI/O��������
		ULONG          Reserved2[2];
		ULONG          PagefilePagesWritten;        //��дҳ�ļ�ҳ����
		ULONG          PagefilePageWriteIos;        //��дҳ�ļ���������
		ULONG          MappedFilePagesWritten;      //��дӳ���ļ�ҳ����
		ULONG          MappedFileWriteIos;          //��дӳ���ļ���������
		ULONG          PagedPoolUsage;              //��ҳ��ʹ�ã�
		ULONG          NonPagedPoolUsage;           //�Ƿ�ҳ��ʹ�ã�
		ULONG          PagedPoolAllocs;             //��ҳ�ط��������
		ULONG          PagedPoolFrees;              //��ҳ���ͷ������
		ULONG          NonPagedPoolAllocs;          //�Ƿ�ҳ�ط��������
		ULONG          NonPagedPoolFress;           //�Ƿ�ҳ���ͷ������
		ULONG          TotalFreeSystemPtes;         //ϵͳҳ�����ͷ�������
		ULONG          SystemCodePage;              //����ϵͳ����ҳ����
		ULONG          TotalSystemDriverPages;      //�ɷ�ҳ��������ҳ����
		ULONG          TotalSystemCodePages;        //����ϵͳ����ҳ������
		ULONG          SmallNonPagedLookasideListAllocateHits; //
		ULONG          SmallPagedLookasideListAllocateHits;
		ULONG          Reserved3;                   
		ULONG          MmSystemCachePage;          //ϵͳ����ҳ����
		ULONG          PagedPoolPage;              //��ҳ��ҳ����
		ULONG          SystemDriverPage;           //�ɷ�ҳ����ҳ����
		ULONG          FastReadNoWait;             //�첽���ٶ���Ŀ��
		ULONG          FastReadWait;               //ͬ�����ٶ���Ŀ��
		ULONG          FastReadResourceMiss;       //���ٶ���Դ��ͻ����
		ULONG          FastReadNotPossible;        //���ٶ�ʧ������
		ULONG          FastMdlReadNoWait;          //�첽MDL���ٶ���Ŀ��
		ULONG          FastMdlReadWait;            //ͬ��MDL���ٶ���Ŀ��
		ULONG          FastMdlReadResourceMiss;    //MDL����Դ��ͻ����
		ULONG          FastMdlReadNotPossible;     //MDL��ʧ������
		ULONG          MapDataNoWait;              //�첽ӳ�����ݴ�����
		ULONG          MapDataWait;                //ͬ��ӳ�����ݴ�����
		ULONG          MapDataNoWaitMiss;          //�첽ӳ�����ݳ�ͻ������
		ULONG          MapDataWaitMiss;            //ͬ��ӳ�����ݳ�ͻ������
		ULONG          PinMappedDataCount;         //ǣ��ӳ��������Ŀ��
		ULONG          PinReadNoWait;              //ǣ���첽����Ŀ��
		ULONG          PinReadWait;                //ǣ��ͬ������Ŀ��
		ULONG          PinReadNoWaitMiss;          //ǣ���첽����ͻ��Ŀ��
		ULONG          PinReadWaitMiss;            //ǣ��ͬ������ͻ��Ŀ��
		ULONG          CopyReadNoWait;             //�첽������������
		ULONG          CopyReadWait;               //ͬ��������������
		ULONG          CopyReadNoWaitMiss;         //�첽���������ϴ�����
		ULONG          CopyReadWaitMiss;           //ͬ�����������ϴ�����
		ULONG          MdlReadNoWait;              //�첽MDL��������
		ULONG          MdlReadWait;                //ͬ��MDL��������
		ULONG          MdlReadNoWaitMiss;          //�첽MDL�����ϴ�����
		ULONG          MdlReadWaitMiss;            //ͬ��MDL�����ϴ�����
		ULONG          ReadAheadIos;               //��ǰ��������Ŀ��
		ULONG          LazyWriteIos;               //LAZYд������Ŀ��
		ULONG          LazyWritePages;             //LAZYдҳ�ļ���Ŀ��
		ULONG          DataFlushes;                //����ˢ�´�����
		ULONG          DataPages;                  //����ˢ��ҳ����
		ULONG          ContextSwitches;            //�����л���Ŀ��
		ULONG          FirstLevelTbFills;          //��һ�㻺������������
		ULONG          SecondLevelTbFills;         //�ڶ��㻺������������
		ULONG          SystemCall;                 //ϵͳ���ô�����
	}SYSTEM_PERFORMANCE_INFORMATION,*PSYSTEM_PERFORMANCE_INFORMATION;

	typedef struct _SYSTEM_PERFORMANCE_INFORMATION64
	{
		LARGE_INTEGER  IdleTime;                    //CPU����ʱ�䣻
		LARGE_INTEGER  ReadTransferCount;           //I/O��������Ŀ��
		LARGE_INTEGER  WriteTransferCount;          //I/Oд������Ŀ��
		LARGE_INTEGER  OtherTransferCount;          //I/O����������Ŀ��
		ULONG          ReadOperationCount;          //I/O��������Ŀ��
		ULONG          WriteOperationCount;         //I/Oд������Ŀ��
		ULONG          OtherOperationCount;         //I/O��������������Ŀ��
		ULONG          AvailablePages;              //�ɻ�õ�ҳ��Ŀ��
		ULONGLONG      TotalCommittedPages;         //�ܹ��ύҳ��Ŀ��
		ULONGLONG      TotalCommitLimit;            //���ύҳ��Ŀ��
		ULONGLONG      PeakCommitment;              //ҳ�ύ��ֵ��
		ULONG          PageFaults;                  //ҳ������Ŀ��
		ULONG          WriteCopyFaults;             //Copy-On-Write������Ŀ��
		ULONG          TransitionFaults;            //��ҳ������Ŀ��
		ULONG          Reserved1;
		ULONG          DemandZeroFaults;            //����0��������
		ULONG          PagesRead;                   //��ҳ��Ŀ��
		ULONG          PageReadIos;                 //��ҳI/O��������
		ULONG          Reserved2[2];
		ULONG          PagefilePagesWritten;        //��дҳ�ļ�ҳ����
		ULONG          PagefilePageWriteIos;        //��дҳ�ļ���������
		ULONG          MappedFilePagesWritten;      //��дӳ���ļ�ҳ����
		ULONG          MappedFileWriteIos;          //��дӳ���ļ���������
		ULONG          PagedPoolUsage;              //��ҳ��ʹ�ã�
		ULONG          NonPagedPoolUsage;           //�Ƿ�ҳ��ʹ�ã�
		ULONG          PagedPoolAllocs;             //��ҳ�ط��������
		ULONG          PagedPoolFrees;              //��ҳ���ͷ������
		ULONG          NonPagedPoolAllocs;          //�Ƿ�ҳ�ط��������
		ULONG          NonPagedPoolFress;           //�Ƿ�ҳ���ͷ������
		ULONG          TotalFreeSystemPtes;         //ϵͳҳ�����ͷ�������
		ULONG          SystemCodePage;              //����ϵͳ����ҳ����
		ULONG          TotalSystemDriverPages;      //�ɷ�ҳ��������ҳ����
		ULONG          TotalSystemCodePages;        //����ϵͳ����ҳ������
		ULONG          SmallNonPagedLookasideListAllocateHits; //
		ULONG          SmallPagedLookasideListAllocateHits;
		ULONG          Reserved3;                   
		ULONG          MmSystemCachePage;          //ϵͳ����ҳ����
		ULONG          PagedPoolPage;              //��ҳ��ҳ����
		ULONG          SystemDriverPage;           //�ɷ�ҳ����ҳ����
		ULONG          FastReadNoWait;             //�첽���ٶ���Ŀ��
		ULONG          FastReadWait;               //ͬ�����ٶ���Ŀ��
		ULONG          FastReadResourceMiss;       //���ٶ���Դ��ͻ����
		ULONG          FastReadNotPossible;        //���ٶ�ʧ������
		ULONG          FastMdlReadNoWait;          //�첽MDL���ٶ���Ŀ��
		ULONG          FastMdlReadWait;            //ͬ��MDL���ٶ���Ŀ��
		ULONG          FastMdlReadResourceMiss;    //MDL����Դ��ͻ����
		ULONG          FastMdlReadNotPossible;     //MDL��ʧ������
		ULONG          MapDataNoWait;              //�첽ӳ�����ݴ�����
		ULONG          MapDataWait;                //ͬ��ӳ�����ݴ�����
		ULONG          MapDataNoWaitMiss;          //�첽ӳ�����ݳ�ͻ������
		ULONG          MapDataWaitMiss;            //ͬ��ӳ�����ݳ�ͻ������
		ULONG          PinMappedDataCount;         //ǣ��ӳ��������Ŀ��
		ULONG          PinReadNoWait;              //ǣ���첽����Ŀ��
		ULONG          PinReadWait;                //ǣ��ͬ������Ŀ��
		ULONG          PinReadNoWaitMiss;          //ǣ���첽����ͻ��Ŀ��
		ULONG          PinReadWaitMiss;            //ǣ��ͬ������ͻ��Ŀ��
		ULONG          CopyReadNoWait;             //�첽������������
		ULONG          CopyReadWait;               //ͬ��������������
		ULONG          CopyReadNoWaitMiss;         //�첽���������ϴ�����
		ULONG          CopyReadWaitMiss;           //ͬ�����������ϴ�����
		ULONG          MdlReadNoWait;              //�첽MDL��������
		ULONG          MdlReadWait;                //ͬ��MDL��������
		ULONG          MdlReadNoWaitMiss;          //�첽MDL�����ϴ�����
		ULONG          MdlReadWaitMiss;            //ͬ��MDL�����ϴ�����
		ULONG          ReadAheadIos;               //��ǰ��������Ŀ��
		ULONG          LazyWriteIos;               //LAZYд������Ŀ��
		ULONG          LazyWritePages;             //LAZYдҳ�ļ���Ŀ��
		ULONG          DataFlushes;                //����ˢ�´�����
		ULONG          DataPages;                  //����ˢ��ҳ����
		ULONG          ContextSwitches;            //�����л���Ŀ��
		ULONG          FirstLevelTbFills;          //��һ�㻺������������
		ULONG          SecondLevelTbFills;         //�ڶ��㻺������������
		ULONG          SystemCall;                 //ϵͳ���ô�����
	}SYSTEM_PERFORMANCE_INFORMATION64,*PSYSTEM_PERFORMANCE_INFORMATION64;

	typedef struct 
	{ 
		LARGE_INTEGER liKeBootTime; 
		LARGE_INTEGER liKeSystemTime; 
		LARGE_INTEGER liExpTimeZoneBias; 
		ULONG uCurrentTimeZoneId; 
		DWORD dwReserved; 
	} SYSTEM_TIME_INFORMATION;


	typedef enum _SYSTEM_INFORMATION_CLASS {
		SystemBasicInformation,
		SystemProcessorInformation,
		SystemPerformanceInformation,
		SystemTimeOfDayInformation,
		SystemPathInformation,
		SystemProcessInformation,
		SystemCallCountInformation,
		SystemDeviceInformation,
		SystemProcessorPerformanceInformation,
		SystemFlagsInformation,
		SystemCallTimeInformation,
		SystemModuleInformation,
		SystemLocksInformation,
		SystemStackTraceInformation,
		SystemPagedPoolInformation,
		SystemNonPagedPoolInformation,
		SystemHandleInformation,
		SystemObjectInformation,
		SystemPageFileInformation,
		SystemVdmInstemulInformation,
		SystemVdmBopInformation,
		SystemFileCacheInformation,
		SystemPoolTagInformation,
		SystemInterruptInformation,
		SystemDpcBehaviorInformation,
		SystemFullMemoryInformation,
		SystemLoadGdiDriverInformation,
		SystemUnloadGdiDriverInformation,
		SystemTimeAdjustmentInformation,
		SystemSummaryMemoryInformation,
		SystemNextEventIdInformation,
		SystemEventIdsInformation,
		SystemCrashDumpInformation,
		SystemExceptionInformation,
		SystemCrashDumpStateInformation,
		SystemKernelDebuggerInformation,
		SystemContextSwitchInformation,
		SystemRegistryQuotaInformation,
		SystemExtendServiceTableInformation,
		SystemPrioritySeperation,
		SystemPlugPlayBusInformation,
		SystemDockInformation,
		SystemPowerInformation,
		SystemProcessorSpeedInformation,
		SystemCurrentTimeZoneInformation,
		SystemLookasideInformation
	} SYSTEM_INFORMATION_CLASS, *PSYSTEM_INFORMATION_CLASS;

	typedef LONG (WINAPI *pfnZwQuerySystemInformation)(
		SYSTEM_INFORMATION_CLASS SystemInformationClass,
		PVOID SystemInformation,
		ULONG SystemInformationLength,
		PULONG ReturnLength
		);

	class CNTSystem
	{
	public:
		CNTSystem();
		virtual ~CNTSystem();

	public:
		BOOL GetPerformanceInfo(SYSTEM_PERFORMANCE_INFORMATION& info);

		BOOL GetPerformanceInfo64(SYSTEM_PERFORMANCE_INFORMATION64& info);

		BOOL GetTimeInfo(SYSTEM_TIME_INFORMATION& info);

		BOOL GetBasicInfo(SYSTEM_BASIC_INFORMATION& info);

	protected:
		VOID _Init();
	protected:
		CWinModule				m_mod;
		pfnZwQuerySystemInformation		m_pfQuery;
	};

	class CNTProcess
	{
	public:
		CNTProcess();
		virtual ~CNTProcess();

	public:
		BOOL QueryIOCounter(HANDLE hProcess,IO_COUNTERS& ioConter );
		BOOL QueryVMMemory(HANDLE hProcess,VM_COUNTERS& vmCounter );

	protected:
		VOID _Init();

	protected:
		CWinModule				m_mod;
		pfnZwQueryInformationProcess	m_pfQuery;
	};
}

#endif // __NTHelper_H__

