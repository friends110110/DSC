// SettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DSC.h"
#include "SettingDlg.h"
#include "winsock2.h"
#include "wcomm_dll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg dialog


CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/,int tabindex,BOOL Enabled)
	: CDialog(CSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingDlg)
	m_bSaveToLog = FALSE;
	m_iLogItemCount = _T("");
	m_iOffLineTime = _T("");
	m_iPollTime = _T("");
	m_iServerPort = _T("");
	m_bSysAutoPoll = FALSE;
	m_bShowHead = FALSE;
	m_bHexShow = FALSE;
	m_szlist = _T("");
	m_udp = -1;
	//}}AFX_DATA_INIT
	m_tabindex=tabindex;
	m_BEnabled=Enabled;
}


void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingDlg)
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_COMBO1, m_comb);
	DDX_Check(pDX, IDC_CKSAVETOLOG, m_bSaveToLog);
	DDX_CBString(pDX, IDC_CMLOGITEMCOUNT, m_iLogItemCount);
	DDX_CBString(pDX, IDC_CMOFFLINETIME, m_iOffLineTime);
	DDX_CBString(pDX, IDC_CMPOLLTIME, m_iPollTime);
	DDX_CBString(pDX, IDC_CMSERVERPORT, m_iServerPort);
	DDX_Check(pDX, IDC_CKSYSAUTOPOLL, m_bSysAutoPoll);
	DDX_Check(pDX, IDC_CKSHOWHEAD, m_bShowHead);
	DDX_Check(pDX, IDC_CKHEXSHOW, m_bHexShow);
	DDX_LBString(pDX, IDC_LIST1, m_szlist);
	DDX_Radio(pDX, IDC_RADIO_UDP, m_udp);
	DDX_Control(pDX, IDC_TAB1, m_tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CSettingDlg)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_BN_CLICKED(IDC_CKSAVETOLOG, OnCksavetolog)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_BN_CLICKED(IDC_DELETEIP, OnDeleteip)
	ON_LBN_DBLCLK(IDC_LIST1, OnDblclkList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg message handlers

void CSettingDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	CImageList img;
	int  iIndex;

	img.Create(IDB_SETTING,16,0,RGB(255,0,255));	
    this->m_tab.SetImageList(&img);
	img.Detach();
	this->m_tab.InsertItem(TCIF_TEXT | TCIF_IMAGE,0,"服务参数",0,NULL);
	this->m_tab.InsertItem(TCIF_TEXT | TCIF_IMAGE,1,"数据处理",1,NULL);
	this->m_tab.SetCurSel(m_tabindex);
	this->ShowItem();
	if (!this->m_bSaveToLog)
        ((CComboBox *)GetDlgItem(IDC_CMLOGITEMCOUNT))->EnableWindow(FALSE);

	((CComboBox *)GetDlgItem(IDC_CMSERVERPORT))->EnableWindow(m_BEnabled);
	((CComboBox *)GetDlgItem(IDC_CMPOLLTIME))->EnableWindow(m_BEnabled);
	((CComboBox *)GetDlgItem(IDC_CMOFFLINETIME))->EnableWindow(m_BEnabled);
	((CButton   *)GetDlgItem(IDC_CKSYSAUTOPOLL))->EnableWindow(m_BEnabled);	
	((CButton   *)GetDlgItem(IDC_RADIO_UDP))->EnableWindow(m_BEnabled);
	((CButton   *)GetDlgItem(IDC_RADIO_TCP))->EnableWindow(m_BEnabled);

    iIndex=0;
	ULONG ulIP;
	char ipaddr[32];
	
	while ((ulIP=GetIP(iIndex++))>0)
	{
		sprintf(ipaddr,"%s",inet_ntoa(*((IN_ADDR *)&ulIP)));
		m_list.AddString(ipaddr);
	}

	WSADATA wsa;
	if (::WSAStartup(MAKEWORD(1,1),&wsa)!=0)
	{
		return;
	}

	struct hostent *hp;
	char name[64];
	
	iIndex=0;

	if (0==gethostname(name,64))
	{
		hp=gethostbyname(name);
		if (hp!=NULL)
		{
			while (hp->h_addr_list[iIndex])
			{
				sprintf(ipaddr,"%s",inet_ntoa(*((IN_ADDR *)hp->h_addr_list[iIndex])));
				if (LB_ERR==m_list.FindString(-1,ipaddr))
					m_comb.AddString(ipaddr);
				iIndex++;
			}
		}
	}

	::WSACleanup();

}

void CSettingDlg::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_tabindex=this->m_tab.GetCurSel();

	this->ShowItem();

	*pResult = 0;
}

