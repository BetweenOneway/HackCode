
// ProcessManagerDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"


// CProcessManagerDlg 对话框
class CProcessManagerDlg : public CDialogEx
{
// 构造
public:
	CProcessManagerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROCESSMANAGER_DIALOG };
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
	// 枚举系统进程
	int ShowProcess();
	// 枚举指定进程中加载的DLL
	int ShowModule();
	// 进程列表
	CListCtrl m_ProcessList;
	// 模块列表
	CListCtrl m_ModuleList;
	afx_msg void OnLvnItemchangedProcesslist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickProcesslist(NMHDR *pNMHDR, LRESULT *pResult);
private:
	int nSelect;
public:
	// 选中的进程号
	int iPid;
	// 调整当前进程权限
	void DebugPrivilege();
	afx_msg void OnBnClickedPauseprocess();
};
