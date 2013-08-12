#include "StdAfx.h"
#include "TrayIcon.h"
#include <CommCtrl.h>

CIcon::CIcon()
{

}

CIcon::~CIcon()
{

}

void CIcon::LoadIcon( UINT nID )
{
	m_hIcon= ::LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(nID));
}

void CTrayIcon::Initialize()
{
	WM_TRAYICONNOTIFY = ::RegisterWindowMessageW(L"WM_TRAYICONNOTIFY");
	WM_TASKBARCREATED = ::RegisterWindowMessageW(L"TaskbarCreated");

	typedef HRESULT (CALLBACK * FnDllGetVersion)(DLLVERSIONINFO*);

	m_version.dwMajorVersion = 4;
	m_version.dwMinorVersion = 0;
	m_version.dwBuildNumber = 0;

	HMODULE hShell32 = ::GetModuleHandle(_T("shell32.dll"));
	FnDllGetVersion lpfnDllGetVersion = (FnDllGetVersion)(::GetProcAddress(hShell32, "DllGetVersion"));
	if (lpfnDllGetVersion)
	{
		m_version.cbSize = sizeof(DLLVERSIONINFO);
		lpfnDllGetVersion(&m_version);
	}
}

CTrayIcon::CTrayIcon()
{
	Initialize();
	m_IsShowIcon = true;
	memset(&m_data, 0, sizeof(NOTIFYICONDATAW));

	DWORD cbSize = sizeof(NOTIFYICONDATAW);
	if (m_version.dwMajorVersion >= 6)
	{
		if (m_version.dwMinorVersion == 0)// && m_version.dwBuildNumber < 6)
			cbSize = NOTIFYICONDATAW_V3_SIZE;
	}
	else if (m_version.dwMajorVersion >= 5)
	{
		cbSize = NOTIFYICONDATAW_V2_SIZE;
	}
	else
	{
		cbSize = NOTIFYICONDATAW_V1_SIZE;
	}

	m_data.cbSize = cbSize;
}

CTrayIcon::~CTrayIcon()
{

}

void CTrayIcon::Destroy()
{
	::KillTimer(m_hNotifyWnd,_defFlashIconTimerID);
	::KillTimer(m_hNotifyWnd,_defIconLeaveTimerID);
}

CTrayIcon::FlashIconData CTrayIcon::DecideFlashIconData()
{
	AutoLocker _lock(&m_Lock);
	if ( m_FlashStack.empty() )
	{
		return m_NoPriorityIconData;
	}
	else
	{
		FlashIconData data = m_FlashStack.back();
		return data;
	}
}

void CTrayIcon::OnTimer( UINT_PTR nIDEvent )
{
	if ( nIDEvent == _defFlashIconTimerID )
	{
		if ( m_IsShowIcon )
		{
			FlashIconData Data = DecideFlashIconData();
			CIcon icon;
			icon.LoadIcon(Data.nIconNormal);
			ChangeIcon(icon);
			ChangeToolTip(Data.strTooltip);
			m_IsShowIcon = false;

		}
		else
		{
			FlashIconData Data = DecideFlashIconData();
			CIcon icon;
			icon.LoadIcon(Data.nIconFlash);
			ChangeIcon(icon);
			ChangeToolTip(Data.strTooltip);
			m_IsShowIcon = true;
		}
	}
	else if ( nIDEvent == _defIconLeaveTimerID )
	{
		POINT pt = {0};
		RECT rc;
		GetCursorPos(&pt);
		if ( GetTrayIconRect(FindTrayWnd(), m_hNotifyWnd, &rc) == S_FALSE)
		{
			GetTrayIconRect(FindNotifyIconOverflowWindow(), m_hNotifyWnd, &rc);
		}

		if ( !PtInRect(&rc,pt) )
		{
			BOOL b;
			OnTrayIconNotify(WM_TRAYICONNOTIFY, 0, WM_MOUSELEAVE, b);
		}
	}
}

void CTrayIcon::CreateIcon( int nId, const LPCTSTR lpText )
{
	m_Lock.Lock();
	m_NoPriorityIconData.nIconFlash	= nId;
	m_NoPriorityIconData.nIconNormal = nId;
	m_NoPriorityIconData.IsPriority = false;
	m_NoPriorityIconData.strTooltip = lpText;
	m_Lock.Unlock();

	CIcon ico;
	ico.LoadIcon(nId);
	m_data.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	m_data.hWnd =m_hNotifyWnd;
	m_data.uID = ELIVE_TRAY_ID;
	m_data.hIcon = (HICON)ico;
	m_data.uCallbackMessage = WM_TRAYICONNOTIFY;

	::lstrcpyn(m_data.szTip, lpText, m_version.dwMajorVersion >= 5 ? 127 : 63);
	::Shell_NotifyIcon(NIM_ADD, &m_data);

	if (m_version.dwMajorVersion >= 5)
	{
		m_data.uVersion = NOTIFYICON_VERSION;
		::Shell_NotifyIcon(NIM_SETVERSION, &m_data);
		int code = GetLastError();
		ATLTRACE("SetVersion error: %d.", code);
	}

	::SetTimer(m_hNotifyWnd,_defFlashIconTimerID,400,NULL);
}

