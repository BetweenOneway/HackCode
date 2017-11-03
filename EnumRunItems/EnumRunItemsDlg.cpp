
// EnumRunItemsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EnumRunItems.h"
#include "EnumRunItemsDlg.h"
#include "afxdialogex.h"
#include "RegAdd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEnumRunItemsDlg 对话框

LPCTSTR REG_RUN = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");

CEnumRunItemsDlg::CEnumRunItemsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ENUMRUNITEMS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEnumRunItemsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RUNLIST, m_RunList);
}

BEGIN_MESSAGE_MAP(CEnumRunItemsDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_RUNLIST, &CEnumRunItemsDlg::OnLvnItemchangedRunlist)
	ON_BN_CLICKED(IDC_ADDRUNITEM, &CEnumRunItemsDlg::OnBnClickedAddrunitem)
	ON_BN_CLICKED(IDC_DELETERUNITEM, &CEnumRunItemsDlg::OnBnClickedDeleterunitem)
	ON_BN_CLICKED(IDC_REFRESH, &CEnumRunItemsDlg::OnBnClickedRefresh)
END_MESSAGE_MAP()


// CEnumRunItemsDlg 消息处理程序

BOOL CEnumRunItemsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	InitRunList();
	ShowRunList();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CEnumRunItemsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CEnumRunItemsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



// 初始化列表框
void CEnumRunItemsDlg::InitRunList()
{
	m_RunList.SetExtendedStyle(m_RunList.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_RunList.InsertColumn(0, _T("NO."));
	m_RunList.InsertColumn(1, _T("键名"));
	m_RunList.InsertColumn(2, _T("键值"));

	m_RunList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_RunList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_RunList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
}


// 显示自启动列表
void CEnumRunItemsDlg::ShowRunList()
{
	m_RunList.DeleteAllItems();

	DWORD dwType = 0;
	DWORD dwBufferSize = MAXBYTE;
	DWORD dwKeySize = MAXBYTE;
	TCHAR szValueName[MAXBYTE] = { 0 };
	TCHAR szValueKey[MAXBYTE] = { 0 };


	/*
	32位程序在64位系统内运行时，访问注册表HKLM\Software 被自动转向到 Software\Wow6432Node。
	64位程序在64位系统内运行时，访问注册表时，可以看到HKLM\Software和HKLM\Software\Wow6432Node
	*/
	HKEY hKey = NULL;
	LONG lRet = RegOpenKey(HKEY_LOCAL_MACHINE, REG_RUN, &hKey);
	if (ERROR_SUCCESS != lRet)
	{
		TCHAR MsgBuf[MAXBYTE] = { 0 };
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS, NULL, lRet, 0, MsgBuf, MAXBYTE,0);
		AfxMessageBox(MsgBuf);
		return;
	}

	int i = 0;
	CString strTmp;
	while (TRUE)
	{
		dwBufferSize = MAXBYTE;
		dwKeySize = MAXBYTE;
		lRet = RegEnumValue(hKey, i, szValueName, &dwBufferSize, NULL, &dwType, (unsigned char *)szValueKey, &dwKeySize); 
		if (ERROR_NO_MORE_ITEMS == lRet)
		{
			break;
		}
		
		strTmp.Format(_T("%d"), i);
		m_RunList.InsertItem(i, strTmp);
		m_RunList.SetItemText(i, 1, szValueName);
		m_RunList.SetItemText(i, 2, szValueKey);
		i++;

		ZeroMemory(szValueName, MAXBYTE);
		ZeroMemory(szValueKey, MAXBYTE);
		dwType = 0;
	}
	RegCloseKey(hKey);
}


void CEnumRunItemsDlg::OnLvnItemchangedRunlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CEnumRunItemsDlg::OnBnClickedAddrunitem()
{
	CRegAdd regAdd;
	if (IDOK == regAdd.DoModal())
	{
		if (_tcslen(regAdd.szKeyName) > 0 && _tcslen(regAdd.szKeyValue) > 0)
		{
			HKEY hKey = NULL;
			LONG lRet = RegOpenKey(HKEY_LOCAL_MACHINE, REG_RUN, &hKey);
			if (ERROR_SUCCESS != lRet)
			{
				return;
			}
			lRet = RegSetValueEx(hKey, regAdd.szKeyName, NULL, REG_SZ, (const unsigned char *)regAdd.szKeyValue, _tcslen(regAdd.szKeyValue) + sizeof(TCHAR));
			if (ERROR_SUCCESS != lRet)
			{
				TCHAR MsgBuf[MAXBYTE] = { 0 };
				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, lRet, 0, MsgBuf, MAXBYTE, 0);
				AfxMessageBox(MsgBuf);
			}
			RegCloseKey(hKey);
			ShowRunList();
		}
		else
		{
			AfxMessageBox(_T("请输入完整的内容"));
			return;
		}
	}
	else
	{
		return;
	}

}

void CEnumRunItemsDlg::OnBnClickedDeleterunitem()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_RunList.GetFirstSelectedItemPosition();
	int nSelected = -1;
	while (pos)
	{
		nSelected = m_RunList.GetNextSelectedItem(pos);
	}
	if (-1 == nSelected)
	{
		AfxMessageBox(_T("请选择要删除的启动项"));
		return;
	}
	TCHAR szKeyName[MAXBYTE] = { 0 };
	m_RunList.GetItemText(nSelected, 1, szKeyName, MAXBYTE);

	AfxMessageBox(szKeyName);
	HKEY hKey = NULL;
	LONG lRet = RegOpenKey(HKEY_LOCAL_MACHINE, REG_RUN, &hKey);
	RegDeleteValue(hKey, szKeyName);
	RegCloseKey(hKey);
	ShowRunList();
}


void CEnumRunItemsDlg::OnBnClickedRefresh()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowRunList();
}
