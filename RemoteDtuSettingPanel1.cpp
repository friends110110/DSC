// RemoteDtuSettingPanel1.cpp : implementation file
//

#include "stdafx.h"
#include "DSC.h"
#include "RemoteDtuSettingPanel1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRemoteDtuSettingPanel1 dialog


CRemoteDtuSettingPanel1::CRemoteDtuSettingPanel1(CWnd* pParent /*=NULL*/)
	: CDialog(CRemoteDtuSettingPanel1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRemoteDtuSettingPanel1)
	m_radio = -1;
	m_serviceCheck = FALSE;
	m_allSelectCheck = FALSE;
	m_pppUserCheck = FALSE;
	m_pppLoginPasswordCheck = FALSE;
	m_simCheck = FALSE;
	//}}AFX_DATA_INIT
}


void CRemoteDtuSettingPanel1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRemoteDtuSettingPanel1)
	DDX_Control(pDX, IDC_CHECK14, m_accessCheck);
	DDX_Control(pDX, IDC_EDIT15, m_simEdit);
	DDX_Control(pDX, IDC_EDIT14, m_accessEdit);
	DDX_Control(pDX, IDC_EDIT13, m_pppLoginPasswordEdit);
	DDX_Control(pDX, IDC_EDIT12, m_pppUserEdit);
	DDX_Control(pDX, IDC_EDIT11, m_serviceEdit);
	DDX_Radio(pDX, IDC_RADIO11, m_radio);
	DDX_Check(pDX, IDC_CHECK11, m_serviceCheck);
	DDX_Check(pDX, IDC_CHECK16, m_allSelectCheck);
	DDX_Check(pDX, IDC_CHECK12, m_pppUserCheck);
	DDX_Check(pDX, IDC_CHECK13, m_pppLoginPasswordCheck);
	DDX_Check(pDX, IDC_CHECK15, m_simCheck);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRemoteDtuSettingPanel1, CDialog)
	//{{AFX_MSG_MAP(CRemoteDtuSettingPanel1)
	ON_BN_CLICKED(IDC_RADIO11, OnRadio)
	ON_BN_CLICKED(IDC_RADIO12, OnRadio)

	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK11, IDC_CHECK11, OnCheckClicked)  
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK12, IDC_CHECK12, OnCheckClicked)  
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK13, IDC_CHECK13, OnCheckClicked)  
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK14, IDC_CHECK14, OnCheckClicked)  
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK15, IDC_CHECK15, OnCheckClicked)  
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK16, IDC_CHECK16, OnCheckClicked)  

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRemoteDtuSettingPanel1 message handlers

void CRemoteDtuSettingPanel1::OnRadio() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (1==m_radio)				//CDMA disable 接入点名称
	{
		m_accessCheck.EnableWindow(FALSE);
		m_accessEdit.EnableWindow(FALSE);
	}
	else if (0==m_radio)
	{
		m_accessCheck.EnableWindow(TRUE);
	}
	UpdateData(FALSE);
}

