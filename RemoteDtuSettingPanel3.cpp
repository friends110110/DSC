// RemoteDtuSettingPanel3.cpp : implementation file
//

#include "stdafx.h"
#include "DSC.h"
#include "RemoteDtuSettingPanel3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRemoteDtuSettingPanel3 dialog


CRemoteDtuSettingPanel3::CRemoteDtuSettingPanel3(CWnd* pParent /*=NULL*/)
	: CDialog(CRemoteDtuSettingPanel3::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRemoteDtuSettingPanel3)
	m_mainDscAddressCheck = FALSE;
	m_DscDomainCheck = FALSE;
	m_domainServerAddressCheck = FALSE;
	m_reservDscPortCheck = FALSE;
	m_reserveDscAddressCheck = FALSE;
	m_domainIntervalCheck = FALSE;
	m_mainDscPortCheck = FALSE;
	m_allSelectCheck = FALSE;
	//}}AFX_DATA_INIT
}


void CRemoteDtuSettingPanel3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRemoteDtuSettingPanel3)
	DDX_Control(pDX, IDC_IPADDRESS4, m_reserveDscAddressCtrl);
	DDX_Control(pDX, IDC_IPADDRESS3, m_mainDscAddressCtrl);
	DDX_Control(pDX, IDC_COMBO31, m_domainServerAddressCombox);
	DDX_Control(pDX, IDC_EDIT36, m_reserveDscPortEdit);
	DDX_Control(pDX, IDC_EDIT34, m_mainDscPortEdit);
	DDX_Control(pDX, IDC_EDIT33, m_domainIntervalEdit);
	DDX_Control(pDX, IDC_EDIT32, m_dscDomainEdit);
	DDX_Check(pDX, IDC_CHECK31, m_mainDscAddressCheck);
	DDX_Check(pDX, IDC_CHECK32, m_DscDomainCheck);
	DDX_Check(pDX, IDC_CHECK35, m_domainServerAddressCheck);
	DDX_Check(pDX, IDC_CHECK37, m_reservDscPortCheck);
	DDX_Check(pDX, IDC_CHECK36, m_reserveDscAddressCheck);
	DDX_Check(pDX, IDC_CHECK33, m_domainIntervalCheck);
	DDX_Check(pDX, IDC_CHECK34, m_mainDscPortCheck);
	DDX_Check(pDX, IDC_CHECK38, m_allSelectCheck);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRemoteDtuSettingPanel3, CDialog)
	//{{AFX_MSG_MAP(CRemoteDtuSettingPanel3)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK31, IDC_CHECK31, OnCheckClicked)  
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK32, IDC_CHECK32, OnCheckClicked)  
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK33, IDC_CHECK33, OnCheckClicked)  
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK34, IDC_CHECK34, OnCheckClicked)  
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK35, IDC_CHECK35, OnCheckClicked)  
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK36, IDC_CHECK36, OnCheckClicked)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK37, IDC_CHECK37, OnCheckClicked)		
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK38, IDC_CHECK38, OnCheckClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRemoteDtuSettingPanel3 message handlers

