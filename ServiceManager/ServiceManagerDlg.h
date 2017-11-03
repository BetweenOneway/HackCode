
// ServiceManagerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// CServiceManagerDlg �Ի���
class CServiceManagerDlg : public CDialogEx
{
// ����
public:
	CServiceManagerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVICEMANAGER_DIALOG };
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
	CListCtrl m_ServiceList;
	void ShowServiceList(DWORD dwServiceType);
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
	afx_msg void OnLvnItemchangedServicelist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclickServicelist(NMHDR *pNMHDR, LRESULT *pResult);
private:
	int nSelect;
	TCHAR szServiceName[MAXBYTE];
public:
	afx_msg void OnLinkclickServicelist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickServicelist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnItemclickServicelist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
};
