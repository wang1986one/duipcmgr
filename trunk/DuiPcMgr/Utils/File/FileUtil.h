#pragma once
#include "Utils/MemBuffer.h"

namespace Utils
{
	//���ص�ǰģ������·��
	CString GetAppPath(void);
	CString GetAppFullPath(void);

	//�ж��ļ��Ƿ����
	BOOL ExistsFile(const LPCTSTR szPath);
	BOOL ExistsDir(const LPCTSTR szPath); 

	// ��һ������·��ת��ΪĿ¼
	CString GetPathString(const LPCTSTR szPath);

	//�ж��ļ��޸������Ƿ��Ѿ���ʱ
	//dwMaxLiveInDays: �������ޣ���λ��
	BOOL isFileExpired(const LPCTSTR szFile, DWORD dwMaxLiveInDays);

	//����ָ���ļ��Ĵ�С����λ�ֽ�,С��0��ζ��ʧ��
	__int64 GetFileSize(const LPCTSTR szFile);
	__int64 GetFileSize(FILE* fp);

	// �ݹ鴴���༶Ŀ¼
	BOOL MakeDirectory(const LPCTSTR pszDir);
	// �ݹ�ɾ���༶Ŀ¼
	BOOL DeleteDirectory(const LPCTSTR pszDir,BOOL bDeleteRoot = TRUE);
	
	//��ȡָ��·�����ļ�������outBuffer�� 
	//��NULL��β
	BOOL ReadFile(const LPCTSTR szFile, CMemBuffer *outBuffer, unsigned __int64 nOffset=0, __int64 nReadCount=-1);

	//����д���ļ�
	BOOL WriteFile(const LPCTSTR szFile, CMemBuffer &inBuffer);
	//���ǻ�������д���ļ�
	BOOL WriteFile(const LPCTSTR szFile, LPVOID pData, DWORD dwLength, bool isAppend);

	//�����ʱ�ļ���
	CString GetTempPath();
	CString GetTempFileName(const LPCTSTR lpszTempPath=NULL, const LPCTSTR lpszPrefix=NULL, long wUnique=0 );
	
	// �ͷ���Դ
	BOOL ExtractFileFromPE(const LPCTSTR lpszPEFilePath ,const LPCTSTR lpszType, LPCTSTR lpszName, LPCTSTR lpszDestFilePath); 
	BOOL ExtractFileFromResource( const LPCTSTR lpszType, LPCTSTR lpszName, LPCTSTR lpszDestFilePath); 

	// �������ļ�����
	BOOL RestartDeleteFile(const LPCTSTR lpszFilePath);
	// �������ƶ��ļ�
	BOOL RestartMoveFile(const LPCTSTR lpszFilePath, const LPCTSTR lpszNewFilePath,BOOL bRelpaseExisting=TRUE); 
}