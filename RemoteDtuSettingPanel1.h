#if !defined(AFX_REMOTEDTUSETTINGPANEL1_H__58FD22B6_9FDC_4C52_B103_DE71DAC952B5__INCLUDED_)
#define AFX_REMOTEDTUSETTINGPANEL1_H__58FD22B6_9FDC_4C52_B103_DE71DAC952B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RemoteDtuSettingPanel1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRemoteDtuSettingPanel1 dialog
#include "wcomm_dll.h"
#include "vector"
#include "Optional.h"
class CRemoteDtuSettingPanel1 : public CDialog
{
// Construction
public:
	CRemoteDtuSettingPanel1(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRemoteDtuSettingPanel1)
	enum { IDD = IDD_DIALOG1 };
	CButton	m_accessCheck;
	CEdit	m_simEdit;
	CEdit	m_accessEdit;
	CEdit	m_pppLoginPasswordEdit;
	CEdit	m_pppUserEdit;
	CEdit	m_serviceEdit;
	int		m_radio;
	BOOL	m_serviceCheck;
	BOOL	m_allSelectCheck;
	BOOL	m_pppUserCheck;
	BOOL	m_pppLoginPasswordCheck;
	BOOL	m_simCheck;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRemoteDtuSettingPanel1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRemoteDtuSettingPanel1)
	afx_msg void OnRadio();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void OnCheckClicked(UINT uId);

public:
	void QueryDtuParamFromRemoteDtu(CString dtuUserId);
	void GetSettingParamToRemoteDtu(vector<SETTINGPARAM> *vec);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REMOTEDTUSETTINGPANEL1_H__58FD22B6_9FDC_4C52_B103_DE71DAC952B5__INCLUDED_)
