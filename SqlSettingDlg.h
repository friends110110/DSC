#if !defined(AFX_SQLSETTINGDLG_H__27E21467_95B5_470A_8DC3_839D30136456__INCLUDED_)
#define AFX_SQLSETTINGDLG_H__27E21467_95B5_470A_8DC3_839D30136456__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SqlSettingDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSqlSettingDlg dialog

class CSqlSettingDlg : public CDialog
{
// Construction
public:
	CSqlSettingDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSqlSettingDlg)
	enum { IDD = IDD_SQLSETTING };
	CString	m_strDataBaseName;
	CString	m_strPassword;
	CString	m_strUserName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSqlSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSqlSettingDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SQLSETTINGDLG_H__27E21467_95B5_470A_8DC3_839D30136456__INCLUDED_)
