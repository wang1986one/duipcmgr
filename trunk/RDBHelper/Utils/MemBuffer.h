#ifndef __MemBuffer_H__
#define __MemBuffer_H__
#pragma once

namespace Utils
{
#pragma pack(push,1)
	typedef struct _stMemBuffer
	{
		UINT nDataLength;		// 数据长度
		UINT nBufferLength;	// 缓冲区总长度
		char pData[1];						// 缓冲区指针

		_stMemBuffer()
			: nBufferLength(0)
			, nDataLength(0)
		{
		}
	}MemBufferBlock,*PMemBufferBlock;
#pragma pack (pop)

	class CMemBuffer
		: public IDataStream
	{
	public:
		CMemBuffer(UINT nInitLength=8196);		// 默认8KB
		CMemBuffer(const CStringA strData);
		CMemBuffer(const CStringW strData);
		CMemBuffer(const CMemBuffer& other);
		CMemBuffer(const LPVOID pData,UINT nDataLength);
		virtual ~CMemBuffer(void);

		char* GetData(void) const;
		UINT GetDataLength(void) const;
		void SetDataLength(UINT nDataLength);
		UINT GetBufferLength(void) const;

		CMemBuffer& Append(const CStringA& other);
		CMemBuffer& Append(const CStringW& other);
		CMemBuffer& Append(const CMemBuffer& other);
		CMemBuffer& Append(const LPCSTR pData);
		CMemBuffer& Append(const LPWSTR pData);
		CMemBuffer& Append(const LPVOID pData,UINT nDataLength);

		// 操作符重载
		CMemBuffer& operator+=(const CMemBuffer& other);
		CMemBuffer& operator+=(const CStringA& other);
		CMemBuffer& operator+=(const CStringW& other);
		CMemBuffer& operator=(const CMemBuffer& other);
		char& operator[](int index);
		operator char *(void);

		bool Alloc(UINT nLength);		// 请求数据缓冲区,不一定重新分配内存
		bool ReAlloc(UINT nNewLength,bool bKeepData);	// 分配内存，指定数据是否保存
		bool IsEmpty(void);
		void Free(void);		// 释放内存
		void Clear(void);	// 清空数据
		void Empty(void);

		//////////////////////////////////////////////////////////////////////////
		// IDataStream
		virtual BOOL Create();
		virtual BOOL SetLength( UINT iLength );
		virtual BOOL Read( LPVOID lpBuffer,DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead );
		virtual BOOL Write( UINT iPosition, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten );
		virtual BOOL Write( LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten );
		virtual VOID Close();
		virtual UINT GetLength();

	protected:
		UINT GetBufferBlockLength(void) const;
		CHAR chNull;
		VOID Initialize(void);

		PMemBufferBlock m_pMemBuffer;
	};
}

#endif // __MemBuffer_H__

