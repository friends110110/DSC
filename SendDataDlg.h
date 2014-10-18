#if !defined(AFX_SENDDATADLG_H__51FAE110_2E48_4AE5_89E5_CDCE36C85629__INCLUDED_)
#define AFX_SENDDATADLG_H__51FAE110_2E48_4AE5_89E5_CDCE36C85629__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SendDataDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSendDataDlg dialog

class CSendDataDlg : public CDialog
{
// Construction
public:
	CSendDataDlg(CWnd* pParent = NULL,CTreeCtrl* ptc=NULL);  

//operation
public:	
	void AddUserId();
	void SetUserId(char *);
	int  StrToHex(char *str,char *hex);
	int  HexToStr(char *hex,char *str);

protected:
	CComboBox	m_cmuserlist;
	CString     m_userid;
	CTreeCtrl*  m_pTree;
    BOOL	    m_hex;
// Dialog Data
	//{{AFX_DATA(CSendDataDlg)
	enum { IDD = IDD_SENDDATADLG };
//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSendDataDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSendDataDlg)
	afx_msg void OnBquit();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnCkhex();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBsend();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCkautosend();
	afx_msg void OnBcr();
	afx_msg void OnBlf();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SENDDATADLG_H__51FAE110_2E48_4AE5_89E5_CDCE36C85629__INCLUDED_)
