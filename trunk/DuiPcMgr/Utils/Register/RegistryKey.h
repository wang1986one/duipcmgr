#ifndef __RegistryKey_H__
#define __RegistryKey_H__
///////////////////////////////////////////////////////////////
//
// �ļ�����	:	RegistryKey.h
// ������		: 	daviyang35@QQ.com
// ����ʱ��	:	2013-7-11 16-35
// ˵��			:	ע��������
//
///////////////////////////////////////////////////////////////
#pragma once
#include <winreg.h>
#include "RegKeyConvertString.h"

namespace Utils
{

	class CRegistryKey
	{
		// Construction
	public:
		CRegistryKey(HKEY hRootKey,LPCTSTR lpKey);
		CRegistryKey(HKEY hKey = NULL);
		CRegistryKey(HKEY hKey,LPCTSTR lpKey,BOOL bCreate);
		virtual ~CRegistryKey();

	public:

		static BOOL IsRegExistKey(HKEY hRoot, LPCTSTR lpKey);
		static BOOL IsKeyExistValue(HKEY hRoot, LPCTSTR lpKey, LPCTSTR lpValue);

		BOOL AttachKey(HKEY hRoot, LPCTSTR lpKey);

		BOOL IsKeyValid();
		BOOL IsValueExist(LPCTSTR lpValue);

		BOOL GetValueByData(LPCTSTR lpData, CString* lpVal);

		BOOL Read(LPCTSTR lpValueName, CString* lpVal);
		BOOL Read(LPCTSTR lpValueName, DWORD* pdwVal);
		BOOL Read(LPCTSTR lpValueName, int* pnVal);
		BOOL Write(LPCTSTR lpSubKey, DWORD dwVal);
		BOOL Write(LPCTSTR lpSubKey, int nVal);
		BOOL Write(LPCTSTR lpSubKey, CStringW strVal);
		BOOL DeleteKey(HKEY hKey, LPCTSTR lpSubKey);
		BOOL DeleteValue(LPCTSTR lpValueName);
		void Close();
		BOOL CreateKey(LPCTSTR lpSubKey);

		CString EnumKey(DWORD dwIndex);
		BOOL Open(LPCTSTR lpSubKey);

	protected:
		HKEY m_hKey;
		HKEY m_hRootKey;
	};
}
#endif // __RegistryKey_H__

