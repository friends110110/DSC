// MenuBar.cpp : 
//

#include "stdafx.h"
#include "MenuBar.h"
#include "resource.h"

#include <afxpriv.h>

// pasted from MFC source
#define _AfxGetDlgCtrlID(hWnd)          ((UINT)(WORD)::GetDlgCtrlID(hWnd))

#define HORZF(dw) (dw & CBRS_ORIENT_HORZ)
#define VERTF(dw) (dw & CBRS_ORIENT_VERT)

static void AdjustRectangle(CRect& rect, CPoint pt)
{
	int nXOffset = (pt.x < rect.left) ? (pt.x - rect.left) :
					(pt.x > rect.right) ? (pt.x - rect.right) : 0;
	int nYOffset = (pt.y < rect.top) ? (pt.y - rect.top) :
					(pt.y > rect.bottom) ? (pt.y - rect.bottom) : 0;
	rect.OffsetRect(nXOffset, nYOffset);
}

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenuBar 


// I want overide EndDrag, but it's not virtual.
// So I have to overide StartDrag!
class CMenuBarDockContext : public CDockContext
{
public:
	CMenuBarDockContext(CControlBar* pBar) : CDockContext(pBar) { }

	virtual void StartDrag(CPoint pt);

private:
	BOOL _Track();
	void _EndDrag();
};

namespace {

	// hook
	CMenuBar* g_pMenuBar = NULL;
	HHOOK	  g_hMsgHook = NULL;

	// message
	const UINT MB_SET_MENU_NULL = WM_USER + 1100;

	// layout , this will change the height of menubar
	const int CXGAP		   = 5;
	const int CYGAP		   = 6;
	const int CYGAPVERT	   = 3;
	const int CXGRIPPER	   = 7;

	int cyMenuButton = 0;

	int	cxBorder2 = ::GetSystemMetrics(SM_CXBORDER) * 2;//bWin4 ? CX_BORDER*2 : CX_BORDER;
	int	cyBorder2 = ::GetSystemMetrics(SM_CYBORDER) * 2;//bWin4 ? CY_BORDER*2 : CY_BORDER;

#ifdef _DEBUG
	// if you won't output TRACE in debug mode, make it FALSE;
	BOOL bTraceOn = TRUE;
#endif

}

#ifdef _DEBUG
	#define LTRACE if (bTraceOn) TRACE
#else
	#define LTRACE
#endif

BOOL CMenuBar::m_bMDIApp = FALSE;

BEGIN_MESSAGE_MAP(CMenuBar, CControlBar)
	//{{AFX_MSG_MAP(CMenuBar)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_KILLFOCUS()
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	ON_WM_DESTROY()
	ON_MESSAGE(MB_SET_MENU_NULL, OnSetMenuNull)
	ON_MESSAGE(WM_SYSCOLORCHANGE, OnSettingChange)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETTINGCHANGE, OnSettingChange)
END_MESSAGE_MAP()

CMenuBar::CMenuBar()
{
	m_nCurIndex  = -1;
	m_nTrackingState = none;
	m_bProcessRightArrow = m_bProcessLeftArrow = TRUE;
	m_bIgnoreAlt = FALSE;
	m_bDown		 = FALSE;
	m_hMenu		 = NULL;
	m_nIDEvent	 = NULL;

	m_bIcon		 = FALSE;
	m_bMDIMaximized = FALSE;
	m_hWndMDIClient = NULL;
	m_hWndActiveChild = NULL;

	m_pMenuIcon = NULL;
	m_pMenuControl = NULL;

	m_nCmdShow = SW_SHOW;
}

BOOL CMenuBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID)
{
	ASSERT_VALID(pParentWnd);	// must have a parent
	ASSERT (!((dwStyle & CBRS_SIZE_FIXED) && (dwStyle & CBRS_SIZE_DYNAMIC)));

	// save the style
	m_dwStyle = dwStyle & CBRS_ALL;   // fixed by Mark Gentry, thanx!
	m_dwStyle |= CBRS_SIZE_DYNAMIC;

	CString strClass = AfxRegisterWndClass(
		CS_HREDRAW | CS_VREDRAW |
		CS_DBLCLKS,	// don't forget!
		AfxGetApp()->LoadStandardCursor(IDC_ARROW),
		(HBRUSH)(COLOR_BTNFACE + 1));

	return CWnd::Create(strClass, _T("MenuBar"), dwStyle, CRect(), pParentWnd, nID);
}

int CMenuBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	CWnd* pFrame = GetOwner();
	ASSERT_VALID(pFrame);

	// hook frame window to trap WM_MENUSELECT
	m_hookFrame.Install(this, pFrame->GetSafeHwnd());

	// If this is an MDI app, hook client window to trap WM_MDISETMENU
	if (pFrame->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd))) {
		CMenuBar::m_bMDIApp = TRUE;
		m_hWndMDIClient = ((CMDIFrameWnd*)pFrame)->m_hWndMDIClient;
		ASSERT(m_hWndMDIClient);
		m_hookMDIClient.Install(this, m_hWndMDIClient);
	}
	
	// my own DockContext!
	m_pDockContext = new CMenuBarDockContext(this);

	return 0;
}

BOOL CMenuBar::InitItems()
{
	ASSERT(m_hMenu);

	// clean up all items
	DeleteItems();

	// a little suitable
	int nCount = ::GetMenuItemCount(m_hMenu);
	ASSERT(nCount > 0);
	m_arrItem.SetSize(nCount);

	if (!CMenuButton::InitCommonResource()) {
		//TRACE("Failed to create bar resource\n");
		return FALSE;
	}

	// buttons
	for (int i = 0; i < nCount; ++i) {
		m_arrItem[i] = new CMenuButton(m_hMenu, i);
	}
	cyMenuButton = m_arrItem[0]->m_sizeHorz.cy;

	// icon
	m_pMenuIcon = new CMenuIcon(this);
	m_arrItem.InsertAt(0, m_pMenuIcon);

	// frame control
	m_pMenuControl = new CMenuControl(this);
	m_arrItem.Add(m_pMenuControl);
    
	// reinitializing
	m_hWndActiveChild = GetActiveChildWnd(m_bMDIMaximized);
	if (m_hWndActiveChild) {// re set 
		m_pMenuIcon->OnActivateChildWnd(m_hWndActiveChild);
	}
	if (m_bMDIMaximized) {
		m_pMenuIcon->Validate(TRUE);
		m_pMenuControl->Validate(TRUE);
	}

	return TRUE;
}

BOOL CMenuBar::LoadMenuBar(UINT nIDResource)
{
	if (m_hMenu) {
		::DestroyMenu(m_hMenu);
		m_hMenu = NULL;
	}

	ASSERT_VALID(m_pDockSite);
	if (m_pDockSite->GetMenu()) {
		PostMessage(MB_SET_MENU_NULL, (WPARAM)m_pDockSite->GetSafeHwnd());
	}
	
	m_hMenu = ::LoadMenu(AfxGetInstanceHandle(), MAKEINTRESOURCE(nIDResource));
	if (m_hMenu == NULL) {
		//TRACE("Failed to load menu\n");
		return FALSE;
	}

	return InitItems();
}

void CMenuBar::RefreshBar()
{
	InvalidateRect(NULL);

	CFrameWnd* pFrame = (CFrameWnd*)GetOwner();
	ASSERT_VALID(pFrame);
	ASSERT(pFrame->IsFrameWnd());
	pFrame->RecalcLayout();

	// floating frame
	pFrame = GetParentFrame();
	if (pFrame->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)))
		pFrame->RecalcLayout();
}


HMENU CMenuBar::LoadMenu(HMENU hMenu, HMENU hWindowMenu)
{
	LTRACE("CMenuBar::LoadMenu\n");
	UINT iPrevID=(UINT)-1;
	ASSERT(::IsMenu(hMenu));
	ASSERT_VALID(this);

	
	CFrameWnd* pFrame = GetParentFrame();
	if (::GetMenu(pFrame->GetSafeHwnd()) != NULL) {
		// not to make MFC ignore SetMenu(NULL), post it.
		PostMessage(MB_SET_MENU_NULL, (WPARAM)pFrame->GetSafeHwnd());
	}
	

	HMENU hOldMenu = m_hMenu;
	m_hMenu = hMenu;			// menu is shared with MFC

	// initialize Items 
	VERIFY(InitItems());

	if (hMenu) {
		m_hWindowMenu = hWindowMenu;
		RefreshBar();				// and menubar itself
	}

	return hOldMenu;
}


CMenuBar::~CMenuBar()
{
	if (m_bMDIApp == FALSE && m_hMenu != NULL)
		::DestroyMenu(m_hMenu);
}

/////////////////////////////////////////////////////////////////////////////
// CMenuBar アイテムの情報


/////////////////////////////////////////////////////////////////////////////
// CMenuBar アイテム描画

