
// ServiceManager.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CServiceManagerApp: 
// �йش����ʵ�֣������ ServiceManager.cpp
//

class CServiceManagerApp : public CWinApp
{
public:
	CServiceManagerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CServiceManagerApp theApp;