void CTrayIcon::ReCreateIcon()
{
	m_data.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	::Shell_NotifyIcon(NIM_ADD, &m_data);

	if (m_version.dwMajorVersion >= 5)
	{
		m_data.uVersion = NOTIFYICON_VERSION;
		::Shell_NotifyIcon(NIM_SETVERSION, &m_data);
	}
}

void CTrayIcon::RemoveIcon()
{
	m_data.uFlags = 0;
	::Shell_NotifyIcon(NIM_DELETE, &m_data);
}

void CTrayIcon::ChangeIcon( CIcon& icon )
{
	m_data.uFlags = NIF_ICON;
	m_data.hIcon = (HICON)icon;
	::Shell_NotifyIcon(NIM_MODIFY, &m_data);
}

void CTrayIcon::ChangeIcon( UINT nID )
{
	CIcon icon;
	icon.LoadIcon(nID);
	this->ChangeIcon(icon);
}

void CTrayIcon::ChangeToolTip( const LPCTSTR lpText )
{
	m_data.uFlags = NIF_TIP;
	::lstrcpyn(m_data.szTip, lpText, m_version.dwMajorVersion >= 5 ? 127 : 63);
	::Shell_NotifyIcon(NIM_MODIFY, &m_data);
}

void CTrayIcon::ShowBallon( const LPCTSTR lpText, const LPCTSTR lpTitle, int msecond /* = 3 */, const InfoType& type /*=Info*/ )
{
	m_data.uFlags = NIF_INFO;
	::lstrcpyn(m_data.szInfo, lpText, 255);
	::lstrcpyn(m_data.szInfoTitle, lpTitle, 63);
	m_data.dwInfoFlags = type;
	m_data.uTimeout = msecond;
	::Shell_NotifyIcon(NIM_MODIFY, &m_data);
}

HWND CTrayIcon::FindTrayWnd()
{
	HWND hWnd = NULL;  
	HWND hPaper = NULL;  

	if ((hWnd = FindWindow(_T("Shell_TrayWnd"), NULL)) != NULL)  
	{  
		if ((hWnd = FindWindowEx(hWnd, 0, _T("TrayNotifyWnd"), NULL)) != NULL)  
		{  
			hPaper = FindWindowEx(hWnd, 0, _T("SysPager"), NULL);  
			if(!hPaper)  
				hWnd = FindWindowEx(hWnd, 0, _T("ToolbarWindow32"), NULL);  
			else  
				hWnd = FindWindowEx(hPaper, 0, _T("ToolbarWindow32"), NULL);  
		}  
	}  

	return hWnd;
}

HWND CTrayIcon::FindNotifyIconOverflowWindow()
{
	HWND hWnd = NULL;  

	hWnd = FindWindow(_T("NotifyIconOverflowWindow"), NULL);  
	if (hWnd != NULL)  
	{  
		hWnd = FindWindowEx(hWnd, NULL, _T("ToolbarWindow32"), NULL);  
	}  

	return hWnd;
}

