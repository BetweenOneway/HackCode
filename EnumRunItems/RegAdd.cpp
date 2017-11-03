// RegAdd.cpp : 实现文件
//

#include "stdafx.h"
#include "EnumRunItems.h"
#include "RegAdd.h"
#include "afxdialogex.h"


// CRegAdd 对话框

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


// CRegAdd 消息处理程序


void CRegAdd::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	mEditKeyName.GetWindowText(szKeyName,MAXBYTE);
	mEditKeyValue.GetWindowText(szKeyValue, MAXBYTE);
	CDialogEx::OnOK();
}


void CRegAdd::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	ZeroMemory(szKeyName, MAXBYTE);
	ZeroMemory(szKeyValue, MAXBYTE);
	CDialogEx::OnCancel();
}
