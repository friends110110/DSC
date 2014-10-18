// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__C433678F_0A4F_4840_87F2_9DA3C23064CD__INCLUDED_)
#define AFX_MAINFRM_H__C433678F_0A4F_4840_87F2_9DA3C23064CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define RV_MESS WM_USER+0x11

class CDSCView;
#include "BottomEditView.h"
#include "LeftView.h"
#include "optional.h"

#include "MyToolBar.h"
#include "MenuBar.h"
#include "DateBase.h" //JC2

#include "RemoteDtuSettingDlg.h"


class CMainFrame : public CFrameWnd
{
public:	
	

protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
    void SetToolBarStyle(void);
	void SetStatusBarStyle(void);
	void init_option(LPOPTION pOption);
	void GetPara(int);   //tabctrl
	int  CompareDateTimeAndNow(char *);
	void AddToSysMenu(void);

// Attributes
protected:
	CSplitterWnd m_wndSplitter;
	CSplitterWnd m_wndSplitter2;
	//CWinThread  *m_thread;

	bool         m_bServerRunning;
	bool         m_bAnswer;

	char         m_userid[12];
	BOOL         m_viewword;

	int			 m_nDateCount; //JC 收到数据的总数
	CDateBase	 m_DataBase;   //JC2 数据库对象
public:
// Operations
	BOOL         m_IsAdduserDlg;
	OPTION       m_option;
	CString      m_dbName;
	CString      m_userName;
	CString      m_password;
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
	CDSCView* GetRightPane();
	CBottomEditView* GetBottompane();
	CLeftView* GetLeftPane();
	void AddTextToEditView(char *,bool newline=true);
	void AddTextToEditView(int,bool newline=true);
	void RefreshUserTable(int flag=0);
	void PrintData(char *, int);
	void SetUserId(char *);
	BOOL IsShowToolBarWord();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	//CToolBar    m_wndToolBar;
	CMyToolBar  m_wndToolBar;
	CMenuBar    m_wndMenuBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMiclear();
	afx_msg void OnMioffline();
	afx_msg void OnMirefreshtable();
	afx_msg void OnMisenddata();
	afx_msg void OnMistartservice();
	afx_msg void OnMistopservice();
	afx_msg void OnQuit();
	afx_msg void OnClose();
	afx_msg void ProcessRvData(WPARAM wparam, LPARAM lparam);
	afx_msg void OnMianswer();
	afx_msg void OnMishowdata();
	afx_msg void OnShowmenu();
	afx_msg void OnHelpcontent();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUpdateMistartservice(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMistopservice(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMianswer(CCmdUI* pCmdUI);
	afx_msg void OnMiserversetting();
	afx_msg void OnUpdateShowmenu(CCmdUI* pCmdUI);
	afx_msg void OnMiofflineall();
	afx_msg void OnMiviewword();
	afx_msg void OnUpdateMiviewword(CCmdUI* pCmdUI);
	afx_msg void OnUserinfoin();
	afx_msg void OnUserinfoout();
	afx_msg void OnMenusmsstart();
	afx_msg void OnMenusmsexit();
	afx_msg void OnAdduser();
	afx_msg void OnDeleteUser();
	afx_msg void OnUpdateAdduser(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDeleteUser(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUserinfoin(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUserinfoout(CCmdUI* pCmdUI);
	afx_msg void OnDisconnectPpp();
	afx_msg void OnUpdateDisconnectPpp(CCmdUI* pCmdUI);
	afx_msg void OnStopToSend();
	afx_msg void OnUpdateStopToSend(CCmdUI* pCmdUI);
	afx_msg void OnStartToSend();
	afx_msg void OnUpdateStartToSend(CCmdUI* pCmdUI);
	afx_msg void OnDiscardData();
	afx_msg void OnUpdateDiscardData(CCmdUI* pCmdUI);
	afx_msg void OnDturemotesetting();
	afx_msg void OnSqlsetting();
	//}}AFX_MSG
	afx_msg void OnUpdateViewStyles(CCmdUI* pCmdUI);
	afx_msg void OnViewStyle(UINT nCommandID);

	DECLARE_MESSAGE_MAP()


};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__C433678F_0A4F_4840_87F2_9DA3C23064CD__INCLUDED_)
