#ifndef __RdbFile_H__
#define __RdbFile_H__
/*/////////////////////////////////////////////////////////////
//
// �ļ�����	:	RdbFile.h
// ������		: 	daviyang35@QQ.com
// ����ʱ��	:	2013-8-8 10:54:01
// ����޸�	:	2013-8-8 10:54:01
// ˵��			:	Rdb�ļ��������������
//*////////////////////////////////////////////////////////////
#pragma once
#include "Utils/File/FileUtil.h"
#include "Utils/File/FileStream.h"
#include <vector>
using std::vector;

#pragma pack(push,1)
typedef struct _stRdbFileHeader
{
	char strFlags[0x10];
	INT32 FileCount;
	INT64 IndexOffset;
	INT64 DataOffset;
}RdbFileHeader,*PRdbFileHeader;

typedef struct _stRdbFileItem
{
	INT64 dataOffset;
	INT64 FileLength;
	CString FileName;
}RdbFileItem,*PRdbFileItem;

#pragma pack(pop)

class CRdbFile
{
public:
	CRdbFile(void);
	~CRdbFile(void);

	int Unpack(LPCTSTR lpszRbdPath,LPCTSTR lpszTargetFolder);
	int Pack(LPCTSTR lpszFolder,LPCTSTR lpszTargetRdb);

	bool CheckRdb(LPCTSTR lpszRdbPath);
private:
	CString ReadFileName(Utils::CFileInStream& fs);
};
#endif // __RdbFile_H__