BOOL CRemoteDtuSettingPanel1::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_radio=0;
	m_serviceCheck=FALSE;
	m_serviceEdit.EnableWindow(FALSE);
	
	m_pppUserCheck=FALSE;
	m_pppUserEdit.EnableWindow(FALSE);
	
	m_pppLoginPasswordCheck=FALSE;
	m_pppLoginPasswordEdit.EnableWindow(FALSE);
	
	m_accessCheck.SetCheck(FALSE);
	m_accessEdit.EnableWindow(FALSE);
	
	m_simCheck=FALSE;
	m_simEdit.EnableWindow(FALSE);
	m_allSelectCheck=FALSE;


	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRemoteDtuSettingPanel1::OnCheckClicked(UINT uId)
{
	UpdateData(TRUE);
	switch(uId)
	{
	case IDC_CHECK11:	//service
		if (TRUE==m_serviceCheck)
		{
			m_serviceEdit.EnableWindow(TRUE);
		}
		else
		{
			m_serviceEdit.EnableWindow(FALSE);
		}
		break;
	case IDC_CHECK12:	//ppp user
		if (TRUE==m_pppUserCheck)
		{
			m_pppUserEdit.EnableWindow(TRUE);
		}
		else
		{
			m_pppUserEdit.EnableWindow(FALSE);
		}
		break;
	case IDC_CHECK13:	//ppp login password
		if (TRUE==m_pppLoginPasswordCheck)
		{
			m_pppLoginPasswordEdit.EnableWindow(TRUE);
		}
		else
		{
			m_pppLoginPasswordEdit.EnableWindow(FALSE);
		}
		break;
	case IDC_CHECK14:	//access
		if (TRUE==m_accessCheck.GetCheck())
		{
			m_accessEdit.EnableWindow(TRUE);
		}
		else
		{
			m_accessEdit.EnableWindow(FALSE);
		}
		break;
	case IDC_CHECK15:	//Sim Pin
		if (TRUE==m_simCheck)
		{
			m_simEdit.EnableWindow(TRUE);
		}
		else
		{
			m_simEdit.EnableWindow(FALSE);
		}
		break;
	case IDC_CHECK16:	//all selected
		if (TRUE==m_allSelectCheck)
		{
			m_serviceCheck=TRUE;
			m_serviceEdit.EnableWindow(TRUE);

			m_pppUserCheck=TRUE;
			m_pppUserEdit.EnableWindow(TRUE);

			m_pppLoginPasswordCheck=TRUE;
			m_pppLoginPasswordEdit.EnableWindow(TRUE);

			m_accessCheck.SetCheck(TRUE);
			m_accessEdit.EnableWindow(TRUE);

			m_simCheck=TRUE;
			m_simEdit.EnableWindow(TRUE);
		}
		else
		{
			m_serviceCheck=FALSE;
			m_serviceEdit.EnableWindow(FALSE);
			
			m_pppUserCheck=FALSE;
			m_pppUserEdit.EnableWindow(FALSE);
			
			m_pppLoginPasswordCheck=FALSE;
			m_pppLoginPasswordEdit.EnableWindow(FALSE);
			
			m_accessCheck.SetCheck(FALSE);
			m_accessEdit.EnableWindow(FALSE);
			
			m_simCheck=FALSE;
			m_simEdit.EnableWindow(FALSE);
		}
		break;
	default:
		AfxMessageBox("check has a mistake");
		break;
	}
	UpdateData(FALSE);
}


void CRemoteDtuSettingPanel1::QueryDtuParamFromRemoteDtu(CString dtuUserId)
{

	char buf[50];
	
	int nParamLenth;
	int i=0;
	nParamLenth=50;//sizeof(buf);
	unsigned char nType=0x00;
	for (i=0;i<5;i++)
	{
		nType++;
		nParamLenth=50;
		if (GetParam(nType,buf,&nParamLenth)==0)
		{
			switch (nType)
			{
			case 0x01:
				m_serviceEdit.SetWindowText(buf);
				break;
			case 0x02:
				m_pppUserEdit.SetWindowText(buf);
				break;
			case 0x03:
				m_pppLoginPasswordEdit.SetWindowText(buf);
				break;
			case 0x04:
				m_accessEdit.SetWindowText(buf);
				break;
			case 0x05:
				m_simEdit.SetWindowText(buf);
				break;
			default:
				AfxMessageBox("panel query have a mistake");
			}

		}
		else
		{
			AfxMessageBox(nType+"  Getparam failed ");
			return ;
		}
	}
	UpdateData(FALSE);
}


void CRemoteDtuSettingPanel1::GetSettingParamToRemoteDtu(vector<SETTINGPARAM> *vec)
{
	SETTINGPARAM setparam;
	setparam.paramLen=-1;
//	uint8 ntype=0x01;
	if (TRUE==m_serviceCheck)
	{
		setparam.nType=0x01;
		setparam.paramName="服务代码";
		CString str;
		m_serviceEdit.GetWindowText(str);
		setparam.value=str;
		vec->push_back(setparam);
	}
	if (TRUE==m_pppUserCheck)
	{
		setparam.nType=0x02;
		setparam.paramName="PPP用户名";
		CString str;
		m_pppUserEdit.GetWindowText(str);
		setparam.value=str;
		vec->push_back(setparam);
	}
	if (TRUE==m_pppLoginPasswordCheck)
	{
		setparam.nType=0x03;
		setparam.paramName="PPP用户密码";
		CString str;
		m_pppLoginPasswordEdit.GetWindowText(str);
		setparam.value=str;
		vec->push_back(setparam);
	}
	if (TRUE==m_accessCheck.GetCheck())
	{
		setparam.nType=0x04;
		setparam.paramName="接入点";
		CString str;
		m_accessEdit.GetWindowText(str);
		setparam.value=str;
		vec->push_back(setparam);
	}
	if (TRUE==m_simCheck)
	{
		setparam.nType=0x05;
		setparam.paramName="SIM PIN";
		CString str;
		m_simEdit.GetWindowText(str);
		setparam.value=str;
		vec->push_back(setparam);
	}
}