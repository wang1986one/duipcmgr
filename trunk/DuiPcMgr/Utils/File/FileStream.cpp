#include "StdAfx.h"
#include "FileStream.h"

namespace Utils
{
	CFileStream::CFileStream( LPCTSTR szFilename )
		: m_hFile(NULL)
		, m_strFilePath(szFilename)
		, m_iFileLength(0)
	{
	}

	CFileStream::~CFileStream()
	{
		Close();
	}

	BOOL CFileStream::Create()
	{
		if ( m_hFile )
			return TRUE;

		m_hFile = ::CreateFile( m_strFilePath, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, NULL );
		if (INVALID_HANDLE_VALUE == m_hFile)
			m_hFile = NULL;
		return m_hFile != NULL;
	}

	BOOL CFileStream::SetLength( UINT iFileSize )
	{
		if ( !m_hFile )
			return FALSE;

		m_iFileLength = iFileSize;
		uint64 iFreeBytesToCaller = 0;
		TCHAR szDriver[4] = { PathGetDriveNumber( m_strFilePath ) + _T( 'A' ), _T( ':' ), _T( '\\' ), _T( '\0' ) };
		if ( GetDiskFreeSpaceEx( szDriver, ( PULARGE_INTEGER )&iFreeBytesToCaller, NULL, NULL ) )
		{
			if (m_iFileLength > 0 && iFreeBytesToCaller > m_iFileLength )
			{
				long iPosHigh = HIINT32( m_iFileLength );
				if ( INVALID_SET_FILE_POINTER == SetFilePointer( m_hFile, LOINT32( m_iFileLength ), &iPosHigh, FILE_BEGIN ) && NO_ERROR != GetLastError() )
				{
					SAFE_CLOSE_HANDLE(m_hFile);
					return FALSE;
				}
				return TRUE;
			}
		}
		return FALSE;
	}

	void CFileStream::Close()
	{
		SAFE_CLOSE_HANDLE( m_hFile );
	}

	BOOL CFileStream::Write( UINT iPosition, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten )
	{
		ATLASSERT(lpNumberOfBytesWritten);
		long iPosHigh = HIINT32( iPosition );
		if ( INVALID_SET_FILE_POINTER == SetFilePointer( m_hFile, LOINT32( iPosition ), &iPosHigh, FILE_BEGIN ) && NO_ERROR != GetLastError() )
			return FALSE;

		DWORD dwWritten = 0;
		if(!lpNumberOfBytesWritten)
			lpNumberOfBytesWritten = &dwWritten;
		*lpNumberOfBytesWritten = 0;
		if ( WriteFile( m_hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, NULL ) && *lpNumberOfBytesWritten == nNumberOfBytesToWrite )
		{
			return TRUE;
		}
		return FALSE;
	}

	BOOL CFileStream::Write( LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten )
	{
		return ::WriteFile( m_hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, NULL );
	}

	CFileInStream::CFileInStream( LPCTSTR szFilename )
		:m_strFilePath(szFilename)
		, m_hFile(NULL)
		, m_iFileLength(0)
	{
	}

	CFileInStream::~CFileInStream()
	{
		Close();
	}

	BOOL CFileInStream::Create()
	{
		if ( m_hFile )
			return TRUE;

		m_hFile = ::CreateFile( m_strFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL );
		if (INVALID_HANDLE_VALUE == m_hFile)
			m_hFile = NULL;
		return m_hFile != NULL;
	}

	void CFileInStream::Close()
	{
		SAFE_CLOSE_HANDLE( m_hFile );
	}

	BOOL CFileInStream::Read( LPVOID lpBuffer,DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead )
	{
		if(!m_hFile)
			return FALSE;
		ATLASSERT(lpNumberOfBytesRead);
		return ReadFile(m_hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, 0);
	}

	UINT CFileInStream::GetLength()
	{
		if(!m_hFile)
			return 0;
		DWORD dwLow=0, dwHigh=0;
		LARGE_INTEGER ll = {0};
		dwLow = ::GetFileSize(m_hFile, &dwHigh);
		ll.LowPart = dwLow;
		ll.HighPart = dwHigh;
		return (UINT)ll.QuadPart;
	}

	BOOL IDataStream::Create()
	{
		return FALSE;
	}

	VOID IDataStream::Close()
	{
	}

	BOOL IDataStream::Write( LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten )
	{
		if( lpNumberOfBytesWritten !=NULL)
			*lpNumberOfBytesWritten=0;
		return TRUE;
	}

	BOOL IDataStream::Write( UINT iPosition, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten )
	{
		if( lpNumberOfBytesWritten !=NULL)
			*lpNumberOfBytesWritten=0;
		return TRUE;
	}

	BOOL IDataStream::Read( LPVOID lpBuffer,DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead )
	{
		if( lpNumberOfBytesRead !=NULL)
			*lpNumberOfBytesRead=0;
		return TRUE;
	}

	UINT IDataStream::GetLength()
	{
		return 0;
	}

	BOOL IDataStream::SetLength( UINT iLength )
	{
		return FALSE;
	}
}

