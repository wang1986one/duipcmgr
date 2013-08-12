#include "StdAfx.h"
#include "MemBuffer.h"

namespace Utils
{
	CMemBuffer::CMemBuffer( UINT nInitLength/*=8196*/ )
	{
		Initialize();
		if ( nInitLength >0 )
			this->Alloc(nInitLength);
	}

	CMemBuffer::CMemBuffer( const CMemBuffer& other )
	{
		Initialize();

		this->ReAlloc(other.GetDataLength(),false);
		if(other.m_pMemBuffer->nDataLength>0)	// 有数据才复制
		{
			memcpy(m_pMemBuffer->pData,other.m_pMemBuffer->pData,other.m_pMemBuffer->nDataLength);
			m_pMemBuffer->nDataLength = other.m_pMemBuffer->nDataLength;
		}

		return;
	}

	CMemBuffer::CMemBuffer( const LPVOID pData,UINT nDataLength )
	{
		Initialize();
		this->Append(pData,nDataLength);
	}

	CMemBuffer::CMemBuffer( const CStringA strData )
	{
		Initialize();
		this->Append(strData);
	}

	CMemBuffer::CMemBuffer( const CStringW strData )
	{
		Initialize();
		this->Append(strData);
	}

	CMemBuffer::~CMemBuffer(void)
	{
		this->Free();
	}

	VOID CMemBuffer::Initialize( void )
	{
		chNull = 0;
		m_pMemBuffer=NULL;
	}

	bool CMemBuffer::Alloc( UINT nLength )
	{
		return this->ReAlloc(nLength,false);
	}

	bool CMemBuffer::ReAlloc( UINT nNewLength,bool bKeepData )
	{
		if ( nNewLength > GetBufferBlockLength())
		{
			// 需要长度大于缓冲区长度,申请新内存块
			PMemBufferBlock pMemBufferBlock=(PMemBufferBlock)new char[nNewLength+8];
			if (pMemBufferBlock ==NULL)
				return false;
			
			ZeroMemory(pMemBufferBlock ,nNewLength+8);
			pMemBufferBlock->nBufferLength=nNewLength;

			if (m_pMemBuffer!=NULL && m_pMemBuffer->nDataLength>0)
			{
				if (bKeepData==true)	// 存在原始数据，并且要求保存数据，拷贝
				{
					memcpy(pMemBufferBlock->pData,m_pMemBuffer->pData,m_pMemBuffer->nDataLength);
					pMemBufferBlock->nDataLength=m_pMemBuffer->nDataLength;
				}
			}
			// 释放当前MemBufferBlock
			this->Free();

			m_pMemBuffer=pMemBufferBlock;
		}

		return true;
	}

	void CMemBuffer::Clear( void )
	{
		if (m_pMemBuffer!=NULL )
		{
			ZeroMemory(m_pMemBuffer->pData,m_pMemBuffer->nDataLength);
		}
	}

	void CMemBuffer::Free( void )
	{
		if (m_pMemBuffer!=NULL )
		{
			delete [] m_pMemBuffer;
			m_pMemBuffer=NULL;
		}
	}

	CMemBuffer& CMemBuffer::Append( const LPVOID pData,UINT nDataLength )
	{
		if (pData!=NULL && nDataLength>0)
		{
			UINT nLenght=this->GetDataLength();
			this->ReAlloc(nLenght+nDataLength*2,true);		// 新开辟缓冲区，数据长度翻倍
			memcpy(m_pMemBuffer->pData+nLenght,pData,nDataLength);
			m_pMemBuffer->nDataLength=nLenght+nDataLength;
		}
		return (*this);
	}

	CMemBuffer& CMemBuffer::Append( const CStringA& other )
	{
		return this->Append((LPVOID)other.GetString(),other.GetLength());
	}

	CMemBuffer& CMemBuffer::Append( const CStringW& other )
	{
		return this->Append((LPVOID)other.GetString(),other.GetLength()*sizeof(wchar_t));
	}

	CMemBuffer& CMemBuffer::Append( const CMemBuffer& other )
	{
		return this->Append((LPVOID)other.GetData(),other.GetDataLength());
	}

	CMemBuffer& CMemBuffer::Append( const LPCSTR pData )
	{
		return this->Append((LPVOID)pData,strlen(pData));
	}

	CMemBuffer& CMemBuffer::Append( const LPWSTR pData )
	{
		return this->Append((LPVOID)pData,wcslen(pData)*2);
	}

	UINT CMemBuffer::GetDataLength( void ) const
	{
		if (m_pMemBuffer!=NULL)
		{
			return m_pMemBuffer->nDataLength;
		}

		return 0;
	}

