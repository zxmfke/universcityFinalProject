
// packageCap20151226Dlg.h : 標頭檔
//

#pragma once
#include "afxwin.h"

class CpackageCap20151226DlgAutoProxy;


// CpackageCap20151226Dlg 對話方塊
class CpackageCap20151226Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CpackageCap20151226Dlg);
	friend class CpackageCap20151226DlgAutoProxy;

// 建構
public:
	CpackageCap20151226Dlg(CWnd* pParent = NULL);	// 標準建構函式
	virtual ~CpackageCap20151226Dlg();

// 對話方塊資料
	enum { IDD = IDD_PACKAGECAP20151226_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援


// 程式碼實作
protected:
	CpackageCap20151226DlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// 產生的訊息對應函式
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
