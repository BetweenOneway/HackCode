
// PEParseDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PEParse.h"
#include "PEParseDlg.h"
#include "afxdialogex.h"
//������
#include <stdexcept>
using namespace std;

#include "PublicFunction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPEParseDlg �Ի���
#define NAMELEN (20*sizeof(TCHAR))
#define SIGNLEN (32*sizeof(BYTE))

typedef struct _SIGN
{
	TCHAR szName[NAMELEN];
	BYTE bSign[SIGNLEN + 1];
}SIGN,*PSIGN;

SIGN Sign[2] = 
{
	//vc6
	{
		_T("VC6"),
		(BYTE)"\x55\x8B\xEC\x6A\xFF\x68\xC0\x54\x41\x00"\
		"\x68\xF8\x26\x40\x00\x64\xA1\x00\x00\x00"\
		"\x00\x50\x64\x89\x25\x00\x00\x00\x00\x83"\
		"\xC4\x94"
	},
	//aspack
	{
		_T("ASPACK"),
		(BYTE)"\x60\xE8\x03\x00\x00\x00\xE9\xEB\x04\x5D"\
		"\x45\x55\xC3\xE8\x01\x00\x00\x00\xEB\x5D"\
		"\xBB\xED\xFF\xFF\xFF\x03\xDD\x81\xEB\x00"\
		"\xB0\x01"
	}
};

CPEParseDlg::CPEParseDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PEPARSE_DIALOG, pParent)
	, filePath(_T(""))
	, m_nSelect(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPEParseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SECTION, m_SectionList);
	DDX_Control(pDX, IDC_EDIT_VA, m_CtlVA);
	DDX_Control(pDX, IDC_EDIT_RVA, m_CtlRVA);
	DDX_Control(pDX, IDC_EDIT_FILEOFFSET, m_CtlFileOffset);
}

BEGIN_MESSAGE_MAP(CPEParseDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CHOOSEFILE, &CPEParseDlg::OnBnClickedButtonChoosefile)
	ON_BN_CLICKED(IDCANCEL, &CPEParseDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_CHECK, &CPEParseDlg::OnBnClickedButtonCheck)
	ON_BN_CLICKED(IDC_BUTTON_VA, &CPEParseDlg::OnBnClickedButtonVa)
	ON_BN_CLICKED(IDC_BUTTON_RVA, &CPEParseDlg::OnBnClickedButtonRva)
	ON_BN_CLICKED(IDC_BUTTON_FILEOFFSET, &CPEParseDlg::OnBnClickedButtonFileoffset)
	ON_BN_CLICKED(IDC_BUTTON_CALC, &CPEParseDlg::OnBnClickedButtonCalc)
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

int filter(unsigned int code, struct _EXCEPTION_POINTERS *ep)
{

	puts("in filter.");

	if (code == EXCEPTION_ACCESS_VIOLATION) 
	{
		puts("caught AV as expected.");
		return EXCEPTION_EXECUTE_HANDLER;
	}
	else 
	{
		puts("didn't catch AV, unexpected.");
		return EXCEPTION_CONTINUE_SEARCH;
	};

}



// �ж��Ƿ�ΪPE�ļ�������ȡ��ؽṹָ��
BOOL CPEParseDlg::IsPEFileAndGetPEPointer()
{
	__try
	{
		m_pDosHead = (PIMAGE_DOS_HEADER)m_lpBase;
		if (IMAGE_DOS_SIGNATURE != m_pDosHead->e_magic)
		{
			return FALSE;
		}
		m_pNtHeader = (PIMAGE_NT_HEADERS)((DWORD)m_lpBase + m_pDosHead->e_lfanew);
		if (IMAGE_NT_SIGNATURE != m_pNtHeader->Signature)
		{
			return FALSE;
		}
		m_pSecHead = (PIMAGE_SECTION_HEADER)((DWORD)&(m_pNtHeader->OptionalHeader) + m_pNtHeader->FileHeader.SizeOfOptionalHeader);
	}
	//__except(EXCEPTION_EXECUTE_HANDLER)
	__except (filter(GetExceptionCode(), GetExceptionInformation()))
	{
		int i = 0;
		i++;
	}

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
	GetPEPackInfo();
}


// ��ȡPE����Ϣ
VOID CPEParseDlg::GetPEPackInfo()
{
	PBYTE pSign = NULL;
	pSign = (PBYTE)((DWORD)m_lpBase + m_pNtHeader->OptionalHeader.AddressOfEntryPoint);
	if (0 == memcmp(Sign[0].bSign,pSign,SIGNLEN))
	{
		SetDlgItemText(IDC_EDIT_PEPACKINFO, Sign[0].szName);
	}
	else if (0 == memcmp(Sign[1].bSign, pSign, SIGNLEN))
	{
		SetDlgItemText(IDC_EDIT_PEPACKINFO, Sign[1].szName);
	}
	else
	{
		SetDlgItemText(IDC_EDIT_PEPACKINFO, _T("δ֪"));
	}
	return VOID();
}


