// RemoteDtuSettingPanel2.cpp : implementation file
//

#include "stdafx.h"
#include "DSC.h"
#include "RemoteDtuSettingPanel2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRemoteDtuSettingPanel2 dialog


CRemoteDtuSettingPanel2::CRemoteDtuSettingPanel2(CWnd* pParent /*=NULL*/)
	: CDialog(CRemoteDtuSettingPanel2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRemoteDtuSettingPanel2)
	m_dtuSignCheck = FALSE;
	m_localPortCheck = FALSE;
	m_onlineCheck = FALSE;
	m_maxPackageCheck = FALSE;
	m_reconnectIntervalCheck = FALSE;
	m_debugStateCheck = FALSE;
	m_lastPackageIntervalCheck = FALSE;
	m_allSelectCheck = FALSE;
	m_packageSplitCheck = FALSE;
	//}}AFX_DATA_INIT
}


void CRemoteDtuSettingPanel2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRemoteDtuSettingPanel2)
	DDX_Control(pDX, IDC_EDIT28, m_packageSplitEdit);
	DDX_Control(pDX, IDC_EDIT27, m_lastPackageIntervalEdit);
	DDX_Control(pDX, IDC_EDIT26, m_debugStateEdit);
	DDX_Control(pDX, IDC_EDIT25, m_reconnectIntervalEdit);
	DDX_Control(pDX, IDC_EDIT24, m_maxPackageEdit);
	DDX_Control(pDX, IDC_EDIT23, m_onlineEdit);
	DDX_Control(pDX, IDC_EDIT22, m_localPortEdit);
	DDX_Control(pDX, IDC_EDIT21, m_DtuSignEdit);
	DDX_Check(pDX, IDC_CHECK21, m_dtuSignCheck);
	DDX_Check(pDX, IDC_CHECK22, m_localPortCheck);
	DDX_Check(pDX, IDC_CHECK23, m_onlineCheck);
	DDX_Check(pDX, IDC_CHECK24, m_maxPackageCheck);
	DDX_Check(pDX, IDC_CHECK25, m_reconnectIntervalCheck);
	DDX_Check(pDX, IDC_CHECK26, m_debugStateCheck);
	DDX_Check(pDX, IDC_CHECK27, m_lastPackageIntervalCheck);
	DDX_Check(pDX, IDC_CHECK29, m_allSelectCheck);
	DDX_Check(pDX, IDC_CHECK28, m_packageSplitCheck);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRemoteDtuSettingPanel2, CDialog)
	//{{AFX_MSG_MAP(CRemoteDtuSettingPanel2)
	// NOTE: the ClassWizard will add message map macros here
		ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK21, IDC_CHECK21, OnCheckClicked)  
		ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK22, IDC_CHECK22, OnCheckClicked)  
		ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK23, IDC_CHECK23, OnCheckClicked)  
		ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK24, IDC_CHECK24, OnCheckClicked)  
		ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK25, IDC_CHECK25, OnCheckClicked)  
		ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK26, IDC_CHECK26, OnCheckClicked)
		ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK27, IDC_CHECK27, OnCheckClicked)		
		ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK28, IDC_CHECK28, OnCheckClicked)
		ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK29, IDC_CHECK29, OnCheckClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRemoteDtuSettingPanel2 message handlers

