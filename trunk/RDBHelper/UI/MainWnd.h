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
	// 控件
	CHorizontalLayoutUI* m_pParserRdbFileLayout;					// 待解压Rdb文件路径区域
	CHorizontalLayoutUI* m_pParserRdbDocumentLayout;		// Rdb文件输出区域
	CHorizontalLayoutUI* m_pPackageRdbFileLayout;				// 待压缩文件夹路径区域
	CHorizontalLayoutUI* m_pPackageRdbDocumentLayout;		// 打包文件输出路径

	CEditUI* m_pRdbIn;
	CEditUI* m_pRdbOutFolder;

private:
	CRdbFile m_RdbFile;
};
