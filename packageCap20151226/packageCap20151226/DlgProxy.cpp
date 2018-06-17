
// DlgProxy.cpp : �����n
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
	
	// ��Ҫ���֑��ó�ʽ���еĕr�g�c Automation ������õĕr�gһ���L��
	//	 ������ʽ��횺��� AfxOleLockApp��
	AfxOleLockApp();

	// ���ɑ��ó�ʽ��ҕ��ָ�ˣ�ȡ�Ì���Ԓ���K�Ĵ�ȡ��
	// �� Proxy �ăȲ�ָ���O����ָ��Ԓ���K��
	// �K�O��ָ��� Proxy ��
	// ��Ԓ���K����ָ�ˡ�
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
	// ��Ҫ��ʹ�� Automation �������������֮��Y�����ó�ʽ��
	//	 �☋��ʽ��횺��� AfxOleUnlockApp��
	// ���⣬�@���ݚ�����Ԓ���K
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CpackageCap20151226DlgAutoProxy::OnFinalRelease()
{
	// ��ጷ� Automation �������ą����r��
	// ������ OnFinalRelease������e���Ԅ�
	// �h����������л���eǰ��Ո�ȼ��������������~����� (Cleanup)
	// ��ʽ�a��

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CpackageCap20151226DlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CpackageCap20151226DlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// ע��: �҂������ˌ� IID_IpackageCap20151226 ��֧Ԯ
// �Ա�� VBA ֧Ԯ��Ͱ�ȫ�M�Y���� IID ����c .IDL �n�У�
// ������������� (Dispinterface) �� GUID �����

// {6420E762-D6D8-434D-8977-1ECA80CEA9AC}
static const IID IID_IpackageCap20151226 =
{ 0x6420E762, 0xD6D8, 0x434D, { 0x89, 0x77, 0x1E, 0xCA, 0x80, 0xCE, 0xA9, 0xAC } };

BEGIN_INTERFACE_MAP(CpackageCap20151226DlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CpackageCap20151226DlgAutoProxy, IID_IpackageCap20151226, Dispatch)
END_INTERFACE_MAP()

// �ڴˌ����� StdAfx.h �ж��x IMPLEMENT_OLECREATE2 �޼�
// {F52E4DB7-08D9-4099-91CA-CD5A9ACB1E9B}
IMPLEMENT_OLECREATE2(CpackageCap20151226DlgAutoProxy, "packageCap20151226.Application", 0xf52e4db7, 0x8d9, 0x4099, 0x91, 0xca, 0xcd, 0x5a, 0x9a, 0xcb, 0x1e, 0x9b)


// CpackageCap20151226DlgAutoProxy ӍϢ̎��ʽ
