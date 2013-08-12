#include "stdafx.h"
#include "UI/MainWnd.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	g_logger.InitLogger();
	g_logger.Info(_T("应用启动"));
	CPaintManagerUI::SetInstance(hInstance);

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) 
		return 0;

	CMainWnd* pFrame = new CMainWnd();//主窗体类
	if(pFrame==NULL)
		return 0;
	pFrame->CreateDuiWindow(NULL,_T("DuiPcMgr"),UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW);
	pFrame->CenterWindow();
	pFrame->SetIcon(101);
	pFrame->ShowWindow(true);

	CPaintManagerUI::MessageLoop();

	::CoUninitialize();
	g_logger.Info(_T("应用退出"));
	return 0;
}