void CMenuBar::UpdateBar(TrackingState nState, int nNewIndex)
{
	if (m_nTrackingState == buttonmouse)	
		m_bIgnoreAlt = FALSE;	// if prev state is BUTTONMOUSE, always should be FALSE!

	m_nTrackingState = nState;

#ifdef _DEBUG
//	static LPCTSTR lpszStates[] = { _T("NONE"), _T("BUTTON"), _T("POPUP"), _T("BUTTONMOUSE") };
//	LTRACE(_T("CMenuBar::UpdateBar state to %s, button=%d\n"),
//		lpszStates[nState], nNewIndex);
#endif

	// clean up
	if (IsValidIndex(m_nCurIndex)) {
		CDC* pDC = GetDC();
		m_arrItem[m_nCurIndex]->SetState(CMenuButton::none);
		m_arrItem[m_nCurIndex]->Update(pDC);

		CRect rcCross = m_pMenuControl->m_rcItem & m_arrItem[m_nCurIndex]->m_rcItem;
		if (!rcCross.IsRectEmpty()) {
			m_pMenuControl->ForceDrawControl(pDC);
		}

		ReleaseDC(pDC);
		m_nCurIndex = -1;
	}

	if (nState != none) {
		ASSERT(IsValidIndex(nNewIndex));
		m_nCurIndex = nNewIndex;
		CDC* pDC = GetDC();

		if (nState == button || nState == buttonmouse) {
			m_arrItem[m_nCurIndex]->SetState(CMenuButton::hot);
			m_arrItem[m_nCurIndex]->Update(pDC);
		}
		else if (nState == popup) {
			m_arrItem[m_nCurIndex]->SetState(CMenuButton::select);
			m_arrItem[m_nCurIndex]->Update(pDC);
		}

		CRect rcCross = m_pMenuControl->m_rcItem & m_arrItem[m_nCurIndex]->m_rcItem;
		if (!rcCross.IsRectEmpty()) {
			m_pMenuControl->ForceDrawControl(pDC);
		}

		ReleaseDC(pDC);
	}
	else {
		// must be default parameter
		ASSERT(nNewIndex == -1);
	}

	m_bProcessRightArrow = m_bProcessLeftArrow = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMenuBar メッセージ ハンドラ

int CMenuBar::HitTestOnTrack(CPoint point)
{
	for (int i = 0; i < GetItemCount(); ++i) {
		CMenuItem* pItem = m_arrItem[i];
		CRect rcItem = pItem->GetItemRect();
		
		if (pItem->IsValid() && pItem->CanTrack() &&
			rcItem.PtInRect(point))
			return i;
	}

	return -1;
}

void CMenuBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
//	LTRACE("CMenuBar::OnLButtonDown\n");

	ASSERT(m_pMenuControl);
	if (m_pMenuControl->OnMouseMsg(WM_LBUTTONDOWN, nFlags, point)) {
		return;			// eat it!
	}	
	
	int nIndex = HitTestOnTrack(point);
	if (IsValidIndex(nIndex) && m_arrItem[nIndex]->CanTrack()) {
/*		HMENU hSubMenu = ::GetSubMenu(m_hMenu, nIndex);
		if (hSubMenu == NULL) {
			UpdateWindow();	// force to repaint
			CDC* pDC = GetDC();
			m_arrItem[m_nCurIndex]->SetState(CMenuButton::select);
			m_arrItem[nIndex]->Update(pDC);
			ReleaseDC(pDC);
			m_bDown = TRUE;
		}
		else {
			TrackPopup(nIndex);
		}
*/
		TrackPopup(nIndex);

		return;		// eat it!
	}

	CControlBar::OnLButtonDown(nFlags, point);
}

void CMenuBar::OnMouseMove(UINT nFlags, CPoint point) 
{
//	TRACE("CMenuBar::OnMouseMove\n");

	if (!IsTopParentActive() || !GetTopLevelParent()->IsWindowEnabled()) {
		// window is not active, ignore
		CControlBar::OnMouseMove(nFlags, point);
		return;
	}

	ASSERT(m_pMenuControl);
	if (m_pMenuControl->OnMouseMsg(WM_MOUSEMOVE, nFlags, point)) {
		CControlBar::OnMouseMove(nFlags, point);
		return;
	}

	int nIndex = HitTestOnTrack(point);
	if (IsValidIndex(nIndex)) {	
		if (m_nCurIndex == -1 || m_nCurIndex != nIndex) {	// other button
			UpdateBar(buttonmouse, nIndex);			// button tracked with mouse
			// I wanna know when mouse is away,
			// but SetCapture makes ALT+F4 uncatchable
			// and WM_CAPTURECHANGED is never sent(why?), so we have to set timer
			_KillTimer();
			m_nIDEvent = SetTimer(1, 250, 0);
		}
	}
	else {
		UpdateBar();
	}

	CControlBar::OnMouseMove(nFlags, point);
}

void CMenuBar::OnTimer(UINT nIDEvent) 
{
	if( nIDEvent == m_nIDEvent && m_nTrackingState == buttonmouse) {
		CPoint pt; ::GetCursorPos(&pt);
		CRect rect;
		GetWindowRect(&rect);
		if (!rect.PtInRect(pt)) {
			UpdateBar();
			_KillTimer();
		}
	}

	CControlBar::OnTimer(nIDEvent);
}

void CMenuBar::OnKillFocus(CWnd* pNewWnd) 
{
	CControlBar::OnKillFocus(pNewWnd);
	
	// TODO: 
	UpdateBar();
}

LRESULT CMenuBar::OnSetMenuNull(WPARAM wParam, LPARAM)
{
	HWND hWnd = (HWND)wParam;
	ASSERT(::IsWindow(hWnd));
	::SetMenu(hWnd, NULL);
	
	return 0;
}

LRESULT CMenuBar::OnSettingChange(WPARAM wParam, LPARAM lParam)
{
	InitItems();

	CFrameWnd* pFrame = GetParentFrame();
	ASSERT_VALID(pFrame);
	pFrame->RecalcLayout();
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMenuBar

void CMenuBar::OnMenuSelect(HMENU hMenu, UINT nIndex)
{
	if (m_nTrackingState == popup) {
		m_bProcessRightArrow = (::GetSubMenu(hMenu, nIndex) == NULL);
		HMENU hSubMenu = ::GetSubMenu(hMenu, m_nCurIndex);
		if (hSubMenu == NULL)
			return;
		m_bProcessLeftArrow = (hMenu == hSubMenu);
	}
}

LRESULT CALLBACK CMenuBar::MenuInputFilter(int code, WPARAM wParam, LPARAM lParam)
{
	return (
		code == MSGF_MENU &&
		g_pMenuBar &&
		g_pMenuBar->OnMenuInput( *((MSG*)lParam) )
		) ? TRUE : CallNextHookEx(g_hMsgHook, code, wParam, lParam);
}

void CMenuBar::TrackPopup(int nIndex)
{
	ASSERT_VALID(this);

	m_nCurIndex = nIndex;
	m_bLoop = TRUE;
	while (m_bLoop == TRUE) {
		UpdateWindow();	// force to repaint when button hidden by other window
		UpdateBar(popup, m_nCurIndex);
	
		// install hook
		ASSERT(g_pMenuBar == NULL);
		g_pMenuBar = this;
		ASSERT(g_hMsgHook == NULL);

		m_bLoop = FALSE;
		g_hMsgHook = ::SetWindowsHookEx(WH_MSGFILTER,
			MenuInputFilter, NULL, AfxGetApp()->m_nThreadID);// m_bLoop may become TRUE

		// popup!!
		m_nTrackingState = popup;
		m_arrItem[m_nCurIndex]->TrackPopup(this);

		// uninstall hook
		::UnhookWindowsHookEx(g_hMsgHook);
		g_hMsgHook = NULL;
		g_pMenuBar = NULL;
	}

	UpdateBar();
}

BOOL CMenuBar::OnMenuInput(MSG& m)
{
	ASSERT_VALID(this);
	int nMsg = m.message;
	CPoint pt = m.lParam;
	ScreenToClient(&pt);

	switch (nMsg) {
	case WM_MOUSEMOVE:
		if (pt != m_ptMouse) {
			int nIndex = HitTestOnTrack(pt);
			if (IsValidIndex(nIndex) && nIndex != m_nCurIndex) {
				// defferent button clicked
				GetOwner()->PostMessage(WM_CANCELMODE); // destroy popupped menu
				UpdateBar();							// clean up
				m_nCurIndex = nIndex;
				m_bLoop = TRUE;							// continue loop
			}
			m_ptMouse = pt;
		}
		break;

	case WM_LBUTTONDOWN:
		if (HitTestOnTrack(pt) != -1 && HitTestOnTrack(pt) == m_nCurIndex) {
			// same button clicked
			GetOwner()->PostMessage(WM_CANCELMODE); // destroy popupped menu
			UpdateBar(button, m_nCurIndex);
			return TRUE;							// eat it!
		}
		break;

	case WM_KEYDOWN:	{
		TCHAR vKey = m.wParam;
		if (m_dwStyle & CBRS_ORIENT_VERT) {	// if vertical
			break; // do nothing
		}

		if ((vKey == VK_LEFT  && m_bProcessLeftArrow) ||
			(vKey == VK_RIGHT && m_bProcessRightArrow)) {
			// no submenu
			int nNewIndex = GetNextOrPrevButton(m_nCurIndex, vKey==VK_LEFT);
			GetOwner()->PostMessage(WM_CANCELMODE); // destroy popupped menu
			UpdateBar();
			m_nCurIndex = nNewIndex;
			m_bLoop = TRUE;							// continue loop			
			return TRUE;							// eat it!
		}
						}
		break;

	case WM_SYSKEYDOWN:
//		LTRACE("    m_bIgnore = TRUE\n");
		m_bIgnoreAlt = TRUE;					// next SysKeyUp will be ignored
		break;
	}

	return FALSE;	// pass along...
}

BOOL CMenuBar::TranslateFrameMessage(MSG* pMsg)
{
	ASSERT_VALID(this);
	ASSERT(pMsg);

	UINT nMsg = pMsg->message;
	if (WM_LBUTTONDOWN <= nMsg && nMsg <= WM_MOUSELAST) {
		if (pMsg->hwnd != m_hWnd && m_nTrackingState > 0) {
			// clicked outside
			UpdateBar();
		}
	}
	else if (nMsg == WM_SYSKEYDOWN || nMsg == WM_SYSKEYUP || nMsg == WM_KEYDOWN) {
		BOOL bAlt = HIWORD(pMsg->lParam) & KF_ALTDOWN;	// Alt key presed?
		TCHAR vkey = pMsg->wParam;						// + X key
		if (vkey == VK_MENU ||
			(vkey == VK_F10 && !((GetKeyState(VK_SHIFT) & 0x80000000) ||
			                   (GetKeyState(VK_CONTROL) & 0x80000000) || bAlt))) {
			// only alt key pressed
			if (nMsg == WM_SYSKEYUP) {
				switch (m_nTrackingState) {
				case none:
					if (m_bIgnoreAlt == TRUE) {
//						LTRACE("    ignore ALT key up\n");
						m_bIgnoreAlt = FALSE;
						break;
					}

					UpdateBar(button, GetNextOrPrevButton(0, FALSE));

					break;
				case button:
					UpdateBar();
					break;
				case buttonmouse:
					break;	// do nothing
				}
			}
			return TRUE;
		}
		else if ((nMsg == WM_SYSKEYDOWN || nMsg == WM_KEYDOWN)) {
			if (m_nTrackingState == button) {
				if (m_dwStyle & CBRS_ORIENT_HORZ) {	// if horizontal
					switch (vkey) {
					case VK_LEFT:
					case VK_RIGHT: {
						int nNewIndex  = GetNextOrPrevButton(m_nCurIndex, vkey == VK_LEFT);
						UpdateBar(button, nNewIndex);
						return TRUE;
								   }
					case VK_SPACE:
					case VK_UP:
					case VK_DOWN:
						TrackPopup(m_nCurIndex);
						return TRUE;

					case VK_ESCAPE:
						UpdateBar();
						return TRUE;
					}
				}
				else {								// if vertical
					switch (vkey) {
					case VK_UP:
					case VK_DOWN:{
						int nNewIndex = GetNextOrPrevButton(m_nCurIndex, vkey == VK_UP);
						UpdateBar(button, nNewIndex);
						return TRUE;
								   }
					case VK_SPACE:
					case VK_RIGHT:
					case VK_LEFT:
						TrackPopup(m_nCurIndex);
						return TRUE;

					case VK_ESCAPE:
						UpdateBar();
						return TRUE;
					}
				}
			}

			// Alt + X pressed
			if ((bAlt || m_nTrackingState == button) && isalnum(vkey)) {
				int nIndex;
				if (MapAccessKey(vkey, nIndex) == TRUE) {
					UpdateBar();
					TrackPopup(nIndex);
					return TRUE;		// eat it!
				}
				else if (m_nTrackingState==button && !bAlt) {
//					MessageBeep(0);		// if you want
					return TRUE;
				}
			}

			if (m_nTrackingState > 0) {	// unknown key
				if (m_nTrackingState != buttonmouse) {	// if tracked with mouse, don't update bar
					UpdateBar();
				}
			}
		}
	}
	return FALSE;	// pass along...
}

BOOL CMenuBar::MapAccessKey(TCHAR cAccessKey, int& nIndex)
{
	for (int i = 0; i < GetItemCount(); ++i) {
		TCHAR cKey;
		if (m_arrItem[i]->GetAccessKey(cKey) == TRUE &&
			cKey == cAccessKey) {
			nIndex = i;
			return TRUE;
		}
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CMenuBar layout

// calculate only layout from m_bWrapped
void CMenuBar::CalcFloatingLayout()
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	int xStart = CXGAP;

	if (!IsFloating()) {
		xStart += CXGRIPPER;
	}

	int xOffset = xStart;
	int yOffset = CYGAP;

	for (int i = 0; i < GetItemCount(); ++i) {
		CMenuItem* pItem = m_arrItem[i];
		if (pItem->IsValid()) {
			CPoint ptItem(xOffset, yOffset);
			pItem->Layout(ptItem, TRUE);	// layout by itself!
			if (pItem->m_bWrapped == TRUE) {
				xOffset = xStart;			// reset xOffset
				yOffset += pItem->m_sizeHorz.cy;
			}
			else
				xOffset += pItem->m_sizeHorz.cx;
		}
	}
}

// calulate ordinary layout and size without m_bWrapped 
CSize CMenuBar::CalcLayout(DWORD dwMode, int nLength)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));
	if (dwMode & LM_HORZDOCK)
		ASSERT(dwMode & LM_HORZ);
	CSize sizeResult(0, 0);

	if (!(dwMode & LM_HORZ)) 	{				// if vertical
		int yOffset = CXGAP;
		if (!IsFloating())
			yOffset += CXGRIPPER;

		for (int i = 0; i < GetItemCount(); ++i) {
			CMenuItem* pItem = m_arrItem[i];
			if (pItem->IsValid()) {
				CPoint	ptItem(CYGAPVERT, yOffset);
				pItem->Layout(ptItem, FALSE);	// layout by itself
				yOffset += pItem->m_rcItem.Height();
			}
		}
		int cxBar = max(::GetSystemMetrics(SM_CXSMICON), cyMenuButton + (CYGAPVERT)*2);
		sizeResult =  CSize(cxBar, yOffset+CXGAP);
	}
	else {										// if horizontal
		int xOffset = CXGAP;
		if (!IsFloating())
			xOffset += CXGRIPPER;

		for (int i = 0; i < GetItemCount(); ++i) {
			CMenuItem* pItem = m_arrItem[i];
			if (pItem->IsValid()) {
				CPoint ptItem(xOffset, CYGAP);
				pItem->Layout(ptItem, TRUE);	// layout by itself
				xOffset += pItem->m_rcItem.Width();
			}
		}
		int cyBar = max(::GetSystemMetrics(SM_CYSMICON), cyMenuButton + CYGAP*2);
		sizeResult = CSize(xOffset+CXGAP, cyBar);
	}

	return sizeResult;
	//return CSize(400,100);
}