void CPEParseDlg::OnBnClickedButtonVa()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_nSelect = 1;
	m_CtlVA.SetReadOnly(FALSE);
	m_CtlRVA.SetReadOnly(TRUE);
	m_CtlFileOffset.SetReadOnly(TRUE);
}


void CPEParseDlg::OnBnClickedButtonRva()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_nSelect = 2;
	m_CtlVA.SetReadOnly(TRUE);
	m_CtlRVA.SetReadOnly(FALSE);
	m_CtlFileOffset.SetReadOnly(TRUE);
}


void CPEParseDlg::OnBnClickedButtonFileoffset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_nSelect = 3;
	m_CtlVA.SetReadOnly(TRUE);
	m_CtlRVA.SetReadOnly(TRUE);
	m_CtlFileOffset.SetReadOnly(FALSE);
}


// ��ȡ����ĵ�ַ
DWORD CPEParseDlg::GetAddr()
{
	TCHAR szAddr[10] = { 0 };
	DWORD dwAddr = 0;

	switch (m_nSelect)
	{
	case 1:
		GetDlgItemText(IDC_EDIT_VA, szAddr, 10);
		HexStrToInt(szAddr, &dwAddr);
		break;
	case 2:
		GetDlgItemText(IDC_EDIT_RVA, szAddr, 10);
		HexStrToInt(szAddr, &dwAddr);
		break;
	case 3:
		GetDlgItemText(IDC_EDIT_FILEOFFSET, szAddr, 10);
		HexStrToInt(szAddr, &dwAddr);
		break;
	default:
		break;
	}
	return dwAddr;
}


// ��ȡָ����ַ���ڵڼ�������
int CPEParseDlg::GetAddrInSecNum(DWORD dwAddr)
{
	int nInNum = 0;
	int nSecNum = m_pNtHeader->FileHeader.NumberOfSections;
	DWORD dwImageBase = m_pNtHeader->OptionalHeader.ImageBase;
	switch (m_nSelect)
	{
	case 1:
		
		for (nInNum = 0; nInNum < nSecNum; nInNum++)
		{
			if (dwAddr >= dwImageBase + m_pSecHead[nInNum].VirtualAddress && dwAddr <= dwImageBase + m_pSecHead[nInNum].VirtualAddress + m_pSecHead[nInNum].Misc.VirtualSize)
			{
				return nInNum;
			}
		}
		break;
	case 2:
		for (nInNum =0;nInNum<nSecNum;nInNum++)
		{
			if (dwAddr>= m_pSecHead[nInNum].VirtualAddress && dwAddr <= m_pSecHead[nInNum].VirtualAddress + m_pSecHead[nInNum].Misc.VirtualSize)
			{
				return nInNum;
			}
		}
		break;
	case 3:
		for (nInNum =0;nInNum < nSecNum;nInNum++)
		{
			if (dwAddr >= m_pSecHead[nInNum].PointerToRawData && dwAddr<=m_pSecHead[nInNum].PointerToRawData + m_pSecHead[nInNum].SizeOfRawData)
			{
				return nInNum;
			}
		}
		break;
	default:
		break;
	}
	return 0;
}


// ��ַ����
VOID CPEParseDlg::CalcAddr(int nInNum, DWORD dwAddr)
{
	DWORD dwVa = 0;
	DWORD dwRva = 0;
	DWORD dwFileOffset = 0;

	switch (m_nSelect)
	{
	case 1:
		dwVa = dwAddr;
		dwRva = dwVa - m_pNtHeader->OptionalHeader.ImageBase;
		dwFileOffset = m_pSecHead[nInNum].PointerToRawData = (dwRva - m_pSecHead[nInNum].VirtualAddress);
		break;
	case 2:
		dwVa = dwAddr + m_pNtHeader->OptionalHeader.ImageBase;
		dwRva = dwAddr;
		dwFileOffset = m_pSecHead[nInNum].PointerToRawData + (dwRva -m_pSecHead[nInNum].VirtualAddress);
		break;
	case 3:
		dwFileOffset = dwAddr;
		dwRva = m_pSecHead[nInNum].VirtualAddress + (dwFileOffset - m_pSecHead[nInNum].PointerToRawData);
		dwVa = dwRva + m_pNtHeader->OptionalHeader.ImageBase;
		break;
	default:
		break;
	}
	USES_CONVERSION;
	SetDlgItemText(IDC_EDIT_SECTION,A2CT((char *)m_pSecHead[nInNum].Name));

	CString str;
	str.Format(_T("%08x"), dwVa);
	SetDlgItemText(IDC_EDIT_VA, str);

	str.Format(_T("%08x"), dwRva);
	SetDlgItemText(IDC_EDIT_RVA, str);

	str.Format(_T("%08x"), dwFileOffset);
	SetDlgItemText(IDC_EDIT_FILEOFFSET, str);

	return VOID();
}


void CPEParseDlg::OnBnClickedButtonCalc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DWORD dwAddr;
	dwAddr = GetAddr();

	int nInNum = GetAddrInSecNum(dwAddr);

	CalcAddr(nInNum, dwAddr);
}
