// SendSettingParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DSC.h"
#include "SendSettingParamDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSendSettingParamDlg dialog


CSendSettingParamDlg::CSendSettingParamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSendSettingParamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSendSettingParamDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

// 
// CSendSettingParamDlg::CSendSettingParamDlg(vector<SETTINGPARAM>* vec,CWnd* pParent /*=NULL*/)
// : CDialog(CSendSettingParamDlg::IDD, pParent)
// {
// 	//{{AFX_DATA_INIT(CSendSettingParamDlg)
// 	// NOTE: the ClassWizard will add member initialization here
// 	//}}AFX_DATA_INIT
// 	this->m_vector=vec;
// }


void CSendSettingParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSendSettingParamDlg)
	DDX_Control(pDX, IDC_LIST1, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSendSettingParamDlg, CDialog)
	//{{AFX_MSG_MAP(CSendSettingParamDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSendSettingParamDlg message handlers

BOOL CSendSettingParamDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//设置格式
	m_list.ModifyStyle( 0, LVS_REPORT );               // 报表模式  
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);  
                                                      // 间隔线+行选中  
	//插入表头
	m_list.InsertColumn(0,"参数名称");  
	m_list.InsertColumn(1,"值");  

	//设置列宽
	CRect rect;  
	m_list.GetClientRect(rect); //获得当前客户区信息  
	m_list.SetColumnWidth(0, rect.Width() / 2); //设置列的宽度。  
	m_list.SetColumnWidth(1, rect.Width() / 2);  


	int i=0;
	int count=m_vector->size();
	SETTINGPARAM setParam;
	for (i=0;i<count;i++)
	{
		setParam=m_vector->at(i);
		m_list.InsertItem(i,setParam.paramName);
		m_list.SetItemText(i,1,setParam.value);
	}
//	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CSendSettingParamDlg::setSendSettingData(vector<SETTINGPARAM>* vec)
{
	m_vector=vec;
}
void CSendSettingParamDlg::setUserId(unsigned char *userId)
{
	m_userId=userId;
}
void CSendSettingParamDlg::OnOK() 
{
	// TODO: Add extra validation here
	ClearParam();
	//设置需要的参数
//	unsigned char buf[100];
	int 	nParamLenth;
	int	iErrorCode;
	vector<SETTINGPARAM>::iterator ite;
	char val[200];
	unsigned long number;
	for (ite=m_vector->begin();ite<m_vector->end();ite++)
	{
		if (1==ite->paramLen)
		{
			number=(unsigned long )atol((LPCTSTR)(ite->value));
			val[0]=(unsigned char)number;
			nParamLenth=1;
		}
		else if (2==ite->paramLen)
		{
			//strcpy(val,(char *)((unsigned short )atoi((LPCTSTR)(ite->value))));
			number=(unsigned long )atol((LPCTSTR)(ite->value));
			val[1]=(unsigned char)number;
			number=number>>8;
			val[0]=(unsigned char)number;
			nParamLenth=2;
		}
		else if(4==ite->paramLen)			// other len
		{
		//	strcpy(val,(char *)((unsigned long)atol((LPCTSTR)(ite->value))));
			number=(unsigned long)atol((LPCTSTR)(ite->value));
			val[3]=(unsigned char)number;
			number=number>>8;
			val[2]=(unsigned char)number;
			number=number>>8;
			val[1]=(unsigned char)number;
			number=number>>8;
			val[0]=(unsigned char)number;
			nParamLenth=4;
		}
		else if (5==ite->paramLen)			//ip address dword
		{
			//strcpy(val,(char *)((unsigned long)(ite->ipAddress)));
			number=ite->ipAddress;
			val[3]=(unsigned char)number;
			number=number>>8;
			val[2]=(unsigned char)number;
			number=number>>8;
			val[1]=(unsigned char)number;
			number=number>>8;
			val[0]=(unsigned char)number;
			nParamLenth=4;
		}
		else if(-1==ite->paramLen)			//character
		{
			strcpy(val,ite->value);
			nParamLenth=ite->value.GetLength();
		}
		if (0==SetParam(ite->nType,val,nParamLenth,&iErrorCode))
		{
			//发送成功
		}
		else		
		{// return -1
			if (iErrorCode!=-1)		//达到上限值
			{
				if(0!=do_update_param(m_userId,NULL))
				{	//发送失败
					//检查SOCKET连接是否正常
					AfxMessageBox("更新参数失败");//do_update_param failed");
					break;
				}
				else
				{
					ClearParam();
				}
			}
		}
	}
//	do_update_param(m_userId,NULL);
	if(0!=do_update_param(m_userId,NULL))
	{	//发送失败
		//检查SOCKET连接是否正常
		AfxMessageBox("更新参数失败");//do_update_param failed");
		
	}
	ClearParam();

	CDialog::OnOK();
}

void CSendSettingParamDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
