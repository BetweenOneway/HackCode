
// PEParseDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"


// CPEParseDlg 对话框
class CPEParseDlg : public CDialogEx
{
// 构造
public:
	CPEParseDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PEPARSE_DIALOG };
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
	afx_msg void OnBnClickedButtonChoosefile();
	// 打开文件，并创建映像视图
	BOOL FileMapping(TCHAR * szFileName);
	// DOS头指针
	PIMAGE_DOS_HEADER m_pDosHead;
	// PE头指针
	PIMAGE_NT_HEADERS m_pNtHeader;
	// 节表指针
	PIMAGE_SECTION_HEADER m_pSecHead;
	// 映像视图指针
	LPVOID m_lpBase;
	// 映像句柄
	HANDLE m_hMap;
	// 文件句柄
	HANDLE m_hFile;
	afx_msg void OnBnClickedCancel();
	// 判断是否为PE文件，并获取相关结构指针
	BOOL IsPEFileAndGetPEPointer();
	// 解析基础PE字段
	VOID ParseBasePE();
	// 节区列表
	CListCtrl m_SectionList;
	VOID EnumSections();
	afx_msg void OnBnClickedButtonCheck();
	CString filePath;
};
