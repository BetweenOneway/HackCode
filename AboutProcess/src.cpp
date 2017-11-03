#include <Windows.h>
#include <Urlmon.h>
#include <iostream>
using namespace std;

#pragma comment(lib,"Urlmon.lib")

void Downloader()
{
	TCHAR szUrl[MAX_PATH] = TEXT("https://raw.githubusercontent.com/wangkuiwu/datastructs_and_algorithm/master/pictures/graph/iterator/03.jpg");
	TCHAR szFile[MAX_PATH] = TEXT("d:\\dowm.jpg");
	HRESULT hResult = URLDownloadToFile(NULL, szUrl, szFile, 0, NULL);
	//WinExec("d:\\mynotepad.exe", SW_SHOW);
}

int StartProcess()
{
	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFO si = { 0 };
	si.cb = sizeof(STARTUPINFO);

	bool bRet = CreateProcess(TEXT("c:\\windows\\system32\\notepad.exe"), NULL, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi);
	if (false == bRet)
	{
		cout << "CreateProcess Error!" << endl;
		return -1;
	}

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}

int TerminateProcess()
{
	HWND hNoteWnd = FindWindow(NULL,TEXT("É¨À×"));
	if (NULL == hNoteWnd)
	{
		return -1;
	}
	DWORD dwNotePid = 0;
	GetWindowThreadProcessId(hNoteWnd,&dwNotePid);
	if (0 == dwNotePid)
	{
		return -1;
	}
	HANDLE hNoteHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwNotePid);
	if (NULL == hNoteHandle)
	{
		return -1;
	}
	TerminateProcess(hNoteHandle, 0);
	CloseHandle(hNoteHandle);
	return 0;
}

int main()
{
	TerminateProcess();
	system("pause");
	return 0;
}