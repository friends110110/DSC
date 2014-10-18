// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "DSC.h"
#include "time.h"
#include "SettingDlg.h"
#include "senddatadlg.h"
#include "WordThread.h"
#include "AddUserDlg1.h"
#include "SqlSettingDlg.h"

#include "MainFrm.h"
#include "DSCView.h"
#include "wcomm_dll.h"

#include "winsock2.h"
#include "GPRS_SMM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NEWMENUITEM WM_USER+0x13
/*
int g_nCount = 0;
UINT pfnReadData(LPVOID pParam)
{
	char mess[512];
	data_record dr;
	CMainFrame* pt = (CMainFrame *)pParam;
	g_nCount = 0;
	SetWorkMode(1);   //0-use Window Messge, nonblock   1-block mode
	start_gprs_server((HWND)0x01,0x01,5003,mess);
	pt->AddTextToEditView(mess);
	while(g_nCount<10)//for (;;)
	{
		if (0 == do_read_proc(&dr,mess,true))
		{
			if (dr.m_data_len>=0)
				pt->AddTextToEditView(dr.m_userid);
			if (dr.m_data_len>0)
				pt->AddTextToEditView(dr.m_data_buf);
		}
        Sleep(20);
	}
	stop_gprs_server(mess);
	pt->AddTextToEditView(mess);
	return 1;
}
*/
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_MICLEAR, OnMiclear)
	ON_COMMAND(ID_MIOFFLINE, OnMioffline)
	ON_COMMAND(ID_MIREFRESHTABLE, OnMirefreshtable)
	ON_COMMAND(ID_MISENDDATA, OnMisenddata)
	ON_COMMAND(ID_MISTARTSERVICE, OnMistartservice)
	ON_COMMAND(ID_MISTOPSERVICE, OnMistopservice)
	ON_COMMAND(ID_QUIT, OnQuit)
	ON_WM_CLOSE()
	ON_MESSAGE(RV_MESS,ProcessRvData)
	ON_COMMAND(ID_MIANSWER, OnMianswer)
	ON_COMMAND(ID_MISHOWDATA, OnMishowdata)
	ON_COMMAND(ID_SHOWMENU, OnShowmenu)
	ON_COMMAND(ID_HELPCONTENT, OnHelpcontent)
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_MISTARTSERVICE, OnUpdateMistartservice)
	ON_UPDATE_COMMAND_UI(ID_MIOFFLINE, OnUpdateMistopservice)
	ON_UPDATE_COMMAND_UI(ID_MIANSWER, OnUpdateMianswer)
	ON_COMMAND(ID_MISERVERSETTING, OnMiserversetting)
	ON_UPDATE_COMMAND_UI(ID_SHOWMENU, OnUpdateShowmenu)
	ON_COMMAND(ID_MIOFFLINEALL, OnMiofflineall)
	ON_COMMAND(ID_MIVIEWWORD, OnMiviewword)
	ON_UPDATE_COMMAND_UI(ID_MIVIEWWORD, OnUpdateMiviewword)
	ON_COMMAND(ID_USERINFOIN, OnUserinfoin)
	ON_COMMAND(ID_USERINFOOUT, OnUserinfoout)
	ON_COMMAND(ID_MENUSMSSTART, OnMenusmsstart)
	ON_COMMAND(ID_MENUSMSEXIT, OnMenusmsexit)
	ON_COMMAND(ID_Adduser, OnAdduser)
	ON_COMMAND(ID_DELETE_USER, OnDeleteUser)
	ON_UPDATE_COMMAND_UI(ID_Adduser, OnUpdateAdduser)
	ON_UPDATE_COMMAND_UI(ID_DELETE_USER, OnUpdateDeleteUser)
	ON_UPDATE_COMMAND_UI(ID_USERINFOIN, OnUpdateUserinfoin)
	ON_UPDATE_COMMAND_UI(ID_USERINFOOUT, OnUpdateUserinfoout)
	ON_COMMAND(ID_DISCONNECT_PPP, OnDisconnectPpp)
	ON_UPDATE_COMMAND_UI(ID_DISCONNECT_PPP, OnUpdateDisconnectPpp)
	ON_COMMAND(ID_STOP_TO_SEND, OnStopToSend)
	ON_UPDATE_COMMAND_UI(ID_STOP_TO_SEND, OnUpdateStopToSend)
	ON_COMMAND(ID_START_TO_SEND, OnStartToSend)
	ON_UPDATE_COMMAND_UI(ID_START_TO_SEND, OnUpdateStartToSend)
	ON_COMMAND(ID_DISCARD_DATA, OnDiscardData)
	ON_UPDATE_COMMAND_UI(ID_DISCARD_DATA, OnUpdateDiscardData)
	ON_COMMAND(IDM_DTUREMOTESETTING, OnDturemotesetting)
	ON_UPDATE_COMMAND_UI(ID_MIREFRESHTABLE, OnUpdateMistopservice)
	ON_UPDATE_COMMAND_UI(ID_MISENDDATA, OnUpdateMistopservice)
	ON_UPDATE_COMMAND_UI(ID_MISTOPSERVICE, OnUpdateMistopservice)
	ON_UPDATE_COMMAND_UI(ID_MIOFFLINEALL, OnUpdateMistopservice)
	ON_COMMAND(IDM_SQLSETTING, OnSqlsetting)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnUpdateViewStyles)
	ON_COMMAND_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnViewStyle)
	ON_MESSAGE(ID_MISENDDATA,OnMisenddata)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR, 
	ID_SEPARATOR, 
    ID_SEPARATOR 
	// status line indicator
	//ID_INDICATOR_CAPS,
	//ID_INDICATOR_NUM,
	//ID_INDICATOR_SCRL,
	//0x1234
};



