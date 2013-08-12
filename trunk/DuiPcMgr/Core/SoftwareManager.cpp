#include "stdafx.h"
#include "softwaremanager.h"

#define KEY_WOW6432NODE _T("SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall")
#define KEY _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall")


using namespace Utils;
CSoftwareManager::CSoftwareManager()
{
	
	
}
DWORD CSoftwareManager::EnumSoftware()
{
	DWORD dwCount = 0;
	CString subkey;
	m_softList.clear();
	LPTSTR szKey[]=
	{
		_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall")
		//,_T("SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall")
		
	};
	for(int i=0;i<sizeof(szKey)/sizeof(szKey[0]);i++)
	{
		DWORD dwIndex = 0;
		CRegistryKey reg(HKEY_LOCAL_MACHINE,KEY);

		do
		{
			SoftwareInfo info;
			CRegistryKey subreg = reg;
			
			subkey = reg.EnumKey(dwIndex++);
			//_TRACE(_T("%s\n"),subkey);

			CString strValue;
			DWORD dwValue = 0;
			if(subreg.Open(subkey))
			{
				info.strKey = subkey;
				if(!subreg.Read(_T("Comments"), &info.strComments))
				{
					info.strComments.Empty();
				}
				if(!subreg.Read(_T("Contact"), &info.strContact))
				{
					info.strContact.Empty();
				}
				if(!subreg.Read(_T("DisplayIcon"), &info.strDisplayIcon))
				{
					info.strDisplayIcon.Empty();
				}
				if(!subreg.Read(_T("DisplayName"), &info.strDisplayName))
				{
					info.strDisplayName.Empty();
				}
				
				if(!subreg.Read(_T("DisplayVersion"), &info.strDisplayVersion))
				{
					info.strDisplayVersion.Empty();
				}
				if(!subreg.Read(_T("HelpLink"), &info.strHelpLink))
				{
					info.strHelpLink.Empty();
				}
				if(!subreg.Read(_T("HelpTelephone"), &info.strHelpTelephone))
				{
					info.strHelpTelephone.Empty();
				}
				if(!subreg.Read(_T("InstallDate"), &info.strInstallDate))
				{
					info.strInstallDate.Empty();

					memset(&info.installData,0,sizeof(tm));
				}else
				{
					int iStart = 0;
					CString strDate[6];		// 月日年时分秒
					int i = 0;
					do
					{
						strDate[i] = info.strInstallDate.Tokenize(_T("/ :"),iStart);

						if(iStart == info.strInstallDate.GetLength()+1)
						{
							strDate[0] = info.strInstallDate.Left(4);
							strDate[1] = info.strInstallDate.Mid(4,2);
							strDate[2] = info.strInstallDate.Right(2);
							break;
						}
					}while(!strDate[i++].IsEmpty());

					info.installData.tm_year = _ttoi(strDate[0]) - 1900;
					info.installData.tm_mon = _ttoi(strDate[1]) - 1;
					info.installData.tm_mday = _ttoi(strDate[2]) ;


					//info.strInstallDate
				}
				if(!subreg.Read(_T("InstallLocation"), &info.strInstallLocation))
				{
					info.strInstallLocation.Empty();
				}
				if(!subreg.Read(_T("ModifyPath"), &info.strModifyPath))
				{
					info.strModifyPath.Empty();
				}
				if(!subreg.Read(_T("UninstallString"), &info.strUninstallString))
				{
					info.strUninstallString.Empty();
				}
				if(!subreg.Read(_T("QuietUninstallString"), &info.strQuietUninstallString))
				{
					info.strQuietUninstallString.Empty();
				}
				
				if(!subreg.Read(_T("Publisher"), &info.strPublisher))
				{
					info.strPublisher.Empty();
				}
				if(!subreg.Read(_T("SystemComponent"), &info.dwSystemComponent))
				{
					info.dwSystemComponent = 0;
				}

				if(!subreg.Read(_T("Size"), &info.dwSize))
				{
					info.dwSize = 0;
				}
				if(!subreg.Read(_T("EstimatedSize"), &info.dwEstimatedSize))
				{
					info.dwEstimatedSize = 0;
				}
				if(subreg.Read(_T("Size"), &strValue))
				{
					info.szSize = _ttoi(strValue.GetBuffer());
				}else
				{
					info.szSize = 0;
				}
				
				m_softList.push_back(info);
				
			}
			subreg.Close();
		}while (!subkey.IsEmpty());
		reg.Close();

		dwCount += dwIndex;
	}
	return dwCount;
}

const SoftwareInfo* CSoftwareManager::GetSoftwareInfoAt(size_t index)
{
	return &m_softList[index];
}