// in fact, it's never called
CSize CMenuBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
//	LTRACE("CMenuBar::CalcFixedLayout\n");

	DWORD dwMode = bStretch ? LM_STRETCH : 0;
	dwMode |= bHorz ? LM_HORZ : 0;

	return CalcLayout(dwMode);
}

CSize CMenuBar::CalcDynamicLayout(int nLength, DWORD dwMode)
{
	LTRACE("CMenuBar::CalcDynamicLayout\n");

	// Who can understand what "dwMode" means?
	// 
	// If you want it be pricisely same as DevStudio style,
	// I think you have to create a "CDockContext" derived class.
	// This is the reason why everyone think "CControlBar class" junk ? 

	ASSERT_VALID(this);
	ASSERT_VALID(m_pDockSite);
	ASSERT_VALID(m_pDockBar);

	if (m_hMenu == NULL)	// if have no menu yet, just return
		return CSize(0, 0);

	if (IsFloating()) {
		CFrameWnd* pFrame = GetParentFrame();
		ASSERT_VALID(pFrame);
		CMenu* pSysMenu = pFrame->GetSystemMenu(FALSE);
		ASSERT_VALID(pSysMenu);
		pSysMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_DISABLED);
	}

	if (dwMode & LM_HORZ) {						// horizontal
	//	LTRACE("    horizontal\n");
		if (dwMode & LM_HORZDOCK) {
			if (IsFloating() || (m_dwStyle & CBRS_ORIENT_VERT)) {	// if Floating->Docking, return min size
	//			LTRACE("    return min size\n");
				return CalcLayout(dwMode);
				//return CalcSize(GetItemCount()); //Modify by xhs
			}
			else {
				if (m_pDockContext->m_pDC) {	// now dragging
					// return DockBar size
					//LTRACE("    now dragging, so calc with DockBar\n");
					CSize size = CalcSize(GetItemCount());//CalcLayout(dwMode);
					CRect rcFrame;
					m_pDockBar->GetWindowRect(rcFrame);
					CRect rcBar;
					GetWindowRect(rcBar);
					rcBar.right = rcFrame.right;
					return CSize(rcBar.Width() + cxBorder2, size.cy);
					//return CalcSize(GetItemCount()); //Modify by xhs
				}
				else {
					//LTRACE("    no draggin calc bigger size\n");
					CRect rcFrame;
					m_pDockSite->GetWindowRect(rcFrame);
					SizeMenuBar(GetItemCount(), rcFrame.Width() - cxBorder2*2, FALSE);
					CalcFloatingLayout();
					CSize size1 = CalcSize(GetItemCount());
					return CSize(rcFrame.Width(), size1.cy);
				    //return CalcSize(GetItemCount()); //Modify by xhs
				}
			}
		}
		else if (dwMode & LM_MRUWIDTH) {		// floating size
			//LTRACE("    return floating size\n");
			SizeMenuBar(GetItemCount(), m_nMRUWidth, FALSE);	// load floating Bar Width! 
			CalcFloatingLayout();
			return CalcSize(GetItemCount());
		}
		else if (dwMode & LM_COMMIT) {
			//LTRACE("    commit, just calc: %d\n", nLength);
			m_nMRUWidth = nLength;								// save floating Bar Width!!! (used when SaveBarState)
			CalcFloatingLayout();
			return CSize(0, 0);									// MFC does'nt use this Size
		}
		else if (dwMode & LM_LENGTHY) {
			//LTRACE("    nLength is height : %d\n", nLength);
			SizeMenuBar(GetItemCount(), nLength, TRUE);
			return CalcSize(GetItemCount());
		}
		else {
			//LTRACE("    nLength is width  : %d\n", nLength);
			SizeMenuBar(GetItemCount(), nLength, FALSE);
			return CalcSize(GetItemCount());
		}
		ASSERT(TRUE);
	}
	else {														// vertical
		InvalidateRect(NULL);									// force to repaint!!!
		//LTRACE("    vertical\n");
		if (IsFloating() || (m_dwStyle & CBRS_ORIENT_HORZ)) {	// if Floating->Docking, return min size
			//LTRACE("    return min size\n");
			return CalcLayout(dwMode);
		}
		else {									// return large size
			if (m_pDockContext->m_pDC) {		// CDockContext::m_bDragging is not helpful :(
				//LTRACE("    now dragging, so calc with DockBar\n");
				CSize size = CalcLayout(dwMode);
				CRect rcFrame;
				m_pDockBar->GetWindowRect(rcFrame);
				CRect rcBar;
				GetWindowRect(rcBar);
				rcBar.bottom = rcFrame.bottom;
				return CSize(size.cx, rcBar.Height());
			}
			else {
				//LTRACE("    no dragging, return biggest size\n");
				
				CSize size = CalcLayout(dwMode);
				CRect rcFrame;
				m_pDockSite->GetWindowRect(rcFrame);
				CRect rcBar;
				GetWindowRect(rcBar);
				rcBar.bottom = rcFrame.bottom;
				return CSize(size.cx, rcFrame.Height());
			}
		}
	}

	ASSERT(TRUE);								// never come here
	return CalcLayout(dwMode, nLength);
}

