
// ProcessManagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ProcessManager.h"
#include "ProcessManagerDlg.h"
#include "afxdialogex.h"
#include <Tlhelp32.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CProcessManagerDlg 对话框



CProcessManagerDlg::CProcessManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROCESSMANAGER_DIALOG, pParent)
	, nSelect(0)
	, iPid(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProcessManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROCESSLIST, m_ProcessList);
	DDX_Control(pDX, IDC_DLLLIST, m_ModuleList);
}

BEGIN_MESSAGE_MAP(CProcessManagerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PROCESSLIST, &CProcessManagerDlg::OnLvnItemchangedProcesslist)
	ON_NOTIFY(NM_CLICK, IDC_PROCESSLIST, &CProcessManagerDlg::OnNMClickProcesslist)
	ON_BN_CLICKED(IDC_PAUSEPROCESS, &CProcessManagerDlg::OnBnClickedPauseprocess)
END_MESSAGE_MAP()


// CProcessManagerDlg 消息处理程序

BOOL CProcessManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//进程列表设置
	m_ProcessList.SetExtendedStyle(m_ProcessList.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	
	m_ProcessList.InsertColumn(0, _T("进程ID"));
	m_ProcessList.InsertColumn(1, _T("进程名"));
	
	m_ProcessList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_ProcessList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);

	//模块列表设置
	m_ModuleList.SetExtendedStyle(m_ModuleList.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	
	m_ModuleList.InsertColumn(0, _T("序号"));
	m_ModuleList.InsertColumn(1, _T("DLL名"));
	m_ModuleList.InsertColumn(2, _T("DLL路径"));

	m_ModuleList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_ModuleList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_ModuleList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);

	ShowProcess();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CProcessManagerDlg::OnPaint()
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
HCURSOR CProcessManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



// 枚举系统进程
int CProcessManagerDlg::ShowProcess()
{
	m_ProcessList.DeleteAllItems();
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	PROCESSENTRY32 pe32 = { 0 };
	pe32.dwSize = sizeof(PROCESSENTRY32);
	BOOL bRet = Process32First(hSnap, &pe32);
	CString strTmp;
	int iIndex = 0;
	while (bRet)
	{
		strTmp.Format(_T("%lu"), pe32.th32ProcessID);
		m_ProcessList.InsertItem(iIndex, strTmp);
		m_ProcessList.SetItemText(iIndex, 1, pe32.szExeFile);
		iIndex++;
		bRet = Process32Next(hSnap, &pe32);
	}
	return 0;
}


// 枚举指定进程中加载的DLL
int CProcessManagerDlg::ShowModule()
{
	m_ModuleList.DeleteAllItems();
	MODULEENTRY32 me32 = { 0 };
	me32.dwSize = sizeof(MODULEENTRY32);
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, iPid);
	BOOL bRet = Module32First(hSnap, &me32);
	CString strTemp;
	int iIndex = 0;
	while (bRet)
	{
		strTemp.Format(_T("%d"), iIndex);
		m_ModuleList.InsertItem(iIndex, strTemp);
		m_ModuleList.SetItemText(iIndex, 1, me32.szModule);
		m_ModuleList.SetItemText(iIndex, 2, me32.szExePath);
		iIndex++;
		bRet = Module32Next(hSnap, &me32);
	}
	return 0;
}


void CProcessManagerDlg::OnLvnItemchangedProcesslist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CProcessManagerDlg::OnNMClickProcesslist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	nSelect = pNMItemActivate->iItem;
	TCHAR szPid[MAXBYTE] = { 0 };
	m_ProcessList.GetItemText(nSelect, 0, szPid,MAXBYTE);
	iPid = _tstoi(szPid);
	ShowModule();
	*pResult = 0;
}


// 调整当前进程权限
void CProcessManagerDlg::DebugPrivilege()
{
	HANDLE hToken = NULL;
	BOOL bRet = OpenProcessToken(GetCurrentProcess(),TOKEN_ALL_ACCESS,&hToken);
	if (TRUE == bRet)
	{
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);

		CloseHandle(hToken);
	}
}


void CProcessManagerDlg::OnBnClickedPauseprocess()
{
	// TODO: Add your control notification handler code here
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, iPid);
	if (INVALID_HANDLE_VALUE == hSnap)
	{
		AfxMessageBox(_T("CreateToolhelp32Snapshot Error"));
		return;
	}
	THREADENTRY32 Te32 = { 0 };
	Te32.dwSize = sizeof(THREADENTRY32);
	BOOL bRet = Thread32First(hSnap, &Te32);
	while (bRet)
	{
		if (iPid == Te32.th32OwnerProcessID)
		{
			HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, Te32.th32ThreadID);
			SuspendThread(hThread);
			CloseHandle(hThread);
		}
		bRet = Thread32Next(hSnap, &Te32);
	}
}
