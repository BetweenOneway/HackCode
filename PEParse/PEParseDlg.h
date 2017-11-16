
// PEParseDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// CPEParseDlg �Ի���
class CPEParseDlg : public CDialogEx
{
// ����
public:
	CPEParseDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PEPARSE_DIALOG };
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
	afx_msg void OnBnClickedButtonChoosefile();
	// ���ļ���������ӳ����ͼ
	BOOL FileMapping(TCHAR * szFileName);
	// DOSͷָ��
	PIMAGE_DOS_HEADER m_pDosHead;
	// PEͷָ��
	PIMAGE_NT_HEADERS m_pNtHeader;
	// �ڱ�ָ��
	PIMAGE_SECTION_HEADER m_pSecHead;
	// ӳ����ͼָ��
	LPVOID m_lpBase;
	// ӳ����
	HANDLE m_hMap;
	// �ļ����
	HANDLE m_hFile;
	afx_msg void OnBnClickedCancel();
	// �ж��Ƿ�ΪPE�ļ�������ȡ��ؽṹָ��
	BOOL IsPEFileAndGetPEPointer();
	// ��������PE�ֶ�
	VOID ParseBasePE();
	// �����б�
	CListCtrl m_SectionList;
	VOID EnumSections();
	afx_msg void OnBnClickedButtonCheck();
	CString filePath;
};
