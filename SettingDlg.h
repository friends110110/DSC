#if !defined(AFX_SETTINGDLG_H__24DC7B5A_BCF1_4673_B2F0_AC3E5CA31B92__INCLUDED_)
#define AFX_SETTINGDLG_H__24DC7B5A_BCF1_4673_B2F0_AC3E5CA31B92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg dialog

class CSettingDlg : public CDialog
{
// Construction
protected:
	BOOL m_BEnabled;
public:
	CSettingDlg(CWnd* pParent = NULL,int tabindex=0,BOOL Enabled=TRUE);   // standard constructor
	CTabCtrl	m_tab;
	int         m_tabindex;
	void        ShowItem(void);
	int m_iProtocalType;
// Dialog Data
	//{{AFX_DATA(CSettingDlg)
	enum { IDD = IDD_SETTINGDLG };
	CListBox	m_list;
	CComboBox	m_comb;
	BOOL	m_bSaveToLog;
	CString	m_iLogItemCount;
	CString	m_iOffLineTime;
	CString	m_iPollTime;
	CString	m_iServerPort;
	BOOL	m_bSysAutoPoll;
	BOOL	m_bShowHead;
	BOOL	m_bHexShow;
	CString	m_szlist;
	int		m_udp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSettingDlg)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCksavetolog();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnDeleteip();
	afx_msg void OnDblclkList1();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGDLG_H__24DC7B5A_BCF1_4673_B2F0_AC3E5CA31B92__INCLUDED_)
