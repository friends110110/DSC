// BottomEditView.cpp : implementation file
//

#include "stdafx.h"
#include "DSC.h"
#include "BottomEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBottomEditView

IMPLEMENT_DYNCREATE(CBottomEditView, CEditView)

CBottomEditView::CBottomEditView()
: m_brush(RGB(211,211,211))
{
}

CBottomEditView::~CBottomEditView()
{
}


BEGIN_MESSAGE_MAP(CBottomEditView, CEditView)
	//{{AFX_MSG_MAP(CBottomEditView)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBottomEditView diagnostics

#ifdef _DEBUG
void CBottomEditView::AssertValid() const
{
	CEditView::AssertValid();
}

void CBottomEditView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

HBRUSH CBottomEditView::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here

	//i don't know why all nCtlColor is 1(CTLCOLOR_STATIC),
	//so in order to change bkcolr of bottomview editctrl
	//to green ,i can only set all color
    //if (CTLCOLOR_EDIT==nCtlColor)
	{
		pDC->SetBkColor(RGB(211,211,211));
		pDC->SetTextColor(RGB(139,0,0));
		return m_brush;
	}

	// TODO: Return a non-NULL brush if the parent's handler should not be called
	return NULL;
}



BOOL CBottomEditView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style = cs.style 
		       | ES_READONLY
			   | ES_MULTILINE
			   | ES_WANTRETURN
			   | WS_VSCROLL
			   | WS_HSCROLL;
	return CEditView::PreCreateWindow(cs);
}
