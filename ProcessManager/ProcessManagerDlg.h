
// ProcessManagerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// CProcessManagerDlg �Ի���
class CProcessManagerDlg : public CDialogEx
{
// ����
public:
	CProcessManagerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESSMANAGER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// ö��ϵͳ����
	int ShowProcess();
	// ö��ָ�������м��ص�DLL
	int ShowModule();
	// �����б�
	CListCtrl m_ProcessList;
	// ģ���б�
	CListCtrl m_ModuleList;
	afx_msg void OnLvnItemchangedProcesslist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickProcesslist(NMHDR *pNMHDR, LRESULT *pResult);
private:
	int nSelect;
public:
	// ѡ�еĽ��̺�
	int iPid;
	// ������ǰ����Ȩ��
	void DebugPrivilege();
	afx_msg void OnBnClickedPauseprocess();
};