void CMenuBar::DrawGripper(CDC* pDC)
{
	if( (m_dwStyle & CBRS_FLOATING) || m_dwDockStyle == 0 )
		return;

	CRect rcGrip;
	GetClientRect(&rcGrip);

	if(m_dwStyle & CBRS_ORIENT_HORZ) {
		// gripper at left
		//rcGrip.DeflateRect(4, 3);
		rcGrip.right = rcGrip.left + 3;
		//pDC->DrawEdge(rcGrip, BDR_RAISEDINNER, BF_RECT);
		rcGrip.OffsetRect(4, 0);
        pDC->DrawEdge(rcGrip,	BDR_RAISEDINNER, BF_RECT);
	}
	else {
		// gripper at top
		rcGrip.DeflateRect(3, 4);
		rcGrip.bottom = rcGrip.top + 3;
		pDC->DrawEdge(rcGrip, BDR_RAISEDINNER, BF_RECT);
		//rcGrip.OffsetRect(0, 4);
		//pDC->DrawEdge(rcGrip, BDR_RAISEDINNER, BF_RECT);
	}
}

#define CX_BORDER   1
#define CY_BORDER   1

void CMenuBar::DrawBorder(CDC* pDC)
{
	if( (m_dwStyle & CBRS_FLOATING) || m_dwDockStyle == 0 )
		return;

	CRect rect;
	GetClientRect(rect);

	DWORD dwStyle = m_dwStyle;
	if (!(dwStyle & CBRS_BORDER_ANY))
		return;

	CRect rect3 = rect;
	if (m_dwStyle & CBRS_ORIENT_HORZ) {
		rect3.left += cxBorder2;//rect.DeflateRect(2, 0);
		pDC->DrawEdge(rect3, BDR_RAISEDINNER ,BF_LEFT);	
	}
	else {
		rect3.top += cyBorder2;//rect.DeflateRect(0, 2);
		pDC->DrawEdge(rect3,BDR_RAISEDINNER ,  BF_TOP);	
	}

	// prepare for dark lines
	ASSERT(rect.top == 0 && rect.left == 0);
	CRect rect1, rect2;
	rect1 = rect;
	rect2 = rect;
	COLORREF clr = ::GetSysColor(COLOR_BTNSHADOW);//afxData.bWin4 ? afxData.clrBtnShadow : afxData.clrWindowFrame;

	// draw dark line one pixel back/up
	if (dwStyle & CBRS_BORDER_3D)
	{
		rect1.right -= CX_BORDER;
		rect1.bottom -= CY_BORDER;
	}
	if (dwStyle & CBRS_BORDER_TOP)
		rect2.top += CY_BORDER;//cyBorder2;

	if (dwStyle & CBRS_BORDER_BOTTOM)
		rect2.bottom -= CY_BORDER;

	if (dwStyle & CBRS_BORDER_LEFT)
		rect2.left += CX_BORDER;

	if (dwStyle & CBRS_BORDER_RIGHT)
		rect2.right -= CX_BORDER;

	if (dwStyle & CBRS_BORDER_3D)
	{
		// draw left and top
		if (dwStyle & CBRS_BORDER_LEFT)
			pDC->DrawEdge(rect2, BDR_RAISEDINNER, BF_LEFT);//pDC->FillSolidRect(1, rect2.top, CX_BORDER, rect2.Height(), clr);
		if (dwStyle & CBRS_BORDER_TOP)
			pDC->DrawEdge(rect2, BDR_RAISEDINNER, BF_TOP);//pDC->FillSolidRect(0, 1, rect.right, CY_BORDER, clr);

		// draw right and bottom
		if (dwStyle & CBRS_BORDER_RIGHT)
			pDC->DrawEdge(rect2, BDR_RAISEDINNER, BF_RIGHT);//pDC->FillSolidRect(rect.right, rect2.top, -CX_BORDER, rect2.Height(), clr);
		if (dwStyle & CBRS_BORDER_BOTTOM)
			pDC->DrawEdge(rect2, BDR_RAISEDINNER, BF_BOTTOM);//pDC->FillSolidRect(0, rect.bottom, rect.right, -CY_BORDER, clr);
	}
	
}

/////////////////////////////////////////////////////////////////////////////
// CMenuBarFrameHook implementation
CMenuBarFrameHook::CMenuBarFrameHook()
{
	m_pMenuBar = NULL;
}

BOOL CMenuBarFrameHook::Install(CMenuBar* pMenuBar, HWND hWndToHook)
{
	ASSERT_VALID(pMenuBar);
	ASSERT(m_pMenuBar == NULL);
	m_pMenuBar = pMenuBar;
	return HookWindow(hWndToHook);//CWnd::FromHandlePermanent(hWndToHook));
}

CMenuBarFrameHook::~CMenuBarFrameHook()
{
}

LRESULT CMenuBarFrameHook::WindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	ASSERT_VALID(m_pMenuBar);

	switch (nMsg) {
	case WM_MENUSELECT:
		m_pMenuBar->OnMenuSelect((HMENU)lParam, (UINT)LOWORD(wParam));
		break;

	// The following messages are trapped for the MDI client window
	case WM_INITMENUPOPUP:
		LTRACE("CMenuBar::WM_INITMENUPOPUP\n");
		if (!HIWORD(lParam) && (HMENU)wParam == m_pMenuBar->m_hWindowMenu)
			m_pMenuBar->OnInitMenuPopup();
		break;


	case WM_MDISETMENU:			// only sent to MDI client window
		// Setting new frame/window menu: bypass MDI. wParam is new menu.
		if (wParam) {
			LTRACE("CMenuBar::WM_MDISETMENU 0x%04x\n", wParam);
//			HMENU hNewMenu = (HMENU)wParam;
			m_pMenuBar->OnSetMenu((HMENU)wParam, (HMENU)lParam);			
		}
		return 0;

	case WM_MDIREFRESHMENU:		// only sent to MDI client window
		// Normally, would call DrawMenuBar, but I have the menu, so eat it.
//		LTRACE("CMenuBar::WM_MDIREFRESHMENU\n");
		return 0;

	case WM_PAINT:
		// After changing the MDI maximized state, the client window gets a
		// paint message. This is the most convenient place to find out; there
		// is no WM_MDIMAXIMIZED message.
		if (/*m_pWndHooked->*/m_hWnd == m_pMenuBar->m_hWndMDIClient)
			m_pMenuBar->OnPaintMDIClient();//CheckMinMaxState();
		break;
	}

	return CSubclassWnd::WindowProc(nMsg, wParam, lParam);
}

void CMenuBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	//LTRACE("CMenuBar::OnLButtonUp\n");
	// TODO: 
	ASSERT(m_pMenuControl);
	if (m_pMenuControl->OnMouseMsg(WM_LBUTTONUP, nFlags, point)) {
		CControlBar::OnLButtonUp(nFlags, point);
		return;
	}	

	int nIndex = HitTestOnTrack(point);
/*
	if (IsValidIndex(nIndex) && m_bDown == TRUE) {
		HMENU hSubMenu = ::GetSubMenu(m_hMenu, nIndex);
		if (hSubMenu == NULL) {
			UINT nID = ::GetMenuItemID(m_hMenu, nIndex);
			ASSERT(nID != -1);
			GetOwner()->SendMessage(WM_COMMAND, (WPARAM)nID, (LPARAM)GetSafeHwnd());

			UpdateBar();
		}
	}
*/
	CControlBar::OnLButtonUp(nFlags, point);
}



void CMenuBar::DoPaint(CDC* pDC)
{
	//LTRACE("CMenuBar::DoPaint\n");
	CRect rect;
	GetClientRect(rect);

	// draw buttons
	for (int i = 0; i < m_arrItem.GetSize(); ++i) {
		CMenuItem* pItem = m_arrItem[i];
		if (pItem->IsValid())
			m_arrItem[i]->Update(pDC);
	}

	// draw decorations
	DrawGripper(pDC);
	DrawBorder(pDC);

	// draw captions
	if (m_pMenuControl) {
		CRect rcDockBar;
		m_pDockBar->GetClientRect(rcDockBar);
		if (m_dwStyle & CBRS_ORIENT_HORZ) {
			m_pMenuControl->DelayLayoutAndDraw(pDC, CSize(rcDockBar.Width() + cxBorder2, rect.Height()));
		}
		else {
			m_pMenuControl->DelayLayoutAndDraw(pDC, CSize(rect.Width(), rcDockBar.Height()));
		}
	}
}

HMENU CMenuBar::GetMyMenu()
{
	return m_hMenu;
}

void CMenuBar::_KillTimer()
{
	if (m_nIDEvent) {
		KillTimer(m_nIDEvent);
		m_nIDEvent = NULL;
	}
}


int getPrevValidIndex(int nIndex)
{
	--nIndex;




	return nIndex;
}

// set only m_bWrapped by nWidth
int CMenuBar::WrapMenuBar(int nCount, int nWidth)
{
//	LTRACE("CMenuBar::WrapMenuBar\n");
	int nResult = 0;
	int xStart = CXGAP;
	if (!IsFloating() && !m_pDockContext->m_pDC)	// if not floating and ****not dragging!!!****
		xStart += CXGRIPPER;
	
	int x = xStart;
	
	for (int i = 0; i < nCount; ++i) {
		CMenuItem* pItem = m_arrItem[i];
		if (pItem->IsValid()) {

			if (i+1 == nCount)
				return ++nResult;
			
			if (x + pItem->m_sizeHorz.cx + CXGAP> nWidth) {// itself is over
				if (pItem->CanWrap()) {
					pItem->m_bWrapped = TRUE;
					++nResult;
					x = xStart;
				}
			}
			else if (x + pItem->m_sizeHorz.cx + m_arrItem[i+1]->m_sizeHorz.cx + CXGAP > nWidth) {
				if (pItem->CanWrap()) {
					pItem->m_bWrapped = TRUE;
					++nResult;
					x = xStart;
				}
			}
			else {
				pItem->m_bWrapped = FALSE;
				x += pItem->m_sizeHorz.cx;
			}
		}
	}
/*			//another algorithm
			int dx = pItem->m_sizeHorz.cx;
			if (dx + CXGAP > nWidth) {		// bigger than nWidth
				if (pItem->CanWrap()) {
					pItem->m_bWrapped = TRUE;
//					if (i != nCount - 1)
						++nResult;
				}
			}
			else if (dx + x + CXGAP > nWidth) {
				if (i > 0 && m_arrItem[i-1]->CanWrap()) {
			//		LTRACE("index %d is wrap\n", i-1);
					m_arrItem[i-1]->m_bWrapped = TRUE;
					pItem->m_bWrapped = FALSE;
					x = xStart;
//					if (i != nCount - 1)///////////////////////////
						++nResult;
				}
			}
			else {
				pItem->m_bWrapped = FALSE;
			}
			x += dx;

		}
	}
	*/

	return nResult + 1;
}