/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CRemoteDtuSettingDlg m_remoteDtuSettingDlg;

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_bAutoMenuEnable=false;
	this->m_bServerRunning=false;	
    this->m_bAnswer=FALSE;
	this->m_viewword=TRUE;
	memset(this->m_userid,0x00,12);
	this->m_IsAdduserDlg =FALSE;

	this->m_nDateCount=0;//JC

	init_option(&m_option);
	this->m_dbName="dscsystem";
	this->m_userName="root";
	this->m_password="123456";
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndMenuBar.Create(this) ||
		!m_wndMenuBar.LoadMenuBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // faile to create
	}

	if (!m_wndToolBar.CreateEx(this, 
		TBSTYLE_FLAT | TBSTYLE_TRANSPARENT | TBSTYLE_LIST,
		WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY 
		| CBRS_SIZE_DYNAMIC
		)
		|| !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndMenuBar);
	DockControlBar(&m_wndToolBar);

	SetToolBarStyle();
	SetStatusBarStyle();
	AddToSysMenu();	

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	// create splitter window
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;
	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CLeftView), CSize(130, 100), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

	if (!m_wndSplitter2.CreateStatic(&m_wndSplitter,2,1,WS_VISIBLE | WS_CHILD | WS_BORDER,m_wndSplitter.IdFromRowCol(0,1)))
		return FALSE;
	if (!m_wndSplitter2.CreateView(0, 0, RUNTIME_CLASS(CDSCView), CSize(100, 200), pContext) ||
		!m_wndSplitter2.CreateView(1, 0, RUNTIME_CLASS(CBottomEditView), CSize(100, 100), pContext))
	{
		m_wndSplitter2.DestroyWindow();
		return FALSE;
	}
	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

CDSCView* CMainFrame::GetRightPane()
{
	CWnd* pWnd = m_wndSplitter2.GetPane(0, 0);
	CDSCView* pView = DYNAMIC_DOWNCAST(CDSCView, pWnd);
	return pView;
}

CLeftView* CMainFrame::GetLeftPane()
{
	CWnd* pWnd = m_wndSplitter.GetPane(0, 0);
	CLeftView* pView = DYNAMIC_DOWNCAST(CLeftView, pWnd);
	return pView;
}

CBottomEditView* CMainFrame::GetBottompane()
{
	CWnd* pWnd = m_wndSplitter2.GetPane(1, 0);
	CBottomEditView* pView = DYNAMIC_DOWNCAST(CBottomEditView, pWnd);
	return pView;
}

void CMainFrame::OnUpdateViewStyles(CCmdUI* pCmdUI)
{
	// TODO: customize or extend this code to handle choices on the
	// View menu.

	CDSCView* pView = GetRightPane(); 

	// if the right-hand pane hasn't been created or isn't a view,
	// disable commands in our range

	if (pView == NULL)
		pCmdUI->Enable(FALSE);
	else
	{
		DWORD dwStyle = pView->GetStyle() & LVS_TYPEMASK;

		// if the command is ID_VIEW_LINEUP, only enable command
		// when we're in LVS_ICON or LVS_SMALLICON mode

		if (pCmdUI->m_nID == ID_VIEW_LINEUP)
		{
			if (dwStyle == LVS_ICON || dwStyle == LVS_SMALLICON)
				pCmdUI->Enable();
			else
				pCmdUI->Enable(FALSE);
		}
		else
		{
			// otherwise, use dots to reflect the style of the view
			pCmdUI->Enable();
			BOOL bChecked = FALSE;

			switch (pCmdUI->m_nID)
			{
			case ID_VIEW_DETAILS:
				bChecked = (dwStyle == LVS_REPORT);
				break;

			case ID_VIEW_SMALLICON:
				bChecked = (dwStyle == LVS_SMALLICON);
				break;

			case ID_VIEW_LARGEICON:
				bChecked = (dwStyle == LVS_ICON);
				break;

			case ID_VIEW_LIST:
				bChecked = (dwStyle == LVS_LIST);
				break;

			default:
				bChecked = FALSE;
				break;
			}

			pCmdUI->SetRadio(bChecked ? 1 : 0);
		}
	}
}


