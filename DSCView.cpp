// DSCView.cpp : implementation of the CDSCView class
//

#include "stdafx.h"
#include "DSC.h"

#include "DSCDoc.h"
#include "DSCView.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDSCView

IMPLEMENT_DYNCREATE(CDSCView, CListView)

BEGIN_MESSAGE_MAP(CDSCView, CListView)
	//{{AFX_MSG_MAP(CDSCView)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustDrawList)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDSCView construction/destruction

CDSCView::CDSCView()
{
	// TODO: add construction code here

}

CDSCView::~CDSCView()
{
}

BOOL CDSCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
    cs.style=cs.style | LVS_REPORT;

	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDSCView drawing

void CDSCView::OnDraw(CDC* pDC)
{
	CDSCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CListCtrl& refCtrl = GetListCtrl();
	refCtrl.InsertItem(0, "Item!");
	// TODO: add draw code for native data here
}

void CDSCView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	m_Font1.CreateFont(-MulDiv(9,this->GetDC()->GetDeviceCaps(LOGPIXELSY), 72),
                        0,0,0,FW_BOLD,FALSE,FALSE,FALSE,0,0,0,0,0,_T("Arial"));
	m_Font2.CreateFont(-MulDiv(9,this->GetDC()->GetDeviceCaps(LOGPIXELSY), 72),
                        0,0,0,FW_BOLD,FALSE,FALSE,FALSE,0,0,0,0,0,_T("Arial"));
	m_Font3.CreateFont(-MulDiv(9,this->GetDC()->GetDeviceCaps(LOGPIXELSY), 72),
                        0,0,0,FW_BOLD,FALSE,FALSE,FALSE,0,0,0,0,0,_T("Arial"));

	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().
}

/////////////////////////////////////////////////////////////////////////////
// CDSCView diagnostics

#ifdef _DEBUG
void CDSCView::AssertValid() const
{
	CListView::AssertValid();
}

void CDSCView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CDSCDoc* CDSCView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDSCDoc)));
	return (CDSCDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDSCView message handlers
void CDSCView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	//TODO: add code to react to the user changing the view style of your window
}

void CDSCView::setViewStyle()
{
	CListCtrl &lc=this->GetListCtrl();
	lc.SetExtendedStyle(lc.GetExStyle() 
		    | LVS_EX_FULLROWSELECT
			| LVS_EX_GRIDLINES
			| LVS_EX_ONECLICKACTIVATE
			| LVS_EX_HEADERDRAGDROP);
	lc.SetTextColor(0xDF16EF);
	lc.SetBkColor(0xE8FDF9);
	lc.SetTextBkColor(0xE8FDF9);

    CImageList imglist;
	imglist.Create(1,24,ILC_COLOR4,0,0);
	lc.SetImageList(&imglist,LVSIL_SMALL);

	lc.InsertColumn(0,"DTU号码",LVCFMT_CENTER,100);
	lc.InsertColumn(1,"登录时间",LVCFMT_CENTER,180);
	lc.InsertColumn(2,"最后注册时间",LVCFMT_CENTER,180);
	lc.InsertColumn(3,"终端IP地址",LVCFMT_CENTER,120);
	lc.InsertColumn(4,"终端端口",LVCFMT_CENTER,80);
	lc.InsertColumn(5,"网关出口IP地址",LVCFMT_CENTER,120);
	lc.InsertColumn(6,"网关出口端口",LVCFMT_CENTER,90);
}

