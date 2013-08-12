#pragma once
#include "../DuiLib/Utils/WinImplBase.h"
#include "Core/RdbFile.h"
#include <shellapi.h>
using namespace Utils;

class CMainWnd
	: public WindowImplBase
{
public:
	CMainWnd(void);
	virtual ~CMainWnd(void);

private:
	virtual BOOL OnEscapeCloseWindow( void ) const;
	virtual LPCTSTR GetSkinFolder();
	virtual LPCTSTR GetSkinFile();
	virtual void InitWindow();
	virtual void OnFinalMessage( HWND hWnd );
	virtual void Notify( TNotifyUI &msg );
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetResID() const;
	virtual LPCTSTR GetResType() const;

	virtual void OnClick( TNotifyUI* pMsg );

	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	bool OnDropFiles(HDROP hDropInfo);
private:
	//////////////////////////////////////////////////////////////////////////
	// �ؼ�
	CHorizontalLayoutUI* m_pParserRdbFileLayout;					// ����ѹRdb�ļ�·������
	CHorizontalLayoutUI* m_pParserRdbDocumentLayout;		// Rdb�ļ��������
	CHorizontalLayoutUI* m_pPackageRdbFileLayout;				// ��ѹ���ļ���·������
	CHorizontalLayoutUI* m_pPackageRdbDocumentLayout;		// ����ļ����·��

	CEditUI* m_pRdbIn;
	CEditUI* m_pRdbOutFolder;

private:
	CRdbFile m_RdbFile;
};