void CRemoteDtuSettingPanel3::OnCheckClicked(UINT uId)
{
	UpdateData(TRUE);
	switch(uId)
	{
	case IDC_CHECK31:
		if (TRUE==m_mainDscAddressCheck)
		{
		//	m_mainDscAddressEdit.EnableWindow(TRUE);
			m_mainDscAddressCtrl.EnableWindow(TRUE);
		}
		else
		{
			//m_mainDscAddressEdit.EnableWindow(FALSE);
			m_mainDscAddressCtrl.EnableWindow(FALSE);
		}
		break;
	case IDC_CHECK32:
		if (TRUE==m_DscDomainCheck)
		{
			m_dscDomainEdit.EnableWindow(TRUE);
		}
		else
		{
			m_dscDomainEdit.EnableWindow(FALSE);
		}
		break;
	case IDC_CHECK33:
		if (TRUE==m_domainIntervalCheck)
		{
			m_domainIntervalEdit.EnableWindow(TRUE);
		}
		else
		{
			m_domainIntervalEdit.EnableWindow(FALSE);
		}
		break;
	case IDC_CHECK34:
		if (TRUE==m_mainDscPortCheck)
		{
			m_mainDscPortEdit.EnableWindow(TRUE);
		}
		else
		{
			m_mainDscPortEdit.EnableWindow(FALSE);
		}
		break;
	case IDC_CHECK35:
		if (TRUE==m_domainServerAddressCheck)
		{
			m_domainServerAddressCombox.EnableWindow(TRUE);
		}
		else
		{
			m_domainServerAddressCombox.EnableWindow(FALSE);
		}
		break;
	case IDC_CHECK36:
		if (TRUE==m_reserveDscAddressCheck)
		{
		//	m_reserveDscAddressEdit.EnableWindow(TRUE);
			m_reserveDscAddressCtrl.EnableWindow(TRUE);
		}
		else
		{
			//m_reserveDscAddressEdit.EnableWindow(FALSE);
			m_reserveDscAddressCtrl.EnableWindow(TRUE);
		}
		break;
	case IDC_CHECK37:
		if (TRUE==m_reservDscPortCheck)
		{
			m_reserveDscPortEdit.EnableWindow(TRUE);
		}
		else
		{
			m_reserveDscPortEdit.EnableWindow(FALSE);
		}
		break;
	case IDC_CHECK38:
		if(TRUE==m_allSelectCheck)
		{
			m_mainDscPortCheck=TRUE;
			//m_mainDscAddressEdit.EnableWindow(TRUE);
			m_mainDscAddressCtrl.EnableWindow(TRUE);

			m_DscDomainCheck=TRUE;
			m_dscDomainEdit.EnableWindow(TRUE);
			
			m_domainIntervalCheck=TRUE;
			m_domainIntervalEdit.EnableWindow(TRUE);
			
			m_mainDscPortCheck=TRUE;
			m_mainDscPortEdit.EnableWindow(TRUE);
			
			m_domainServerAddressCheck=TRUE;
			m_domainServerAddressCombox.EnableWindow(TRUE);
			
			m_reserveDscAddressCheck=TRUE;
		//	m_reserveDscAddressEdit.EnableWindow(TRUE);
			m_reserveDscAddressCtrl.EnableWindow(TRUE);

			m_reservDscPortCheck=TRUE;
			m_reserveDscPortEdit.EnableWindow(TRUE);
		}
		else
		{
			m_mainDscPortCheck=FALSE;
			//m_mainDscAddressEdit.EnableWindow(FALSE);
			m_mainDscAddressCtrl.EnableWindow(FALSE);

			m_DscDomainCheck=FALSE;
			m_dscDomainEdit.EnableWindow(FALSE);
			
			m_domainIntervalCheck=FALSE;
			m_domainIntervalEdit.EnableWindow(FALSE);
			
			m_mainDscPortCheck=FALSE;
			m_mainDscPortEdit.EnableWindow(FALSE);
			
			m_domainServerAddressCheck=FALSE;
			m_domainServerAddressCombox.EnableWindow(FALSE);
			
			m_reserveDscAddressCheck=FALSE;
		//	m_reserveDscAddressEdit.EnableWindow(FALSE);
			m_reserveDscAddressCtrl.EnableWindow(FALSE);
			
			m_reservDscPortCheck=FALSE;
			m_reserveDscPortEdit.EnableWindow(FALSE);
		}
		break;
	default:
		AfxMessageBox("PANEL3 HAVE A MISTAKE");
		break;
	}
	UpdateData(FALSE);
}
long CRemoteDtuSettingPanel3::Convert16NumberTo10(char *buf,int bufLen)
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
void CRemoteDtuSettingPanel3::QueryDtuParamFromRemoteDtu(CString dtuUserId)
{
	char buf[200];
	unsigned char nParaType[7]={0x0d,0x0e,0x0f,0x10,0x11,0x20,0x21};
	int nParamLenth;
	int i=0;
	nParamLenth=sizeof(buf);
	unsigned char nType;
	CString str;
	for (i=0;i<7;i++)
	{
		nParamLenth=200;
		nType=nParaType[i];
		if (GetParam(nType,buf,&nParamLenth)==0)
		{
			buf[nParamLenth]=0;
			if (nParamLenth!=0)
			{
				str.Format("%ld",Convert16NumberTo10(buf,nParamLenth));
			}
			switch (nType)
			{
			case 0x0d:
// 				if(nParamLenth!=0)
// 				{
// 					CString strTemp;
// 					str.Format("%ld",Convert16NumberTo10(buf,1));
// 					strTemp+=str+".";
// 					str.Format("%ld",Convert16NumberTo10(buf+1,1));
// 					strTemp+=str+".";
// 					str.Format("%ld",Convert16NumberTo10(buf+2,1));
// 					strTemp+=str+".";
// 					str.Format("%ld",Convert16NumberTo10(buf+3,1));
// 					strTemp+=str;
// 					str=strTemp;
// 				}
				if(4==nParamLenth)
					m_mainDscAddressCtrl.SetAddress(buf[0],buf[1],buf[2],buf[3]);
			//	m_mainDscAddressEdit.SetWindowText(str);
				break;
			case 0x0e:
				m_dscDomainEdit.SetWindowText(buf);
				break;
			case 0x0f:
				m_domainIntervalEdit.SetWindowText(str);
				break;
			case 0x10:
				m_mainDscPortEdit.SetWindowText(str);
				break;
			case 0x11:
				if(nParamLenth!=0)
				{
					CString strTemp;
					str.Format("%ld",Convert16NumberTo10(buf,1));
					strTemp+=str+".";
					str.Format("%ld",Convert16NumberTo10(buf+1,1));
					strTemp+=str+".";
					str.Format("%ld",Convert16NumberTo10(buf+2,1));
					strTemp+=str+".";
					str.Format("%ld",Convert16NumberTo10(buf+3,1));
					strTemp+=str;
					str=strTemp;
				}
				m_domainServerAddressCombox.AddString(str);
				break;
			case 0x20:
// 				if(nParamLenth!=0)
// 				{
// 					CString strTemp;
// 					str.Format("%ld",Convert16NumberTo10(buf,1));
// 					strTemp+=str+".";
// 					str.Format("%ld",Convert16NumberTo10(buf+1,1));
// 					strTemp+=str+".";
// 					str.Format("%ld",Convert16NumberTo10(buf+2,1));
// 					strTemp+=str+".";
// 					str.Format("%ld",Convert16NumberTo10(buf+3,1));
// 					strTemp+=str;
// 					str=strTemp;
// 				}
// 				m_reserveDscAddressEdit.SetWindowText(str);
				if(4==nParamLenth)
					m_reserveDscAddressCtrl.SetAddress(buf[0],buf[1],buf[2],buf[3]);
				break;
			case 0x21:
				m_reserveDscPortEdit.SetWindowText(str);
				break;
			default:
				AfxMessageBox("panel query have a mistake");
				break;
			}
			
		}
		else
		{
			AfxMessageBox(nType+"  Getparam failed and return ");
			return ;
		}
	}
	UpdateData(FALSE);
}

