
// packageCap20151226.h : PROJECT_NAME 應用程式的主要標頭檔
//

#pragma once

#ifndef __AFXWIN_H__
	#error "對 PCH 包含此檔案前先包含 'stdafx.h'"
#endif

#include "resource.h"		// 主要符號


// CpackageCap20151226App: 
// 請參閱實作此類別的 packageCap20151226.cpp
//

class CpackageCap20151226App : public CWinApp
{
public:
	CpackageCap20151226App();

// 覆寫
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 程式碼實作

	DECLARE_MESSAGE_MAP()
};

extern CpackageCap20151226App theApp;