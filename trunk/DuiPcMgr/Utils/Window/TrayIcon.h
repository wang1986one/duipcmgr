#ifndef __TRAYICON_H__
#define __TRAYICON_H__
#pragma once
#include "Utils/Thread/ThreadUtil.h"
#include <queue>
#include <list>
#include <ShellAPI.h>
#define ELIVE_TRAY_ID 2530
#define _defIconLeaveTimerID 1000
#define _defFlashIconTimerID 1001

using namespace Utils;

class ITrayIconNotify
{
public:
	virtual LRESULT OnTrayIconNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) =0;
	virtual LRESULT OnTaskbarCreated(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) =0;
};

class CIcon
{
public:
	CIcon();
	~CIcon();
	
	void LoadIcon(UINT nID);

	operator HICON() const
	{
		return m_hIcon;
	}
private:
	HICON m_hIcon;
};

class CTrayIcon
{
	typedef struct
	{
		INT iBitmap;
		INT idCommand;
		BYTE fsState;
		BYTE fsStyle;
		BYTE bReserved[6];
		UINT64 dwData;
		UINT64 iString;
	} TBBUTTON64;

	struct FlashIconData
	{
		int				nIconNormal;
		int				nIconFlash;
		bool			IsPriority;
		CString			strTooltip;
	};
public:
	static void Initialize();

public:
	enum InfoType
	{
		None = NIIF_NONE,
		Info = NIIF_INFO,
		Warning = NIIF_WARNING,
		Error = NIIF_ERROR,
		User = NIIF_USER
	};

	CTrayIcon();
	~CTrayIcon();

	void SetNotify(HWND hWnd);
	void Destroy();

	FlashIconData DecideFlashIconData();
	void OnTimer(UINT_PTR nIDEvent);
	void CreateIcon(int nId, const LPCTSTR lpText);
	void ReCreateIcon();
	void RemoveIcon();
	void ChangeIcon(UINT nID);
	void ChangeIcon(CIcon& icon);
	void ChangeToolTip(const LPCTSTR lpText);
	void ShowBallon(const LPCTSTR lpText, const LPCTSTR lpTitle, int msecond=3, const InfoType& type=Info);
	HWND FindTrayWnd();  

	//获取溢出托盘区窗口句柄   
	HWND FindNotifyIconOverflowWindow();
	HRESULT GetTrayIconRect(HWND hWnd, HWND hMsgRecv, LPRECT pRect);
	LRESULT OnTrayIconNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTaskbarCreated(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	class LISTREMOVEOP
	{
	public:
		LISTREMOVEOP(FlashIconData const& data);
		bool operator()(FlashIconData& data);
	private:
		FlashIconData	m_Data;
	};
	
	void PushStatus(int nIdiFlash, BOOL IsPriority, LPCTSTR lpszToolTip = _T(""));
	void RemoveStatus(int nIdiFlash);
	void CleanupStatus();
	int GetCurFlashId();

private:
	HWND m_hNotifyWnd;
	NOTIFYICONDATA					m_data;
	FlashIconData					m_NoPriorityIconData;
	bool							m_IsShowIcon;
	std::list< FlashIconData >		m_FlashStack;
	Utils::ThreadLock				m_Lock;

public:
	static DWORD WM_TRAYICONNOTIFY;
	static DWORD WM_TASKBARCREATED;
	static DLLVERSIONINFO m_version;
};

#endif // __TRAYICON_H__