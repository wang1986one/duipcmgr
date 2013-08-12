#ifndef __FileVersion_H__
#define __FileVersion_H__
#pragma once

namespace Utils
{
	BOOL GetPEFileVersion(LPCTSTR lpszFileName, ULARGE_INTEGER *pullVersion);
	BOOL GetFileVersion(LPCTSTR lpszFileName, ULARGE_INTEGER *pullVersion);
	BOOL GetFileVersionString(ULARGE_INTEGER ullVersion, CString &strVersion);
	BOOL GetFileVersion(LPCTSTR lpszFileName, CString& strVersion);
	ULARGE_INTEGER GetDigitalVersionByString(LPCTSTR lpszVersion);
	BOOL VersionLess(LPCTSTR lpszVersionLeft, LPCTSTR lpszVersionRight);
}

#endif // __FileVersion_H__

