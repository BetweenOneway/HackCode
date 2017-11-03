// RegAdd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EnumRunItems.h"
#include "RegAdd.h"
#include "afxdialogex.h"


// CRegAdd �Ի���

IMPLEMENT_DYNAMIC(CRegAdd, CDialogEx)

CRegAdd::CRegAdd(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLGADDRUNITEM, pParent)
{
	ZeroMemory(szKeyName, MAXBYTE);
	ZeroMemory(szKeyValue, MAXBYTE);
}

CRegAdd::~CRegAdd()
{
}

void CRegAdd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_KEYNAME, mEditKeyName);
	DDX_Control(pDX, IDC_KEYVALUE, mEditKeyValue);
}


BEGIN_MESSAGE_MAP(CRegAdd, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRegAdd::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CRegAdd::OnBnClickedCancel)
END_MESSAGE_MAP()


// CRegAdd ��Ϣ�������


void CRegAdd::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	mEditKeyName.GetWindowText(szKeyName,MAXBYTE);
	mEditKeyValue.GetWindowText(szKeyValue, MAXBYTE);
	CDialogEx::OnOK();
}


void CRegAdd::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ZeroMemory(szKeyName, MAXBYTE);
	ZeroMemory(szKeyValue, MAXBYTE);
	CDialogEx::OnCancel();
}
