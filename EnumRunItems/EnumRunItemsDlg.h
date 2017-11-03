
// EnumRunItemsDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"

// CEnumRunItemsDlg �Ի���
class CEnumRunItemsDlg : public CDialogEx
{
// ����
public:
	CEnumRunItemsDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENUMRUNITEMS_DIALOG };
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
	CListCtrl m_RunList;
	// ��ʼ���б��
	void InitRunList();
	// ��ʾ�������б�
	void ShowRunList();
	afx_msg void OnLvnItemchangedRunlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedAddrunitem();
	afx_msg void OnBnClickedDeleterunitem();
	afx_msg void OnBnClickedRefresh();
};
