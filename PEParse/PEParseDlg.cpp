
// PEParseDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PEParse.h"
#include "PEParseDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPEParseDlg 对话框



CPEParseDlg::CPEParseDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PEPARSE_DIALOG, pParent)
	, filePath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPEParseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SECTION, m_SectionList);
}

BEGIN_MESSAGE_MAP(CPEParseDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CHOOSEFILE, &CPEParseDlg::OnBnClickedButtonChoosefile)
	ON_BN_CLICKED(IDCANCEL, &CPEParseDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_CHECK, &CPEParseDlg::OnBnClickedButtonCheck)
END_MESSAGE_MAP()


// CPEParseDlg 消息处理程序

BOOL CPEParseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_SectionList.SetExtendedStyle(m_SectionList.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_SectionList.InsertColumn(0, _T("节名"));
	m_SectionList.InsertColumn(1, _T("V.偏移"));
	m_SectionList.InsertColumn(2, _T("V.大小"));
	m_SectionList.InsertColumn(3, _T("R.偏移"));
	m_SectionList.InsertColumn(4, _T("R.大小"));
	m_SectionList.InsertColumn(5, _T("标志"));

	m_SectionList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_SectionList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_SectionList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
	m_SectionList.SetColumnWidth(3, LVSCW_AUTOSIZE_USEHEADER);
	m_SectionList.SetColumnWidth(4, LVSCW_AUTOSIZE_USEHEADER);
	m_SectionList.SetColumnWidth(5, LVSCW_AUTOSIZE_USEHEADER);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPEParseDlg::OnPaint()
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
HCURSOR CPEParseDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPEParseDlg::OnBnClickedButtonChoosefile()
{
	// TODO: 在此添加控件通知处理程序代码
	CString filter = _T("可执行文件|*.exe|所有文件|*.*");;	//文件过虑的类型
	CFileDialog openFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_READONLY, filter, NULL);
	INT_PTR result = openFileDlg.DoModal();
	if (result == IDOK) 
	{
		filePath = openFileDlg.GetPathName();
	}
	CWnd::SetDlgItemTextW(IDC_EDIT_FILEPATH, filePath);
}


// 打开文件，并创建映像视图
BOOL CPEParseDlg::FileMapping(TCHAR * szFileName)
{
	m_hFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == m_hFile)
	{
		return FALSE;
	}
	m_hMap = CreateFileMapping(m_hFile, NULL, PAGE_READWRITE | SEC_IMAGE, 0, 0, 0);
	if (NULL == m_hMap)
	{
		CloseHandle(m_hFile);
		return FALSE;
	}

	m_lpBase = MapViewOfFile(m_hMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
	if (NULL == m_lpBase)
	{
		CloseHandle(m_hMap);
		CloseHandle(m_hFile);
		return FALSE;
	}
	return TRUE;
}


void CPEParseDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (NULL == m_lpBase)
	{
		UnmapViewOfFile(m_lpBase);
	}
	if (NULL != m_hMap)
	{
		CloseHandle(m_hMap);
	}
	
	if (INVALID_HANDLE_VALUE != m_hFile)
	{
		CloseHandle(m_hFile);
	}
	
	CDialogEx::OnCancel();
}


// 判断是否为PE文件，并获取相关结构指针
BOOL CPEParseDlg::IsPEFileAndGetPEPointer()
{
	m_pDosHead = (PIMAGE_DOS_HEADER)m_lpBase;
	if (IMAGE_DOS_SIGNATURE != m_pDosHead->e_magic )
	{
		return FALSE;
	}
	m_pNtHeader = (PIMAGE_NT_HEADERS)((DWORD)m_lpBase + m_pDosHead->e_lfanew);
	if (IMAGE_NT_SIGNATURE != m_pNtHeader->Signature)
	{
		return FALSE;
	}
	m_pSecHead = (PIMAGE_SECTION_HEADER)((DWORD)&(m_pNtHeader->OptionalHeader) + m_pNtHeader->FileHeader.SizeOfOptionalHeader);

	return TRUE;
}


// 解析基础PE字段
VOID CPEParseDlg::ParseBasePE()
{
	CString strToShow;
	//入口地址
	strToShow.Format(_T("%08x"), m_pNtHeader->OptionalHeader.AddressOfEntryPoint);
	SetDlgItemText(IDC_EDIT_ENTRYPOINT, strToShow);

	//映像基地址
	strToShow.Format(_T("%08x"), m_pNtHeader->OptionalHeader.ImageBase);
	SetDlgItemText(IDC_EDIT_MAPADDRESS, strToShow);

	//连接器版本号
	strToShow.Format(_T("%d.%d"), m_pNtHeader->OptionalHeader.MajorLinkerVersion,m_pNtHeader->OptionalHeader.MinorLinkerVersion);
	SetDlgItemText(IDC_EDIT_LINKVERSION, strToShow);

	//节表数量
	strToShow.Format(_T("%02x"),  m_pNtHeader->FileHeader.NumberOfSections);
	SetDlgItemText(IDC_EDIT_NUMSECTIONS, strToShow);

	//文件对齐值大小
	strToShow.Format(_T("%08x"), m_pNtHeader->OptionalHeader.FileAlignment);
	SetDlgItemText(IDC_EDIT_ALIGNMENT, strToShow);

	//内存对齐值大小
	strToShow.Format(_T("%08x"), m_pNtHeader->OptionalHeader.SectionAlignment);
	SetDlgItemText(IDC_EDIT_MEMALIGNMENT, strToShow);

	return VOID();
}


VOID CPEParseDlg::EnumSections()
{
	int iSecNum = m_pNtHeader->FileHeader.NumberOfSections;
	int iLoop = 0;
	CString strToShow;
	for (iLoop = 0; iLoop < iSecNum; iLoop++)
	{
		//unsigned char数组不能直接强转为LPCTSTR
		USES_CONVERSION;
		m_SectionList.InsertItem(iLoop, A2CT((char *)m_pSecHead[iLoop].Name));

		strToShow.Format(_T("%08x"), m_pSecHead[iLoop].VirtualAddress);
		m_SectionList.SetItemText(iLoop, 1, strToShow);

		strToShow.Format(_T("%08x"), m_pSecHead[iLoop].Misc.VirtualSize);
		m_SectionList.SetItemText(iLoop, 2, strToShow);

		strToShow.Format(_T("%08x"), m_pSecHead[iLoop].PointerToRawData);
		m_SectionList.SetItemText(iLoop, 3, strToShow);

		strToShow.Format(_T("%08x"), m_pSecHead[iLoop].SizeOfRawData);
		m_SectionList.SetItemText(iLoop, 4, strToShow);

		strToShow.Format(_T("%08x"), m_pSecHead[iLoop].Characteristics);
		m_SectionList.SetItemText(iLoop, 5, strToShow);
	}
	return VOID();
}


void CPEParseDlg::OnBnClickedButtonCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	FileMapping(filePath.GetBuffer());
	if (FALSE == IsPEFileAndGetPEPointer())
	{
		return;
	}
	ParseBasePE();
	EnumSections();
}
