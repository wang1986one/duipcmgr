#include "stdafx.h"
#include "RdbFile.h"

#include "Utils/MemBuffer.h"
using Utils::CMemBuffer;

CRdbFile::CRdbFile(void)
{
}

CRdbFile::~CRdbFile(void)
{
}

int CRdbFile::Unpack( LPCTSTR lpszRbdPath,LPCTSTR lpszTargetFolder )
{
	if ( !Utils::ExistsFile(lpszRbdPath))
		return -1;		//  文件不存在

	Utils::CFileInStream fs(lpszRbdPath);
	if ( !fs.Create() || fs.GetLength() < sizeof(RdbFileHeader))
		return -2;

	CMemBuffer dataBuffer;
	DWORD dwNumberOfBytesRead=0;

	fs.Read(dataBuffer.GetData(),sizeof(RdbFileHeader),&dwNumberOfBytesRead);
	dataBuffer.SetDataLength(dwNumberOfBytesRead);
	if ( dwNumberOfBytesRead != sizeof(RdbFileHeader))
		return -3;

	PRdbFileHeader pRdbHeader=(PRdbFileHeader)dataBuffer.GetData();
	if ( strncmp(pRdbHeader->strFlags,"531E98204F8542F0",0x10) !=0)
		return -4;	// 不是QQ rdb文件

	RdbFileHeader rdbFileHeander;
	rdbFileHeander.FileCount=pRdbHeader->FileCount;
	rdbFileHeander.IndexOffset=pRdbHeader->IndexOffset;
	rdbFileHeander.DataOffset=pRdbHeader->DataOffset;

	g_Logger.Info(_T("文件数：%d 索引区数据偏移：%ld 文件区数据偏移：%ld"),rdbFileHeander.FileCount,rdbFileHeander.IndexOffset,rdbFileHeander.DataOffset);

	g_Logger.Info(_T("解析文件索引"));
	fs.Seek((DWORD)rdbFileHeander.IndexOffset);

	vector<RdbFileItem> itemList;
	for ( int i=0;i<rdbFileHeander.FileCount;i++)
	{
		RdbFileItem item;
		item.FileName= ReadFileName(fs);

		PRdbFileItem pFileItem=NULL;
		dataBuffer.Clear();
		fs.Read(dataBuffer.GetData(),sizeof(RdbFileItem)-4 ,&dwNumberOfBytesRead);
		dataBuffer.SetDataLength(dwNumberOfBytesRead);
		pFileItem=(PRdbFileItem)dataBuffer.GetData();
		item.dataOffset = pFileItem->dataOffset;
		item.FileLength = pFileItem->FileLength;
		itemList.push_back(item);
	}
	g_Logger.Info(_T("文件索引解析完成，展开文件"));

	for ( int i=0;i<rdbFileHeander.FileCount;i++)
	{
		g_Logger.Info(_T("%s\t长度：%ld"),itemList[i].FileName,itemList[i].FileLength);

		CMemBuffer fileDataBuffer((UINT)itemList[i].FileLength);
		fs.Read(fileDataBuffer.GetData(),(DWORD)itemList[i].FileLength,&dwNumberOfBytesRead);
		fileDataBuffer.SetDataLength(dwNumberOfBytesRead);

		bool bIsProcessed = false;
		if ( itemList[i].FileName.Right(4).CompareNoCase(_T(".gft")) == 0)
		{	// gft格式处理为png
			CString strFilePath;
			strFilePath.Format(_T("%s\\%s.png"), lpszTargetFolder, itemList[i].FileName.Left(itemList[i].FileName.GetLength()-4));
			Utils::MakeDirectory(strFilePath);

			// 89 50 4E 47 0D 0A 1A 0A	Png图片头
			// 为减少计算量，只在头50个字节查找PNG标志
			UINT uCount= dwNumberOfBytesRead < 50 ? dwNumberOfBytesRead : 50;
			for (UINT i=0; i<uCount ; i++)
			{
				if ( (UCHAR)fileDataBuffer[i] == 0x89 &&		// 0x89		137
					fileDataBuffer[i+1] == 0x50 &&	// 0x50		80
					fileDataBuffer[i+2] == 0x4E &&	// 0x4E		78
					fileDataBuffer[i+3] == 0x47 &&	// 0x47		71
					fileDataBuffer[i+4] == 0x0D &&	// 0x0D	13
					fileDataBuffer[i+5] == 0x0A &&	// 0x0A	10
					fileDataBuffer[i+6] == 0x1A &&	// 0x1A	26
					fileDataBuffer[i+7] == 0x0A )		// 0x0A	10
				{
					Utils::CFileStream fw(strFilePath);
					fw.Create();
					fw.Write(fileDataBuffer.GetData()+i,fileDataBuffer.GetDataLength() -i,&dwNumberOfBytesRead);
					fw.Close();
					bIsProcessed = true;
					break;
				}
			}
		}
		if ( bIsProcessed )
			continue;

		CString strFilePath;
		strFilePath.Format(_T("%s\\%s"), lpszTargetFolder, itemList[i].FileName);
		Utils::MakeDirectory(strFilePath);
		Utils::CFileStream fw(strFilePath);
		fw.Create();
		fw.Write(fileDataBuffer.GetData(),fileDataBuffer.GetDataLength(),&dwNumberOfBytesRead);
		fw.Close();
	}
	g_Logger.Info(_T("解包完成"));
	return 0;
}

CString CRdbFile::ReadFileName( Utils::CFileInStream& fs )
{
	WCHAR NameChar;
	CMemBuffer nameBuffer;
	DWORD dwNumberOfBytesRead;
	while( fs.Read((LPVOID)&NameChar,2,&dwNumberOfBytesRead) &&
		dwNumberOfBytesRead==2 &&
		NameChar !=0
		)
	{
		nameBuffer.Append(&NameChar,2);
	}

	return CStringW((LPCWSTR)nameBuffer.GetData());
}

bool CRdbFile::CheckRdb( LPCTSTR lpszRdbPath )
{
	if ( !Utils::ExistsFile(lpszRdbPath))
		return false;		//  文件不存在

	Utils::CFileInStream fs(lpszRdbPath);
	if ( !fs.Create() || fs.GetLength() < sizeof(RdbFileHeader))
		return false;

	CMemBuffer dataBuffer;
	DWORD dwNumberOfBytesRead=0;

	fs.Read(dataBuffer.GetData(),sizeof(RdbFileHeader),&dwNumberOfBytesRead);
	dataBuffer.SetDataLength(dwNumberOfBytesRead);
	if ( dwNumberOfBytesRead != sizeof(RdbFileHeader))
		return false;

	PRdbFileHeader pRdbHeader=(PRdbFileHeader)dataBuffer.GetData();
	if ( strncmp(pRdbHeader->strFlags,"531E98204F8542F0",0x10) !=0)
		return false;	// 不是QQ rdb文件
	else
		return true;
}
