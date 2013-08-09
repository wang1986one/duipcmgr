#pragma once

typedef struct tagSoftwareInfo
{
	tagSoftwareInfo():
	dwSize(0)
	{}
	CString strComments;		// ע��
	CString strContact;			// ��ϵ��ʽ
	CString strDisplayIcon;		// ͼ��
	CString strDisplayName;		// ����
	CString strDisplayVersion;	// �汾
	CString strHelpLink;		// ��������
	CString strHelpTelephone;	// �����绰
	CString strInstallDate;		// ��װ����
	CString strInstallLocation;	// ��װ·��
	CString strModifyPath;
	CString strUninstallString;
	CString strPublisher;
	DWORD dwSystemComponent;	// 1Ϊϵͳ�����Ĭ�ϲ���ʾ

	DWORD dwSize;				// �������ȼ���ʾ ��dwSize > dwEstimatedSize > szSize
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