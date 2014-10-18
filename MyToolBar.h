#if !defined(AFX_MYTOOLBAR_H__F281D671_F696_4B70_9C5A_0F7715A5BD15__INCLUDED_)
#define AFX_MYTOOLBAR_H__F281D671_F696_4B70_9C5A_0F7715A5BD15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyToolBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyToolBar window

#define SHOWTEXTLEN 75
#define NOTEXTLEN   39

class CMyToolBar : public CToolBar
{
// Construction
public:
	CMyToolBar();

// Attributes
public:
    void  SetShowWord(BOOL vw=TRUE);
	void  ChangeButtonSize();
	void  DrawToolBarEdge(CDC *pDC);
	void  DoPaint(CDC *pDC);
protected:
	BOOL m_viewword;
// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyToolBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyToolBar();

protected:
    virtual CSize CalcDynamicLayout( int nLength, DWORD dwMode );
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	// Generated message map functions
protected:
	//{{AFX_MSG(CMyToolBar)
	afx_msg void OnToolBarButtonDropDown(WPARAM wParam,LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTOOLBAR_H__F281D671_F696_4B70_9C5A_0F7715A5BD15__INCLUDED_)
