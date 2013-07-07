#pragma once

namespace Utils
{

	class CFileStream
		: public IDataStream
	{
	public:
		CFileStream(LPCTSTR szFilename);
		virtual ~CFileStream();
		virtual BOOL SetLength(UINT iLength);
		virtual BOOL Create();
		virtual BOOL Write(UINT iPosition, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten);
		virtual BOOL Write(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten);
		virtual VOID  Close();

	protected:
		HANDLE m_hFile;
		UINT m_iFileLength;
		CString m_strFilePath;
	};

	class CFileInStream
		: IDataStream
	{
	public:
		CFileInStream(LPCTSTR szFilename);
		~CFileInStream();
		virtual BOOL Create();
		virtual BOOL Read(LPVOID lpBuffer,DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead);
		virtual void Close();
		virtual UINT GetLength();

	protected:
		HANDLE m_hFile;
		UINT m_iFileLength;
		CString m_strFilePath;
	};
}

