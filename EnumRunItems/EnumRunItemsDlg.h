
// EnumRunItemsDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"

// CEnumRunItemsDlg 对话框
class CEnumRunItemsDlg : public CDialogEx
{
// 构造
public:
	CEnumRunItemsDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENUMRUNITEMS_DIALOG };
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
	CListCtrl m_RunList;
	// 初始化列表框
	void InitRunList();
	// 显示自启动列表
	void ShowRunList();
	afx_msg void OnLvnItemchangedRunlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedAddrunitem();
	afx_msg void OnBnClickedDeleterunitem();
	afx_msg void OnBnClickedRefresh();
};
