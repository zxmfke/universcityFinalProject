
// stdafx.h : ���ڴ˘��^�n�а����˜ʵ�ϵ�y Include �n��
// ���ǽ���ʹ�Ås����׃����
// �������� Include �n��

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ���^�ų�����ʹ�õĳɆT
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ���_���x���ֵ� CString ������ʽ

// �P�] MFC �[��һЩ��Ҋ��ɺ��Ծ���ӍϢ�Ĺ���
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �����c�˜�Ԫ��
#include <afxext.h>         // MFC �U�书��


#include <afxdisp.h>        // MFC Automation e



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC ֧Ԯ�� Internet Explorer 4 ͨ�ÿ����
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC ֧Ԯ�� Windows ͨ�ÿ����
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // ���܅^�Ϳ����е� MFC ֧Ԯ








// �˾޼��c IMPLEMENT_OLECREATE ��ͬ (�����䌦 
// bMultiInstance �������f TRUE �� COleObjectFactory ������ʽ��)��
// �҂���Ҫ�ˑ��ó�ʽ�ķ��_���Ђ��w���Ԍ� Automation ������
// ��Ҫ��� Automation Proxy ������ӡ�
#ifndef IMPLEMENT_OLECREATE2
#define IMPLEMENT_OLECREATE2(class_name, external_name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
        AFX_DATADEF COleObjectFactory class_name::factory(class_name::guid, \
                RUNTIME_CLASS(class_name), TRUE, _T(external_name)); \
        const AFX_DATADEF GUID class_name::guid = \
                { l, w1, w2, { b1, b2, b3, b4, b5, b6, b7, b8 } };
#endif // IMPLEMENT_OLECREATE2

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


