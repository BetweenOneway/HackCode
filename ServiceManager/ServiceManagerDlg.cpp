
// ServiceManagerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ServiceManager.h"
#include "ServiceManagerDlg.h"
#include "afxdialogex.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <Winsvc.h>
// CServiceManagerDlg �Ի���


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


// CServiceManagerDlg ��Ϣ�������

BOOL CServiceManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_ServiceList.SetExtendedStyle(m_ServiceList.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ServiceList.InsertColumn(0, _T("���"));
	m_ServiceList.InsertColumn(1, _T("������"));
	m_ServiceList.InsertColumn(2, _T("��ʾ��"));
	m_ServiceList.InsertColumn(3, _T("״̬"));
	m_ServiceList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_ServiceList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_ServiceList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
	m_ServiceList.SetColumnWidth(3, LVSCW_AUTOSIZE_USEHEADER);
	ShowServiceList(SERVICE_DRIVER);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CServiceManagerDlg::OnPaint()
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
			m_ServiceList.SetItemText(i, 3, _T("��ͣ"));
			break;
		case SERVICE_STOPPED:
			m_ServiceList.SetItemText(i, 3, _T("ֹͣ"));
			break;
		case SERVICE_RUNNING:
			m_ServiceList.SetItemText(i, 3, _T("����"));
			break;
		default:
			m_ServiceList.SetItemText(i, 3, _T("����"));
			break;
		}
	}
	CloseServiceHandle(hSCM);
}


void CServiceManagerDlg::OnBnClickedStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		m_ServiceList.SetItemText(nSelect, 3, _T("����"));
	}
	else
	{
		int n = GetLastError();
	}
}


void CServiceManagerDlg::OnBnClickedStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		m_ServiceList.SetItemText(nSelect, 3, _T("ֹͣ"));
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CServiceManagerDlg::OnLvnColumnclickServicelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CServiceManagerDlg::OnLinkclickServicelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CServiceManagerDlg::OnNMClickServicelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	nSelect = pNMItemActivate->iItem;
	
	*pResult = 0;
}


void CServiceManagerDlg::OnHdnItemclickServicelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CServiceManagerDlg::OnBnClickedRadio1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowServiceList(SERVICE_WIN32);
}


void CServiceManagerDlg::OnBnClickedRadio2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowServiceList(SERVICE_DRIVER);
}
