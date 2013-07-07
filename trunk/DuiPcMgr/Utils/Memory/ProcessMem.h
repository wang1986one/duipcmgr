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

#if !defined(AFX_PROCESSMEM_H__F9050559_BB86_47D8_848A_D1D5A6A880F0__INCLUDED_)
#define AFX_PROCESSMEM_H__F9050559_BB86_47D8_848A_D1D5A6A880F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CProcessMem : public CWnd
{

public:
	CProcessMem();
	bool InitModule (HWND hWnd);
	bool InitModule (DWORD processID);
	bool InitModule (CString wndTitle);

	bool ReadVal	(DWORD iAddress, BYTE &value);
	bool WriteVal	(DWORD iAddress, BYTE value);

	bool ReadVal	(DWORD iAddress, short int &value);
	bool WriteVal	(DWORD iAddress, short int value);
	
	bool ReadVal	(DWORD iAddress, int &value);
	bool WriteVal	(DWORD iAddress, int value);

	bool ReadVal	(DWORD iAddress, CString &text);
	bool WriteVal	(DWORD iAddress, CString text);

	virtual ~CProcessMem();
	
	CString m_sError;

	struct _ProcessInfo {
		CString windowTitle;
		DWORD pID;
		DWORD dwPageSize;
		HANDLE hProcess;
		LPVOID lpMinAppAddress;
		LPVOID lpMaxAppAddress;
	} ProcessInfo;

private:

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProcessMem)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CProcessMem)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSMEM_H__F9050559_BB86_47D8_848A_D1D5A6A880F0__INCLUDED_)