void CRemoteDtuSettingPanel2::OnCheckClicked(UINT uId)
{
	UpdateData(TRUE);
	switch(uId)
	{
	case IDC_CHECK21:		//DTU标识
		if (TRUE==m_dtuSignCheck)
		{
			m_DtuSignEdit.EnableWindow(TRUE);
		}
		else
		{
			m_DtuSignEdit.EnableWindow(FALSE);
		}
		break;
	case IDC_CHECK22:		//本地端口
		if (TRUE==m_localPortCheck)
		{
			m_localPortEdit.EnableWindow(TRUE);
		}
		else
		{
			m_localPortEdit.EnableWindow(FALSE);
		}
		break;
	case IDC_CHECK23:		//在线报告间隔
		if (TRUE==m_onlineCheck)
		{
			m_onlineEdit.EnableWindow(TRUE);
		}
		else
		{
			m_onlineEdit.EnableWindow(FALSE);
		}
		break;
	case IDC_CHECK24:		//最大包长
		if (TRUE==m_maxPackageCheck)
		{
			m_maxPackageEdit.EnableWindow(TRUE);
		}
		else
		{
			m_maxPackageEdit.EnableWindow(FALSE);
		}
		break;
	case IDC_CHECK25:		//重连接间隔
		if (TRUE==m_reconnectIntervalCheck)
		{
			m_reconnectIntervalEdit.EnableWindow(TRUE);
		}
		else
		{
			m_reconnectIntervalEdit.EnableWindow(FALSE);
		}
		break;
	case IDC_CHECK26:		//调试状态
		if (TRUE==m_debugStateCheck)
		{
			m_debugStateEdit.EnableWindow(TRUE);
		}
		else
		{
			m_debugStateEdit.EnableWindow(FALSE);
		}
		break;
	case IDC_CHECK27:		//最后包空闲间隔
		if (TRUE==m_lastPackageIntervalCheck)
		{
			m_lastPackageIntervalEdit.EnableWindow(TRUE);
		}
		else
		{
			m_lastPackageIntervalEdit.EnableWindow(FALSE);
		}
		break;
	case IDC_CHECK28:		//包分隔符
		if (TRUE==m_packageSplitCheck)
		{
			m_packageSplitEdit.EnableWindow(TRUE);
		}
		else
		{
			m_packageSplitEdit.EnableWindow(FALSE);
		}
		break;
	case IDC_CHECK29:		//全选
		if (TRUE==m_allSelectCheck)
		{
			m_dtuSignCheck=TRUE;
			m_DtuSignEdit.EnableWindow(TRUE);

			m_localPortCheck=TRUE;
			m_localPortEdit.EnableWindow(TRUE);

			m_onlineCheck=TRUE;
			m_onlineEdit.EnableWindow(TRUE);

			m_maxPackageCheck=TRUE;
			m_maxPackageEdit.EnableWindow(TRUE);

			m_reconnectIntervalCheck=TRUE;
			m_reconnectIntervalEdit.EnableWindow(TRUE);

			m_debugStateCheck=TRUE;
			m_debugStateEdit.EnableWindow(TRUE);

			m_lastPackageIntervalCheck=TRUE;
			m_lastPackageIntervalEdit.EnableWindow(TRUE);

			m_packageSplitCheck=TRUE;
			m_packageSplitEdit.EnableWindow(TRUE);
		
		}
		else
		{
			m_dtuSignCheck=FALSE;
			m_DtuSignEdit.EnableWindow(FALSE);
			
			m_localPortCheck=FALSE;
			m_localPortEdit.EnableWindow(FALSE);
			
			m_onlineCheck=FALSE;
			m_onlineEdit.EnableWindow(FALSE);
			
			m_maxPackageCheck=FALSE;
			m_maxPackageEdit.EnableWindow(FALSE);
			
			m_reconnectIntervalCheck=FALSE;
			m_reconnectIntervalEdit.EnableWindow(FALSE);
			
			m_debugStateCheck=FALSE;
			m_debugStateEdit.EnableWindow(FALSE);
			
			m_lastPackageIntervalCheck=FALSE;
			m_lastPackageIntervalEdit.EnableWindow(FALSE);
			
			m_packageSplitCheck=FALSE;
			m_packageSplitEdit.EnableWindow(FALSE);
		}
		break;
	default:
		break;
	}
	UpdateData(FALSE);
}
long CRemoteDtuSettingPanel2::Convert16NumberTo10(char *buf,int bufLen)
{
	long nValue=0;
	unsigned char bit4;
	for (int i=0;i<bufLen;i++)
	{
		bit4=buf[i]>>4;
		bit4=bit4&(0x0f);
		nValue=nValue*16+(long)bit4;
		bit4=buf[i]&(0x0f);
		nValue=nValue*16+(long)bit4;
	}
	return nValue;
}
void CRemoteDtuSettingPanel2::QueryDtuParamFromRemoteDtu(CString dtuUserId)
{
	unsigned char paramType[8]={0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x23};
	char buf[50];
	
	int nParamLenth;
	int i=0;
	nParamLenth=sizeof(buf);
	unsigned char nType;
	CString str;
//	int nValue;
	for (i=0;i<8;i++)
	{
		nType=paramType[i];
		nParamLenth=50;
		if (GetParam(nType,buf,&nParamLenth)==0)
		{
			buf[nParamLenth]=0;
			str.Format("%ld",Convert16NumberTo10(buf,nParamLenth));
			switch (nType)
			{
			case 0x06:
				m_DtuSignEdit.SetWindowText(buf);
				break;
			case 0x07:

				m_localPortEdit.SetWindowText(str);
				break;
			case 0x08:
				m_onlineEdit.SetWindowText(str);
				break;
			case 0x09:
				m_maxPackageEdit.SetWindowText(str);
				break;
			case 0x0a:
				m_reconnectIntervalEdit.SetWindowText(str);
				break;
			case 0x0b:
				m_debugStateEdit.SetWindowText(str);
				break;
			case 0x0c:
				m_lastPackageIntervalEdit.SetWindowText(str);
				break;
			case 0x23:
				m_packageSplitEdit.SetWindowText(str);
				break;
			default:
				AfxMessageBox("panel query have a mistake");
			}
		}
		else
		{
			AfxMessageBox(nType+"  Getparam failed  ");
			return ;
		}
	}
	UpdateData(FALSE);
}

