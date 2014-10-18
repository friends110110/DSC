// LeftView.cpp : implementation of the CLeftView class
//

#include "stdafx.h"
#include "DSC.h"
#include "mainfrm.h"

#include "DSCDoc.h"
#include "LeftView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
	//{{AFX_MSG_MAP(CLeftView)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeftView construction/destruction

CLeftView::CLeftView()
{
	// TODO: add construction code here

}

CLeftView::~CLeftView()
{
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style=cs.style
            | TVS_HASLINES  
			| TVS_LINESATROOT 
			| TVS_HASBUTTONS;

	return CTreeView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CLeftView drawing

void CLeftView::OnDraw(CDC* pDC)
{
	CDSCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}


void CLeftView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: You may populate your TreeView with items by directly accessing
	//  its tree control through a call to GetTreeCtrl().
}

/////////////////////////////////////////////////////////////////////////////
// CLeftView diagnostics

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CDSCDoc* CLeftView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDSCDoc)));
	return (CDSCDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLeftView message handlers

void CLeftView::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CTreeView::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
    setTreeStyle();
}

void CLeftView::setTreeStyle()
{
	CTreeCtrl &treectrl=this->GetTreeCtrl();
	//treectrl.SetBkColor(0xF1C7ED); //JC2	
	//treectrl.SetBkColor(0xEEEED1);	 //JC2蓝色	
	treectrl.SetBkColor(0xCDC9C9);	 //JC2银色

	treectrl.SetTextColor(0xA20000);

	CImageList img;
	img.Create(IDB_TREE, 16, 0, RGB(255, 0, 255));
	treectrl.SetImageList(&img,TVSIL_NORMAL);
    img.Detach();
	treectrl.InsertItem("在线DTU",0,1,TVI_ROOT,TVI_LAST);
}

void CLeftView::ClearAllItem()
{
	CTreeCtrl &tc=GetTreeCtrl();
	tc.DeleteAllItems();
	tc.InsertItem("在线DTU",0,1,TVI_ROOT,TVI_LAST);
}

void CLeftView::InsertUserItem(char *szUserId)
{
	CTreeCtrl &tc=GetTreeCtrl();
	tc.InsertItem(szUserId,2,2,tc.GetFirstVisibleItem(),TVI_LAST);
}

void CLeftView::DeleteUserItem(char *szUserId)
{
	CTreeCtrl &tc=GetTreeCtrl();
	HTREEITEM hti;

	hti=tc.GetChildItem(tc.GetFirstVisibleItem());

	while (hti)
	{
		if (0==tc.GetItemText(hti).Compare(szUserId))
		{
			tc.DeleteItem(hti);
			break;
		}
	}
}

void CLeftView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CTreeCtrl &tc=this->GetTreeCtrl();
	CString STreeItem;

	if (tc.GetSelectedItem()) 
	{
		STreeItem=tc.GetItemText(tc.GetSelectedItem());

		if (11==STreeItem.GetLength())
		{
			((CMainFrame *)::AfxGetMainWnd())->SetUserId(STreeItem.GetBuffer(0));
			::PostMessage(((CMainFrame *)::AfxGetMainWnd())->m_hWnd,ID_MISENDDATA,0,0);
		}
		else
			((CMainFrame *)::AfxGetMainWnd())->SetUserId("");		
	}
	
	*pResult = 0;
}
