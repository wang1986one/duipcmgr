#include "stdafx.h"
#include "MainWnd.h"
#include "Utils/MemBuffer.h"
#include <ShellAPI.h>
#include "Utils/File/FileUtil.h"

CMainWnd::CMainWnd(void)
{
}

CMainWnd::~CMainWnd(void)
{
	::PostQuitMessage(0);
}

LPCTSTR CMainWnd::GetSkinFolder()
{
	return _T("RDBHelper\\");
}

LPCTSTR CMainWnd::GetSkinFile()
{
	return _T("MainWnd.xml");
}

BOOL CMainWnd::OnEscapeCloseWindow( void ) const
{
	return FALSE;
}

void CMainWnd::OnClick( TNotifyUI* pMsg )
{
	CString strControlName = pMsg->pSender->GetName();
	if (strControlName.Compare(_T("RdbInBtn"))==0)		// 浏览Rdb文件
	{

		return;
	}
	else if (strControlName.Compare(_T("RdbOutDocumentBtn"))==0)	// 浏览Rdb输出文件夹
	{
		return;
	}
	else if (strControlName.Compare(_T("StartRdbParser"))==0)		// 开始解压
	{
		CString strRdbFile=m_pRdbIn->GetText();
		CString strTargetFolder=m_pRdbOutFolder->GetText();

		if ( !m_RdbFile.CheckRdb(strRdbFile) )
		{
			// 错误提示
			return;
		}
		CRdbFile file;
		m_RdbFile.Unpack(strRdbFile,strTargetFolder);
		return;
	}

	else if (strControlName.Compare(_T("DocumentInBtn"))==0)		// 浏览带打包文件夹
	{
		return;
	}
	else if (strControlName.Compare(_T("RdbOutBtn"))==0)	// 浏览Rdb文件输出
	{
		return;
	}
	else if (strControlName.Compare(_T("StartRdbPackage"))==0)		// 开始压缩
	{
		return;
	}
}

void CMainWnd::InitWindow()
{
	::DragAcceptFiles(m_hWnd,TRUE);
	m_pParserRdbFileLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("ParserRdbFileLayout")));
	m_pParserRdbDocumentLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("ParserRdbDocumentLayout")));
	m_pPackageRdbFileLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("PackageRdbFileLayout")));
	m_pPackageRdbDocumentLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("PackageRdbDocumentLayout")));

	m_pRdbIn = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("RdbIn")));
	m_pRdbOutFolder = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("RdbOutFolder")));
}

void CMainWnd::OnFinalMessage( HWND hWnd )
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

void CMainWnd::Notify( TNotifyUI &msg )
{
	if (msg.sType==_T("click"))
	{
		CString strControlName=msg.pSender->GetName();
		if (strControlName.CompareNoCase(_T("btnClose"))==0)
		{
			Close();
			return;
		}
		else if (strControlName.CompareNoCase(_T("btnMin"))==0)
		{
			::PostMessage(m_hWnd,WM_SYSCOMMAND,SC_MINIMIZE,0);
			return;
		}
		else
			OnClick(&msg);
	}
}

DuiLib::UILIB_RESOURCETYPE CMainWnd::GetResourceType() const
{
#ifdef _DEBUG
	return UILIB_FILE;
#else
	return UILIB_ZIPRESOURCE;
#endif
}

LPCTSTR CMainWnd::GetResID() const
{
	return MAKEINTRESOURCE(102);
}

LPCTSTR CMainWnd::GetResType() const
{
	return _T("RDB001");
}

LRESULT CMainWnd::HandleCustomMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bHandled=TRUE;
	if ( uMsg == WM_DROPFILES)
	{
		if ( OnDropFiles((HDROP)wParam))
			return S_OK;
	}
	bHandled =FALSE;
	return S_OK;
}

bool CMainWnd::OnDropFiles( HDROP hDropInfo )
{
	UINT nFilesCount=DragQueryFile(hDropInfo,INFINITE,NULL,0);
	if ( nFilesCount > 1)		// 多文件，不符合要求，不处理
		return false;

	CDuiPoint pt;
	CDuiRect rect;
	bool bInParserRdbFileLayout=false;
	bool bInParserRdbDocumentLayout=false;
	bool bInPackageRdbFileLayout=false;
	bool bInPackageRdbDocumentLayout=false;

	::DragQueryPoint(hDropInfo,&pt);
	
	rect=m_pParserRdbFileLayout->GetPos();
	if ( rect.PtInRect(pt) == true)
		bInParserRdbFileLayout = true;

	rect=m_pParserRdbDocumentLayout->GetPos();
	if ( rect.PtInRect(pt) == true)
		bInParserRdbDocumentLayout = true;

	rect=m_pPackageRdbFileLayout->GetPos();
	if ( rect.PtInRect(pt) == true)
		bInPackageRdbFileLayout = true;

	rect=m_pPackageRdbDocumentLayout->GetPos();
	if ( rect.PtInRect(pt) == true)
		bInPackageRdbDocumentLayout = true;

	if ( !bInParserRdbFileLayout && 
		!bInParserRdbDocumentLayout && 
		!bInPackageRdbFileLayout && 
		!bInPackageRdbDocumentLayout )
	{	// 不在四个接受拖动区域内，不处理
		return false;
	}

	CString strFile;
	bool bIsFolder=false;
	int pathLen = DragQueryFile(hDropInfo, 0, strFile.GetBuffer(MAX_PATH), MAX_PATH);
	strFile.ReleaseBuffer(pathLen);
	DWORD dwFileAttr = ::GetFileAttributes(strFile);
	if ((dwFileAttr & FILE_ATTRIBUTE_DIRECTORY)==FILE_ATTRIBUTE_DIRECTORY)
	{	// 是文件夹
		bIsFolder=true;
	}
	::DragFinish(hDropInfo);

	if ( bInParserRdbFileLayout)
	{	// 需要一个Rdb文件
		if ( bIsFolder)	// 是目录，不处理
			return false;

		if ( m_RdbFile.CheckRdb(strFile) )	// 确定为Rdb文件
		{
			m_pRdbIn->SetText(strFile);
			m_pRdbIn->SetToolTip(strFile);

			int iPos=strFile.ReverseFind(_T('.'));
			if ( iPos!=-1)
			{
				m_pRdbOutFolder->SetText(strFile.Left(iPos));
			}
			else
			{
				m_pRdbOutFolder->SetText(strFile);
			}

			return true;
		}
	}

	if ( bInParserRdbDocumentLayout)
	{
		if ( !bIsFolder)	// 不是目录，不处理
			return false;

		m_pRdbOutFolder->SetText(strFile);
		m_pRdbOutFolder->SetToolTip(strFile);
	}

	return false;
}

