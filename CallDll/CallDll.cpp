// CallDll.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

extern "C" void MsgBox(TCHAR *szMsg);
#pragma comment(lib,"FirstDll")
int main()
{
	MsgBox(_T("Hello First Dll!"));
    return 0;
}

