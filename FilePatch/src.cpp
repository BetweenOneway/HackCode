#include <windows.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	//文件偏移地址
	DWORD dwFileOffset = 0x0019AF;
	BYTE bCode = 0;
	DWORD dwReadNum = 0;

	if (2 != argc)
	{
		printf("Please Input two arguments \r\n");
		return -1;
	}

	HANDLE hFile = CreateFile(argv[1], GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		return -1;
	}
	SetFilePointer(hFile,dwFileOffset,0,FILE_BEGIN);
	ReadFile(hFile, (LPVOID)&bCode, sizeof(BYTE), &dwReadNum, NULL);

	//\x75是JNZ命令的ASCII码
	if ('\x75' != bCode)
	{
		printf("%02x\r\n", bCode);
		CloseHandle(hFile);
		return -1;
	}

	//\x74是命令JZ的ASCII码
	bCode = '\x74';
	SetFilePointer(hFile, dwFileOffset, 0, FILE_BEGIN);
	WriteFile(hFile, (LPVOID)&bCode, sizeof(BYTE), &dwReadNum, NULL);

	printf("Write JZ is successfully! \r\n");
	CloseHandle(hFile);

	WinExec(argv[1], SW_SHOW);

	getchar();

	return 0;
}