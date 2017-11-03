#pragma once
#include "afxwin.h"


// CRegAdd 对话框

class CRegAdd : public CDialogEx
{
	DECLARE_DYNAMIC(CRegAdd)

public:
	CRegAdd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRegAdd();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLGADDRUNITEM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 键名控件变量
	CEdit mEditKeyName;
	// 键值控件变量
	CEdit mEditKeyValue;
	TCHAR szKeyName[MAXBYTE];
	TCHAR szKeyValue[MAXBYTE];
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
