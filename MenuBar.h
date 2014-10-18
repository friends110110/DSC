#if !defined(AFX_MENUBAR_H__AF01DCA8_266A_11D3_8FA2_444553540000__INCLUDED_)
#define AFX_MENUBAR_H__AF01DCA8_266A_11D3_8FA2_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxtempl.h>
#include "SubclassWnd.h"


// CMenuBar inspired by *PixieLib(TM) Copyright 1997-1998 Paul DiLascia*
//
// This class simulates VC++ style menu bar.
// It does'nt any new products(IE4 or a new comctrl32.dll).
//
// It is possible this code has many bugs :(
//
// Written by MB <mb2@geocities.co.jp>


//It is used when loading bar state from ini files, never change it!
#define AFX_IDW_MENUBAR 0xE804  // Menu bar

/////////////////////////////////////////////////////////////////////////////
// CMenuBar Control Bar

class CMenuBar;
class CMenuIcon;
class CMenuControl;
class CMenuButton;

class CMenuBarFrameHook : public CSubclassWnd
{
public:
	CMenuBarFrameHook();
	BOOL Install(CMenuBar* pMenuBar, HWND hWndToHook);
	virtual ~CMenuBarFrameHook();

protected:
	virtual LRESULT WindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam);

private:
	CMenuBar* m_pMenuBar;
};

class CMenuItem  
{
// Constructors
public:
	CMenuItem();

// Attributes
	enum State { none, select, hot };
	CRect& GetItemRect() { return m_rcItem; }

// Operations
	void SetState(State state) { m_itemState = state; }
	State GetState() { return m_itemState; }

// Overidables
	virtual void Update(CDC*) = 0;
	virtual void Layout(CPoint, BOOL bHorz) = 0;
	virtual void TrackPopup(CWnd* pWnd) = 0;
	virtual BOOL CanTrack() = 0;
	virtual BOOL CanWrap() = 0;
	virtual void Validate(BOOL bValid) = 0;
	virtual BOOL IsValid() = 0;
	virtual BOOL GetAccessKey(TCHAR&) = 0;


// Implementation
	virtual ~CMenuItem();

	// Implementation helper
	CPoint ComputeMenuTrackPoint(CWnd* pWnd, HMENU hSubMenu, TPMPARAMS& tpm, CFont* pFont);

protected:
	State	m_itemState;	// current state
	CRect	m_rcItem;		// current rect
	CSize	m_sizeHorz;		// ordinary size
	BOOL	m_bHorz;		// now horizontal?
	BOOL	m_bWrapped;		// start a new line?

	friend class CMenuBar;
};

class CMenuBar : public CControlBar
{
// Constructors
public:
	CMenuBar();
	BOOL Create(CWnd* pParentWnd,
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP,
		UINT nID = AFX_IDW_MENUBAR);

	BOOL LoadMenuBar(UINT nIDResource);
	HMENU LoadMenu(HMENU hMenu, HMENU hMenuWindow);

// Attributes
public:

// Operations
public:
	BOOL TranslateFrameMessage(MSG* pMsg);

// Overidables
	//{{AFX_VIRTUAL(CMenuBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	int OnActivateFrame(int nCmdShow);
	virtual ~CMenuBar();
	virtual void DoPaint(CDC* pDC);
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler) { }
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	virtual CSize CalcDynamicLayout(int nLength, DWORD dwMode);
    HMENU   GetMyMenu();
// message map functions
protected:
	//{{AFX_MSG(CMenuBar)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LRESULT OnSetMenuNull(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSettingChange(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	//	Implementation helper
	enum TrackingState { none = 0, button, popup, buttonmouse };
	void UpdateBar(TrackingState nState = none, int nNewIndex = -1);

	// decoration
	void DrawBorder(CDC* pDC);
	void DrawGripper(CDC* pDC);
	void RefreshBar();

	// layout
	void CalcFloatingLayout();
	CSize CalcLayout(DWORD dwMode, int nLength = -1);
	void SizeMenuBar(int nCount, int nLength, BOOL bVert);
	int WrapMenuBar(int nCount, int nWidth);
	CSize CalcSize(int nCount);

	// Items
	BOOL InitItems();
	int GetItemCount() const { return m_arrItem.GetSize(); }
	BOOL IsValidIndex(int nIndex) const { return 0 <= nIndex && nIndex < GetItemCount(); }
	BOOL MapAccessKey(TCHAR cKey, int& nIndex);
	int HitTestOnTrack(CPoint point);

	// system hook
	static LRESULT CALLBACK MenuInputFilter(int code, WPARAM wParam, LPARAM lParam);
	BOOL OnMenuInput(MSG&);

	// popup
	void TrackPopup(int nIndex);
	int GetNextOrPrevButton(int nIndex, BOOL bPrev);

	// message hook
	void OnMenuSelect(HMENU hMenu, UINT nIndex);

	// timer
	void _KillTimer();

	void DeleteItems();
	void AddIcon(HICON hIcon);
	void OnPaintMDIClient();