void CMainFrame::OnViewStyle(UINT nCommandID)
{
	// TODO: customize or extend this code to handle choices on the
	// View menu.
	CDSCView* pView = GetRightPane();

	// if the right-hand pane has been created and is a CDSCView,
	// process the menu commands...
	if (pView != NULL)
	{
		DWORD dwStyle = -1;

		switch (nCommandID)
		{
		case ID_VIEW_LINEUP:
			{
				// ask the list control to snap to grid
				CListCtrl& refListCtrl = pView->GetListCtrl();
				refListCtrl.Arrange(LVA_SNAPTOGRID);
			}
			break;

		// other commands change the style on the list control
		case ID_VIEW_DETAILS:
			dwStyle = LVS_REPORT;
			break;

		case ID_VIEW_SMALLICON:
			dwStyle = LVS_SMALLICON;
			break;

		case ID_VIEW_LARGEICON:
			dwStyle = LVS_ICON;
			break;

		case ID_VIEW_LIST:
			dwStyle = LVS_LIST;
			break;
		}

		// change the style; window will repaint automatically
		if (dwStyle != -1)
			pView->ModifyStyle(LVS_TYPEMASK, dwStyle);
	}
}

void CMainFrame::SetToolBarStyle()
{
	CImageList img;
	m_wndToolBar.GetToolBarCtrl().SetButtonWidth(40, 100);
	m_wndToolBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
    //m_wndToolBar.SetButtonStyle(8,TBSTYLE_CHECK);
	m_wndToolBar.SetButtonStyle(4,m_wndToolBar.GetButtonStyle(4) 
		                           | TBSTYLE_DROPDOWN);

	img.Create(IDB_HOTTOOLBAR, 32, 0, RGB(255, 0, 255));
	m_wndToolBar.GetToolBarCtrl().SetHotImageList(&img);
	img.Detach();
	img.Create(IDB_COLDTOOLBAR, 32, 0, RGB(255, 0, 255));
	m_wndToolBar.GetToolBarCtrl().SetImageList(&img);
	img.Detach();
	
	for(int i = 0; i < m_wndToolBar.GetCount(); i++) 
	{ 
		UINT id = m_wndToolBar.GetItemID(i); 
		CString s;
	    if (!s.LoadString(id)) 
			continue;
	    int j = s.Find(_T('\n')); 
	    if(j < 0) continue;
	    s = s.Right(s.GetLength() - j - 1); 
	    m_wndToolBar.SetButtonText(i,s); 
	}

	CRect rect; 
	m_wndToolBar.GetItemRect(0,&rect); 
	m_wndToolBar.SetSizes(rect.Size(),CSize(32,32));
}

//清除日志
void CMainFrame::OnMiclear() 
{
	// TODO: Add your command handler code here
	CEdit &ce=this->GetBottompane()->GetEditCtrl();
	ce.SetWindowText("");
}

void CMainFrame::OnMioffline() 
{
	char buf[64];

	if (11==strlen(m_userid))
	{
		sprintf(buf,"确认使  %s 下线吗？",this->m_userid);
	    if (IDYES==MessageBox(buf,"确认",MB_YESNO | MB_ICONINFORMATION))
		{
			//do_close_one_user((unsigned char *)m_userid,NULL);
			do_close_one_user2((unsigned char *)(LPCTSTR)m_userid,NULL);

			this->RefreshUserTable();
		}
	}

}

void CMainFrame::OnMirefreshtable() 
{
	this->RefreshUserTable();
}

void CMainFrame::OnMisenddata() 
{
	CTreeCtrl &tc=this->GetLeftPane()->GetTreeCtrl();

	/*
	CSendDataDlg sdd(NULL,&tc);
	sdd.SetUserId(this->m_userid);	
	sdd.DoModal();
	*/

	//非模态对话框
	CSendDataDlg* sdd = new CSendDataDlg(NULL,&tc);
	sdd->SetUserId(this->m_userid);
	sdd->Create(IDD_SENDDATADLG,this);
	sdd->ShowWindow(SW_SHOW);
/*
	CAddUserDlg * adduserdlg=new CAddUserDlg;
	adduserdlg->Create (IDD_AddUser,this);
	adduserdlg->ShowWindow (SW_SHOW);
	*/

}

