#if !defined(AFX_REMOTEDTUSETTINGPANEL4_H__5558BF62_1B8E_44AB_AA3B_4EF7382CB340__INCLUDED_)
#define AFX_REMOTEDTUSETTINGPANEL4_H__5558BF62_1B8E_44AB_AA3B_4EF7382CB340__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RemoteDtuSettingPanel4.h : header file
//
#include "wcomm_dll.h"
#include "Optional.h"
/////////////////////////////////////////////////////////////////////////////
// CRemoteDtuSettingPanel4 dialog

class CRemoteDtuSettingPanel4 : public CDialog
{
// Construction
public:
	CRemoteDtuSettingPanel4(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRemoteDtuSettingPanel4)
	enum { IDD = IDD_SettingPanel4 };
	CComboBox	m_verifyBitsCombox;
	CComboBox	m_stopBitsCombox;
	CComboBox	m_dataBitsCombox;
	CComboBox	m_baudRateCombox;
	CComboBox	m_streamControlCombox;
	BOOL	m_selectAllCheck;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRemoteDtuSettingPanel4)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRemoteDtuSettingPanel4)
	afx_msg void OnCheck1();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void QueryDtuParamFromRemoteDtu(CString dtuUserId);
	bool GetSettingParamToRemoteDtu(vector<SETTINGPARAM> *vec);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REMOTEDTUSETTINGPANEL4_H__5558BF62_1B8E_44AB_AA3B_4EF7382CB340__INCLUDED_)
