// AdduserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DSC.h"
#include "AdduserDlg.h"
#include "MainFrm.h"
#include <winsock2.h>
#include "wcomm_dll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdduserDlg dialog


CAdduserDlg::CAdduserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAdduserDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAdduserDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAdduserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdduserDlg)
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_date);
	DDX_Control(pDX, IDC_COMBO3, m_dscport);
	DDX_Control(pDX, IDC_COMBO2, m_dtuport);
	DDX_Control(pDX, IDC_COMBO1, m_status);
	DDX_Control(pDX, IDC_IPADDRESS2, m_dscip);
	DDX_Control(pDX, IDC_IPADDRESS1, m_dtuip);
	DDX_Control(pDX, IDC_EDIT1, m_userid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdduserDlg, CDialog)
	//{{AFX_MSG_MAP(CAdduserDlg)
	ON_BN_CLICKED(IDOK, OnAddUserB)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdduserDlg message handlers

BOOL CAdduserDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	HICON hIcon=LoadIcon (AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_AddUser));
	SetIcon(hIcon,TRUE);

	//初始化登录时间控件
	CTime stime=CTime::GetCurrentTime (); 
	CTime etime( 2030, 12, 31, 23, 59, 59 ); 
	m_date.SetFormat("yyyy/MM/dd HH:mm:ss");
	m_date.SetRange(&stime,&etime);
	m_date.SetTime(&stime);

	//初始化端口与在线状态控件
	m_dtuport.SetWindowText ("5001");
	m_dscport.SetWindowText ("5002");
	m_status.SelectString (1,"在线");

	//初始化IP地址控件
	CString str1;
	str1=GetLocalIP();
	m_dtuip.SetWindowText (str1);
	m_dscip.SetWindowText (str1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAdduserDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
    delete this;
}

void CAdduserDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CMainFrame* pFram=(CMainFrame*)AfxGetMainWnd ();
    pFram->m_IsAdduserDlg =FALSE;	
	CDialog::OnCancel();
}

CString CAdduserDlg::GetLocalIP()
{
	char szHostname[100]; 
    CString str1="",str2;
	struct hostent *pHostEnt;

	memset(szHostname,0,100);

	if( gethostname (szHostname, sizeof( szHostname ))!=SOCKET_ERROR) 
	{ 
		//取本机的名称成功后，再根据名字取IP地址
		  pHostEnt = gethostbyname(szHostname); 
	      if ( pHostEnt != NULL ) 
		  { 
			  for(int i=0 ; pHostEnt->h_addr_list[i]!=NULL ; i++)
			  {
				  //转换IP地址为"xxx.xxx.xxx.xxx"形式的IP地址字符串
				for(int j=0 ; j<pHostEnt->h_length ; j++)
				{
					if( j>0 )
					{
						str1+=".";
					}//end if
					str2.Format ("%u",
					(unsigned int)((unsigned char*)pHostEnt->h_addr_list[i])[j]);

					str1+=str2;
				}//end for

			  }//end for

			  return str1;            
		  } 
		  else
		  {
			  //取不到本机的IP地址，返回空字符串
			  return str1;  
		  }//end if
	} 
	else
	{
		//取不到本机的名字，返回空字符串
		return str1;
	}//end if

}

void CAdduserDlg::IPHightToLower(const char *IPstr, char *ipaddrstr)
{
	CString iparry[4];
	int i;
	int m=0,n;
	CString ipaddr;
    
	iparry[m].Empty ();
	ipaddr.Empty ();
	n=strlen(IPstr);
    for(i=0; i<n; ++i)
	{
        if(*IPstr == '.')
		{
			++m;
			iparry[m].Empty ();
		}
		else
		{
			iparry[m] += *IPstr;
		}
		IPstr++;
	}

	ipaddr=iparry[3];
	ipaddr+='.';
	ipaddr+=iparry[2];
	ipaddr+='.';
	ipaddr+=iparry[1];
	ipaddr+='.';
	ipaddr+=iparry[0];

    strcpy(ipaddrstr,(LPSTR)(LPCTSTR)ipaddr);
	return ;

}

void CAdduserDlg::OnAddUserB() 
{
	// TODO: Add your control notification handler code here
	int j;
	int count,i;
    user_info userinfo;
	CString userid,logondate;
	char dscip[17];
	char dtuip[17];
	char *iptmp;
	char port[10];
	char a[3];
	VARIANT sinaddr,localaddr;
	short sinport,localport;
	CString str1[7];
	char puserid[12];
	CMainFrame* pFram=(CMainFrame*)AfxGetMainWnd ();   //取主框架的句柄

	memset(puserid,0,12);
	userid.Empty ();
	m_userid.GetWindowText (userid);
	if((!userid.IsEmpty ()) && (userid.GetLength ()==11))   //检查用户ID的有效性规则
	{
		//检查通过，则赋值
        memset(userinfo.m_userid ,0 ,12);
		memcpy(userinfo.m_userid ,(LPSTR)(LPCTSTR)userid ,userid.GetLength ());

		iptmp=new char[17];
		memset(dscip,0,17);
		memset(dtuip,0,17);
		memset(iptmp,0,17);
		memset(port,0,10);

		//给sinaddr赋值
		m_dscip.GetWindowText (dscip,17);
		IPHightToLower(dscip, iptmp);  //IP地址高低位反向
		userinfo.m_sin_addr  =inet_addr(iptmp);//将字符串形式的IP地址转为unsigned long类型

		//给localaddr赋值
		m_dtuip.GetWindowText (dtuip,17);
		IPHightToLower(dtuip, iptmp);
		userinfo.m_local_addr  =inet_addr(iptmp);

		//给sinport赋值
		m_dscport.GetWindowText (port,10);
		userinfo.m_sin_port =atoi(port);

		//给localport赋值
		memset(port,0,10);
		m_dtuport.GetWindowText (port,10);
		userinfo.m_local_port =atoi(port);

		//给logondate赋值
		logondate.Empty ();
		m_date.GetWindowText (logondate);
        memset(userinfo.m_logon_date ,0,20);
		memcpy(userinfo.m_logon_date ,(LPSTR)(LPCTSTR)logondate,logondate.GetLength ());

		//取在线状态
		userinfo.m_status =m_status.GetCurSel();

		//调用控件添加用户接口
		i=AddOneUser(&userinfo);

		pFram->RefreshUserTable (0);
		UpdateData (FALSE);
		delete []iptmp;
	}
	else
	{
		//检查失败，给出告警提示
		AfxMessageBox ("用户ID不能为空或输入长度不正确!",0,0);
	}
}