protected:
	HWND GetActiveChildWnd(BOOL& bMaximized);
	void OnSetMenu(HMENU hNewMenu, HMENU hWindowMenu);
	void OnInitMenuPopup();
	// CMenu is unavaiable, cause we have to share menu with MFC in MDI app.
	HMENU		  m_hMenu;			
	TrackingState m_nTrackingState;
	CArray<CMenuItem*, CMenuItem*> m_arrItem;	// menu items include buttons

private:
	int m_nCmdShow;
	// used for most important function
	BOOL	m_bLoop;
	int		m_nCurIndex;
	BOOL	m_bIgnoreAlt;

	CPoint	m_ptMouse;		// cursor position while hooking
	UINT	m_nIDEvent;		// timer ID
	BOOL	m_bDown;
	BOOL	m_bProcessRightArrow, m_bProcessLeftArrow;

	CMenuBarFrameHook m_hookFrame;
	CMenuBarFrameHook m_hookMDIClient;

	HWND	m_hWndMDIClient;
	static BOOL	m_bMDIApp;	// this is MDI application?

	BOOL m_bIcon;
	CMenuControl* m_pMenuControl;
	CMenuIcon*    m_pMenuIcon;
	BOOL m_bMDIMaximized;
	HMENU m_hWindowMenu;
	HWND m_hWndActiveChild;

	friend class CMenuBarFrameHook;
};

class CMenuIcon : public CMenuItem  
{
// Constructors
public:
	CMenuIcon(CWnd* pWnd);

// Operations
	void OnActivateChildWnd(HWND hWndChild);

// Overidables
	virtual void Update(CDC*);
	virtual void Layout(CPoint, BOOL bHorz);
	virtual void TrackPopup(CWnd* pWnd);
	virtual BOOL CanTrack() { return TRUE; }
	virtual BOOL CanWrap() { return FALSE; }
	virtual void Validate(BOOL bValid) { m_bValid = bValid; }
	virtual BOOL IsValid() { return m_bValid; }
	virtual BOOL GetAccessKey(TCHAR&) { return FALSE; }

// Implementation
	virtual ~CMenuIcon();

private:
	HICON	m_hDocIcon;
	HMENU	m_hSysMenu;
	BOOL	m_bValid;
};

class CMenuControl : public CMenuItem  
{
// Constructors
public:
	void ForceDrawControl(CDC* pDC);
	BOOL OnMouseMsg(UINT msg, UINT nFlags, CPoint pt);
	CMenuControl(CControlBar* pBar);

// Operations
	void DelayLayoutAndDraw(CDC* pDC, CSize sizeBar);

// Overidables
	virtual void Update(CDC*);
	virtual void Layout(CPoint, BOOL bHorz);
	virtual void TrackPopup(CWnd* pWnd) { ASSERT(TRUE); }
	virtual BOOL CanTrack() { return FALSE; }
	virtual BOOL CanWrap() { return TRUE; }
	virtual void Validate(BOOL bValid) { m_bValid = bValid; }
	virtual BOOL IsValid() { return m_bValid; }
	virtual BOOL GetAccessKey(TCHAR&) { return FALSE; }

// Implementation
	virtual ~CMenuControl();

private:
	// Implementation helper
	void DrawControl(CDC* pDC, int nIndex, BOOL bDown);
	int HitTest(CPoint point);

	CControlBar* m_pBar;
	BOOL	m_bValid;
//	UINT	m_nState;
	CRect	m_arrCaption[3];
	BOOL	m_bDown;
	int		m_nTracking;
};

class CMenuButton : public CMenuItem  
{
// Constructors
public:
	CMenuButton(HMENU hMenu, int nIndex);
	static BOOL InitCommonResource();

// Operations
// Overidables
	virtual void Update(CDC*);
	virtual void Layout(CPoint, BOOL bHorz);
	virtual void TrackPopup(CWnd* pWnd);
	virtual BOOL CanTrack() { return TRUE; }
	virtual BOOL CanWrap() { return TRUE; }
	virtual void Validate(BOOL) { /*always valid*/}
	virtual BOOL IsValid() { return TRUE; }
	virtual BOOL GetAccessKey(TCHAR& cKey) { cKey = m_cAccessKey; return TRUE; }

// Implementation
	virtual ~CMenuButton();
	
	// Implementation helper
	// used on Update
	void DrawHot(CDC* pDC);
	void DrawSelect(CDC* pDC);
	void DrawNone(CDC* pDC);
	void DrawHorzText(CDC* pDC, CPoint ptOffset = CPoint(0, 0));
	void DrawVertText(CDC* pDC, CPoint ptOffset = CPoint(0, 0));

	// used on constructing
	void InitButtonStringAndSubMenuHandle(HMENU hMenu, int nIndex);
	void InitHorizontalButtonSize();
	void InitAccessKeyAndVerticalLinePoint();

private:
	CString		m_strBtn;		// string on button
	TCHAR		m_cAccessKey;	// access key
	CPoint		m_ptLineFrom, m_ptLineTo;	// vertical line point on button
	HMENU		m_hSubMenu;		// handle to sub menu
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio 

#endif // !defined(AFX_MENUBARCTRL_H__AF01DCA8_266A_11D3_8FA2_444553540000__INCLUDED_)
