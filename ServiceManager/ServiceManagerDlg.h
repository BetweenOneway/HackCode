
// ServiceManagerDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"


// CServiceManagerDlg 对话框
class CServiceManagerDlg : public CDialogEx
{
// 构造
public:
	CServiceManagerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVICEMANAGER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
