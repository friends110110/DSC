// RemoteDtuSettingPanel4.cpp : implementation file
//

#include "stdafx.h"
#include "DSC.h"
#include "RemoteDtuSettingPanel4.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRemoteDtuSettingPanel4 dialog


CRemoteDtuSettingPanel4::CRemoteDtuSettingPanel4(CWnd* pParent /*=NULL*/)
	: CDialog(CRemoteDtuSettingPanel4::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRemoteDtuSettingPanel4)
	m_selectAllCheck = FALSE;
	//}}AFX_DATA_INIT
}


void CRemoteDtuSettingPanel4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRemoteDtuSettingPanel4)
	DDX_Control(pDX, IDC_COMBO6, m_verifyBitsCombox);
	DDX_Control(pDX, IDC_COMBO5, m_stopBitsCombox);
	DDX_Control(pDX, IDC_COMBO4, m_dataBitsCombox);
	DDX_Control(pDX, IDC_COMBO1, m_baudRateCombox);
	DDX_Control(pDX, IDC_COMBO7, m_streamControlCombox);
	DDX_Check(pDX, IDC_CHECK1, m_selectAllCheck);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRemoteDtuSettingPanel4, CDialog)
	//{{AFX_MSG_MAP(CRemoteDtuSettingPanel4)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_WM_SHOWWINDOW()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRemoteDtuSettingPanel4 message handlers

void CRemoteDtuSettingPanel4::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (TRUE==m_selectAllCheck)
	{
		this->m_baudRateCombox.EnableWindow(TRUE);
		this->m_dataBitsCombox.EnableWindow(TRUE);
		this->m_stopBitsCombox.EnableWindow(TRUE);
		this->m_verifyBitsCombox.EnableWindow(TRUE);
		this->m_streamControlCombox.EnableWindow(TRUE);
	}
	else
	{
		this->m_baudRateCombox.EnableWindow(FALSE);
		this->m_dataBitsCombox.EnableWindow(FALSE);
		this->m_stopBitsCombox.EnableWindow(FALSE);
		this->m_verifyBitsCombox.EnableWindow(FALSE);
		this->m_streamControlCombox.EnableWindow(FALSE);
	}
	UpdateData(FALSE);

}
void CRemoteDtuSettingPanel4::QueryDtuParamFromRemoteDtu(CString dtuUserId)
{
	char buf[50];
	int nParamLenth;
	int i=0;
	nParamLenth=50;//sizeof(buf);
	unsigned char nType=0X12;		//查询串口参数
	CString str;
	
	//	int nValue;

	nParamLenth=50;
	if (GetParam(nType,buf,&nParamLenth)==0)
	{
		buf[nParamLenth]=0;
		str=buf;
		CStringArray result;//=new CStringArray;
		int index;
		CString strTemp;
		while (TRUE)
		{
			index=str.Find(_T("-"));
			if (-1==index)
			{
				result.Add(str);
				break;
			}
			strTemp=str.Left(index);
			result.Add(strTemp);
			str=str.Right(str.GetLength()-index-1);
		}
		m_baudRateCombox.SetWindowText(result.GetAt(0));

		int number;
		number=atoi(result.GetAt(1));
		if(6==number)
			m_dataBitsCombox.SetCurSel(0);
		else if(7==number)
			m_dataBitsCombox.SetCurSel(1);
		else if(8==number)
			int x=m_dataBitsCombox.SetCurSel(2);
		else 
			{
				AfxMessageBox("数据位获取的数字出错");
				return ;
			}

		number=atoi(result.GetAt(2));
		if(1==number)
			m_stopBitsCombox.SetCurSel(0);
		else if(2==number)
			m_stopBitsCombox.SetCurSel(1);
		else
			{
				AfxMessageBox("停止位出错");
				return ;
			}
		number=atoi(result.GetAt(3));
		if (0==number)
		{
			if (strcmp(result.GetAt(3),"0"))
			{
				m_verifyBitsCombox.SetCurSel(0);
			}
			else
			{
				AfxMessageBox("验证位出错");
				return;
			}
		}
		else if(1==number)
		{
			m_verifyBitsCombox.SetCurSel(1);
		}
		else if (2==number)
		{
			m_verifyBitsCombox.SetCurSel(2);
		}
		else
		{
			AfxMessageBox("验证位出错");
			
			return;
		}
		number=atoi(result.GetAt(4));
		if(number<=0||number>=6)
		{
			AfxMessageBox("流控制参数出错");
			return;
		}
		CString strDefine[5]={"Xon/Xoff","硬件","无","半双工(RS485)","全双工(RS422)"};
		m_streamControlCombox.SetCurSel(number-1);
		

	}
	else
	{
		AfxMessageBox(nType+"  Getparam failed and return ");
		return ;
	}

	UpdateData(FALSE);
}

bool CRemoteDtuSettingPanel4::GetSettingParamToRemoteDtu(vector<SETTINGPARAM> *vec)
{
	SETTINGPARAM setparam;
	if (TRUE==m_selectAllCheck)
	{
		CString str[5];
		CString strcom;
		this->m_baudRateCombox.GetWindowText(str[0]);
		this->m_dataBitsCombox.GetWindowText(str[1]);
		this->m_stopBitsCombox.GetWindowText(str[2]);
		this->m_verifyBitsCombox.GetWindowText(str[3]);
		this->m_streamControlCombox.GetWindowText(str[4]);
		for (int i=0;i<5;i++)
		{
			if (strcmp(str[i],"")==0)
			{
				AfxMessageBox("serial port should be entered all");
				return false;
			} 
			if (i==3)
			{
				CString s;
				s.Format("%d",this->m_verifyBitsCombox.GetCurSel());
				strcom+=s;
			}
			else if(i==4)
			{
				CString s;
				s.Format("%d",this->m_streamControlCombox.GetCurSel());
				strcom+=s;
				break;
			}
			else
				strcom+=str[i];
			strcom+="-";
		}
		
		setparam.nType=0x12;
		setparam.value=strcom;
		setparam.paramName="串口参数";
		setparam.paramLen=-1;//strlen((LPSTR)(LPCTSTR)strcom);
		vec->push_back(setparam);
	}
	return true;
}

void CRemoteDtuSettingPanel4::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here

}

int CRemoteDtuSettingPanel4::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	return 0;
}

BOOL CRemoteDtuSettingPanel4::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->m_selectAllCheck=false;
	this->m_baudRateCombox.EnableWindow(FALSE);
	this->m_dataBitsCombox.EnableWindow(FALSE);
	this->m_stopBitsCombox.EnableWindow(FALSE);
	this->m_verifyBitsCombox.EnableWindow(FALSE);
	this->m_streamControlCombox.EnableWindow(FALSE);


	CString temp1[8]={"300","1200","4800","9600","19200","38400","57600","115200"};
	for (int i=0;i<8;i++)
	{
		this->m_baudRateCombox.AddString(temp1[i]);
	}

	this->m_dataBitsCombox.AddString("6");
	this->m_dataBitsCombox.AddString("7");
	this->m_dataBitsCombox.AddString("8");

	this->m_stopBitsCombox.AddString("1");
	this->m_stopBitsCombox.AddString("2");

	this->m_verifyBitsCombox.AddString("无");
	this->m_verifyBitsCombox.AddString("奇校验");
	this->m_verifyBitsCombox.AddString("偶校验");

	CString temp2[5]={"Xon/Xoff","硬件","无","半双工(RS485)","全双工(RS422)"};
	for ( i=0;i<5;i++)
		this->m_streamControlCombox.AddString(temp2[i]);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