	char* CMemBuffer::GetData( void ) const
	{
		if (m_pMemBuffer!=NULL)
		{
			return m_pMemBuffer->pData;
		}
		return NULL;
	}

	CMemBuffer& CMemBuffer::operator+=( const CStringA& other )
	{
		return this->Append((LPVOID)other.GetString(),other.GetLength());
	}

	CMemBuffer& CMemBuffer::operator+=( const CStringW& other )
	{
		return this->Append((LPVOID)other.GetString(),other.GetLength()*sizeof(wchar_t));
	}

	CMemBuffer& CMemBuffer::operator+=( const CMemBuffer& other )
	{
		return this->Append(other);
	}

	CMemBuffer& CMemBuffer::operator=( const CMemBuffer& other )
	{
		if (other.m_pMemBuffer==NULL)
			return (*this);

		if (this!=&other)	// 不同对象
		{
			if (this->m_pMemBuffer != other.m_pMemBuffer)	// MemBufferBlock不同
			{	
				if (this->m_pMemBuffer->nBufferLength >=other.m_pMemBuffer->nDataLength) // 当前缓冲区有足够空间容纳数据，拷贝数据
				{	
					memset(m_pMemBuffer->pData,0,m_pMemBuffer->nBufferLength);

					if(other.m_pMemBuffer->nDataLength>0)	// 有数据才复制
						memcpy(m_pMemBuffer->pData,other.m_pMemBuffer->pData,other.m_pMemBuffer->nDataLength);

					m_pMemBuffer->nDataLength=other.m_pMemBuffer->nDataLength;
					return (*this);
				}

				// 当前缓冲区不够容纳数据，清理MemBlock，引用other的MemBlock
				this->ReAlloc(other.GetDataLength(),false);
				this->Append(other);
			}
		}
		return (*this);
	}

	void CMemBuffer::SetDataLength( UINT nDataLength )
	{
		if (m_pMemBuffer!=NULL && nDataLength <= m_pMemBuffer->nBufferLength )
		{
			m_pMemBuffer->nDataLength=nDataLength;
		}
	}

	void CMemBuffer::Empty( void )
	{
		if (m_pMemBuffer!=NULL && m_pMemBuffer->nDataLength >0 )
		{
			memset(m_pMemBuffer->pData,0,m_pMemBuffer->nBufferLength);
			m_pMemBuffer->nDataLength=0;
		}
	}

	bool CMemBuffer::IsEmpty( void )
	{
		if (m_pMemBuffer!=NULL)
			return m_pMemBuffer->nDataLength ==0 ;
		return true;
	}

	UINT CMemBuffer::GetBufferBlockLength( void ) const
	{
		if (m_pMemBuffer!=NULL)
			return m_pMemBuffer->nBufferLength;
		else
			return 0;
	}

	char& CMemBuffer::operator[]( int index )
	{
		if (index>=(int)GetDataLength())
		{
			return chNull;
		}

		return GetData()[index];
	}

	CMemBuffer::operator char *(void)
	{
		if ( m_pMemBuffer != NULL && m_pMemBuffer->nBufferLength >0)
			return m_pMemBuffer->pData;
		return NULL;
	}

	BOOL CMemBuffer::Create()
	{
		if ( m_pMemBuffer==NULL)
			return Alloc(8192);

		return TRUE;
	}

	BOOL CMemBuffer::SetLength( UINT iLength )
	{
		return ReAlloc((UINT)iLength,true);
	}

	BOOL CMemBuffer::Read( LPVOID lpBuffer,DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead )
	{
		*lpNumberOfBytesRead = 0;
		if (m_pMemBuffer ==NULL)
			return FALSE;

		if ( m_pMemBuffer->nDataLength <=nNumberOfBytesToRead)
		{
			memcpy(lpBuffer,m_pMemBuffer->pData,m_pMemBuffer->nDataLength);
			*lpNumberOfBytesRead = m_pMemBuffer->nDataLength;
		}
		else
		{
			memcpy(lpBuffer,m_pMemBuffer->pData, nNumberOfBytesToRead);
			*lpNumberOfBytesRead = nNumberOfBytesToRead;
		}

		return TRUE;
	}

	BOOL CMemBuffer::Write( UINT iPosition, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten )
	{
		return FALSE;
	}

	BOOL CMemBuffer::Write( LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten )
	{
		Append((LPVOID)lpBuffer,nNumberOfBytesToWrite);
		return TRUE;
	}

	VOID CMemBuffer::Close()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	UINT CMemBuffer::GetLength()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	UINT CMemBuffer::GetBufferLength( void ) const
	{
		if( m_pMemBuffer ==NULL)
			return 0;
		return m_pMemBuffer->nBufferLength;
	}

}

