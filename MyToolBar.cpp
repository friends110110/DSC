// MyToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "DSC.h"
#include "MyToolBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyToolBar

CMyToolBar::CMyToolBar()
{
	m_viewword=TRUE;
}

CMyToolBar::~CMyToolBar()
{
}

CSize CMyToolBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	CSize cs(0,0);

    if (!IsFloating())
	{
		if (bHorz)
			cs+=CSize(500,0);
		ChangeButtonSize();
	}    

	return CToolBar::CalcFixedLayout(bStretch,bHorz)+cs;
}

CSize CMyToolBar::CalcDynamicLayout(int nLength,DWORD dwMode)
{
	if (IsFloating())
	{
	    CMenu * mu;
        mu=this->GetParentFrame()->GetSystemMenu(FALSE);
	    mu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_DISABLED);
		ChangeButtonSize();
	}

	if (dwMode & LM_HORZ)
	{
		this->SetButtonStyle(4,this->GetButtonStyle(4) | TBSTYLE_DROPDOWN);
	}
	else
	{
		this->SetButtonStyle(4,this->GetButtonStyle(4) & (~TBSTYLE_DROPDOWN));
	}

	return CToolBar::CalcDynamicLayout(nLength,dwMode);
}

BEGIN_MESSAGE_MAP(CMyToolBar, CToolBar)
	//{{AFX_MSG_MAP(CMyToolBar)
	ON_NOTIFY_REFLECT(TBN_DROPDOWN,OnToolBarButtonDropDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyToolBar message handlers

void CMyToolBar::OnToolBarButtonDropDown(WPARAM wParam,LPARAM lParam)
{   //LPNMTOOLBAR lParam
	CMenu mu,*smu;
	CRect rt;
	int   iButtonWidth,x,y;
	
	if (IsFloating())
	{
		x=8;
		y=-2;
	}
	else
	{
        x=14;
	    y=-2;
	}

	this->GetWindowRect(&rt);
    iButtonWidth=(this->GetToolBarCtrl().GetButtonSize())&0xFFFF;

	mu.LoadMenu(IDR_DROPDOWNMENU);
	smu=mu.GetSubMenu(0);
	smu->TrackPopupMenu(TPM_LEFTALIGN,
		   rt.left+iButtonWidth*3+x,
		   rt.Height()+rt.top+y,
		   ::AfxGetMainWnd());
}

void CMyToolBar::ChangeButtonSize()
{
	CRect rect; 
	this->GetToolBarCtrl().GetItemRect(0,&rect); 

	if (m_viewword)
	{
		this->SetSizes(CSize(SHOWTEXTLEN,rect.Size().cy),CSize(32,32));
	}
	else
	{
        this->SetSizes(CSize(NOTEXTLEN,rect.Size().cy),CSize(32,32));
	}
}

void CMyToolBar::SetShowWord(BOOL vw)
{
	m_viewword=vw;
}

void CMyToolBar::DrawToolBarEdge(CDC *pDC)
{
	CRect rt;
	this->GetWindowRect(&rt);
	pDC->DrawEdge(&rt,BDR_RAISEDOUTER,BF_RECT);
}

void CMyToolBar::DoPaint(CDC *pDC)
{
	//DrawToolBarEdge(pDC);
}
