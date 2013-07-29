#include "stdafx.h"
#include "MainWnd.h"
#include "Utils/MemBuffer.h"
#include <ShellAPI.h>
#include "Utils/File/FileUtil.h"
#include "Utils/System/Vista.h"

CMainWnd::CMainWnd(void)
: m_btnClose(NULL)
, m_btnMin(NULL)
, m_btnMax(NULL)
, m_btnRestore(NULL)
{
}

CMainWnd::~CMainWnd(void)
{
	::PostQuitMessage(0);
}

LPCTSTR CMainWnd::GetSkinFolder()
{
	return _T("Skin\\");
}

LPCTSTR CMainWnd::GetSkinFile()
{
	return _T("MainWnd.xml");
}

BOOL CMainWnd::OnEscapeCloseWindow( void ) const
{
	return FALSE;
}

void CMainWnd::OnClick( TNotifyUI &msg )
{
	CString strControlName = msg.pSender->GetName();
	//if (strControlName.Compare(_T("btnTest"))==0)
	//{

	//}
}

void CMainWnd::InitWindow()
{
	m_btnClose		= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("closebtn")));
	m_btnMin		= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("minbtn")));
	m_btnMax		= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("maxbtn")));
	m_btnRestore	= static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("restorebtn")));
}

void CMainWnd::OnFinalMessage( HWND hWnd )
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

void CMainWnd::Notify( TNotifyUI &msg )
{
	CDuiString strControlName=msg.pSender->GetName();
	if (msg.sType==_T("click"))
	{
		if ( msg.pSender == m_btnClose)
		{
			Close();
			return;
		}
		else if (msg.pSender == m_btnMin)
		{
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE , 0);
			return;
		}
		else if ( msg.pSender == m_btnMax)
		{
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE | HTCAPTION , 0);
			return;
		}
		else if ( msg.pSender == m_btnRestore)
		{
			SendMessage(WM_SYSCOMMAND, SC_RESTORE | HTCAPTION , 0);
			return;
		}
		else	// 除全局控制按钮，经常改动的功能按钮控制代码分发到独立函数
			OnClick(msg);
		return;
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
	return _T("00001");
}

LRESULT CMainWnd::HandleCustomMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	//bHandled=TRUE;
	//if ( uMsg == WM_UPDATE_NOTIFY)
	//{
	//	return S_OK;
	//}
	bHandled =FALSE;
	return S_OK;
}

LRESULT CMainWnd::OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	switch ( wParam & 0xFFF0)
	{
	case SC_CLOSE:	// 处理Alt F4
		Close();
		break;
	}
	bHandled =FALSE;
	return S_OK;
}

UINT CMainWnd::GetClassStyle() const
{
	return CS_DROPSHADOW | WindowImplBase::GetClassStyle();
}

LRESULT CMainWnd::OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	switch(wParam)
	{
	case SIZE_MAXIMIZED:		// Win7拖动标题栏也能全屏，处理Size消息才能正确控制状态
		{
			if ( m_btnMax)
				m_btnMax->SetVisible(false);
			if ( m_btnRestore)
				m_btnRestore->SetVisible(true);
		}
		break;
	case SIZE_RESTORED:
		{
			if ( ::IsMinimized(m_hWnd) ==FALSE )	// 非最小化模式恢复
			{
				if ( m_btnMax)
					m_btnMax->SetVisible(true);
				if ( m_btnRestore)
					m_btnRestore->SetVisible(false);
			}

			if ( ::IsMaximized(m_hWnd) == TRUE )	// 最大化模式恢复
			{
				if ( m_btnMax)
					m_btnMax->SetVisible(true);
				if ( m_btnRestore)
					m_btnRestore->SetVisible(false);
			}
		}
		break;
	}

	return WindowImplBase::OnSize(uMsg,wParam,lParam,bHandled);
}