void CDSCView::OnCustDrawList(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLVCUSTOMDRAW pDraw = (LPNMLVCUSTOMDRAW)pNMHDR;

    // Determine the drawing stage.
    DWORD dwDrawStage = pDraw->nmcd.dwDrawStage;
    *pResult = CDRF_NOTIFYITEMDRAW|CDRF_NOTIFYPOSTPAINT|CDRF_NOTIFYPOSTERASE;

    if(dwDrawStage & CDDS_ITEM)
    {
        //CSubDate* pHotel = (CSubDate*)pDraw->nmcd.lItemlParam;
		if((dwDrawStage & CDDS_SUBITEM)&&(pDraw->iSubItem == 0))
        {
			pDraw->clrText = 0x0000FF;//RGB(88,33,255);
			pDraw->clrTextBk=0xFFFEEE;
			SelectObject(pDraw->nmcd.hdc,
				(HFONT)m_Font1.m_hObject);
			*pResult |= CDRF_NEWFONT;
        }
        else if((dwDrawStage & CDDS_SUBITEM) 
			&& (pDraw->iSubItem <= 2))
        {
			pDraw->clrText = 0xDF16EF;//RGB(200,100,40);
			pDraw->clrTextBk=0xE8FDF9;
			SelectObject(pDraw->nmcd.hdc,
				(HFONT)m_Font3.m_hObject);
			*pResult |= CDRF_NEWFONT;
        }else{
			pDraw->clrText = 0x8888FF;//RGB(200,100,40);
			pDraw->clrTextBk=0xE8FDF9;
			SelectObject(pDraw->nmcd.hdc,
				(HFONT)m_Font1.m_hObject);
			*pResult |= CDRF_NEWFONT;
		}
    }
}

void CDSCView::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CListView::OnShowWindow(bShow, nStatus);
	this->setViewStyle();
	// TODO: Add your message handler code here	
}

void CDSCView::
    InsertUserItem(char * userid,char * logon_date,
				   char * update_date,unsigned long localip,
				   unsigned short localport,unsigned long gwip,
				   unsigned short gwport)
{
    char buf[64],*ch;
	int listitemcount;
	CListCtrl &lc=this->GetListCtrl();
    struct tm * ptm;

    listitemcount=lc.GetItemCount();

	lc.InsertItem(listitemcount,userid,0);

	//logon date time
	lc.SetItemText(listitemcount,1,logon_date);

	//update date time
	ptm=localtime((time_t *)update_date);
	sprintf(buf,"%d-%02d-%02d %02d:%02d:%02d",
		ptm->tm_year+1900,ptm->tm_mon+1,ptm->tm_mday,
		ptm->tm_hour,ptm->tm_min,ptm->tm_sec);
	lc.SetItemText(listitemcount,2,buf);						
			
	//DTU local ip and local port
	ch=(char *)&localip;
    sprintf(buf,"%u.%u.%u.%u",(*(ch+3))&0xFF,
				              (*(ch+2))&0xFF,
				              (*(ch+1))&0xFF,
							  (*(ch+0))&0xFF);
	lc.SetItemText(listitemcount,3,buf);
	sprintf(buf,"%d",localport);
	lc.SetItemText(listitemcount,4,buf);

	//ip and port of the gateway that DTU pass
	ch=(char *)&gwip;
    sprintf(buf,"%u.%u.%u.%u",(*(ch+3))&0xFF,
	                          (*(ch+2))&0xFF,
		                      (*(ch+1))&0xFF,
							  (*(ch+0))&0xFF);
	lc.SetItemText(listitemcount,5,buf);
	sprintf(buf,"%d",gwport);
	lc.SetItemText(listitemcount,6,buf);

	listitemcount++;
}

void CDSCView::ClearAllItem()
{
	this->GetListCtrl().DeleteAllItems();
}

void CDSCView::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	char userid[12];
	CListCtrl &lc=GetListCtrl();
	POSITION p;
	p=lc.GetFirstSelectedItemPosition();
	int index=lc.GetNextSelectedItem(p);
	lc.GetItemText(index,0,userid,12);
	if (11==strlen(userid))
		((CMainFrame *)::AfxGetMainWnd())->SetUserId(userid);
	else
		((CMainFrame *)::AfxGetMainWnd())->SetUserId("");

	*pResult = 0;
}

void CDSCView::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
    CMenu mu,*pSubMenu;
	CPoint p;

	this->OnClick(pNMHDR,pResult);
	mu.LoadMenu(IDR_SPEEDMENU);
	pSubMenu=mu.GetSubMenu(0);
	::GetCursorPos(&p);	

	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN,p.x,p.y,::AfxGetMainWnd());
	
	*pResult = 0;
}
