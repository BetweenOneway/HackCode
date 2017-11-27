#include "stdafx.h"
BOOL HexStrToInt(TCHAR *szAddr, DWORD *pDwAddr)
{
	TCHAR   *pCH, c;
	DWORD  dwVal = 0, dw;

	pCH = szAddr;
	while (*pCH)
	{
		c = toupper(*pCH++);
		if (c >= _T('A') && c <= _T('F'))
		{
			dw = (DWORD)c - ((DWORD)'A' - 10);
		}
		else if (c >= '0' && c <= '9')
		{
			dw = (DWORD)c - (DWORD)'0';
		}
		else
		{
			return FALSE;
		}
		dwVal = (dwVal << 4) + dw;
	}

	*pDwAddr = dwVal;
	return TRUE;
}