// calc only size, by using m_bWrapped
CSize CMenuBar::CalcSize(int nCount)
{
	ASSERT(nCount > 0);
	CPoint cur(CXGAP, CYGAP);
	CSize sizeResult(0, 0);
	int nWrap = 0;
	for (int i = 0; i < nCount; ++i) {
		CMenuItem* pItem = m_arrItem[i];
		if (pItem->IsValid()) {
			sizeResult.cx = max(cur.x + pItem->m_sizeHorz.cx, sizeResult.cx);
			sizeResult.cy = max(cur.y + pItem->m_sizeHorz.cy, sizeResult.cy);

			cur.x += pItem->m_sizeHorz.cx;
			
			if (pItem->m_bWrapped == TRUE) {
				LTRACE("    nIndex:%d is wrapped\n", i);
				cur.x = CXGAP;					// reset x pos
				cur.y += pItem->m_sizeHorz.cy;
				++nWrap;
			}

		}
	}

	//sizeResult.cy += CYGAP;
	//sizeResult.cx += CXGAP;
	// Modify by xhs , and the width of menubar will be shorter
	sizeResult.cy += (CYGAP-4);
	sizeResult.cx += (CXGAP+2);  

	return sizeResult;
}

void CMenuBar::SizeMenuBar(int nCount, int nLength, BOOL bVert)
{
	//LTRACE("CMenuBar::SizeMenuBar\n");
	ASSERT(nCount > 0);

	if (!bVert) {				// nLength is horizontal length

		if (IsFloating()) {		// half size wrapping

			CSize sizeMax, sizeMin, sizeMid;

			// Wrap MenuBar vertically
			WrapMenuBar(nCount, 0);
			sizeMin = CalcSize(nCount);

			// Wrap MenuBar horizontally
			WrapMenuBar(nCount, 32767);
			sizeMax = CalcSize(nCount);

			// we can never know this algorithm :), see CToolBar implementation
			while (sizeMin.cx < sizeMax.cx) {
//				LTRACE("looping sizeMin.cx:%d < sizeMax.cx:%d\n", sizeMin.cx, sizeMax.cx);
				sizeMid.cx = (sizeMin.cx + sizeMax.cx) / 2;
				WrapMenuBar(nCount, sizeMid.cx);
				sizeMid = CalcSize(nCount);
				if (sizeMid.cx == sizeMax.cx) {	// if you forget, it loops forever!
					return;
				}
//				LTRACE("    sizeMid : %d %d\n", sizeMid.cx, sizeMid.cy);

				if (nLength >= sizeMax.cx) {
//					LTRACE("    nLength:%d >= sizeMax.cx:%d\n", nLength, sizeMax.cx);
					if (sizeMin == sizeMid) {
						WrapMenuBar(nCount, sizeMax.cx);
//						LTRACE("out SizeMenuBar\n");
						return;
					}
					sizeMin = sizeMid;
				}
				else if (nLength < sizeMax.cx) {
//					LTRACE("    nLength:%d < sizeMax.cx:%d\n", nLength, sizeMax.cx);
					sizeMax = sizeMid;
				}
				else {
//					LTRACE("out SizeMenuBar\n");
					return;
				}
			}
		}
		else {								// each one wrapping
			LTRACE("    just each one wrapping\n");
			WrapMenuBar(nCount, nLength);
		}
	}
	else {						// nLength is vertical length
		CSize sizeMax, sizeMin, sizeMid;

		// Wrap MenuBar vertically
		WrapMenuBar(nCount, 0);
		sizeMin = CalcSize(nCount);

		// Wrap MenuBar horizontally
		WrapMenuBar(nCount, 32767);
		sizeMax = CalcSize(nCount);

		while (sizeMin.cx < sizeMax.cx) {
			sizeMid.cx = (sizeMin.cx + sizeMax.cx) / 2;
			WrapMenuBar(nCount, sizeMid.cx);
			sizeMid = CalcSize(nCount);
			if (sizeMid.cx == sizeMax.cx) {
				return;
			}
			if (nLength < sizeMid.cy) {
				if (sizeMin == sizeMid) {
					WrapMenuBar(nCount, sizeMax.cx);
					//LTRACE("out SizeMenuBar\n");
					return;
				}
				sizeMin = sizeMid;
			}
			else if (nLength > sizeMid.cy)
				sizeMax = sizeMid;
			else {
				//LTRACE("out SizeMenuBar\n");
				return;
			}
		}
	}

	//LTRACE("out SizeMenuBar\n");
}


void CMenuBar::DeleteItems()
{
	for(int i = 0; i < m_arrItem.GetSize(); ++i) {
		CMenuItem* pItem = m_arrItem[i];
		delete pItem;
	}

	m_arrItem.RemoveAll();

	m_pMenuIcon = NULL;
	m_pMenuControl = NULL;
}

void CMenuBar::OnDestroy() 
{
	CControlBar::OnDestroy();	
	
	_KillTimer();
	DeleteItems();
}

int CMenuBar::GetNextOrPrevButton(int nIndex, BOOL bPrev)
{
	int nCount = GetItemCount();
	if (bPrev) {				// <-
		--nIndex;
		if (nIndex < 0)
			nIndex = nCount - 1;
	}
	else {						// ->
		++nIndex;
		if (nIndex >= nCount)
			nIndex = 0;
	}

	if (!m_arrItem[nIndex]->CanTrack() || !m_arrItem[nIndex]->IsValid()) {
		return GetNextOrPrevButton(nIndex, bPrev);
	}

	return nIndex;
}

void CMenuBar::AddIcon(HICON hIcon)
{
//	ASSERT(m_bIcon == FALSE);

//	m_bIcon = TRUE;
}

CMenuControl::CMenuControl(CControlBar* pBar)
{
	m_bValid = FALSE;
	m_pBar = pBar;
	m_bDown = FALSE;
	m_nTracking = -1;

	int cxCaption = cyMenuButton - CYGAP + 1;
	m_sizeHorz = CSize(cxCaption*3 + CXGAP/2, cyMenuButton);
}

CMenuControl::~CMenuControl()
{

}

void CMenuControl::Update(CDC* pDC)
{
	// do nothing
}

void CMenuControl::Layout(CPoint point, BOOL bHorz)
{
	LTRACE("CMenuControl::Layout bHorz:%d\n", bHorz);
	m_bHorz = bHorz;

	// just layout easily
	if (bHorz) {
		m_rcItem = CRect(point, m_sizeHorz);
	}
	else {
		m_rcItem = CRect(point, CSize(m_sizeHorz.cy, m_sizeHorz.cx));
	}
}

void CMenuControl::DelayLayoutAndDraw(CDC* pDC, CSize sizeBar)
{
//	if (!IsValid())
//		return;

	// layout
	if (m_bHorz) {
		int cxCaption = cyMenuButton - CYGAP + 1;
		int cyCaption = cxCaption - 1;
		CRect rcCaption;
		rcCaption.right = sizeBar.cx;
		rcCaption.bottom = sizeBar.cy - CYGAP;
		rcCaption.left = rcCaption.right - cxCaption;
		rcCaption.top = rcCaption.bottom - cyCaption;

		m_arrCaption[0] = rcCaption;

		rcCaption -= CPoint(cxCaption+CXGAP/2, 0);
		m_arrCaption[1] = rcCaption;

		rcCaption -= CPoint(cxCaption, 0);
		m_arrCaption[2] = rcCaption;

		m_rcItem = CRect(m_arrCaption[2].left, m_arrCaption[2].top,
			m_arrCaption[0].right, m_arrCaption[0].bottom);
	}
	else {
		int cxCaption = cyMenuButton - CYGAPVERT - 1;
		int cyCaption = cxCaption - 1;
		CRect rcCaption;
		rcCaption.left = CYGAPVERT;
		rcCaption.bottom = sizeBar.cy;
		rcCaption.right = rcCaption.left + cxCaption;
		rcCaption.top = rcCaption.bottom - cyCaption;

		m_arrCaption[0] = rcCaption;	

		rcCaption -= CPoint(0, cyCaption+CXGAP/2);
		m_arrCaption[1] = rcCaption;

		rcCaption -= CPoint(0, cyCaption);
		m_arrCaption[2] = rcCaption;

		m_rcItem = CRect(m_arrCaption[2].left, m_arrCaption[2].top,
			m_arrCaption[0].right, m_arrCaption[0].bottom);
	}

	if (!IsValid())
		return;

	// draw frame controls
	for (int i = 0; i < 3; ++i) {
		DrawControl(pDC, i, FALSE);
	}

//	pDC->DrawFrameControl(m_arrCaption[0], DFC_CAPTION, DFCS_CAPTIONCLOSE);
//	pDC->DrawFrameControl(m_arrCaption[1], DFC_CAPTION, DFCS_CAPTIONRESTORE);
//	pDC->DrawFrameControl(m_arrCaption[2], DFC_CAPTION, DFCS_CAPTIONMIN);		
}


