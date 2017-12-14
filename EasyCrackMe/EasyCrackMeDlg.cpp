
// EasyCrackMeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EasyCrackMe.h"
#include "EasyCrackMeDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEasyCrackMeDlg �Ի���



CEasyCrackMeDlg::CEasyCrackMeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EASYCRACKME_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEasyCrackMeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CEasyCrackMeDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CEasyCrackMeDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CEasyCrackMeDlg ��Ϣ�������

BOOL CEasyCrackMeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CEasyCrackMeDlg::OnPaint()
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
HCURSOR CEasyCrackMeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CEasyCrackMeDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR szUserName[MAXBYTE] = { 0 };
	TCHAR szPassWord[MAXBYTE] = { 0 };
	TCHAR szTmpPassWord[MAXBYTE] = { 0 };

	GetDlgItemText(IDC_EDIT_USERNAME, szUserName, MAXBYTE);
	GetDlgItemText(IDC_EDIT_PASSWORD, szPassWord, MAXBYTE);
	if (0 == _tcslen(szUserName))
	{
		return;
	}
	if (0 == _tcslen(szPassWord))
	{
		return;
	}
	if (7 > _tcslen(szUserName))
	{
		return;
	}

	for (int i=0;i<_tcslen(szUserName);i++)
	{
		if ('Z' == szUserName[i] || 'z' == szUserName[i] || '9' == szUserName[i])
		{
			szTmpPassWord[i] = szUserName[i];
		}
		else
		{
			szTmpPassWord[i] = szUserName[i] + 1;
		}
	}

	if (0 == _tcscmp(szTmpPassWord,szPassWord))
	{
		MessageBox("Password Correct");
	}
	else
	{
		MessageBox("Password Wrong");
	}
	//CDialogEx::OnOK();
}