HRESULT CTrayIcon::GetTrayIconRect( HWND hWnd, HWND hMsgRecv, LPRECT pRect )
{
	HRESULT hr = S_FALSE;
	int nItems = 0;	
	DWORD pIdExplorer = 0;
	HANDLE hExplorer = NULL;
	BOOL lv_Result = FALSE;
	LPVOID pTB = NULL;


	nItems = SendMessage(hWnd, TB_BUTTONCOUNT, 0, 0);
	if ( nItems <= 0 )
	{
		hr = E_FAIL;
		goto Exit0;
	}

	GetWindowThreadProcessId(hWnd, &pIdExplorer);
	hExplorer = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, pIdExplorer);
	if ( NULL == hExplorer )
	{
		hr = E_FAIL;
		goto Exit0;
	}

	pTB = ::VirtualAllocEx(hExplorer, NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); 
	if ( NULL == pTB )
	{
		hr = E_FAIL;
		goto Exit0;
	}

	int nIndex = 0;
	for ( ; nIndex < nItems; ++nIndex )
	{
		::SendMessage(hWnd, TB_GETBUTTON, nIndex, (LPARAM)pTB); 
		if ( AppGetSystemInfo()->IsWOW64() )
		{
			UINT64 lv_UserData64[2] = {0};
			TBBUTTON64 bInfo = {0};
			::ReadProcessMemory(hExplorer, pTB, &bInfo, sizeof(bInfo), NULL);
			::ReadProcessMemory(hExplorer, (LPVOID)bInfo.dwData, lv_UserData64, 2*sizeof(UINT64), NULL);
			if ( lv_UserData64[0] == (UINT64)hMsgRecv && (lv_UserData64[1]&0xFFFFFFFF) == ELIVE_TRAY_ID )
			{
				lv_Result = TRUE;
				break;
			}
		}
		else
		{
			TBBUTTON bInfo = {0};
			INT_PTR lv_UserData[2] = {0};
			::ReadProcessMemory(hExplorer, pTB, &bInfo, sizeof(bInfo), NULL);  
			::ReadProcessMemory(hExplorer, (LPVOID)bInfo.dwData, lv_UserData, 2*sizeof(INT_PTR), NULL);
			if ( lv_UserData[0] == (INT_PTR)hMsgRecv && lv_UserData[1] == ELIVE_TRAY_ID )
			{
				lv_Result = TRUE;
				break;
			}
		}
	}

	if (lv_Result)
	{
		// get the item rectangle, reuse processmemory as a RECT is smaller than TBBUTTON
		lv_Result = SendMessage(hWnd, TB_GETITEMRECT, nIndex, (LPARAM)pTB);

		// get the contents
		if (lv_Result)
		{
			lv_Result = ReadProcessMemory(hExplorer, pTB, pRect, sizeof(RECT), NULL);
		}

		// to screen coordinates
		if (lv_Result)
		{
			::GetWindowRect(hWnd,pRect);
			//CWindow(hWnd).ClientToScreen(&rc);
			hr = S_OK;
		}
	}

Exit0:

	if ( NULL != pTB )
	{
		::VirtualFreeEx(hExplorer, pTB, 0, MEM_RELEASE);  
	}

	if ( NULL != hExplorer )
	{
		CloseHandle(hExplorer);
	}

	return hr;
}

LRESULT CTrayIcon::OnTrayIconNotify( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	int nMsg = LOWORD(lParam);
	if ( nMsg == WM_MOUSELEAVE )
	{
		::KillTimer(m_hNotifyWnd,_defIconLeaveTimerID);
	}
	else if ( nMsg == WM_MOUSEMOVE )
	{
		::SetTimer(m_hNotifyWnd,_defIconLeaveTimerID, 120, NULL);
	}

	//(static_cast<T*>(this))->OnTrayIconNotify(uMsg, wParam, lParam, bHandled);
	return 0;
}

void CTrayIcon::PushStatus( int nIdiFlash, BOOL IsPriority, LPCTSTR lpszToolTip /*= _T("")*/ )
{
	FlashIconData IconData;
	IconData.nIconFlash = nIdiFlash;
	IconData.strTooltip =  lpszToolTip;
	m_Lock.Lock();
	if ( IsPriority )
	{
		IconData.nIconNormal = 101;
		m_FlashStack.remove_if(LISTREMOVEOP(IconData));
		m_FlashStack.push_back(IconData);
	}
	else
	{
		IconData.nIconNormal = nIdiFlash;
		m_NoPriorityIconData = IconData;
	}
	m_Lock.Unlock();
}

void CTrayIcon::RemoveStatus( int nIdiFlash )
{
	FlashIconData IconData;
	IconData.nIconFlash = nIdiFlash;
	m_Lock.Lock();
	m_FlashStack.remove_if(LISTREMOVEOP(IconData));
	m_Lock.Unlock();
}

void CTrayIcon::CleanupStatus()
{
	m_Lock.Lock();
	m_FlashStack.clear();
	m_Lock.Unlock();
}

int CTrayIcon::GetCurFlashId()
{
	AutoLocker _lock(&m_Lock);
	if ( m_FlashStack.empty() )
	{
		return m_NoPriorityIconData.nIconNormal;
	}

	return m_FlashStack.back().nIconFlash;
}

LRESULT CTrayIcon::OnTaskbarCreated( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	return S_FALSE;
}

void CTrayIcon::SetNotify( HWND hWnd )
{
	m_hNotifyWnd=hWnd;
}

CTrayIcon::LISTREMOVEOP::LISTREMOVEOP( FlashIconData const& data ) : m_Data(data)
{

}

bool CTrayIcon::LISTREMOVEOP::operator()( FlashIconData& data )
{
	if ( data.nIconFlash == m_Data.nIconFlash )
	{
		return true;
	}

	return false;
}

DWORD CTrayIcon::WM_TRAYICONNOTIFY = 0;
DWORD CTrayIcon::WM_TASKBARCREATED = 0;
DLLVERSIONINFO CTrayIcon::m_version;