// pasted from CDockContext
void CMenuBarDockContext::StartDrag(CPoint pt)
{
	ASSERT_VALID(m_pBar);
	m_bDragging = TRUE;

	InitLoop();

	if (m_pBar->m_dwStyle & CBRS_SIZE_DYNAMIC)
	{
		// get TRUE bar size (including borders)
		CRect rect;
		m_pBar->GetWindowRect(rect);
		m_ptLast = pt;
		CSize sizeHorz = m_pBar->CalcDynamicLayout(0, LM_HORZ | LM_HORZDOCK);
		CSize sizeVert = m_pBar->CalcDynamicLayout(0, LM_VERTDOCK);
		CSize sizeFloat = m_pBar->CalcDynamicLayout(0, LM_HORZ | LM_MRUWIDTH);

		m_rectDragHorz = CRect(rect.TopLeft(), sizeHorz);
		m_rectDragVert = CRect(rect.TopLeft(), sizeVert);

		// calculate frame dragging rectangle
		m_rectFrameDragHorz = CRect(rect.TopLeft(), sizeFloat);
		m_rectFrameDragVert = CRect(rect.TopLeft(), sizeFloat);

#ifdef _MAC
		CMiniFrameWnd::CalcBorders(&m_rectFrameDragHorz,
			WS_THICKFRAME | WS_CAPTION, WS_EX_FORCESIZEBOX);
		CMiniFrameWnd::CalcBorders(&m_rectFrameDragVert,
			WS_THICKFRAME | WS_CAPTION, WS_EX_FORCESIZEBOX);
#else
		CMiniFrameWnd::CalcBorders(&m_rectFrameDragHorz);
		CMiniFrameWnd::CalcBorders(&m_rectFrameDragVert);
#endif
		m_rectFrameDragHorz.InflateRect(-cxBorder2, -cyBorder2);
		m_rectFrameDragVert.InflateRect(-cxBorder2, -cyBorder2);
	}
	else if (m_pBar->m_dwStyle & CBRS_SIZE_FIXED)
	{
		// get TRUE bar size (including borders)
		CRect rect;
		m_pBar->GetWindowRect(rect);
		m_ptLast = pt;
		CSize sizeHorz = m_pBar->CalcDynamicLayout(-1, LM_HORZ | LM_HORZDOCK);
		CSize sizeVert = m_pBar->CalcDynamicLayout(-1, LM_VERTDOCK);

		// calculate frame dragging rectangle
		m_rectFrameDragHorz = m_rectDragHorz = CRect(rect.TopLeft(), sizeHorz);
		m_rectFrameDragVert = m_rectDragVert = CRect(rect.TopLeft(), sizeVert);

		CMiniFrameWnd::CalcBorders(&m_rectFrameDragHorz);
		CMiniFrameWnd::CalcBorders(&m_rectFrameDragVert);
		m_rectFrameDragHorz.InflateRect(-cxBorder2, -cyBorder2);
		m_rectFrameDragVert.InflateRect(-cxBorder2, -cyBorder2);
	}
	else
	{
		// get TRUE bar size (including borders)
		CRect rect;
		m_pBar->GetWindowRect(rect);
		m_ptLast = pt;
		BOOL bHorz = HORZF(m_dwStyle);
		DWORD dwMode = !bHorz ? (LM_HORZ | LM_HORZDOCK) : LM_VERTDOCK;
		CSize size = m_pBar->CalcDynamicLayout(-1, dwMode);

		// calculate inverted dragging rect
		if (bHorz)
		{
			m_rectDragHorz = rect;
			m_rectDragVert = CRect(CPoint(pt.x - rect.Height()/2, rect.top), size);
		}
		else // vertical orientation
		{
			m_rectDragVert = rect;
			m_rectDragHorz = CRect(CPoint(rect.left, pt.y - rect.Width()/2), size);
		}

		// calculate frame dragging rectangle
		m_rectFrameDragHorz = m_rectDragHorz;
		m_rectFrameDragVert = m_rectDragVert;

		CMiniFrameWnd::CalcBorders(&m_rectFrameDragHorz);
		CMiniFrameWnd::CalcBorders(&m_rectFrameDragVert);
		m_rectFrameDragHorz.InflateRect(-cxBorder2, -cyBorder2);
		m_rectFrameDragVert.InflateRect(-cxBorder2, -cyBorder2);
	}

	// adjust rectangles so that point is inside
	AdjustRectangle(m_rectDragHorz, pt);
	AdjustRectangle(m_rectDragVert, pt);
	AdjustRectangle(m_rectFrameDragHorz, pt);
	AdjustRectangle(m_rectFrameDragVert, pt);

	// initialize tracking state and enter tracking loop
	m_dwOverDockStyle = CanDock();
	Move(pt);   // call it here to handle special keys
	_Track();
}

BOOL CMenuBarDockContext::_Track()
{
	// don't handle if capture already set
	if (::GetCapture() != NULL)
		return FALSE;

	// set capture to the window which received this message
	m_pBar->SetCapture();
	ASSERT(m_pBar == CWnd::GetCapture());

#ifndef _MAC
	// get messages until capture lost or cancelled/accepted
	while (CWnd::GetCapture() == m_pBar)
	{
		MSG msg;
		if (!::GetMessage(&msg, NULL, 0, 0))
		{
			AfxPostQuitMessage(msg.wParam);
			break;
		}

		switch (msg.message)
		{
		case WM_LBUTTONUP:
			if (m_bDragging)
				_EndDrag();
			else
				EndResize();
			return TRUE;
		case WM_MOUSEMOVE:
			if (m_bDragging)
				Move(msg.pt);
			else
				Stretch(msg.pt);
			break;
		case WM_KEYUP:
			if (m_bDragging)
				OnKey((int)msg.wParam, FALSE);
			break;
		case WM_KEYDOWN:
			if (m_bDragging)
				OnKey((int)msg.wParam, TRUE);
			if (msg.wParam == VK_ESCAPE)
			{
				CancelLoop();
				return FALSE;
			}
			break;
		case WM_RBUTTONDOWN:
			CancelLoop();
			return FALSE;

		// just dispatch rest of the messages
		default:
			DispatchMessage(&msg);
			break;
		}
	}
#else
	Point   ptCur = {0};

	// get messages until capture lost or cancelled/accepted
	while (CWnd::GetCapture() == m_pBar)
	{
		EventRecord     er;

		if (OSEventAvail(everyEvent, &er))
		{
			GetNextEvent(everyEvent, &er);
			switch (er.what)
			{
			case mouseUp:
				if (m_bDragging)
					EndDrag();
				else
					EndResize();
				return TRUE;

			case keyDown:
			case keyUp:
			case autoKey:
			case app2Evt:
			{
				MSG     msg;

				if (WrapEvent(&er, &msg, PM_REMOVE))
				{
					if (m_bDragging)
						OnKey((int)msg.wParam, msg.message == WM_KEYDOWN);
					if (msg.message == WM_KEYUP && msg.wParam == VK_ESCAPE)
					{
						CancelLoop();
						return FALSE;
					}
				}
				break;
			}

			default:
				break;
			}
		}
		else
		{
			if (!EqualPt(er.where, ptCur))
			{
				POINT pt = {er.where.h, er.where.v};
				if (m_bDragging)
					Move(pt);
				else
					Stretch(pt);
			}
		}
	}
#endif
	CancelLoop();

	return FALSE;
}

void CMenuBarDockContext::_EndDrag()
{
	CancelLoop();

	if (m_dwOverDockStyle != 0)
	{
		CDockBar* pDockBar = GetDockBar(m_dwOverDockStyle);
		ASSERT(pDockBar != NULL);

		CRect rect = (m_dwOverDockStyle & CBRS_ORIENT_VERT) ?
			m_rectDragVert : m_rectDragHorz;

		UINT uID = _AfxGetDlgCtrlID(pDockBar->m_hWnd);
		if (uID >= AFX_IDW_DOCKBAR_TOP &&
			uID <= AFX_IDW_DOCKBAR_BOTTOM)
		{
			m_uMRUDockID = uID;
			m_rectMRUDockPos = rect;
			pDockBar->ScreenToClient(&m_rectMRUDockPos);
		}

		// dock it at the specified position, RecalcLayout will snap

		// insisting own line trick!
		CRect rcDockBar;
		pDockBar->GetWindowRect(rcDockBar);

		if (m_dwOverDockStyle & CBRS_ORIENT_VERT) {
			rect.top = rcDockBar.top - 1;
		}
		else {
			rect.left = rcDockBar.left - 10;
		}

		m_pDockSite->DockControlBar(m_pBar, pDockBar, &rect);
		m_pDockSite->RecalcLayout();

	}
	else if ((m_dwStyle & CBRS_SIZE_DYNAMIC) || (HORZF(m_dwStyle) && !m_bFlip) ||
			(VERTF(m_dwStyle) && m_bFlip))
	{
		m_dwMRUFloatStyle = CBRS_ALIGN_TOP | (m_dwDockStyle & CBRS_FLOAT_MULTI);
		m_ptMRUFloatPos = m_rectFrameDragHorz.TopLeft();
		m_pDockSite->FloatControlBar(m_pBar, m_ptMRUFloatPos, m_dwMRUFloatStyle);
	}
	else // vertical float
	{
		m_dwMRUFloatStyle = CBRS_ALIGN_LEFT | (m_dwDockStyle & CBRS_FLOAT_MULTI);
		m_ptMRUFloatPos = m_rectFrameDragVert.TopLeft();
		m_pDockSite->FloatControlBar(m_pBar, m_ptMRUFloatPos, m_dwMRUFloatStyle);
	}
}


void CMenuBar::OnPaintMDIClient()
{
	LTRACE("CMenuBar::OnPaintMDIClient\n");

	if (m_hWndMDIClient) {
		BOOL bMax = FALSE;
		// get active MDI child window
		HWND hWndChild = (HWND)::SendMessage(m_hWndMDIClient, WM_MDIGETACTIVE, 0,
			(LPARAM)&bMax);
		//ASSERT(::IsWindow(hWndChild)); crash! tell me why!

		if (bMax != m_bMDIMaximized) {
			LTRACE("    max state changed\n");
			m_bMDIMaximized = bMax;
			
			if (bMax) {
				LTRACE("        maximized\n");
				m_pMenuControl->Validate(TRUE);
				m_pMenuIcon->Validate(TRUE);
			}
			else {
				LTRACE("        not maximized\n");
				m_pMenuControl->Validate(FALSE);
				m_pMenuIcon->Validate(FALSE);
			}

			RefreshBar();			
		}
	}	
}