void CMainFrame::OnMistartservice() 
{
	char mess[512];
	int iResult;

/*//-----------------------------
	HINSTANCE hInst = NULL;
	hInst =LoadLibrary("wcomm_dll.dll");
	typedef int (*ADDPROC)(int a);
	ADDPROC addproc=(ADDPROC)GetProcAddress(hInst,"SelectProtocol");
	if(NULL == hInst)
	{
		MessageBox("加载DLL失败！");
		return;
	}

	addproc(this->m_option.m_iProtocalType);
//-----------------------------------------*/

	//设置协议
	//SelectProtocol(this->m_option.m_iProtocalType);

//	SelectProtocol(0);
	//选择消息模式
//	SetWorkMode(2);
	//选择UDP协议
//	SelectProtocol(1);



	//AddFilterIP(inet_addr("192.168.0.243"));
	//SetCustomIP(inet_addr("192.168.0.1"));	
	
	/*iResult=start_gprs_server(this->m_hWnd,
		                      RV_MESS,
							  m_option.m_iServerPort,
							  mess);*/
	iResult=start_net_service(this->m_hWnd,			//接收消息的窗口
		                      RV_MESS,				//消息类型
							  m_option.m_iServerPort,//端口号，init_option() 中已经设置
							  mess);				//执行信息缓存，可以设置为NULL

	//启动成功
	if (iResult>=0) //成功返回0，失败返回-1
	{	   
		//设置有自动切换，启动定时器
		if ((m_option.m_bSysAutoPoll) && (m_option.m_iPollTimeInterval>0) && (m_option.m_iOffLineTime>0))
            this->SetTimer(1,(m_option.m_iPollTimeInterval)*1000,NULL);

	    m_bServerRunning=true; //标志服务已经启动
		
		//状态栏信息		//JC2
		this->m_wndStatusBar.SetPaneText(1,"服务已启动",TRUE);

		//连接数据库		//JC2
		if(!this->m_DataBase.OpenDataBase(this->m_dbName,this->m_userName,this->m_password))
		{
			this->m_wndStatusBar.SetPaneText(1,"数据库打开失败",TRUE);
		}
	}
	
	//将启动服务的信息显示到View中
	AddTextToEditView(mess,true);

	//AfxBeginThread(RUNTIME_CLASS(CWordThread));
}
void CMainFrame::OnMistopservice() 
{
	// TODO: Add your command handler code here
	char mess[512];

	if (m_option.m_bSysAutoPoll)
		this->KillTimer(1);

	do_close_all_user(mess);
	//this->AddTextToEditView(mess);
	//stop_gprs_server(mess);
	stop_net_service(mess);
	this->AddTextToEditView(mess,true);
	this->m_bServerRunning=false;

	this->GetRightPane()->ClearAllItem();
	this->GetLeftPane()->ClearAllItem();

	//状态栏信息		//JC2
	this->m_wndStatusBar.SetPaneText(1,"服务已停止",TRUE);

	//关闭数据库		//JC2
	if(!this->m_DataBase.CloseDataBase())
	{
		this->m_wndStatusBar.SetPaneText(1,"数据库关闭失败",TRUE);
	}
}

void CMainFrame::OnQuit() 
{
	// TODO: Add your command handler code here
	this->OnClose();	
}

void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if (IDYES==MessageBox("确定要退出吗？","退出",MB_YESNO | MB_ICONINFORMATION))
	{
		if (m_bServerRunning)
		{
			this->OnMistopservice();
		}

		CFrameWnd::OnClose();
	}
}

void CMainFrame::ProcessRvData(WPARAM wparam, LPARAM lparam)
{
	data_record dr;
	static char buf[2048];

	if (do_read_proc(&dr,NULL,m_bAnswer)>=0)
	{
		//AfxMessageBox("收到数据");//JC2
		if (dr.m_data_len>0)
		{
			//如果是注册信息包，则不显示
			if (0x01==dr.m_data_type)
			{
				return;
			}

			//AfxMessageBox("数据包");
			this->m_nDateCount++;				//JC 显示收到的数据条数 
			if(this->m_nDateCount > 10000000) 
			{
				this->m_nDateCount =0 ;
				sprintf(buf,"数据包总数已重置为：0");
				this->m_wndStatusBar.SetPaneText(1,buf,TRUE);
				//this->m_wndStatusBar.SetPaneText(1,buf,TRUE);
			}
			else
			{
				sprintf(buf,"收到数据包总数为：%d",this->m_nDateCount);
				this->m_wndStatusBar.SetPaneText(1,buf,TRUE);
			}

			//写入数据库  //JC2
			if(this->m_DataBase.isConnected())
			{
				CTime tmCur = CTime::GetCurrentTime();              //当前时间
				CString szCurTime;
				szCurTime.Format("%d-%02d-%02d %02d:%02d:%02d",tmCur.GetYear(),tmCur.GetMonth(),tmCur.GetDay(),tmCur.GetHour(),tmCur.GetMinute(),tmCur.GetSecond());
				CString str_UserId;
				str_UserId.Format("%s",dr.m_userid);

				this->m_DataBase.InsertData(str_UserId,szCurTime,dr.m_data_buf);
			}

			//whether show head or not
			if (m_option.m_bShowHead)				//JC2 显示中文
			{
				//sprintf(buf,"\r\nuserid:%s,time:%s,len:%d",
				//	dr.m_userid,dr.m_recv_date,dr.m_data_len);

				sprintf(buf,"\r\nDTU号码:%s,时间:%s,数据长度:%d",
					dr.m_userid,dr.m_recv_date,dr.m_data_len);

				this->AddTextToEditView(buf);
			}

			//Hex or not to show data
			if (m_option.m_bHexShow)
			{
				this->PrintData(dr.m_data_buf, dr.m_data_len);
			}
			else
			{
				this->AddTextToEditView(dr.m_data_buf);
			}
		}
		else //data_len=0,it means logon or logout
		{
			//AfxMessageBox("登陆信息包");
			bool show = true;//JC

			this->RefreshUserTable();

			if (0x01==dr.m_data_type)
				sprintf(buf,"%s  登录",dr.m_userid);
			else if (0x02==dr.m_data_type)
				sprintf(buf,"%s  注销",dr.m_userid);
			else if (0x04==dr.m_data_type)
			{
				AfxMessageBox("参数配置出错");
				sprintf(buf,"%s  参数设置成功",dr.m_userid);
			}
			else if (0x0d==dr.m_data_type)
			{
				AfxMessageBox("参数设置成功");
				sprintf(buf,"%s  参数设置成功",dr.m_userid);
			}
			else if (0x0b==dr.m_data_type)
	//wf add
			{
				AfxMessageBox("命令发送成功 开始查询参数");
				m_remoteDtuSettingDlg.doQueryDtuParam(dr.m_userid);
				sprintf(buf,"%s  参数查询成功",dr.m_userid);
			}
			else if (0x06==dr.m_data_type)
			{
				sprintf(buf,"%s  断开PPP连接成功",dr.m_userid);
				delete_one_user((unsigned char *)dr.m_userid,NULL);
				this->RefreshUserTable();
			}
			else if (0x07==dr.m_data_type)
				sprintf(buf,"%s  停止向DSC发送数据",dr.m_userid);
			else if (0x08==dr.m_data_type)
				sprintf(buf,"%s  允许向DSC发送数据",dr.m_userid);
			else if (0x0A==dr.m_data_type)
				sprintf(buf,"%s   丢弃用户数据",dr.m_userid);
			else
				show = false;//JC
				//sprintf(buf,"%s",dr.m_userid);

			if(show)
				this->m_wndStatusBar.SetPaneText(1,buf,TRUE);
		}
	}
}

