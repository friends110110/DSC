#if !defined(AFX_ADDUSERDLG1_H__A389FEA5_F4BA_44A0_B3E7_00B22EAB1081__INCLUDED_)
#define AFX_ADDUSERDLG1_H__A389FEA5_F4BA_44A0_B3E7_00B22EAB1081__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddUserDlg1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddUserDlg dialog

class CAddUserDlg : public CDialog
{
// Construction
public:
	CString GetLocalIP();
	void IPHightToLower(const char *IPstr, char *ipaddrstr);
	CAddUserDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddUserDlg)
	enum { IDD = IDD_AddUser };
	CDateTimeCtrl	m_time;
	CComboBox	m_dscport;
	CComboBox	m_dtuport;
	CComboBox	m_status;
	CDateTimeCtrl	m_date;
	CIPAddressCtrl	m_dscip;
	CIPAddressCtrl	m_dtuip;
	CEdit	m_userid;
	CString	m_edit1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddUserDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddUserDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdduserB();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDUSERDLG1_H__A389FEA5_F4BA_44A0_B3E7_00B22EAB1081__INCLUDED_)
