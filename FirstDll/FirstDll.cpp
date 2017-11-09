// FirstDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "FirstDll.h"

extern "C" void MsgBox(TCHAR *szMsg)
{
	TCHAR szModuleName[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szModuleName, MAX_PATH);
	MessageBox(NULL, szMsg, szModuleName, MB_OK);
}
