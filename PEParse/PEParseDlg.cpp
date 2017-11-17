
// PEParseDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PEParse.h"
#include "PEParseDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPEParseDlg �Ի���



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


// CPEParseDlg ��Ϣ�������

BOOL CPEParseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_SectionList.SetExtendedStyle(m_SectionList.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_SectionList.InsertColumn(0, _T("����"));
	m_SectionList.InsertColumn(1, _T("V.ƫ��"));
	m_SectionList.InsertColumn(2, _T("V.��С"));
	m_SectionList.InsertColumn(3, _T("R.ƫ��"));
	m_SectionList.InsertColumn(4, _T("R.��С"));
	m_SectionList.InsertColumn(5, _T("��־"));

	m_SectionList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_SectionList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_SectionList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
	m_SectionList.SetColumnWidth(3, LVSCW_AUTOSIZE_USEHEADER);
	m_SectionList.SetColumnWidth(4, LVSCW_AUTOSIZE_USEHEADER);
	m_SectionList.SetColumnWidth(5, LVSCW_AUTOSIZE_USEHEADER);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPEParseDlg::OnPaint()
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
HCURSOR CPEParseDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPEParseDlg::OnBnClickedButtonChoosefile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString filter = _T("��ִ���ļ�|*.exe|�����ļ�|*.*");;	//�ļ����ǵ�����
	CFileDialog openFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_READONLY, filter, NULL);
	INT_PTR result = openFileDlg.DoModal();
	if (result == IDOK) 
	{
		filePath = openFileDlg.GetPathName();
	}
	CWnd::SetDlgItemTextW(IDC_EDIT_FILEPATH, filePath);
}


// ���ļ���������ӳ����ͼ
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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


// �ж��Ƿ�ΪPE�ļ�������ȡ��ؽṹָ��
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


// ��������PE�ֶ�
VOID CPEParseDlg::ParseBasePE()
{
	CString strToShow;
	//��ڵ�ַ
	strToShow.Format(_T("%08x"), m_pNtHeader->OptionalHeader.AddressOfEntryPoint);
	SetDlgItemText(IDC_EDIT_ENTRYPOINT, strToShow);

	//ӳ�����ַ
	strToShow.Format(_T("%08x"), m_pNtHeader->OptionalHeader.ImageBase);
	SetDlgItemText(IDC_EDIT_MAPADDRESS, strToShow);

	//�������汾��
	strToShow.Format(_T("%d.%d"), m_pNtHeader->OptionalHeader.MajorLinkerVersion,m_pNtHeader->OptionalHeader.MinorLinkerVersion);
	SetDlgItemText(IDC_EDIT_LINKVERSION, strToShow);

	//�ڱ�����
	strToShow.Format(_T("%02x"),  m_pNtHeader->FileHeader.NumberOfSections);
	SetDlgItemText(IDC_EDIT_NUMSECTIONS, strToShow);

	//�ļ�����ֵ��С
	strToShow.Format(_T("%08x"), m_pNtHeader->OptionalHeader.FileAlignment);
	SetDlgItemText(IDC_EDIT_ALIGNMENT, strToShow);

	//�ڴ����ֵ��С
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
		//unsigned char���鲻��ֱ��ǿתΪLPCTSTR
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	FileMapping(filePath.GetBuffer());
	if (FALSE == IsPEFileAndGetPEPointer())
	{
		return;
	}
	ParseBasePE();
	EnumSections();
}
