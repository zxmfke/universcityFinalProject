
// packageCap20151226.cpp : ���x���ó�ʽ��e�О顣
//

#include "stdafx.h"
#include "packageCap20151226.h"
#include "packageCap20151226Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CpackageCap20151226App

BEGIN_MESSAGE_MAP(CpackageCap20151226App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CpackageCap20151226App ����

CpackageCap20151226App::CpackageCap20151226App()
{
	// ֧Ԯ�����ӹ���T
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO:  �ڴ˼��뽨����ʽ�a��
	// ��������Ҫ�ĳ�ʼ�O������ InitInstance ��
}


// �H�е�һ�� CpackageCap20151226App ���

CpackageCap20151226App theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x91916E3C, 0x9D2C, 0x4BDA, { 0xA4, 0xF, 0x66, 0x6B, 0x23, 0x3C, 0x80, 0x3A } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


// CpackageCap20151226App ��ʼ�O��

BOOL CpackageCap20151226App::InitInstance()
{
	// ���瑪�ó�ʽ�YӍ���ָ��ʹ�� ComCtl32.dll 6 (��) ����汾��
	// �톢��ҕ�X����ʽ���� Windows XP �ϣ��t��Ҫ InitCommonControls()��
	// ��t�κ�ҕ���Ľ�������ʧ����
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// �O��Ҫ������������Ҫ��춑��ó�ʽ�е�
	// ͨ�ÿ����e��
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// ��ʼ�� OLE ��ʽ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// �������ӹ���T���Է���Ԓ���K����
	// �κΚ��Ә��zҕ�򚤌���Ιzҕ����헡�
	CShellManager *pShellManager = new CShellManager;

	// ���� [Windows ԭ��] ҕ�X������T�Ɇ��� MFC ������е����}
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// �˜ʳ�ʼ�O��
	// �������ʹ���@Щ���܁K����p��
	// ������ɵĿɈ��Йn��С��������
	// �����г�ʽ�a�Ƴ�����Ҫ�ĳ�ʼ����ʽ��
	// ׃�������O��ֵ�ĵ�䛙C�a
	// TODO:  ����ԓ�m���޸Ĵ��ִ�
	// (���磬��˾���Q��M�����Q)
	SetRegistryKey(_T("���C AppWizard ���a���đ��ó�ʽ"));
	// ���� Automation �� reg/unreg �����������С�
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ��ʹ�� /Embedding �� /Automation �������ӑ��ó�ʽ��
	// �����ó�ʽ���� Automation �ŷ������С�
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// ���� CoRegisterClassObject() ���e�򔵡�
		COleTemplateServer::RegisterAll();
	}
	// ��ʹ�� /Unregserver �� /Unregister �������ӑ��ó�ʽ��
	// �ĵ���Ƴ��Ŀ��
	else if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
	{
		COleObjectFactory::UpdateRegistryAll(FALSE);
		AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor);
		return FALSE;
	}
	// �цΪ�������������� (�� /Register �� /Regserver) ���ӑ��ó�ʽ��
	// ���µ���Ŀ��������ͳ�ʽ�졣
	else
	{
		COleObjectFactory::UpdateRegistryAll();
		AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid);
		if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppRegister)
			return FALSE;
	}

	CpackageCap20151226Dlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO:  �ڴ˷����ʹ�� [�_��] ��ֹͣʹ�Ì�Ԓ���K�r
		// ̎��ĳ�ʽ�a
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO:  �ڴ˷����ʹ�� [ȡ��] ��ֹͣʹ�Ì�Ԓ���K�r
		// ̎��ĳ�ʽ�a
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: ��Ԓ���K����ʧ������ˣ����ó�ʽ����Kֹ��\n");
		TRACE(traceAppMsg, 0, "����: �����Ҫ�ڌ�Ԓ���K��ʹ�� MFC ����헣��t�o�� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// �h�������������Ě��ӹ���T��
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ����ѽ��P�]��Ԓ���K������ FALSE�������҂����Y�����ó�ʽ��
	// ������ʾ�_ʼ���ó�ʽ��ӍϢ��
	return FALSE;
}

int CpackageCap20151226App::ExitInstance()
{
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}
