
// EasyCrackMe.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CEasyCrackMeApp: 
// �йش����ʵ�֣������ EasyCrackMe.cpp
//

class CEasyCrackMeApp : public CWinApp
{
public:
	CEasyCrackMeApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CEasyCrackMeApp theApp;