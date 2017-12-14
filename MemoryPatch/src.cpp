#include <windows.h>
#include <stdio.h>

int main(int argc,char *argv[])
{
	//–Èƒ‚µÿ÷∑
	DWORD dwVAddress = 0x010525AF;
	BYTE bCode = 0;
	DWORD dwReadNum = 0;

	if (2 != argc)
	{
		printf("Please input two argument \r\n");
		return -1;
	}

	STARTUPINFO si = { 0 };
	si.cb = sizeof(STARTUPINFO);
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW;
	PROCESS_INFORMATION pi = { 0 };

	BOOL bRet = CreateProcess(argv[1], NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi);
	if (bRet = FALSE)
	{
		printf("Create Process Error ! \r\n");
		return -1;
	}

	ReadProcessMemory(pi.hProcess, (LPVOID)dwVAddress, (LPVOID)&bCode, sizeof(BYTE), &dwReadNum);
	if (bCode != '\x75')
	{
		printf("%02x \r\n", bCode);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		return -1;
	}

	bCode = '\x74';
	WriteProcessMemory(pi.hProcess, (LPVOID)dwVAddress, (LPVOID)&bCode, sizeof(BYTE), &dwReadNum);

	ResumeThread(pi.hThread);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	printf("Write JZ is successfully ! \r\n");

	system("pause");
	return 0;
}