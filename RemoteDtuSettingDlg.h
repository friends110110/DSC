#if !defined(AFX_REMOTEDTUSETTINGDLG_H__E95BE916_F831_4730_8126_9448FBC72C18__INCLUDED_)
#define AFX_REMOTEDTUSETTINGDLG_H__E95BE916_F831_4730_8126_9448FBC72C18__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RemoteDtuSettingDlg.h : header file
//
#include "RemoteDtuSettingPanel1.h"
#include "RemoteDtuSettingPanel2.h"
#include "RemoteDtuSettingPanel3.h"
#include "RemoteDtuSettingPanel4.h"

#include "SendSettingParamDlg.h"

#include "wcomm_dll.h"
//#include "Optional.h"
#include "MainFrm.h"
#include "vector"
/////////////////////////////////////////////////////////////////////////////
// CRemoteDtuSettingDlg dialog
class CRemoteDtuSettingDlg : public CDialog
{
// Construction
public:
	CRemoteDtuSettingDlg(CWnd* pParent = NULL);   // standard constructor
	void showItem();
	bool AddDtuInformationToSelectCombo();
	void doDefaultParam();
	void doSettingParam();
// Dialog Data
	//{{AFX_DATA(CRemoteDtuSettingDlg)
	enum { IDD = IDD_REMOTEDTUSETTINGDLG };
	CEdit	m_timeoutEdit;
	CComboBox	m_chooseDtuCombo;
	CTabCtrl	m_tab;
	BOOL	m_timeoutCheck;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRemoteDtuSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRemoteDtuSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelendokChoosecombo();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheck1();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void OnButtonClicked(UINT uId);
	CRemoteDtuSettingPanel1 m_panel1;
	CRemoteDtuSettingPanel2 m_panel2;
	CRemoteDtuSettingPanel3 m_panel3;
	CRemoteDtuSettingPanel4 m_panel4;
private:
	UINT m_timenEvent;
	void sendQueryCommand();
	//use for timeout setting , seconds
	time_t m_timeout;

public:
	//±»frame µ÷ÓÃ
	void doQueryDtuParam(char userId[12]);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REMOTEDTUSETTINGDLG_H__E95BE916_F831_4730_8126_9448FBC72C18__INCLUDED_)
