
// packageCap20151226.h : PROJECT_NAME ���ó�ʽ����Ҫ���^�n
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�� PCH �����˙n��ǰ�Ȱ��� 'stdafx.h'"
#endif

#include "resource.h"		// ��Ҫ��̖


// CpackageCap20151226App: 
// Ո��醌�����e�� packageCap20151226.cpp
//

class CpackageCap20151226App : public CWinApp
{
public:
	CpackageCap20151226App();

// ����
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ��ʽ�a����

	DECLARE_MESSAGE_MAP()
};

extern CpackageCap20151226App theApp;