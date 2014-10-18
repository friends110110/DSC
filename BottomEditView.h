#if !defined(AFX_BOTTOMEDITVIEW_H__E26743E8_3E6C_4A07_BCCC_E1A20C4E0C86__INCLUDED_)
#define AFX_BOTTOMEDITVIEW_H__E26743E8_3E6C_4A07_BCCC_E1A20C4E0C86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BottomEditView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBottomEditView view

class CBottomEditView : public CEditView
{
protected:
	CBottomEditView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CBottomEditView)

// Attributes
public:

protected:
    CBrush  m_brush;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBottomEditView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CBottomEditView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CBottomEditView)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BOTTOMEDITVIEW_H__E26743E8_3E6C_4A07_BCCC_E1A20C4E0C86__INCLUDED_)
