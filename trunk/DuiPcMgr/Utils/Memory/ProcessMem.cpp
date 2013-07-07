// ProcessMem.cpp : implementation file
//
// Class to read memory of other processes. For NT and XP only. For
// further information check www.codeproject.com.
//
// Written by Griffith Sutherns (Griffith_Sutherns@softhome.net)
// Copyright (c) 2002.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. If 
// the source code in  this file is used in any commercial application 
// then a simple email would be nice.
//
// This file is provided "as is" with no expressed or implied warranty.
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file.
// 

#include "stdafx.h"
#include "ProcessMem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProcessMem

CProcessMem::CProcessMem()
{
	ProcessInfo.dwPageSize			= 0;
	ProcessInfo.lpMaxAppAddress		= 0;
	ProcessInfo.lpMinAppAddress		= 0;
	ProcessInfo.pID					= 0;
	ProcessInfo.hProcess			= 0;
}

CProcessMem::~CProcessMem()
{
	CloseHandle(ProcessInfo.hProcess);
}


BEGIN_MESSAGE_MAP(CProcessMem, CWnd)
	//{{AFX_MSG_MAP(CProcessMem)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CProcessMem message handlers
bool CProcessMem::InitModule(CString wndTitle)
{
	HWND hwnd = GetForegroundWindow()->GetSafeHwnd();
	CWnd cWnd;
	CString wndtext;

	if (wndTitle.IsEmpty())
	{
		m_sError = _T("wndTitle is empty.");
		return FALSE;
	}

	for(int i=0;i<1000;i++)
	{
		hwnd = ::GetNextWindow(hwnd,GW_HWNDNEXT);
		
		if(!IsWindow(hwnd))
			break;
		
		cWnd.Attach(hwnd);
		cWnd.GetWindowText(wndtext);
		
		if(wndtext==wndTitle)
			break;
		cWnd.Detach();
	}
		
	if(wndtext!=wndTitle)
	{
		m_sError = _T("No window found with matching title.");
		return FALSE;
	}

	ProcessInfo.windowTitle = wndTitle;
	
	GetWindowThreadProcessId(cWnd.GetSafeHwnd(),&ProcessInfo.pID);
	cWnd.Detach();

	InitModule(ProcessInfo.pID);
	m_sError.Empty();
	return TRUE;
}


bool CProcessMem::InitModule(HWND hWnd)
{
	if(!IsWindow(hWnd) || hWnd == 0)
	{
		m_sError = _T("hWnd handle invalid.");
		return FALSE;
	}

	GetWindowThreadProcessId(hWnd,&ProcessInfo.pID);
	InitModule(ProcessInfo.pID);

	m_sError.Empty();
	return TRUE;
}

bool CProcessMem::InitModule(DWORD processID)
{	
	SetLastError(0);

	SYSTEM_INFO sysinfo;

	CloseHandle(ProcessInfo.hProcess);

	ProcessInfo.pID = processID;
	// Note: This function will only work if the user has ADMIN access
	ProcessInfo.hProcess = OpenProcess(PROCESS_VM_READ|PROCESS_VM_WRITE|
		PROCESS_VM_OPERATION|PROCESS_QUERY_INFORMATION, FALSE, ProcessInfo.pID);
	
	if(GetLastError()!=0)
	{
		m_sError = _T("Failed to open the Proccess. Use GetLastError() for further information.");
		return FALSE;
	}
	
	GetSystemInfo(&sysinfo);
	ProcessInfo.lpMinAppAddress = sysinfo.lpMinimumApplicationAddress;
	ProcessInfo.lpMaxAppAddress = sysinfo.lpMaximumApplicationAddress;
	ProcessInfo.dwPageSize = sysinfo.dwPageSize;
	
	m_sError.Empty();
	return TRUE;
}

bool CProcessMem::ReadVal(DWORD iAddress, int &value)
{
	// Reads 4 bytes
	DWORD dummy;

	if (!ReadProcessMemory(ProcessInfo.hProcess,(void*) iAddress,
		(void*) &value,sizeof(value),&dummy))
	{
		m_sError = _T("Failed to read memory.");
		return FALSE;
	}
	
	m_sError.Empty();
	return TRUE;
}


bool CProcessMem::WriteVal(DWORD iAddress, int value)
{
	// Writes 4 bytes
	
	DWORD dummy;
	if (!WriteProcessMemory(ProcessInfo.hProcess,(void*) iAddress,
		(void*) &value,sizeof(value),&dummy))
	{
		m_sError = _T("Failed to write to memory.");
		return FALSE;
	}
	
	m_sError.Empty();
	return TRUE;
}

bool CProcessMem::ReadVal(DWORD iAddress, short int &value)
{
	// Reads 2 bytes
	DWORD dummy;

	if (!ReadProcessMemory(ProcessInfo.hProcess,(void*) iAddress,
		(void*) &value,sizeof(value),&dummy))
	{
		m_sError = _T("Failed to read memory.");
		return FALSE;
	}
	
	m_sError.Empty();
	return TRUE;
}

bool CProcessMem::WriteVal(DWORD iAddress, short int value)
{
	// Writes 4 bytes
	
	DWORD dummy;
	if (!WriteProcessMemory(ProcessInfo.hProcess,(void*) iAddress,
		(void*) &value,sizeof(value),&dummy))
	{
		m_sError = _T("Failed to write to memory.");
		return FALSE;
	}
	
	m_sError.Empty();
	return TRUE;
}

bool CProcessMem::ReadVal(DWORD iAddress, BYTE &value)
{
	// Reads 1 byte
	DWORD dummy;

	if (!ReadProcessMemory(ProcessInfo.hProcess,(void*) iAddress,
		(void*) &value,sizeof(value),&dummy))
	{
		m_sError = _T("Failed to read memory.");
		return FALSE;
	}
	
	m_sError.Empty();
	return TRUE;
}

bool CProcessMem::WriteVal(DWORD iAddress, BYTE value)
{
	// Writes 1 byte
	
	DWORD dummy;
	if (!WriteProcessMemory(ProcessInfo.hProcess,(void*) iAddress,
		(void*) &value,sizeof(value),&dummy))
	{
		m_sError = _T("Failed to write to memory.");
		return FALSE;
	}
	
	m_sError.Empty();
	return TRUE;
}

bool CProcessMem::ReadVal(DWORD iAddress, CString &text)
{
	// Reads 1024 bytes and returns the memory block as a CString
	DWORD dummy;
	char buffer[1024];

	if (!ReadProcessMemory(ProcessInfo.hProcess,(void*) iAddress,
		(void*) &buffer,1024,&dummy))
	{
		m_sError = _T("Failed to read memory.");
		return FALSE;
	}
	
	text = buffer;
	m_sError.Empty();
	return TRUE;
}

bool CProcessMem::WriteVal(DWORD iAddress, CString text)
{
	DWORD dummy;
	char buffer[1024];

	strcpy(buffer,text);
	
	buffer[text.GetLength()] = '\0';

	if(text.GetLength()>= 1024)
	{
		m_sError = _T("Cannot write strings longer than 1024 characters");
		return FALSE;
	}
	
	if (!WriteProcessMemory(ProcessInfo.hProcess,(void*) iAddress,
		(void*) &buffer,text.GetLength()+1,&dummy))
	{
		m_sError = _T("Failed to read memory.");
		return FALSE;
	}

	m_sError.Empty();
	return TRUE;
}