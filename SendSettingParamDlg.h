#if !defined(AFX_SENDSETTINGPARAMDLG_H__AFCB392D_5EEE_4FDA_85B0_7674CA1AB14F__INCLUDED_)
#define AFX_SENDSETTINGPARAMDLG_H__AFCB392D_5EEE_4FDA_85B0_7674CA1AB14F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SendSettingParamDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSendSettingParamDlg dialog
#include "vector"
#include "Optional.h"
class CSendSettingParamDlg : public CDialog
{
// Construction
public:
	CSendSettingParamDlg(CWnd* pParent = NULL);   // standard constructor
	void setSendSettingData(vector<SETTINGPARAM>* vec);
	void setUserId(unsigned char *userId);
//	CSendSettingParamDlg(vector<SETTINGPARAM>* vec, CWnd* pParent = NULL); 
// Dialog Data
	//{{AFX_DATA(CSendSettingParamDlg)
	enum { IDD = IDD_SendSettingParamDLG };
	CListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSendSettingParamDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSendSettingParamDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	vector<SETTINGPARAM> *m_vector;
	unsigned char *m_userId;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SENDSETTINGPARAMDLG_H__AFCB392D_5EEE_4FDA_85B0_7674CA1AB14F__INCLUDED_)