void CMenuBar::OnInitMenuPopup()
{
	CMenu menu;
	menu.Attach((HMENU)m_hWindowMenu);

	// scan for first window command
	int n = menu.GetMenuItemCount();
	BOOL bAddSeperator = TRUE;
	for (int iPos=0; iPos<n; iPos++) {
		if (menu.GetMenuItemID(iPos) >= AFX_IDM_FIRST_MDICHILD) {
			bAddSeperator = FALSE;
			break;
		}
	}

	// iPos is either first window item, or end if none found.

	// delete everything after.
	while (iPos < (int)menu.GetMenuItemCount())
		menu.RemoveMenu(iPos, MF_BYPOSITION);

	// get active window so I can check its menu item
	ASSERT(m_hWndMDIClient);
	HWND hwndActive = (HWND)::SendMessage(m_hWndMDIClient,
		WM_MDIGETACTIVE, 0, NULL);

	// append window names in the form "# title"
	int iWin=1;
	for (HWND hwnd=::GetWindow(m_hWndMDIClient, GW_CHILD);
		  hwnd;
		  hwnd = ::GetWindow(hwnd, GW_HWNDNEXT)) {

		if (bAddSeperator) {
			menu.InsertMenu(iPos++, MF_BYPOSITION|MF_SEPARATOR);
			bAddSeperator = FALSE;
		}

		// build item name and add it to the menu
		CString sWinName, sMenuItem;
		CWnd::FromHandle(hwnd)->GetWindowText(sWinName);
		sMenuItem.Format(_T("&%d  %s"), iWin++, (LPCTSTR)sWinName);
		menu.InsertMenu(iPos, MF_BYPOSITION,
			::GetDlgCtrlID(hwnd), sMenuItem);
		if (hwnd==hwndActive)
			menu.CheckMenuItem(iPos, MF_BYPOSITION|MF_CHECKED);
		iPos++;
	}
	menu.Detach();
}

void CMenuBar::OnSetMenu(HMENU hNewMenu, HMENU hWindowMenu)
{
	// We can get active MDI child window on this message!
	BOOL bMax;
	HWND hWndChild = GetActiveChildWnd(bMax);

	if (!m_hWndActiveChild || m_hWndActiveChild != hWndChild) {
		// active child window changed
		LTRACE("    active child window changed\n");
		m_hWndActiveChild = hWndChild;
		// tell MenuIcon child window has been changed
		m_pMenuIcon->OnActivateChildWnd(hWndChild);
	}

	LTRACE("CMenuBar::OnSetMenu\n");

	if (!m_hMenu || m_hMenu != hNewMenu) {	// menu changed
		LTRACE("    menu changed\n");
		LoadMenu(hNewMenu, hWindowMenu);	// set toolbar menu
		GetOwner()->SetMenu(NULL);			// clear frame menu
	}
}

HWND CMenuBar::GetActiveChildWnd(BOOL& bMaximized)
{
	if (!m_hWndMDIClient)
		return NULL;

	BOOL bMax = FALSE;
	HWND hWnd = (HWND)::SendMessage(m_hWndMDIClient,
		WM_MDIGETACTIVE, 0, (LPARAM)&bMax);
	bMaximized = bMax;

	return hWnd;
}


CMenuItem::CMenuItem()
{
	m_itemState = none;
	m_rcItem	= CRect(0, 0, 0, 0);
	m_sizeHorz	= CSize(0, 0);
	m_bHorz		= TRUE;
	m_bWrapped	= FALSE;
}

CMenuItem::~CMenuItem()
{

}

CPoint CMenuItem::ComputeMenuTrackPoint(CWnd* pWnd, HMENU hSubMenu, TPMPARAMS& tpm, CFont* pFont)
{
	ASSERT_VALID(pWnd);
	ASSERT_VALID(pFont);
	ASSERT(::IsMenu(hSubMenu));

	tpm.cbSize = sizeof(tpm);
	CRect& rcExclude = (CRect&)tpm.rcExclude;
	CWnd::GetDesktopWindow()->GetWindowRect(&rcExclude);

	CPoint pt;
	CRect  rcItem = m_rcItem;
	pWnd->ClientToScreen(&rcItem);

//	if (hSubMenu == NULL)		// it's possible no sub menu
//		return CPoint();

	if (m_bHorz) {			// horizontal
		int nCount = ::GetMenuItemCount(hSubMenu);
		ASSERT(nCount != -1);
		int cyPopup = nCount * rcItem.Height();	// I want it be not owner drawn but ordinary menu..

		if (rcItem.bottom + cyPopup > rcExclude.bottom) {
			pt = CPoint(rcItem.left, rcItem.top);	// over Screen
			rcExclude.top = rcItem.top;
		}
		else {
			pt = CPoint(rcItem.left, rcItem.bottom);
		}

		pt += CPoint(-1, 1);						// precisely same as DevStudio
	}
	else {										// vertical
		// we never get the width of popup up menu, but I will try
		int nCount = ::GetMenuItemCount(hSubMenu);
		ASSERT(nCount != -1);
	
		int cxPopup = 0;
		CWindowDC dc(NULL);

		CFont* pOldFont = dc.SelectObject(pFont);
		for (int i = 0; i < nCount; ++i) {
			char szName[256];		
			MENUITEMINFO info;
			::ZeroMemory(&info, sizeof(MENUITEMINFO));
			info.cbSize		= sizeof(MENUITEMINFO);
			info.fMask		= MIIM_ID | MIIM_TYPE;
			info.dwTypeData = szName;
			info.cch		= sizeof(szName);
			::GetMenuItemInfo(hSubMenu, i, TRUE, &info);
			
			CString strItem(szName);

			CRect rcText(0, 0, 0, 0);
			dc.DrawText(strItem, &rcText,
				DT_SINGLELINE | DT_VCENTER | DT_CALCRECT | DT_NOPREFIX);
			int cxOffset = ::GetSystemMetrics(SM_CXMENUCHECK) * 2;
			cxPopup = max(rcText.Width() + 4*2 + CXGAP*2 + cxOffset, cxPopup);
		}
		dc.SelectObject(pOldFont);

		if (rcItem.right + cxPopup > rcExclude.right) {	// over right-side
			pt = CPoint(rcItem.left, rcItem.top);
			rcExclude.left = rcItem.left;
		}
		else {
			pt = CPoint(rcItem.right, rcItem.top);
			rcExclude.right = rcItem.right;
		}

		pt += CPoint(1, -1);							// precisely same as DevStudio
	}

	return pt;
}

CMenuIcon::CMenuIcon(CWnd* pWnd)
{
	m_hDocIcon = NULL;//hIcon;
	m_sizeHorz = CSize(::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	m_bValid = FALSE;
	m_hSysMenu = NULL;//::GetSystemMenu(pWnd->GetSafeHwnd(), FALSE);
}

CMenuIcon::~CMenuIcon()
{

}

void CMenuIcon::Update(CDC* pDC)
{
	ASSERT(m_hDocIcon);
	ASSERT(m_bValid);

	::DrawIconEx(pDC->m_hDC, m_rcItem.left, m_rcItem.top, m_hDocIcon,
		m_sizeHorz.cx, m_sizeHorz.cy, 0, NULL, DI_NORMAL);
}

void CMenuIcon::TrackPopup(CWnd* pWnd)
{
	ASSERT(m_hDocIcon);
	ASSERT(m_bValid);
	ASSERT(::IsMenu(m_hSysMenu));

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);
	::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0);
	CFont fontSys;
	if (!fontSys.CreateFontIndirect(&info.lfMenuFont))
		return;

	TPMPARAMS tpm;
	CPoint pt = ComputeMenuTrackPoint(pWnd, m_hSysMenu, tpm, &fontSys);

	::TrackPopupMenuEx(m_hSysMenu,
		TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
		pt.x, pt.y, pWnd->GetOwner()->GetSafeHwnd(), &tpm);
}

void CMenuIcon::Layout(CPoint point, BOOL bHorz)
{
	ASSERT(m_bValid);

	m_bHorz = bHorz;
	m_rcItem = CRect(point, m_sizeHorz);
}

void CMenuIcon::OnActivateChildWnd(HWND hWndChild)
{
	//TRACE("CMenuIcon::OnActivateChildWnd\n");
	ASSERT(::IsWindow(hWndChild));
	m_hSysMenu = ::GetSystemMenu(hWndChild, FALSE);
	ASSERT(::IsMenu(m_hSysMenu));
	m_hDocIcon = (HICON)GetClassLong(hWndChild, GCL_HICONSM);
}


BOOL CMenuControl::OnMouseMsg(UINT msg, UINT nFlags, CPoint pt)
{
	if (!IsValid())
		return FALSE;

	if (msg == WM_LBUTTONDOWN) {
		m_nTracking = HitTest(pt);
		if (m_nTracking >= 0) {
			CClientDC dc(m_pBar);
			DrawControl(&dc, m_nTracking, TRUE);
			m_bDown = TRUE;
			m_pBar->SetCapture();				  // grab mouse input
			return TRUE;
		}
	}
	else if ((msg == WM_MOUSEMOVE) && m_nTracking >= 0) {
		// mouse moved, and I am tracking: possibly draw button up/down
		BOOL bOldDown = m_bDown;
		m_bDown = m_arrCaption[m_nTracking].PtInRect(pt);
		if (bOldDown != m_bDown) {
			// up/down state changed: need to redraw button
			CClientDC dc(m_pBar);
			DrawControl(&dc, m_nTracking, m_bDown);
		}
		return TRUE; // handled

	}
	else if (msg == WM_LBUTTONUP && m_nTracking >= 0) {
		// user released the mouse and I am tracking: do button command
		ReleaseCapture(); // let go the mouse
		if (m_bDown) {
			// if button was down when released: draw button up, and do system cmd
			CClientDC dc(m_pBar);
			DrawControl(&dc, m_nTracking, FALSE);
			CFrameWnd* pFrame = m_pBar->GetTopLevelFrame()->GetActiveFrame();
			ASSERT_VALID(pFrame);
			static syscmd[3] =
			{ /*SC_MOUSEMENU,*/ SC_CLOSE, SC_RESTORE, SC_MINIMIZE };
			pFrame->SendMessage(WM_SYSCOMMAND, syscmd[m_nTracking]);
		}
		m_nTracking = -1; // stop tracking
		return TRUE;	  // handled (eat)
	}

	return FALSE;
}

