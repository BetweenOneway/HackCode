
// ServiceManagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ServiceManager.h"
#include "ServiceManagerDlg.h"
#include "afxdialogex.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <Winsvc.h>
// CServiceManagerDlg 对话框


CServiceManagerDlg::CServiceManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SERVICEMANAGER_DIALOG, pParent)
	, nSelect(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServiceManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERVICELIST, m_ServiceList);
}

BEGIN_MESSAGE_MAP(CServiceManagerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &CServiceManagerDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_STOP, &CServiceManagerDlg::OnBnClickedStop)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SERVICELIST, &CServiceManagerDlg::OnLvnItemchangedServicelist)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_SERVICELIST, &CServiceManagerDlg::OnLvnColumnclickServicelist)
	ON_NOTIFY(LVN_LINKCLICK, IDC_SERVICELIST, &CServiceManagerDlg::OnLinkclickServicelist)
	ON_NOTIFY(NM_CLICK, IDC_SERVICELIST, &CServiceManagerDlg::OnNMClickServicelist)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CServiceManagerDlg::OnHdnItemclickServicelist)
	ON_BN_CLICKED(IDC_RADIO1, &CServiceManagerDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CServiceManagerDlg::OnBnClickedRadio2)
END_MESSAGE_MAP()


// CServiceManagerDlg 消息处理程序

BOOL CServiceManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_ServiceList.SetExtendedStyle(m_ServiceList.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ServiceList.InsertColumn(0, _T("序号"));
	m_ServiceList.InsertColumn(1, _T("服务名"));
	m_ServiceList.InsertColumn(2, _T("显示名"));
	m_ServiceList.InsertColumn(3, _T("状态"));
	m_ServiceList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_ServiceList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_ServiceList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
	m_ServiceList.SetColumnWidth(3, LVSCW_AUTOSIZE_USEHEADER);
	ShowServiceList(SERVICE_DRIVER);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CServiceManagerDlg::OnPaint()
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
HCURSOR CServiceManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CServiceManagerDlg::ShowServiceList(DWORD dwServiceType)
{
	m_ServiceList.DeleteAllItems();
	
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == hSCM)
	{
		AfxMessageBox(_T("OpenSCManager Error"));
	}

	DWORD dwBuffSize = 1024 * sizeof(ENUM_SERVICE_STATUS);
	DWORD dwByteNeeded, dwServicesReturned, lpResumeHandle = 0;
	ENUM_SERVICE_STATUS SerStatus[1024] = { 0 };

	BOOL bRet = EnumServicesStatus(hSCM, dwServiceType, SERVICE_STATE_ALL, SerStatus, dwBuffSize, &dwByteNeeded, &dwServicesReturned, &lpResumeHandle);
	if (FALSE == bRet)
	{
		DWORD dwErrorCode = GetLastError();
		AfxMessageBox(_T("EnumServicesStatus Error"));
		CloseServiceHandle(hSCM);
		return;
	}
	for (DWORD i = 0; i < dwServicesReturned; i++)
	{
		CString str;
		str.Format(_T("%d"), i);
		m_ServiceList.InsertItem(i, str);
		m_ServiceList.SetItemText(i, 1, SerStatus[i].lpServiceName);
		m_ServiceList.SetItemText(i, 2, SerStatus[i].lpDisplayName);
		switch (SerStatus[i].ServiceStatus.dwCurrentState)
		{
		case SERVICE_PAUSED:
			m_ServiceList.SetItemText(i, 3, _T("暂停"));
			break;
		case SERVICE_STOPPED:
			m_ServiceList.SetItemText(i, 3, _T("停止"));
			break;
		case SERVICE_RUNNING:
			m_ServiceList.SetItemText(i, 3, _T("运行"));
			break;
		default:
			m_ServiceList.SetItemText(i, 3, _T("其他"));
			break;
		}
	}
	CloseServiceHandle(hSCM);
}


void CServiceManagerDlg::OnBnClickedStart()
{
	// TODO: 在此添加控件通知处理程序代码
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == hSCM)
	{
		AfxMessageBox(_T("OpenSCManager error"));
		return;
	}
	SC_HANDLE hSCService = OpenService(hSCM, szServiceName, SERVICE_ALL_ACCESS);
	BOOL bRet = StartService(hSCService, 0, NULL);
	if (TRUE == bRet)
	{
		m_ServiceList.SetItemText(nSelect, 3, _T("运行"));
	}
	else
	{
		int n = GetLastError();
	}
}


void CServiceManagerDlg::OnBnClickedStop()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR szServiceName[MAXBYTE] = {0};
	m_ServiceList.GetItemText(nSelect, 1, szServiceName, MAXBYTE);

	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == hSCM)
	{
		AfxMessageBox(_T("OpenSCManager error"));
		return;
	}
	SC_HANDLE hSCService = OpenService(hSCM, szServiceName, SERVICE_ALL_ACCESS);
	SERVICE_STATUS ServiceStatus;
	BOOL bRet = ControlService(hSCService, SERVICE_CONTROL_STOP, &ServiceStatus);
	if (TRUE == bRet)
	{
		m_ServiceList.SetItemText(nSelect, 3, _T("停止"));
	}
	else
	{
		int n = GetLastError();
	}
	CloseServiceHandle(hSCService);
	CloseServiceHandle(hSCM);
}


void CServiceManagerDlg::OnLvnItemchangedServicelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CServiceManagerDlg::OnLvnColumnclickServicelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CServiceManagerDlg::OnLinkclickServicelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CServiceManagerDlg::OnNMClickServicelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	nSelect = pNMItemActivate->iItem;
	
	*pResult = 0;
}


void CServiceManagerDlg::OnHdnItemclickServicelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CServiceManagerDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowServiceList(SERVICE_WIN32);
}


void CServiceManagerDlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowServiceList(SERVICE_DRIVER);
}
