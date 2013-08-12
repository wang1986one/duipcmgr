#include "stdafx.h"
#include "UI/MainWnd.h"
#include "Utils/System/Vista.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	g_Logger.InitLogger();
	CPaintManagerUI::SetInstance(hInstance);
	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) 
		return 0;

	if (AppGetSystemInfo()->IsVistaOrLater())	// Vista以后系统打开 WM_DROPFILES消息接收
		AllowMeesageForVistaAbove(WM_DROPFILES);

	CMainWnd* pFrame = new CMainWnd();//主窗体类
	if(pFrame==NULL)
		return 0;
	pFrame->CreateDuiWindow(NULL,_T("RDB文件助手"),UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW);
	pFrame->CenterWindow();
	pFrame->SetIcon(101);
	pFrame->ShowWindow(true);

	CPaintManagerUI::MessageLoop();

	::CoUninitialize();
	return 0;
}

