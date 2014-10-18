#if !defined(AFX_REMOTEDTUSETTINGPANEL3_H__C831642E_9B3B_4129_80B1_46BD985A3B16__INCLUDED_)
#define AFX_REMOTEDTUSETTINGPANEL3_H__C831642E_9B3B_4129_80B1_46BD985A3B16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RemoteDtuSettingPanel3.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRemoteDtuSettingPanel3 dialog
#include "wcomm_dll.h"
#include "Optional.h"
class CRemoteDtuSettingPanel3 : public CDialog
{
// Construction
public:
	CRemoteDtuSettingPanel3(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRemoteDtuSettingPanel3)
	enum { IDD = IDD_DIALOG3 };
	CIPAddressCtrl	m_reserveDscAddressCtrl;
	CIPAddressCtrl	m_mainDscAddressCtrl;
	CComboBox	m_domainServerAddressCombox;
	CEdit	m_reserveDscPortEdit;
	CEdit	m_mainDscPortEdit;
	CEdit	m_domainIntervalEdit;
	CEdit	m_dscDomainEdit;
	BOOL	m_mainDscAddressCheck;
	BOOL	m_DscDomainCheck;
	BOOL	m_domainServerAddressCheck;
	BOOL	m_reservDscPortCheck;
	BOOL	m_reserveDscAddressCheck;
	BOOL	m_domainIntervalCheck;
	BOOL	m_mainDscPortCheck;
	BOOL	m_allSelectCheck;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRemoteDtuSettingPanel3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRemoteDtuSettingPanel3)
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

#endif // !defined(AFX_REMOTEDTUSETTINGPANEL3_H__C831642E_9B3B_4129_80B1_46BD985A3B16__INCLUDED_)