void CMainFrame::AddTextToEditView(char * szContent,bool newline)
{	
	if (szContent)
	{
	    CString text;
	    CEdit &ce=this->GetBottompane()->GetEditCtrl();

	    if (ce.GetLineCount()>100)
		    ce.SetWindowText("");

		ce.GetWindowText(text);
		if (newline)
			ce.SetWindowText(text+"\r\n"+szContent);
		else
			ce.SetWindowText(text+szContent);

		ce.LineScroll(ce.GetLineCount());
	}	
}

void CMainFrame::AddTextToEditView(int iContent,bool newline)
{
	CString text;
	char buf[32];

	sprintf(buf,"%d",iContent);
    CEdit &ce=this->GetBottompane()->GetEditCtrl();

	if (ce.GetLineCount()>100)
		ce.SetWindowText("");

	ce.GetWindowText(text);
	if (newline)
		ce.SetWindowText(text+"\r\n"+buf);
	else
		ce.SetWindowText(text+buf);

	ce.LineScroll(ce.GetLineCount());
}

//刷新用户列表
void CMainFrame::RefreshUserTable(int flag)
{
	int i,iMaxDTUAmount;
	user_info ui;
	bool   bShow;
	time_t t_now,t_update;

	this->GetRightPane()->ClearAllItem();
	this->GetLeftPane()->ClearAllItem();

	iMaxDTUAmount=get_max_user_amount();
	for (i=0;i<iMaxDTUAmount;i++)
	{
		ui.m_status=0;
		get_user_at(i,&ui);
		if (1==ui.m_status) // 1 在线；0 不在线
		{
			bShow=true; //是否显示该用户到列表，默认是显示，如果判断出离线了，则不显示

			//flag=1 means refresh usertable , 0 means not
			if ((m_option.m_bSysAutoPoll) && (1==flag))
			{
				/* if type of update time is ulong then use */
				/* the following code                       */

				t_now=time(NULL);
				t_update=*((time_t *)ui.m_update_time);
				//判断当前时间和上次更新时间之差，如果大于离线的阈值，表示该用户已经离线了，则删除
				if ((t_now-t_update)>=m_option.m_iOffLineTime)
				{
					do_close_one_user((unsigned char *)ui.m_userid,NULL);
					bShow=false;
				}                

				/* if type of update time is char * then use */
				/* following code                            */
				/*
				if (this->CompareDateTimeAndNow(ui.m_update_time)>=m_option.m_iOffLineTime)
				{
					do_close_one_user((unsigned char *)ui.m_userid,NULL);
					bShow=false;
				}
				*/
			}

			if (bShow)
			{
				//add to ListView of right top
				this->GetRightPane()->InsertUserItem(
					ui.m_userid,ui.m_logon_date,
					ui.m_update_time,ui.m_local_addr,
					ui.m_local_port,ui.m_sin_addr,ui.m_sin_port);

				//add to TreeView of left
				this->GetLeftPane()->InsertUserItem(ui.m_userid);
			}
		}
	}

	this->GetLeftPane()->GetTreeCtrl().Expand(
		this->GetLeftPane()->GetTreeCtrl().GetFirstVisibleItem(),TVE_EXPAND);
}

