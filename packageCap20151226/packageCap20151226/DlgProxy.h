
// DlgProxy.h: 標頭檔
//

#pragma once

class CpackageCap20151226Dlg;


// CpackageCap20151226DlgAutoProxy 命令目標

class CpackageCap20151226DlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CpackageCap20151226DlgAutoProxy)

	CpackageCap20151226DlgAutoProxy();           // 動態建立所使用的保護建構函式

// 屬性
public:
	CpackageCap20151226Dlg* m_pDialog;

// 作業
public:

// 覆寫
	public:
	virtual void OnFinalRelease();

// 程式碼實作
protected:
	virtual ~CpackageCap20151226DlgAutoProxy();

	// 產生的訊息對應函式

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CpackageCap20151226DlgAutoProxy)

	// 產生的 OLE 分派對應函式

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