void CRemoteDtuSettingPanel2::GetSettingParamToRemoteDtu(vector<SETTINGPARAM> *vec)
{
	SETTINGPARAM setparam;
	setparam.paramLen=-1;
	if (TRUE==m_dtuSignCheck)
	{
		setparam.nType=0x06;
		setparam.paramName="DTU标识";
		CString str;
		m_DtuSignEdit.GetWindowText(str);
		setparam.value=str;
		vec->push_back(setparam);
	}
	if (TRUE==m_localPortCheck)
	{
		setparam.nType=0x07;
		setparam.paramName="DTU本地端口";
		CString str;
		m_localPortEdit.GetWindowText(str);
		setparam.value=str;
		setparam.paramLen=2;
		vec->push_back(setparam);
	}
	if (TRUE==m_onlineCheck)
	{
		setparam.nType=0x08;
		setparam.paramName="在线报告时间间隔";
		CString str;
		m_onlineEdit.GetWindowText(str);
		setparam.value=str;
		setparam.paramLen=2;
		vec->push_back(setparam);
	}
	if (TRUE==m_maxPackageCheck)
	{
		setparam.nType=0x09;
		setparam.paramName="最大包长";
		CString str;
		m_maxPackageEdit.GetWindowText(str);
		setparam.value=str;
		setparam.paramLen=2;
		vec->push_back(setparam);
	}
	if (TRUE==m_reconnectIntervalCheck)
	{
		setparam.nType=0x0a;
		setparam.paramName="重连接时间间隔";
		CString str;
		m_reconnectIntervalEdit.GetWindowText(str);
		setparam.value=str;
		setparam.paramLen=2;
		vec->push_back(setparam);
	}
	if (TRUE==m_debugStateCheck)
	{
		setparam.nType=0xb;
		setparam.paramName="调试信息类型";
		CString str;
		m_debugStateEdit.GetWindowText(str);
		setparam.value=str;
		setparam.paramLen=1;
		vec->push_back(setparam);
	}
	if (TRUE==m_lastPackageIntervalCheck)
	{
		setparam.nType=0x0c;
		setparam.paramName="最后包空闲时间间隔";
		CString str;
		m_lastPackageIntervalEdit.GetWindowText(str);
		setparam.value=str;
		setparam.paramLen=2;
		vec->push_back(setparam);
	}
	if (TRUE==m_packageSplitCheck)
	{
		setparam.nType=0x23;
		setparam.paramName="数据包分隔符";
		CString str;
		m_packageSplitEdit.GetWindowText(str);
		setparam.value=str;
		setparam.paramLen=1;
		vec->push_back(setparam);
	}
}
BOOL CRemoteDtuSettingPanel2::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_dtuSignCheck=FALSE;
	m_DtuSignEdit.EnableWindow(FALSE);
	
	m_localPortCheck=FALSE;
	m_localPortEdit.EnableWindow(FALSE);
	
	m_onlineCheck=FALSE;
	m_onlineEdit.EnableWindow(FALSE);
	
	m_maxPackageCheck=FALSE;
	m_maxPackageEdit.EnableWindow(FALSE);
	
	m_reconnectIntervalCheck=FALSE;
	m_reconnectIntervalEdit.EnableWindow(FALSE);
	
	m_debugStateCheck=FALSE;
	m_debugStateEdit.EnableWindow(FALSE);
	
	m_lastPackageIntervalCheck=FALSE;
	m_lastPackageIntervalEdit.EnableWindow(FALSE);
	
	m_packageSplitCheck=FALSE;
	m_packageSplitEdit.EnableWindow(FALSE);
	m_allSelectCheck=FALSE;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
