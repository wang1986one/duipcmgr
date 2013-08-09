#pragma once

typedef struct tagSoftwareInfo
{
	tagSoftwareInfo():
	dwSize(0)
	{}
	CString strComments;		// 注释
	CString strContact;			// 联系方式
	CString strDisplayIcon;		// 图标
	CString strDisplayName;		// 名称
	CString strDisplayVersion;	// 版本
	CString strHelpLink;		// 帮助连接
	CString strHelpTelephone;	// 帮助电话
	CString strInstallDate;		// 安装日期
	CString strInstallLocation;	// 安装路径
	CString strModifyPath;
	CString strUninstallString;
	CString strPublisher;
	DWORD dwSystemComponent;	// 1为系统组件，默认不显示

	DWORD dwSize;				// 按此优先级显示 ：dwSize > dwEstimatedSize > szSize
	DWORD dwEstimatedSize;
	DWORD szSize;

}SoftwareInfo,*PSoftwareInfo;

class CSoftwareManager
{
public:
	CSoftwareManager();

	DWORD EnumSoftware();
	const SoftwareInfo* GetSoftwareInfoAt(size_t index);
private:


	vector<SoftwareInfo> m_softList;
};