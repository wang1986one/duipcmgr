#pragma once
#include <time.h>
typedef struct tagSoftwareInfo
{
	tagSoftwareInfo():
	dwSize(0)
	,dwEstimatedSize(0)
	,szSize(0)
	,dwSystemComponent(0)
	{}
	CString strKey;				//
	CString strComments;		// ע��
	CString strContact;			// ��ϵ��ʽ
	CString strDisplayIcon;		// ͼ��
	CString strDisplayName;		// ����
	CString strDisplayVersion;	// �汾
	CString strHelpLink;		// ��������
	CString strHelpTelephone;	// �����绰
	CString strInstallDate;		// ��װ����
	CString strInstallLocation;	// ��װ·��
	CString strModifyPath;		// �޸�
	CString strUninstallString;	// ж��
	CString strQuietUninstallString;	// ��Ĭж��
	CString strPublisher;		// ������
	DWORD dwSystemComponent;	// 1Ϊϵͳ�����Ĭ�ϲ���ʾ
	DWORD dwSize;				// �������ȼ���ʾ ��dwSize > dwEstimatedSize > szSize
	DWORD dwEstimatedSize;
	DWORD szSize;
	tm installData;
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