void CMainFrame::OnMianswer() 
{
    m_bAnswer=!m_bAnswer;	
}

void CMainFrame::OnMiserversetting() 
{
	this->GetPara(0);
}

void CMainFrame::OnMishowdata() 
{
	this->GetPara(1);
}

void CMainFrame::OnShowmenu() 
{
    ShowControlBar(&m_wndMenuBar,!m_wndMenuBar.IsWindowVisible(),FALSE);
	m_wndToolBar.SetButtonStyle(6,TBSTYLE_BUTTON);
}

void CMainFrame::OnHelpcontent() 
{
	MessageBox("杭州诺朗科技有限公司");
}

void CMainFrame::SetStatusBarStyle()
{
	m_wndStatusBar.SetPaneInfo(0,ID_SEPARATOR,SBPS_STRETCH,100);  
	m_wndStatusBar.SetPaneText(1,"",TRUE);		
	//m_wndStatusBar.SetPaneText(2,"GPRS VC DEMO 3.0   宏电技术",TRUE);
	//m_wndStatusBar.SetPaneText(2,"DSC API演示程序 2.1 版(VC++6.0)",TRUE);
	m_wndStatusBar.SetPaneText(2,"DSC 中心程序1.0 版 (c) 杭州诺朗科技有限公司",TRUE);

}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (1==nIDEvent)
		this->RefreshUserTable(1);

	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::PrintData(char *szData, int datalen)
{
	char buf[128];

	int i,j,len,totallen;

	totallen=datalen;//strlen(szData);  原来的错误
	for (i=0;i<totallen;i+=16)
	{
		len=(totallen-i)>16?16:(totallen-i);
		sprintf(buf,"%04X  :  ",i);
		for (j=0;j<len;j++)
		{
			sprintf(buf+9+j*3,"%02X ",szData[i+j]&0xFF);
			buf[64+j]=szData[i+j];
		}
        memset(buf+len*3+9,' ',64-len*3-9);
		buf[64+len]='\0';

		this->AddTextToEditView(buf);
	}
}

void CMainFrame::OnUpdateMistartservice(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!m_bServerRunning);
}

void CMainFrame::OnUpdateMistopservice(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bServerRunning);
}

void CMainFrame::OnUpdateMianswer(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bAnswer);	
}

void CMainFrame::init_option(LPOPTION pOption)
{
	//通过读取config.ini文件区配置
	int port = GetPrivateProfileInt("Service", "ServerPort", 7003, "./config.ini");

	/*
	CString str;
	str.Format("%d",port);
	AfxMessageBox(str);
	*/

	pOption->m_iServerPort=port;   //端口


	pOption->m_bSaveToLog=FALSE;	//是否保存到日志
	pOption->m_iLogItemCount=200;	//
	pOption->m_bShowDataHead=TRUE;	//是否显示数据头
//	pOption->m_iServerPort=7003;   //端口
	pOption->m_bSysAutoPoll=TRUE;	//（自动切换,GPRS双模）
	pOption->m_iPollTimeInterval=30;//
	pOption->m_iOffLineTime=120;	//
	pOption->m_bShowHead=TRUE;		//
	pOption->m_bHexShow=TRUE;		//
	pOption->m_iProtocalType = 0;	// 默认是0: UDP  ;(1 表示TCP)
}

void CMainFrame::GetPara(int tabindex)
{
	CSettingDlg sdlg(NULL,tabindex,!m_bServerRunning);

	sdlg.m_bSaveToLog=m_option.m_bSaveToLog;
	sdlg.m_iLogItemCount.Format("%d",m_option.m_iLogItemCount);
	sdlg.m_iOffLineTime.Format("%d",m_option.m_iOffLineTime);
	sdlg.m_iPollTime.Format("%d",m_option.m_iPollTimeInterval);
	sdlg.m_iServerPort.Format("%d",m_option.m_iServerPort);
	sdlg.m_bSysAutoPoll=m_option.m_bSysAutoPoll;
	sdlg.m_bShowHead=m_option.m_bShowHead;
	sdlg.m_bHexShow=m_option.m_bHexShow;
	sdlg.m_udp = m_option.m_iProtocalType; //JC3

	if (IDOK==sdlg.DoModal())
	{
		m_option.m_bSaveToLog=sdlg.m_bSaveToLog;
		m_option.m_iLogItemCount=atoi(sdlg.m_iLogItemCount.GetBuffer(0));
		m_option.m_iOffLineTime=atoi(sdlg.m_iOffLineTime.GetBuffer(0));
		m_option.m_iPollTimeInterval=atoi(sdlg.m_iPollTime.GetBuffer(0));
		m_option.m_iServerPort=atoi(sdlg.m_iServerPort.GetBuffer(0));
		m_option.m_bSysAutoPoll=sdlg.m_bSysAutoPoll;
	    m_option.m_bShowHead=sdlg.m_bShowHead;
	    m_option.m_bHexShow=sdlg.m_bHexShow;
		m_option.m_iProtocalType=sdlg.m_udp; //JC3

		//将serverport 写入config.ini
		CString str ;
		str.Format("%d",m_option.m_iServerPort);
		WritePrivateProfileString("Service", "ServerPort",str, "./config.ini");//TODO:

/*		if(m_option.m_iProtocalType == 0)
			AfxMessageBox("0");
		else
			AfxMessageBox("1");
*/

	}
}

