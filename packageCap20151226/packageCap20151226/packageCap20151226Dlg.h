
// packageCap20151226Dlg.h : ���^�n
//

#pragma once
#include "afxwin.h"

class CpackageCap20151226DlgAutoProxy;


// CpackageCap20151226Dlg ��Ԓ���K
class CpackageCap20151226Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CpackageCap20151226Dlg);
	friend class CpackageCap20151226DlgAutoProxy;

// ����
public:
	CpackageCap20151226Dlg(CWnd* pParent = NULL);	// �˜ʽ�����ʽ
	virtual ~CpackageCap20151226Dlg();

// ��Ԓ���K�Y��
	enum { IDD = IDD_PACKAGECAP20151226_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧Ԯ


// ��ʽ�a����
protected:
	CpackageCap20151226DlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// �a����ӍϢ������ʽ
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_code;
	CListBox m_List_device;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
