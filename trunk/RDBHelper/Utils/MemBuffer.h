#ifndef __MemBuffer_H__
#define __MemBuffer_H__
#pragma once

namespace Utils
{
#pragma pack(push,1)
	typedef struct _stMemBuffer
	{
		UINT nDataLength;		// ���ݳ���
		UINT nBufferLength;	// �������ܳ���
		char pData[1];						// ������ָ��

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
		CMemBuffer(UINT nInitLength=8196);		// Ĭ��8KB
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

		// ����������
		CMemBuffer& operator+=(const CMemBuffer& other);
		CMemBuffer& operator+=(const CStringA& other);
		CMemBuffer& operator+=(const CStringW& other);
		CMemBuffer& operator=(const CMemBuffer& other);
		char& operator[](int index);
		operator char *(void);

		bool Alloc(UINT nLength);		// �������ݻ�����,��һ�����·����ڴ�
		bool ReAlloc(UINT nNewLength,bool bKeepData);	// �����ڴ棬ָ�������Ƿ񱣴�
		bool IsEmpty(void);
		void Free(void);		// �ͷ��ڴ�
		void Clear(void);	// �������
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

