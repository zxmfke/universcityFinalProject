
// DlgProxy.h: ���^�n
//

#pragma once

class CpackageCap20151226Dlg;


// CpackageCap20151226DlgAutoProxy ����Ŀ��

class CpackageCap20151226DlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CpackageCap20151226DlgAutoProxy)

	CpackageCap20151226DlgAutoProxy();           // �ӑB������ʹ�õı��o������ʽ

// ����
public:
	CpackageCap20151226Dlg* m_pDialog;

// ���I
public:

// ����
	public:
	virtual void OnFinalRelease();

// ��ʽ�a����
protected:
	virtual ~CpackageCap20151226DlgAutoProxy();

	// �a����ӍϢ������ʽ

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CpackageCap20151226DlgAutoProxy)

	// �a���� OLE ���Ɍ�����ʽ

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

