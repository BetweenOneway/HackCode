
// EnumRunItemsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EnumRunItems.h"
#include "EnumRunItemsDlg.h"
#include "afxdialogex.h"
#include "RegAdd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEnumRunItemsDlg �Ի���

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


// CEnumRunItemsDlg ��Ϣ�������

BOOL CEnumRunItemsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	InitRunList();
	ShowRunList();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CEnumRunItemsDlg::OnPaint()
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
HCURSOR CEnumRunItemsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



// ��ʼ���б��
void CEnumRunItemsDlg::InitRunList()
{
	m_RunList.SetExtendedStyle(m_RunList.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_RunList.InsertColumn(0, _T("NO."));
	m_RunList.InsertColumn(1, _T("����"));
	m_RunList.InsertColumn(2, _T("��ֵ"));

	m_RunList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_RunList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_RunList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
}


// ��ʾ�������б�
void CEnumRunItemsDlg::ShowRunList()
{
	m_RunList.DeleteAllItems();

	DWORD dwType = 0;
	DWORD dwBufferSize = MAXBYTE;
	DWORD dwKeySize = MAXBYTE;
	TCHAR szValueName[MAXBYTE] = { 0 };
	TCHAR szValueKey[MAXBYTE] = { 0 };


	/*
	32λ������64λϵͳ������ʱ������ע���HKLM\Software ���Զ�ת�� Software\Wow6432Node��
	64λ������64λϵͳ������ʱ������ע���ʱ�����Կ���HKLM\Software��HKLM\Software\Wow6432Node
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
			AfxMessageBox(_T("����������������"));
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = m_RunList.GetFirstSelectedItemPosition();
	int nSelected = -1;
	while (pos)
	{
		nSelected = m_RunList.GetNextSelectedItem(pos);
	}
	if (-1 == nSelected)
	{
		AfxMessageBox(_T("��ѡ��Ҫɾ����������"));
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowRunList();
}
