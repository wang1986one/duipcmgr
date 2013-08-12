#pragma once
#include "Utils/MemBuffer.h"

namespace Utils
{
	//返回当前模块所在路径
	CString GetAppPath(void);
	CString GetAppFullPath(void);

	//判断文件是否存在
	BOOL ExistsFile(const LPCTSTR szPath);
	BOOL ExistsDir(const LPCTSTR szPath); 

	// 将一个完整路径转换为目录
	CString GetPathString(const LPCTSTR szPath);

	//判断文件修改日期是否已经超时
	//dwMaxLiveInDays: 声明上限，单位天
	BOOL isFileExpired(const LPCTSTR szFile, DWORD dwMaxLiveInDays);

	//返回指定文件的大小，单位字节,小于0意味着失败
	__int64 GetFileSize(const LPCTSTR szFile);
	__int64 GetFileSize(FILE* fp);

	// 递归创建多级目录
	BOOL MakeDirectory(const LPCTSTR pszDir);
	// 递归删除多级目录
	BOOL DeleteDirectory(const LPCTSTR pszDir,BOOL bDeleteRoot = TRUE);
	
	//读取指定路径的文件数据至outBuffer中 
	//以NULL结尾
	BOOL ReadFile(const LPCTSTR szFile, CMemBuffer *outBuffer, unsigned __int64 nOffset=0, __int64 nReadCount=-1);

	//覆盖写入文件
	BOOL WriteFile(const LPCTSTR szFile, CMemBuffer &inBuffer);
	//覆盖或者质量写入文件
	BOOL WriteFile(const LPCTSTR szFile, LPVOID pData, DWORD dwLength, bool isAppend);

	//获得临时文件夹
	CString GetTempPath();
	CString GetTempFileName(const LPCTSTR lpszTempPath=NULL, const LPCTSTR lpszPrefix=NULL, long wUnique=0 );
	
	// 释放资源
	BOOL ExtractFileFromPE(const LPCTSTR lpszPEFilePath ,const LPCTSTR lpszType, LPCTSTR lpszName, LPCTSTR lpszDestFilePath); 
	BOOL ExtractFileFromResource( const LPCTSTR lpszType, LPCTSTR lpszName, LPCTSTR lpszDestFilePath); 

	// 重启后文件操作
	BOOL RestartDeleteFile(const LPCTSTR lpszFilePath);
	// 重启后移动文件
	BOOL RestartMoveFile(const LPCTSTR lpszFilePath, const LPCTSTR lpszNewFilePath,BOOL bRelpaseExisting=TRUE); 
}