void CSettingDlg::ShowItem()
{    
	if (0==m_tabindex)
	{
		((CStatic *)GetDlgItem(IDC_STATIC1))->SetWindowText("服务端口");
		((CStatic *)GetDlgItem(IDC_STATIC2))->SetWindowText("轮询时间");
		((CStatic *)GetDlgItem(IDC_STATIC3))->SetWindowText("下线时间");
        ((CStatic *)GetDlgItem(IDC_STATIC4))->SetWindowText("选中IP");
		((CComboBox *)GetDlgItem(IDC_CMSERVERPORT))->ShowWindow(SW_SHOW);
		((CComboBox *)GetDlgItem(IDC_CMPOLLTIME))->ShowWindow(SW_SHOW);
		((CComboBox *)GetDlgItem(IDC_CMOFFLINETIME))->ShowWindow(SW_SHOW);
		((CComboBox *)GetDlgItem(IDC_CMLOGITEMCOUNT))->ShowWindow(SW_HIDE);
		((CButton *)GetDlgItem(IDC_CKSYSAUTOPOLL))->ShowWindow(SW_SHOW);
		((CListBox *)GetDlgItem(IDC_LIST1))->ShowWindow(SW_SHOW);
		((CButton *)GetDlgItem(IDC_DELETEIP))->ShowWindow(SW_SHOW);
		((CComboBox *)GetDlgItem(IDC_COMBO1))->ShowWindow(SW_SHOW);
		((CButton *)GetDlgItem(IDC_CKSAVETOLOG))->ShowWindow(SW_HIDE);
		((CButton *)GetDlgItem(IDC_CKSHOWHEAD))->ShowWindow(SW_HIDE);
		((CButton *)GetDlgItem(IDC_CKHEXSHOW))->ShowWindow(SW_HIDE);
	}
	else
	{
        ((CStatic *)GetDlgItem(IDC_STATIC1))->SetWindowText("日志数量");
		((CStatic *)GetDlgItem(IDC_STATIC2))->SetWindowText("");
		((CStatic *)GetDlgItem(IDC_STATIC3))->SetWindowText("");
		((CStatic *)GetDlgItem(IDC_STATIC4))->SetWindowText("");
		((CComboBox *)GetDlgItem(IDC_CMSERVERPORT))->ShowWindow(SW_HIDE);
		((CComboBox *)GetDlgItem(IDC_CMPOLLTIME))->ShowWindow(SW_HIDE);
		((CComboBox *)GetDlgItem(IDC_CMOFFLINETIME))->ShowWindow(SW_HIDE);
		((CComboBox *)GetDlgItem(IDC_CMLOGITEMCOUNT))->ShowWindow(SW_SHOW);
		((CButton *)GetDlgItem(IDC_CKSYSAUTOPOLL))->ShowWindow(SW_HIDE);
		((CListBox *)GetDlgItem(IDC_LIST1))->ShowWindow(SW_HIDE);
		((CButton *)GetDlgItem(IDC_DELETEIP))->ShowWindow(SW_HIDE);
		((CComboBox *)GetDlgItem(IDC_COMBO1))->ShowWindow(SW_HIDE);
		((CButton *)GetDlgItem(IDC_CKSAVETOLOG))->ShowWindow(SW_SHOW);
		((CButton *)GetDlgItem(IDC_CKSAVETOLOG))->EnableWindow(true);
		((CButton *)GetDlgItem(IDC_CKSHOWHEAD))->ShowWindow(SW_SHOW);
		((CButton *)GetDlgItem(IDC_CKHEXSHOW))->ShowWindow(SW_SHOW);
	}
}

void CSettingDlg::OnCksavetolog() 
{
    CButton *ck;

	ck=(CButton *)GetDlgItem(IDC_CKSAVETOLOG);
	if (1==ck->GetCheck())
	{
		((CComboBox *)GetDlgItem(IDC_CMLOGITEMCOUNT))->EnableWindow(TRUE);
	}
	else
	{
		((CComboBox *)GetDlgItem(IDC_CMLOGITEMCOUNT))->EnableWindow(FALSE);
	}
}

void CSettingDlg::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	char buf[32];

	m_comb.GetLBText(m_comb.GetCurSel(),buf);
	m_list.AddString(buf);
}

void CSettingDlg::OnDeleteip() 
{
	int iIndex;

	iIndex=m_list.GetCurSel();
	if (iIndex>=0)
		m_list.DeleteString(iIndex);	
}

void CSettingDlg::OnDblclkList1() 
{
	OnDeleteip();
}

void CSettingDlg::OnOK() 
{
	// TODO: Add extra validation here
	char buf[32];

	DelFilterIP(0);

	for (int i=0;i<m_list.GetCount();i++)
	{
		m_list.GetText(i,buf);
		if (-1==AddFilterIP(inet_addr(buf)))
		{
			break;
		}
	}

	CDialog::OnOK();
}
