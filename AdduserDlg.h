#if !defined(AFX_ADDUSERDLG_H__F16FE596_3DD3_48B8_82F6_B0F23C12C299__INCLUDED_)
#define AFX_ADDUSERDLG_H__F16FE596_3DD3_48B8_82F6_B0F23C12C299__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdduserDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdduserDlg dialog

class CAdduserDlg : public CDialog
{
// Construction
public:
	void IPHightToLower(const char *IPstr, char *ipaddrstr);
	CString GetLocalIP();
	CAdduserDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAdduserDlg)
	enum { IDD = IDD_Adduser };
	CDateTimeCtrl	m_date;
	CComboBox	m_dscport;
	CComboBox	m_dtuport;
	CComboBox	m_status;
	CIPAddressCtrl	m_dscip;
	CIPAddressCtrl	m_dtuip;
	CEdit	m_userid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdduserDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdduserDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnAddUserB();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDUSERDLG_H__F16FE596_3DD3_48B8_82F6_B0F23C12C299__INCLUDED_)