void CRemoteDtuSettingPanel3::GetSettingParamToRemoteDtu(vector<SETTINGPARAM> *vec)
{
	SETTINGPARAM setparam;

	if (TRUE==m_mainDscAddressCheck)
	{
		setparam.nType=0x0d;
		setparam.paramName="数据中心IP地址";
		CString str;
		m_mainDscAddressCtrl.GetWindowText(str);
		setparam.value=str;
		setparam.paramLen=5;
		m_mainDscAddressCtrl.GetAddress(setparam.ipAddress);
		vec->push_back(setparam);
	}

	if (TRUE==m_DscDomainCheck)
	{
		setparam.nType=0x0e;
		setparam.paramName="数据中心域名";
		CString str;
		m_dscDomainEdit.GetWindowText(str);
		setparam.value=str;
		setparam.paramLen=-1;
		vec->push_back(setparam);
	}
	if (TRUE==m_domainIntervalCheck)
	{
		setparam.nType=0x0f;
		setparam.paramName="域名检测时间间隔";
		CString str;
		m_domainIntervalEdit.GetWindowText(str);
		setparam.value=str;
		setparam.paramLen=2;
		vec->push_back(setparam);
	}
	if (TRUE==m_mainDscPortCheck)
	{
		setparam.nType=0x10;
		setparam.paramName="数据中心端口";
		CString str;
		m_mainDscPortEdit.GetWindowText(str);
		setparam.value=str;
		setparam.paramLen=2;
		vec->push_back(setparam);
	}
	if (TRUE==m_domainServerAddressCheck)
	{
		setparam.nType=0x11;
		setparam.paramName="域名服务器IP地址";
		CString str;
		m_domainServerAddressCombox.GetWindowText(str);
		setparam.value=str;

		//ip address get the four hex
		setparam.paramLen=5;
		unsigned char ipconvert[4];
		int count=0;
		int k=0;
		int val;
		for (int i=0;i<str.GetLength();i++)
		{
			if (str.GetAt(i)=='.')
			{
				ipconvert[count]=0;
				val=atoi((const char *)ipconvert);
				ipconvert[k++]=(unsigned char )val;
				memset(ipconvert,0,4*sizeof(unsigned char));
				count=0;
			}
			else
			{
				ipconvert[count++]=str.GetAt(i);
			}
		}
		ipconvert[count]=0;
		val=atoi((const char *)ipconvert);
		ipconvert[k++]=(unsigned char )val;
		//setparam.ipAddress
		vec->push_back(setparam);
	}
	if (TRUE==m_reserveDscAddressCheck)
	{
		setparam.nType=0x20;
		setparam.paramName="备中心IP地址";
		CString str;
		m_reserveDscAddressCtrl.GetWindowText(str);
		setparam.value=str;
		setparam.paramLen=5;
		m_mainDscAddressCtrl.GetAddress(setparam.ipAddress);
		vec->push_back(setparam);
	}
	if (TRUE==m_reservDscPortCheck)
	{
		setparam.nType=0x21;
		setparam.paramName="备中心端口";
		CString str;
		m_reserveDscPortEdit.GetWindowText(str);
		setparam.value=str;
		setparam.paramLen=2;
		vec->push_back(setparam);
	}


}

BOOL CRemoteDtuSettingPanel3::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
		m_mainDscPortCheck=FALSE;
	//m_mainDscAddressEdit.EnableWindow(FALSE);
	m_mainDscAddressCtrl.EnableWindow(FALSE);

	m_DscDomainCheck=FALSE;
	m_dscDomainEdit.EnableWindow(FALSE);
	
	m_domainIntervalCheck=FALSE;
	m_domainIntervalEdit.EnableWindow(FALSE);
	
	m_mainDscPortCheck=FALSE;
	m_mainDscPortEdit.EnableWindow(FALSE);
	
	m_domainServerAddressCheck=FALSE;
	m_domainServerAddressCombox.EnableWindow(FALSE);
	
	m_reserveDscAddressCheck=FALSE;
//	m_reserveDscAddressEdit.EnableWindow(FALSE);
	m_reserveDscAddressCtrl.EnableWindow(FALSE);
	
	m_reservDscPortCheck=FALSE;
	m_reserveDscPortEdit.EnableWindow(FALSE);
	m_allSelectCheck=FALSE;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