void CMainFrame::SetUserId(char *szUserId)
{
	strncpy(this->m_userid,szUserId,11);
	this->m_userid[11]='\0';
}

void CMainFrame::OnUpdateShowmenu(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck(m_wndMenuBar.IsWindowVisible());
}


void CMainFrame::OnMiofflineall() 
{
    do_close_all_user2(NULL);
	this->RefreshUserTable();
}


void CMainFrame::OnMiviewword() 
{
	// TODO: Add your command handler code here
    if (!IsWindowVisible()) 
		return;

	CRect rect; 

	m_viewword=!m_viewword;
    m_wndToolBar.SetShowWord(m_viewword);
    m_wndToolBar.GetItemRect(0,&rect); 
	if (m_viewword)
	{
	    m_wndToolBar.SetSizes(CSize(SHOWTEXTLEN,rect.Size().cy),CSize(32,32));
	}
	else
	{
        m_wndToolBar.SetSizes(CSize(NOTEXTLEN,rect.Size().cy),CSize(32,32));
	}
	ShowControlBar(&m_wndToolBar,TRUE,FALSE);
}

void CMainFrame::OnUpdateMiviewword(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_wndToolBar.IsWindowVisible());
    pCmdUI->SetCheck(m_viewword);
}

BOOL CMainFrame::IsShowToolBarWord()
{
	return m_viewword;
}

int CMainFrame::CompareDateTimeAndNow(char * szDateTime)
{
    int nYear,nMonth,nDay,nHour,nMin,nSec;
	char temp[20];

    memset(temp,0,20);
	strncpy(temp,szDateTime,4);
	nYear = atoi(temp);

	memset(temp,0,20);
	strncpy(temp,&(szDateTime[5]),2);
	nMonth = atoi(temp);

	memset(temp,0,20);
	strncpy(temp,&(szDateTime[8]),2);
	nDay = atoi(temp);

	memset(temp,0,20);
	strncpy(temp,&(szDateTime[11]),2);
	nHour = atoi(temp);

	memset(temp,0,20);
	strncpy(temp,&(szDateTime[14]),2);
	nMin = atoi(temp);

	memset(temp,0,20);
	strncpy(temp,&(szDateTime[17]),4);
	nSec = atoi(temp);

	CTime updatetime(nYear,nMonth,nDay,nHour,nMin,nSec);
	time_t stUpdatetime = updatetime.GetTime();
	CTime currenttime = CTime::GetCurrentTime();

    return (currenttime.GetTime()-stUpdatetime);
}

void CMainFrame::AddToSysMenu()
{
	CMenu *pMenu;

	pMenu=this->GetSystemMenu(FALSE);
	ASSERT(pMenu);
	pMenu->AppendMenu(MF_SEPARATOR);
	//pMenu->AppendMenu(MF_STRING,NEWMENUITEM,"显示工具栏");
	pMenu->AppendMenu(MF_STRING,ID_SHOWMENU,"显示/隐藏菜单栏(&U)");
	pMenu->AppendMenu(MF_STRING,ID_VIEW_TOOLBAR,"显示/隐藏工具栏(&T)");
}



LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
    if (WM_SYSCOMMAND==message)
	{		
		if (ID_VIEW_TOOLBAR==wParam)
		{
			ShowControlBar(&m_wndToolBar,!m_wndToolBar.IsWindowVisible(),FALSE);
			return 0;
		}
		else if (ID_SHOWMENU==wParam)
		{
			ShowControlBar(&m_wndMenuBar,!m_wndMenuBar.IsWindowVisible(),FALSE);
			return 0;
		}
	}	

	return CFrameWnd::WindowProc(message, wParam, lParam);
}

void CMainFrame::OnUserinfoin() 
{
	int i;
	int nMaxUser;
	user_info UserInfo;
	FILE *fp;
	int  nBytesRead;

	fp = fopen("UserInfo.dat", "r");
	if (NULL == fp)
	{
		return;
	}

	DeleteAllUser();

	nMaxUser = get_max_user_amount();
	for (i=0; i<nMaxUser; i++)
	{
		nBytesRead = fread((char *)&UserInfo, 1, sizeof(user_info), fp);
		*((time_t *)UserInfo.m_update_time) = time(NULL);
		if (nBytesRead < sizeof(user_info))
		{
			break;
		}
		
		AddOneUser(&UserInfo);
	}

	fclose(fp);

	RefreshUserTable(0);
}