int CMenuControl::HitTest(CPoint point)
{
	for (int i = 0; i < 3; ++i) {
		if (m_arrCaption[i].PtInRect(point))
			return i;
	}

	return -1;
}


void CMenuControl::DrawControl(CDC* pDC, int nIndex, BOOL bPressed)
{
	// draw frame controls
	CRect& rc = m_arrCaption[nIndex];
	static UINT dfcs[3] = { DFCS_CAPTIONCLOSE, DFCS_CAPTIONRESTORE, DFCS_CAPTIONMIN };
	UINT uState = dfcs[nIndex];
	if (bPressed)
		uState |= DFCS_PUSHED;

	pDC->DrawFrameControl(rc, DFC_CAPTION, uState);
}

void CMenuControl::ForceDrawControl(CDC * pDC)
{
	if (!IsValid())
		return;

	for (int i = 0; i < 3; ++i) {
		DrawControl(pDC, i, FALSE);
	}
}


namespace {

	const int CXTEXTMARGIN	  = 5;
	int CYTEXTMARGIN		  = 0;
	const int CYTEXTMARGINMIN = 2;
	
	CFont fontHorz, fontVert;

//	const int CXGAP = 3;

}// anonymouse namespace

CMenuButton::CMenuButton(HMENU hMenu, int nIndex)
{
	ASSERT(hMenu);
	InitButtonStringAndSubMenuHandle(hMenu, nIndex);
	InitHorizontalButtonSize();
	InitAccessKeyAndVerticalLinePoint();
}

CMenuButton::~CMenuButton()
{

}

BOOL CMenuButton::InitCommonResource()
{
	// clean up
	fontHorz.DeleteObject();
	fontVert.DeleteObject();

	// create fonts
	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);
	::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0);
	if (!fontHorz.CreateFontIndirect(&info.lfMenuFont))
		return FALSE;

	info.lfMenuFont.lfEscapement = -900;
	info.lfMenuFont.lfOrientation = -900;
	if (!fontVert.CreateFontIndirect(&info.lfMenuFont))
		return FALSE;


	// get font height
	LOGFONT logfont;
	fontHorz.GetLogFont(&logfont);
	int cyFont = abs(logfont.lfHeight);

	// calc Y text margin
	int cyText = cyFont + CYTEXTMARGINMIN*2;
	int cyMenu = ::GetSystemMetrics(SM_CYMENU);
	if (cyMenu > cyText) {
		CYTEXTMARGIN = (cyMenu - cyFont)/2;
	}
	else {
		CYTEXTMARGIN = CYTEXTMARGINMIN;
	}

	return TRUE;
}

void CMenuButton::InitButtonStringAndSubMenuHandle(HMENU hMenu, int nIndex)
{
	// get menu button Text
	char szText[256];		
	MENUITEMINFO info; ::ZeroMemory(&info, sizeof(MENUITEMINFO));
	info.cbSize		= sizeof(MENUITEMINFO);
	info.fMask		= MIIM_ID | MIIM_TYPE;
	info.dwTypeData = szText;
	info.cch		= sizeof(szText);
	::GetMenuItemInfo(hMenu, nIndex, TRUE, &info);
	m_strBtn = CString(szText);

	m_hSubMenu = ::GetSubMenu(hMenu, nIndex);
}

void CMenuButton::InitHorizontalButtonSize()
{
	// get menu button Text size
	ASSERT(m_strBtn.IsEmpty() == FALSE);

	CWindowDC dc(NULL);
	CRect rcText(0, 0, 0, 0);
	CFont* pOldFont = dc.SelectObject(&fontHorz);
	dc.DrawText(m_strBtn, &rcText, DT_SINGLELINE | DT_CALCRECT);
	dc.SelectObject(pOldFont);
	m_sizeHorz.cx = rcText.Width() + CXTEXTMARGIN*2;

	LOGFONT logfont;
	fontHorz.GetLogFont(&logfont);
	int cyFont = abs(logfont.lfHeight);

	//this is decide the height of the menubar button
	m_sizeHorz.cy = cyFont + CYTEXTMARGIN*2+2;
}

void CMenuButton::InitAccessKeyAndVerticalLinePoint()
{
	int nIndex = m_strBtn.Find('&');
	m_cAccessKey = m_strBtn[nIndex + 1];	// -1 + 1 = 0; it's ok

	if (nIndex == -1) {
		m_ptLineFrom = m_ptLineTo = CPoint(0, 0);
	}
	else if (nIndex == 0) {
		CRect rcTo;
		CWindowDC dc(NULL);
		CFont* pOldFont = dc.SelectObject(&fontHorz);
		dc.DrawText(m_strBtn.Left(nIndex+2), &rcTo, DT_SINGLELINE | DT_CALCRECT);
		dc.SelectObject(pOldFont);

		m_ptLineFrom = CPoint(CYTEXTMARGIN, CXTEXTMARGIN);
		m_ptLineTo	 = CPoint(CYTEXTMARGIN, CXTEXTMARGIN + rcTo.Width());
	}
	else {
		CRect rcFrom, rcTo;
		CWindowDC dc(NULL);
		CFont* pOldFont = dc.SelectObject(&fontHorz);
		dc.DrawText(m_strBtn.Left(nIndex), &rcFrom, DT_SINGLELINE | DT_CALCRECT);
		dc.DrawText(m_strBtn.Left(nIndex+2), &rcTo, DT_SINGLELINE | DT_CALCRECT);
		dc.SelectObject(pOldFont);

		m_ptLineFrom = CPoint(CYTEXTMARGIN, CXTEXTMARGIN + rcFrom.Width());
		m_ptLineTo = CPoint(CYTEXTMARGIN, CXTEXTMARGIN + rcTo.Width());
	}
}

void CMenuButton::Layout(CPoint point, BOOL bHorz)
{
	if (bHorz == TRUE) {
		//this can change the rect of menubutton modify by xhs
		CPoint p(point.x-3,point.y-2);
		m_rcItem = CRect(p, m_sizeHorz);
	}
	else {
		m_rcItem = CRect(point, CSize(m_sizeHorz.cy, m_sizeHorz.cx));
	}

	m_bHorz = bHorz;
}

void CMenuButton::Update(CDC* pDC)
{
	// clean background
	COLORREF clr = ::GetSysColor(COLOR_BTNFACE);
	pDC->FillSolidRect(m_rcItem, clr);

	switch (m_itemState) {
	case hot:
		DrawHot(pDC);
		break;
	case select:
		DrawSelect(pDC);
		break;
	case none:
		DrawNone(pDC);
		break;
	default:
		ASSERT(TRUE);
	}
}

void CMenuButton::TrackPopup(CWnd* pWnd)
{
	TPMPARAMS tpm;
	CPoint pt = ComputeMenuTrackPoint(pWnd, m_hSubMenu, tpm, &fontHorz);

	::TrackPopupMenuEx(m_hSubMenu,
		TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
		pt.x, pt.y, pWnd->GetOwner()->GetSafeHwnd(), &tpm);
}

void CMenuButton::DrawHorzText(CDC* pDC, CPoint ptOffset)
{
	CRect rcBtn = m_rcItem;
	pDC->SetBkMode(TRANSPARENT);
	CFont* pOldFont = pDC->SelectObject(&fontHorz);
	// I know precise text size
	rcBtn.DeflateRect(CXTEXTMARGIN, CYTEXTMARGIN);
	pDC->DrawText(m_strBtn, rcBtn + ptOffset,
		DT_SINGLELINE);// | DT_CENTER | DT_VCENTER);	no need
	
	pDC->SelectObject(pOldFont);
}

void CMenuButton::DrawVertText(CDC* pDC, CPoint ptOffset)
{
	CRect rcBtn = m_rcItem;
	int nLength = m_strBtn.GetLength();
	int nIndex = m_strBtn.Find('&');
	CString strBtn = m_strBtn.Left(nIndex) + m_strBtn.Right(nLength - (nIndex+1));

	pDC->SetBkMode(TRANSPARENT);
	CFont* pOldFont = pDC->SelectObject(&fontVert);
	// I must know precise text size
	CRect rcString = CRect(
		CPoint(rcBtn.right - CYTEXTMARGIN, rcBtn.top + CXTEXTMARGIN), m_sizeHorz);
	pDC->DrawText(strBtn, rcString + ptOffset,
		DT_SINGLELINE | DT_NOCLIP | DT_NOPREFIX);	// don't forget DT_NOCLIP
	
	pDC->SelectObject(pOldFont);

	// DrawText is poor, so we have to draw vertical line by ourselves
	pDC->MoveTo(rcBtn.TopLeft() + m_ptLineFrom + ptOffset);
	pDC->LineTo(rcBtn.TopLeft() + m_ptLineTo + ptOffset);
}

void CMenuButton::DrawHot(CDC* pDC)
{
	if (m_bHorz) {
		// draw down button
		pDC->DrawEdge(m_rcItem, BDR_RAISEDINNER, BF_RECT);
		DrawHorzText(pDC);
	}
	else {
		pDC->DrawEdge(m_rcItem, BDR_RAISEDINNER, BF_RECT);
		DrawVertText(pDC);
	}
}

void CMenuButton::DrawSelect(CDC* pDC)
{
	if (m_bHorz) {
		// draw pressed button
		pDC->DrawEdge(m_rcItem, BDR_SUNKENOUTER, BF_RECT);
		DrawHorzText(pDC, CPoint(1, 1));
	}
	else {
		pDC->DrawEdge(m_rcItem, BDR_SUNKENOUTER, BF_RECT);
		DrawVertText(pDC, CPoint(1, 1));
	}
}

void CMenuButton::DrawNone(CDC* pDC)
{
	if (m_bHorz) {
		DrawHorzText(pDC);
	}
	else {
		DrawVertText(pDC);
	}
}

int CMenuBar::OnActivateFrame(int nCmdShow)
{
	CFrameWnd* pFrame = GetParentFrame();
	ASSERT_VALID(pFrame);
	if (pFrame->GetMenu() != NULL) {
		LTRACE(" has menu\n");
		pFrame->SetMenu(NULL);
	}
	m_nCmdShow = nCmdShow;
	return SW_HIDE;
}
