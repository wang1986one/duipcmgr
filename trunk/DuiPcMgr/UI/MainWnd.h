#pragma once
#include "../DuiLib/Utils/WinImplBase.h"
using namespace Utils;

class CMainWnd
	: public WindowImplBase ,IListCallbackUI
{
public:
	CMainWnd(void);
	virtual ~CMainWnd(void);

private:
	// ��Դѡ��ӿ�
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetResID() const;
	virtual LPCTSTR GetResType() const;
	virtual LPCTSTR GetSkinFolder();
	virtual LPCTSTR GetSkinFile();

	virtual UINT GetClassStyle() const;
	virtual BOOL OnEscapeCloseWindow( void ) const;
	virtual void OnFinalMessage( HWND hWnd );
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSysCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	virtual LRESULT OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	virtual void InitWindow();

	// UI�¼�����
	virtual void Notify( TNotifyUI &msg );
	virtual void OnClick(TNotifyUI &msg);

	LPCTSTR GetItemText(CControlUI* pList, int iItem, int iSubItem);

	int EnumSoftware();
private:
	// ���ڿ��ư�ť
	CButtonUI* m_btnClose;
	CButtonUI* m_btnMin;
	CButtonUI* m_btnMax;
	CButtonUI* m_btnRestore;

	CListUI* m_listSoftware;

	CLabelUI * m_softwareCount;
};
