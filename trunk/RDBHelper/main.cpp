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

	if (AppGetSystemInfo()->IsVistaOrLater())	// Vista�Ժ�ϵͳ�� WM_DROPFILES��Ϣ����
		AllowMeesageForVistaAbove(WM_DROPFILES);

	CMainWnd* pFrame = new CMainWnd();//��������
	if(pFrame==NULL)
		return 0;
	pFrame->CreateDuiWindow(NULL,_T("RDB�ļ�����"),UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW);
	pFrame->CenterWindow();
	pFrame->SetIcon(101);
	pFrame->ShowWindow(true);

	CPaintManagerUI::MessageLoop();

	::CoUninitialize();
	return 0;
}

