
// EnumRunItems.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CEnumRunItemsApp: 
// �йش����ʵ�֣������ EnumRunItems.cpp
//

class CEnumRunItemsApp : public CWinApp
{
public:
	CEnumRunItemsApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CEnumRunItemsApp theApp;