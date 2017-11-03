
// ProcessManagerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ProcessManager.h"
#include "ProcessManagerDlg.h"
#include "afxdialogex.h"
#include <Tlhelp32.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CProcessManagerDlg �Ի���



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


// CProcessManagerDlg ��Ϣ�������

BOOL CProcessManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//�����б�����
	m_ProcessList.SetExtendedStyle(m_ProcessList.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	
	m_ProcessList.InsertColumn(0, _T("����ID"));
	m_ProcessList.InsertColumn(1, _T("������"));
	
	m_ProcessList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_ProcessList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);

	//ģ���б�����
	m_ModuleList.SetExtendedStyle(m_ModuleList.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	
	m_ModuleList.InsertColumn(0, _T("���"));
	m_ModuleList.InsertColumn(1, _T("DLL��"));
	m_ModuleList.InsertColumn(2, _T("DLL·��"));

	m_ModuleList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_ModuleList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_ModuleList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);

	ShowProcess();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CProcessManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CProcessManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



// ö��ϵͳ����
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


// ö��ָ�������м��ص�DLL
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


// ������ǰ����Ȩ��
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
