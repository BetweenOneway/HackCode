
// DLLInjectAndUnjectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DLLInjectAndUnject.h"
#include "DLLInjectAndUnjectDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDLLInjectAndUnjectDlg dialog



CDLLInjectAndUnjectDlg::CDLLInjectAndUnjectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLLINJECTANDUNJECT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDLLInjectAndUnjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DLL, DllFullPath);
	DDX_Control(pDX, IDC_PID, Pid);
}

BEGIN_MESSAGE_MAP(CDLLInjectAndUnjectDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INJECT, &CDLLInjectAndUnjectDlg::OnBnClickedInject)
END_MESSAGE_MAP()


// CDLLInjectAndUnjectDlg message handlers

BOOL CDLLInjectAndUnjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDLLInjectAndUnjectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDLLInjectAndUnjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



// DLL×¢Èë
void CDLLInjectAndUnjectDlg::InjectDll(DWORD dwPid, TCHAR * szDllName)
{
	if (0 == dwPid || 0 == _tcslen(szDllName))
	{
		return;
	}

#ifdef UNICODE
	char* pFunName = "LoadLibraryW";
#else
	char* pFunName = "LoadLibraryA";
#endif

	
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	if (INVALID_HANDLE_VALUE == hProcess)
	{
		return;
	}
	int nDllLen = _tcslen(szDllName) + sizeof(TCHAR);
	PVOID pDllAddr = VirtualAllocEx(hProcess, NULL, nDllLen, MEM_COMMIT, PAGE_READWRITE);
	if (NULL == pDllAddr)
	{
		CloseHandle(hProcess);
		return;
	}
	SIZE_T dwWriteNum = 0;
	WriteProcessMemory(hProcess, pDllAddr, szDllName, nDllLen,&dwWriteNum);
	FARPROC pFunAddr = GetProcAddress(GetModuleHandle(_T("Kernel32.dll")), pFunName);

	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFunAddr, pDllAddr, 0, NULL);
	if (NULL == hThread)
	{
		DWORD dwErrorCode = GetLastError();
		TCHAR MsgBuf[MAXBYTE] = { 0 };
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwErrorCode, 0, MsgBuf, MAXBYTE, 0);
		AfxMessageBox(MsgBuf);
		return;
	}
	WaitForSingleObject(hThread, INFINITE);

	CloseHandle(hThread);
	CloseHandle(hProcess);
}


void CDLLInjectAndUnjectDlg::OnBnClickedInject()
{
	// TODO: Add your control notification handler code here
	TCHAR szDllFullPath[MAX_PATH] = { 0 };
	TCHAR szPid[MAX_PATH] = { 0 };
	
	DllFullPath.GetWindowText(szDllFullPath, MAX_PATH);
	Pid.GetWindowText(szPid, MAX_PATH);
	
	DWORD dwPid = _ttol(szPid);

	Privilege();
	InjectDll(dwPid, szDllFullPath);
}


void CDLLInjectAndUnjectDlg::Privilege()
{
	HANDLE hToken = NULL;
	BOOL bRet = OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken);
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
