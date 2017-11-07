
// DLLInjectAndUnjectDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CDLLInjectAndUnjectDlg dialog
class CDLLInjectAndUnjectDlg : public CDialogEx
{
// Construction
public:
	CDLLInjectAndUnjectDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLLINJECTANDUNJECT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// DLL注入
	void InjectDll(DWORD dwPid, TCHAR * szDllName);
	// 待注入或卸载DLL（带路径）
	CEdit DllFullPath;
	// 进程ID
	CEdit Pid;
	afx_msg void OnBnClickedInject();
	void Privilege();
};
