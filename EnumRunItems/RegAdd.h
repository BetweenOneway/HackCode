#pragma once
#include "afxwin.h"


// CRegAdd �Ի���

class CRegAdd : public CDialogEx
{
	DECLARE_DYNAMIC(CRegAdd)

public:
	CRegAdd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRegAdd();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLGADDRUNITEM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// �����ؼ�����
	CEdit mEditKeyName;
	// ��ֵ�ؼ�����
	CEdit mEditKeyValue;
	TCHAR szKeyName[MAXBYTE];
	TCHAR szKeyValue[MAXBYTE];
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
