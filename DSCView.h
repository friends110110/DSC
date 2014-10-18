// DSCView.h : interface of the CDSCView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSCVIEW_H__A51DAC3D_39C7_4BA4_9FDE_5DC962E28C5C__INCLUDED_)
#define AFX_DSCVIEW_H__A51DAC3D_39C7_4BA4_9FDE_5DC962E28C5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDSCView : public CListView
{
protected: // create from serialization only
	CDSCView();
	DECLARE_DYNCREATE(CDSCView)
	void setViewStyle(void);

// Attributes
public:
	CDSCDoc* GetDocument();
    void InsertUserItem(char *,char *,char *,
		unsigned long,unsigned short,unsigned long,unsigned short);
    void ClearAllItem(); 

    CFont m_Font1,m_Font2,m_Font3;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDSCView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDSCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDSCView)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnCustDrawList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DSCView.cpp
inline CDSCDoc* CDSCView::GetDocument()
   { return (CDSCDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSCVIEW_H__A51DAC3D_39C7_4BA4_9FDE_5DC962E28C5C__INCLUDED_)