void CMainFrame::OnUserinfoout() 
{
	int i;
	int nMaxUser;
	user_info UserInfo;
	FILE *fp;

	fp = fopen("UserInfo.dat", "w");
	if (NULL == fp)
	{
		return;
	}

	nMaxUser = get_max_user_amount();
	for (i=0; i<nMaxUser; i++)
	{
		get_user_at(i, &UserInfo);
		if (1 == UserInfo.m_status)
		{
			fwrite((char *)&UserInfo, 1, sizeof(user_info), fp);
		}
	}

	fclose(fp);
}

void CMainFrame::OnMenusmsstart() 
{
	// TODO: Add your command handler code here
	//需要配置波特率、数据位、停止位、校验码，其余不需配置；
	/*Test SMM call 
	 *Add by sea on sep 16,2003"*/
	int iResult = 0;
	DCB dcb;		 //Defines the control setting for a serial communications device.
	dcb.BaudRate = 115200;
	dcb.ByteSize = 8;
	dcb.StopBits = 0;//ONESTOPBITS;
	dcb.Parity = 0 ;
    iResult = SMMInit("202.96.134.133",5001,
			 "COM5", &dcb,
			 "13800755500","13510006407",1);
	if(iResult<0) 
				AddTextToEditView("smm error",false);
	SMMSetting("10.0.0.1",5002);
	SetModemType(1);
	iResult = MakeDTUOnLine(1,"13902944285");

//	AfxBeginThread(pfnReadData,this,THREAD_PRIORITY_NORMAL);
}

void CMainFrame::OnMenusmsexit() 
{
	// TODO: Add your command handler code here
//	g_nCount = 200;
	SMMFree();		
}

void CMainFrame::OnAdduser() 
{
	// TODO: Add your command handler code here
	if(!m_IsAdduserDlg)
	{
		m_IsAdduserDlg=TRUE;
	    CAddUserDlg * adduserdlg=new CAddUserDlg;
	    adduserdlg->Create (IDD_AddUser,this);
	    adduserdlg->ShowWindow (SW_SHOW);
	}
    return;

}

void CMainFrame::OnDeleteUser() 
{
	// TODO: Add your command handler code here
	if (11==strlen(m_userid))
	{
			delete_one_user((unsigned char *)m_userid,NULL);
			this->RefreshUserTable();
	}	
}

void CMainFrame::OnUpdateAdduser(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bServerRunning);	
}

void CMainFrame::OnUpdateDeleteUser(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bServerRunning);	
}

void CMainFrame::OnUpdateUserinfoin(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bServerRunning);	
}

void CMainFrame::OnUpdateUserinfoout(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bServerRunning);	
}

void CMainFrame::OnDisconnectPpp() 
{
	// TODO: Add your command handler code here
	if (11==strlen(m_userid))
	{
			do_disconnect_ppp_link((unsigned char *)m_userid,NULL);
	}		
}

void CMainFrame::OnUpdateDisconnectPpp(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bServerRunning);	
}

//由DSC发往DTU，要求DTU停止向DSC发送数据
void CMainFrame::OnStopToSend() 
{
	// TODO: Add your command handler code here
	if (11==strlen(m_userid))
	{
			do_stop_send_data((unsigned char *)m_userid,NULL);
	}			
}

void CMainFrame::OnUpdateStopToSend(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bServerRunning);	
}

//DSC向DTU发送指令，要求DTU恢复向DSC发送数据，DTU收到后会将恢复向DSC发送用户数据
void CMainFrame::OnStartToSend() 
{
	// TODO: Add your command handler code here
	if (11==strlen(m_userid))
	{
			do_start_send_data((unsigned char *)m_userid,NULL);
	}				
}

void CMainFrame::OnUpdateStartToSend(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bServerRunning);	
}

//用来向DTU发送指令，要求DTU将用户数据缓存清空。
//该功能很少使用，只在3.0以前软件版本的DTU中生效。

void CMainFrame::OnDiscardData() 
{
	// TODO: Add your command handler code here
	if (11==strlen(m_userid))
	{
			do_flush_cache_data((unsigned char *)m_userid,NULL);
	}				
}

void CMainFrame::OnUpdateDiscardData(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bServerRunning);	
}

void CMainFrame::OnDturemotesetting() 
{
	// TODO: Add your command handler code here
//	CRemoteDtuSettingDlg remoteDtuSettingDlg;

	if(IDOK==m_remoteDtuSettingDlg.DoModal())
	{
		
	}
	//m_remoteDtuSettingDlg
}

void CMainFrame::OnSqlsetting() 
{
	// TODO: Add your command handler code here
	CSqlSettingDlg dlg;
	dlg.m_strDataBaseName=this->m_dbName;
	dlg.m_strUserName=this->m_userName;
	dlg.m_strPassword=this->m_password;
	if(IDOK==dlg.DoModal())
	{
		this->m_password=dlg.m_strPassword;
		this->m_userName=dlg.m_strUserName;
		this->m_dbName=dlg.m_strDataBaseName;
	}
}
