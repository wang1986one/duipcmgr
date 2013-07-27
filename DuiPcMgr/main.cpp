#include "stdafx.h"
#include "UI/MainWnd.h"
#include "log.h"
#include <iostream>
#include <string>
using namespace log4cplus;
using namespace log4cplus::helpers;


DECLARE_SUBLOGITEM(DuiPcMgr_WinMain);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	std::locale::global(std::locale("chs"));
	log4cplus::PropertyConfigurator::doConfigure(L".\\config\\log4cplus.cfg");
	//日志的几种写法，还有LOG_DEBUG(),LOG_ERROR，具体的可在log.h中参看
	LOG_TRACE_METHOD();
	LOG_INFO(L"日志初始化成功......");
	int i = 1024;
	LOG_INFO(L"试试<<的作用1:"<< i);
	std::wstring str = L"shaoyuan";
	std::wstring str2 = L"少原";
	LOG_INFO(L"试试<<的作用2:"<< str.c_str());
	LOG_INFO(L"试试<<的作用3:"<<str2.c_str());


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
	return 0;
}

