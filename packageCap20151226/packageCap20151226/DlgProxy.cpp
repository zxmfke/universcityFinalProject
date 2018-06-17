
// DlgProxy.cpp : 實作檔
//

#include "stdafx.h"
#include "packageCap20151226.h"
#include "DlgProxy.h"
#include "packageCap20151226Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CpackageCap20151226DlgAutoProxy

IMPLEMENT_DYNCREATE(CpackageCap20151226DlgAutoProxy, CCmdTarget)

CpackageCap20151226DlgAutoProxy::CpackageCap20151226DlgAutoProxy()
{
	EnableAutomation();
	
	// 若要保持應用程式執行的時間與 Automation 物件作用的時間一樣長，
	//	 建構函式必須呼叫 AfxOleLockApp。
	AfxOleLockApp();

	// 經由應用程式主視窗指標，取得對對話方塊的存取。
	// 將 Proxy 的內部指標設定為指向對話方塊，
	// 並設定指向此 Proxy 的
	// 對話方塊返回指標。
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CpackageCap20151226Dlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CpackageCap20151226Dlg)))
		{
			m_pDialog = reinterpret_cast<CpackageCap20151226Dlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CpackageCap20151226DlgAutoProxy::~CpackageCap20151226DlgAutoProxy()
{
	// 若要在使用 Automation 建立了所有物件之後結束應用程式，
	//	 解構函式必須呼叫 AfxOleUnlockApp。
	// 此外，這會摧毀主對話方塊
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CpackageCap20151226DlgAutoProxy::OnFinalRelease()
{
	// 當釋放 Automation 物件最後的參考時，
	// 會呼叫 OnFinalRelease。基底類別會自動
	// 刪除物件。呼叫基底類別前，請先加入您物件所需的額外清除 (Cleanup)
	// 程式碼。

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CpackageCap20151226DlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CpackageCap20151226DlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// 注意: 我們加入了對 IID_IpackageCap20151226 的支援
// 以便從 VBA 支援類型安全繫結。此 IID 必須與 .IDL 檔中，
// 附加至分配介面 (Dispinterface) 的 GUID 相符。

// {6420E762-D6D8-434D-8977-1ECA80CEA9AC}
static const IID IID_IpackageCap20151226 =
{ 0x6420E762, 0xD6D8, 0x434D, { 0x89, 0x77, 0x1E, 0xCA, 0x80, 0xCE, 0xA9, 0xAC } };

BEGIN_INTERFACE_MAP(CpackageCap20151226DlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CpackageCap20151226DlgAutoProxy, IID_IpackageCap20151226, Dispatch)
END_INTERFACE_MAP()

// 在此專案的 StdAfx.h 中定義 IMPLEMENT_OLECREATE2 巨集
// {F52E4DB7-08D9-4099-91CA-CD5A9ACB1E9B}
IMPLEMENT_OLECREATE2(CpackageCap20151226DlgAutoProxy, "packageCap20151226.Application", 0xf52e4db7, 0x8d9, 0x4099, 0x91, 0xca, 0xcd, 0x5a, 0x9a, 0xcb, 0x1e, 0x9b)


// CpackageCap20151226DlgAutoProxy 訊息處理常式
