#pragma once

class CUnInstallSoftware
{
public:
	CUnInstallSoftware(const wchar_t* wsUnInstallString)
	{
		if ( wsUnInstallString == nullptr )
		{
			this->wsUnInstallPath = nullptr;
		}
		else
			this->wsUnInstallPath = wsUnInstallString;
	}
	~CUnInstallSoftware()
	{

	}
public:
	BOOL UnInstallSoftWare()
	{
		if ( wsUnInstallPath == nullptr )
		{
			return FALSE;
		}
		if ( this->IsMsiExceUnInstall(wsUnInstallPath) )
		{
			int isSuccess = (int)::ShellExecute(NULL, NULL, L"cmd.exe", (std::wstring(L"/c ") + std::wstring(wsUnInstallPath)).c_str(), NULL, SW_HIDE);
			return (BOOL)isSuccess;
		}
		int isSuccess = (int)::ShellExecute(NULL, L"open", wsUnInstallPath, NULL, NULL, SW_HIDE);
		return (BOOL)isSuccess;
	}
private:
	BOOL IsMsiExceUnInstall(const wchar_t* wsUnInstallPath)
	{
		if ( std::wstring(wsUnInstallPath).find_first_of('{') == std::wstring::npos )
		{
			return FALSE;	//²»ÎªMsiExceÐ¶ÔØ
		}
		return TRUE; //ÎªMsiExceÐ¶ÔØ
	}
private:
	wchar_t* wsUnInstallPath;

};