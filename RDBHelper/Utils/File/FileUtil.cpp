#include "StdAfx.h"
#include <io.h>
#include "Utils/StringUtil.h"
#include "FileUtil.h"

CString Utils::GetAppPath(void)
{
	static CString s_appPath;
	if (s_appPath.IsEmpty())
	{
		s_appPath=GetAppFullPath();
		if (s_appPath.GetLength()>0)
		{
			int pos = s_appPath.ReverseFind(_T('\\'));
			if (pos>=0)
			{
				s_appPath.ReleaseBufferSetLength(pos+1);
			}
		}
	}
	return s_appPath;
}

CString Utils::GetAppFullPath(void)
{
	static CString s_appFullPath;
	if (s_appFullPath.IsEmpty())
	{
		s_appFullPath.ReleaseBufferSetLength(::GetModuleFileName(NULL, s_appFullPath.GetBuffer(MAX_PATH), MAX_PATH));
	}
	return s_appFullPath.GetString();
}

BOOL Utils::ExistsFile(const LPCTSTR szPath )
{
	return ::PathFileExists(szPath);
}

BOOL Utils::ExistsDir( const LPCTSTR szPath )
{
	return ::PathIsDirectory(szPath);
}

BOOL Utils::isFileExpired( const LPCTSTR szFile, DWORD dwMaxLiveInDays )
{
	BOOL result = TRUE;//默认是超时的
	HANDLE hFile = ::CreateFile(szFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile!=INVALID_HANDLE_VALUE)
	{
		// get time current file size
		FILETIME ftCreate, ftAccess, ftModify;
		if (::GetFileTime(hFile, &ftCreate, &ftAccess, &ftModify))
		{
			unsigned __int64 qwFileMTime = *((unsigned __int64*)&ftModify);
			unsigned __int64 ftNow;
			GetSystemTimeAsFileTime((LPFILETIME)&ftNow);
			double diff = _abs64(ftNow - qwFileMTime) /10000.0 / 1000.0 / 3600.0 / 24.0;
			if (diff<dwMaxLiveInDays) {
				result = FALSE;
			}
		}
		::CloseHandle(hFile);
	}
	return result;
}

__int64 Utils::GetFileSize( const LPCTSTR szFile )
{
	LARGE_INTEGER li64;
	li64.QuadPart = 0;
	HANDLE hFile = ::CreateFile(szFile, GENERIC_READ, FILE_SHARE_WRITE|FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile!=INVALID_HANDLE_VALUE)
	{
		li64.LowPart = ::GetFileSize(hFile, (LPDWORD)&li64.HighPart);//单位Bytes
		::CloseHandle(hFile);
	}
	return li64.QuadPart;
}

__int64 Utils::GetFileSize( FILE* fp )
{
	__int64 result = -1;
	if (fp)
	{
		result = _filelengthi64(fp->_file);
	}
	return result;
}

BOOL Utils::MakeDirectory( const LPCTSTR pszDir )
{
	BOOL bResult = FALSE;
	TCHAR tmpPath[MAX_PATH]={0};
	_tcscpy_s(tmpPath,MAX_PATH,pszDir);
	::PathRemoveFileSpec(tmpPath);
	CString szDir;
	Utils::VString vDir = Utils::SplitStr(tmpPath, _T('\\'));
	if (vDir.size()>=2)
	{
		szDir = vDir[0];
		for (size_t i=1;i<vDir.size();++i)
		{
			szDir.AppendFormat(_T("\\%s"), vDir[i]);
			bResult = ::CreateDirectory(szDir, NULL);
		}
		if (!bResult && ::GetLastError()==ERROR_ALREADY_EXISTS)
		{
			bResult = TRUE;//忽略已存在的错误
		}
	}
	return bResult;//以最后一次创建目录的返回值为准
}

BOOL Utils::ReadFile( const LPCTSTR szFile, CMemBuffer *outBuffer, unsigned __int64 nOffset/*=0*/, __int64 nReadCount/*=-1*/ )
{
	HANDLE hFile=::CreateFile(szFile,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile!=INVALID_HANDLE_VALUE && hFile!=NULL)
	{
		DWORD dwFileSize=::GetFileSize(hFile,NULL);

		if (nReadCount== -1)
			outBuffer->SetDataLength(dwFileSize+1);
		else
			outBuffer->SetDataLength((UINT)nReadCount+1);

		DWORD dwRead=0;
		if (::ReadFile(hFile,outBuffer->GetData(),(DWORD)outBuffer->GetDataLength()-1,&dwRead,NULL) && dwRead==outBuffer->GetDataLength()-1)
		{
			(*outBuffer)[outBuffer->GetDataLength()+1]='\0';
			::CloseHandle(hFile);
			hFile=NULL;
			return TRUE;
		}
		else
		{
			::CloseHandle(hFile);
			hFile=NULL;
			return FALSE;
		}
	}
	return FALSE;
}

BOOL Utils::WriteFile( const LPCTSTR szFile, CMemBuffer &inBuffer )
{
	return Utils::WriteFile(szFile,inBuffer.GetData(),(DWORD)inBuffer.GetDataLength(),false);
}

BOOL Utils::WriteFile( const LPCTSTR szFile, LPVOID pData, DWORD dwLength, bool isAppend )
{
	BOOL bRet=FALSE;
	HANDLE hFile=::CreateFile(szFile,GENERIC_ALL,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile!=INVALID_HANDLE_VALUE && hFile!=NULL)
	{
		if (isAppend)
		{
			::SetFilePointer(hFile,0,0,FILE_END);
		}
		DWORD dwWrite=0;
		if (::WriteFile(hFile,pData,dwLength,&dwWrite,NULL) && dwWrite==dwLength)
		{
			::FlushFileBuffers(hFile);
			bRet=TRUE;
		}
		else
		{
			bRet=FALSE;
		}
		::CloseHandle(hFile);
		hFile=NULL;
	}
	return bRet;
}

