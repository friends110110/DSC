#if !defined(AFX_REMOTEDTUSETTINGPANEL2_H__0365A692_6A5C_4630_92C7_BA9AC651664D__INCLUDED_)
#define AFX_REMOTEDTUSETTINGPANEL2_H__0365A692_6A5C_4630_92C7_BA9AC651664D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RemoteDtuSettingPanel2.h : header file
//
#include "Optional.h"

/////////////////////////////////////////////////////////////////////////////
// CRemoteDtuSettingPanel2 dialog
#include "wcomm_dll.h"
class CRemoteDtuSettingPanel2 : public CDialog
{
// Construction
public:
	CRemoteDtuSettingPanel2(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRemoteDtuSettingPanel2)
	enum { IDD = IDD_DIALOG2 };
	CEdit	m_packageSplitEdit;
	CEdit	m_lastPackageIntervalEdit;
	CEdit	m_debugStateEdit;
	CEdit	m_reconnectIntervalEdit;
	CEdit	m_maxPackageEdit;
	CEdit	m_onlineEdit;
	CEdit	m_localPortEdit;
	CEdit	m_DtuSignEdit;
	BOOL	m_dtuSignCheck;
	BOOL	m_localPortCheck;
	BOOL	m_onlineCheck;
	BOOL	m_maxPackageCheck;
	BOOL	m_reconnectIntervalCheck;
	BOOL	m_debugStateCheck;
	BOOL	m_lastPackageIntervalCheck;
	BOOL	m_allSelectCheck;
	BOOL	m_packageSplitCheck;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRemoteDtuSettingPanel2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRemoteDtuSettingPanel2)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void OnCheckClicked(UINT uId);
public:
	void QueryDtuParamFromRemoteDtu(CString dtuUserId);
	long Convert16NumberTo10(char *buf,int bufLen);
	void GetSettingParamToRemoteDtu(vector<SETTINGPARAM> *vec);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REMOTEDTUSETTINGPANEL2_H__0365A692_6A5C_4630_92C7_BA9AC651664D__INCLUDED_)
