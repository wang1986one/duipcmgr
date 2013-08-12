#include "StdAfx.h"
#include "Vista.h"
#include <Windows.h>
namespace Utils
{

	BOOL WINAPI	IsVistaLater()
	{
		OSVERSIONINFO osInfo;
		osInfo.dwOSVersionInfoSize = sizeof(osInfo);
		::GetVersionEx(&osInfo);

		return osInfo.dwMajorVersion >= 6;
	}

	BOOL WINAPI	IsOpenUAC()
	{
		BOOL bResult = FALSE;

		if( IsVistaLater() )
		{
			LONG	lResult;
			HKEY	hKey;
			DWORD	dwEnableLUA;
			DWORD	dwType = REG_DWORD;
			DWORD	dwSize = sizeof( DWORD );

			lResult = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
				_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System\\"),
				0,
				KEY_READ,
				&hKey
				);
			if( lResult == ERROR_SUCCESS )
			{
				lResult = RegQueryValueEx(hKey,
					_T("EnableLUA"),
					NULL,
					&dwType,
					(BYTE*)&dwEnableLUA,
					&dwSize
					);
				bResult = (lResult == ERROR_SUCCESS) && (0 != dwEnableLUA);

				RegCloseKey(hKey);
			}
		}

		return bResult;
	}

	BOOL WINAPI EnablePrivilege( LPCTSTR szPrivName )
	{
		HANDLE _hToken = INVALID_HANDLE_VALUE;  
		if (!OpenProcessToken(GetCurrentProcess(),  
			TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,  
			&_hToken))  
		{  
			return FALSE;  
		}  
		if (_hToken == INVALID_HANDLE_VALUE)  
		{  
			return FALSE;  
		}  
		TOKEN_PRIVILEGES tp = {0};  
		LUID luid = {0};  
		TOKEN_PRIVILEGES tpPrevious= {0};  
		DWORD cbPrevious=sizeof(TOKEN_PRIVILEGES);  
		if(!LookupPrivilegeValue( NULL, szPrivName, &luid ))  
		{  
			return FALSE;  
		}  
		//  
		// first pass.  get current privilege setting  
		//  
		tp.PrivilegeCount           = 1;  
		tp.Privileges[0].Luid       = luid;  
		tp.Privileges[0].Attributes = 0;  
		if (!AdjustTokenPrivileges(_hToken,  
			FALSE,  
			&tp,  
			sizeof(TOKEN_PRIVILEGES),  
			&tpPrevious,  
			&cbPrevious))  
		{  
			return FALSE;  
		}  
		//  
		// second pass.  set privilege based on previous setting  
		//  
		tpPrevious.PrivilegeCount       = 1;  
		tpPrevious.Privileges[0].Luid   = luid;  
		tpPrevious.Privileges[0].Attributes |= (SE_PRIVILEGE_ENABLED);  
		if (!AdjustTokenPrivileges(_hToken,  
			FALSE,  
			&tpPrevious,  
			cbPrevious,  
			NULL,  
			NULL))  
		{  
			return FALSE;  
		}  
		if (_hToken != INVALID_HANDLE_VALUE)  
		{  
			CloseHandle(_hToken);
			_hToken = INVALID_HANDLE_VALUE;  
		}  
		return TRUE;
	}

	typedef BOOL (WINAPI *PFN_ChangeWindowMessageFilter)(UINT uMessage, int nFlag);

	BOOL	WINAPI AllowMeesageForVistaAbove( UINT uMessage, BOOL bAllow/*=TRUE*/ )
	{
		if ( IsVistaLater() ==FALSE)
			return TRUE;	// Vista之前，不需要操作

		HMODULE hUserMod;
		PFN_ChangeWindowMessageFilter pfnChangeWindowMessageFilter;

		hUserMod = GetModuleHandle(_T("user32.dll"));
		pfnChangeWindowMessageFilter = (PFN_ChangeWindowMessageFilter)GetProcAddress(hUserMod, "ChangeWindowMessageFilter");
		if (pfnChangeWindowMessageFilter ==NULL )
			return FALSE;

		return pfnChangeWindowMessageFilter(uMessage, bAllow ? 1 : 2);
	}

	BOOL WINAPI Pin2Taskbar( LPCTSTR lpFullPath, bool bPin/*=true*/ )
	{
		CString strFullPath(lpFullPath);
		if (strFullPath.IsEmpty())
			return FALSE;

		int iPos=strFullPath.ReverseFind(_T('\\'));
		if (iPos<2)
			return FALSE;

		iPos++;
		CString strPath=strFullPath.Left(iPos);
		CString strFile=strFullPath.Right(strFullPath.GetLength() - iPos);

		HMODULE hShell32 = ::LoadLibrary(L"shell32.dll");
		if (!hShell32)
			return FALSE;

		int iResIndex = bPin ? 5386 : 5387; //<By张方雪 2013-5-24>不晓得这个index是什么, 有文档吗?
		TCHAR szCmd[512];
		int nLen = ::LoadString(hShell32, iResIndex, szCmd, _countof(szCmd));
		HRESULT hr = S_OK;

		VARIANT vPath;
		::VariantInit(&vPath);
		vPath.vt = VT_BSTR;
		vPath.bstrVal = strPath.AllocSysString();

		CComPtr<IShellDispatch> pShell;
		CComPtr<Folder> pFolder;
		if (FAILED(::CoCreateInstance(CLSID_Shell, NULL, CLSCTX_SERVER, IID_IDispatch, (LPVOID*)&pShell)) || !pShell)
			return FALSE;

		if (FAILED(pShell->NameSpace(vPath, &pFolder)) || !pFolder)
			return FALSE;

		CComPtr<FolderItem> pItem;
		BSTR bstrPathName = strFile.AllocSysString();
		if (FAILED(pFolder->ParseName(bstrPathName, &pItem)) || !pItem.p)
			return FALSE;

		CComPtr<FolderItemVerbs> pVerbs;

		if (FAILED(pItem->Verbs(&pVerbs)) || !pVerbs)
			return FALSE;

		long lCount = 0;
		pVerbs->get_Count(&lCount);

		BOOL bReturn =FALSE;
		BSTR bstrCmd = ::SysAllocString(szCmd);
		for(long i = 0; i < lCount; i++)
		{
			CComPtr<FolderItemVerb> pVerb;
			VARIANT var;
			::VariantInit(&var);
			var.vt = VT_I4;
			var.lVal = i;
			pVerbs->Item(var, &pVerb);
			if(pVerb)
			{
				BSTR bstrName;
				pVerb->get_Name(&bstrName);
				if(_tcscmp(bstrCmd, bstrName) == 0)
				{
					pVerb->DoIt();
					::SysFreeString(bstrName);
					bReturn = TRUE;
					break;
				}
				::SysFreeString(bstrName);
			}
			::VariantClear(&var);
		}
		::SysFreeString(bstrCmd);
		::SysFreeString(bstrPathName);

		return bReturn;
	}

}