CString Utils::GetTempPath()
{
	CString szTempPath;
	DWORD dwPath=::GetTempPath(MAX_PATH,szTempPath.GetBuffer(MAX_PATH));
	szTempPath.ReleaseBuffer(dwPath);
	return szTempPath;
}

CString Utils::GetTempFileName( const LPCTSTR lpszTempPath/*=NULL*/, const LPCTSTR lpszPrefix/*=NULL*/, long wUnique/*=0 */ )
{
	CString szTempFileName;
	if (lpszTempPath==NULL)
		::GetTempFileName(GetTempPath(),lpszPrefix,wUnique,szTempFileName.GetBuffer(MAX_PATH));
	else
		::GetTempFileName(lpszTempPath,lpszPrefix,wUnique,szTempFileName.GetBuffer(MAX_PATH));
	szTempFileName.ReleaseBuffer();
	return szTempFileName;
}

BOOL Utils::RestartMoveFile( const LPCTSTR lpszFilePath, const LPCTSTR lpszNewFilePath,BOOL bRelpaseExisting/*=TRUE*/ )
{
	return ::MoveFileEx(
		lpszFilePath,
		lpszNewFilePath,
		MOVEFILE_DELAY_UNTIL_REBOOT | 
		bRelpaseExisting ? MOVEFILE_REPLACE_EXISTING : 0);
}

BOOL Utils::RestartDeleteFile( const LPCTSTR lpszFilePath )
{
	return  ::MoveFileEx(
		lpszFilePath,
		NULL,
		MOVEFILE_DELAY_UNTIL_REBOOT);
}

CString Utils::GetPathString( const LPCTSTR szPath )
{
	TCHAR szPathBuffer[MAX_PATH]={0};
	_tcscpy_s(szPathBuffer, _countof(szPathBuffer), szPath);
	::PathRemoveFileSpec(szPathBuffer);
	return CString(szPathBuffer);
}

BOOL Utils::ExtractFileFromPE( const LPCTSTR lpszPEFilePath ,const LPCTSTR lpszType, LPCTSTR lpszName, LPCTSTR lpszDestFilePath )
{
	HMODULE hModule;
	if (lpszPEFilePath!=NULL)
	{
		hModule=::LoadLibrary(lpszPEFilePath);
		if (hModule==NULL)
			return FALSE;
	}
	else
		hModule=NULL;

	HRSRC hResource=::FindResource(hModule,lpszName,lpszType);
	HGLOBAL hGlobal=::LoadResource(hModule,hResource);
	DWORD dwResourceSize=::SizeofResource(hModule,hResource);
	LPVOID lpData=::LockResource(hGlobal);
	if(lpData==NULL)
		return FALSE;

	BOOL bRet=Utils::WriteFile(lpszDestFilePath,lpData,dwResourceSize,false);
	::FreeResource(hGlobal);
	return bRet;
}

BOOL Utils::ExtractFileFromResource( const LPCTSTR lpszType, LPCTSTR lpszName, LPCTSTR lpszDestFilePath )
{
	return ExtractFileFromPE(NULL,lpszType,lpszName,lpszDestFilePath);
}

BOOL Utils::DeleteDirectory( const LPCTSTR pszDir,BOOL bDeleteRoot /*= TRUE*/ )
{
	HANDLE hFind; // file handle
	WIN32_FIND_DATA FindFileData;
	TCHAR PathToSearchInto [MAX_PATH] = {0};

	// Construct the path to search into "C:\\Windows\\System32\\*"
	_tcscpy(PathToSearchInto, pszDir);
	_tcscat(PathToSearchInto, _T("\\*"));

	hFind = ::FindFirstFile(PathToSearchInto,&FindFileData); // find the first file
	if(hFind == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	while(true) // until we finds an entry
	{
		if(::FindNextFile(hFind,&FindFileData))
		{
			// Don't care about . and ..
			if ((_tcscmp(FindFileData.cFileName, _T(".")) == 0) ||
				(_tcscmp(FindFileData.cFileName, _T("..")) == 0))
				continue;

			// We have found a directory
			if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				TCHAR AbsolutePathNewFileFound[MAX_PATH] = {0};
				_tcscat(AbsolutePathNewFileFound, pszDir);
				_tcscat(AbsolutePathNewFileFound, _T("\\"));
				_tcscat(AbsolutePathNewFileFound, FindFileData.cFileName);

				// Recursive call with the new directory found
				DeleteDirectory(AbsolutePathNewFileFound);
				::RemoveDirectory(AbsolutePathNewFileFound);
			}
			else	// 是一个文件，删除
			{
				TCHAR AbsolutePathNewFileFound[MAX_PATH] = {0};

				_tcscat(AbsolutePathNewFileFound, pszDir);
				_tcscat(AbsolutePathNewFileFound, _T("\\"));
				_tcscat(AbsolutePathNewFileFound, FindFileData.cFileName);
				::DeleteFile(AbsolutePathNewFileFound);
			}

		}//FindNextFile
		else
		{
			break;
		}
	}//while

	::FindClose(hFind); // closing file handle
	if ( bDeleteRoot == TRUE)
		::RemoveDirectory(pszDir);
	return TRUE;
}

