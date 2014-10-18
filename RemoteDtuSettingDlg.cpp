// RemoteDtuSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DSC.h"
#include "RemoteDtuSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRemoteDtuSettingDlg dialog


CRemoteDtuSettingDlg::CRemoteDtuSettingDlg(CWnd* pParent /*=NULL*/)
: CDialog(CRemoteDtuSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRemoteDtuSettingDlg)
	m_timeoutCheck = FALSE;
	//}}AFX_DATA_INIT
}


void CRemoteDtuSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRemoteDtuSettingDlg)
	DDX_Control(pDX, IDC_TIMEEDIT, m_timeoutEdit);
	DDX_Control(pDX, IDC_CHOOSEDTUCOMBO, m_chooseDtuCombo);
	DDX_Control(pDX, IDC_TAB, m_tab);
	DDX_Check(pDX, IDC_CHECK1, m_timeoutCheck);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRemoteDtuSettingDlg, CDialog)
//{{AFX_MSG_MAP(CRemoteDtuSettingDlg)
		ON_WM_SHOWWINDOW()
		ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
		ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
		ON_WM_TIMER()
		ON_WM_CLOSE()
		ON_CBN_SELENDOK(IDC_CHOOSEDTUCOMBO, OnSelendokChoosecombo)


		ON_CONTROL_RANGE(BN_CLICKED,IDC_CHECKBTN , IDC_CHECKBTN, OnButtonClicked)  
		ON_CONTROL_RANGE(BN_CLICKED,IDC_SETBTN , IDC_SETBTN, OnButtonClicked)  
		ON_CONTROL_RANGE(BN_CLICKED,IDC_DEFAULTBTN , IDC_DEFAULTBTN, OnButtonClicked)  
		ON_CONTROL_RANGE(BN_CLICKED,IDC_CANCELBTN , IDC_CANCELBTN, OnButtonClicked)  
		ON_CONTROL_RANGE(BN_CLICKED,IDC_REBOOTBTN , IDC_REBOOTBTN, OnButtonClicked)  
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRemoteDtuSettingDlg message handlers
//timer event
UINT const   timer_nEvent1=50;
UINT const   timer_nEvent2=51;

BOOL CRemoteDtuSettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->m_tab.InsertItem(0,"移动服务设置");
	this->m_tab.InsertItem(1,"DTU设置");
	this->m_tab.InsertItem(2,"数据中心设置");
	this->m_tab.InsertItem(3,"串口设置");
//	this->m_tab.InsertItem(4,"特殊设置");
//	this->m_tab.InsertItem(5,"其他信息");
	
	
	this->m_panel1.Create(IDD_SettingPanel1,this->GetDlgItem(IDC_TAB));
	this->m_panel2.Create(IDD_SettingPanel2,this->GetDlgItem(IDC_TAB));
	this->m_panel3.Create(IDD_SettingPanel3,this->GetDlgItem(IDC_TAB));
	this->m_panel4.Create(IDD_SettingPanel4,this->GetDlgItem(IDC_TAB));	


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CRemoteDtuSettingDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	CRect rect; 
	m_tab.GetClientRect(&rect); 
	rect.top+=20; 
	rect.bottom-=4; 
	rect.left+=4; 
	rect.right-=4; 
	
	this->m_panel1.MoveWindow(&rect);
	this->m_panel2.MoveWindow(&rect);
	this->m_panel3.MoveWindow(&rect);
	this->m_panel4.MoveWindow(&rect);
	this->m_tab.SetCurSel(0);
	this->showItem();


//	this.m_chooseDtuCombo.dropdo = ComboBoxStyle.DropDownList;
// 	this->m_chooseDtuCombo.AddString("wfwf");
// 	this->m_chooseDtuCombo.AddString("sfd");
// 	if(-1==this->m_chooseDtuCombo.GetCurSel())
// 	{
		((CButton *)GetDlgItem(IDC_SETBTN))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_CHECKBTN))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_REBOOTBTN))->EnableWindow(FALSE);
// 	}

//	if(FALSE==AddDtuInformationToSelectCombo())
	{
		//m_timenEvent=10;
		//the first show window perform
		AddDtuInformationToSelectCombo();

		//and later setTimer and onTimer detect the dtu state
		SetTimer(timer_nEvent1,40000,NULL);	
	}
}
bool CRemoteDtuSettingDlg::AddDtuInformationToSelectCombo()
{
	int i;
	user_info ui;
	int iMaxDTUAmount=get_max_user_amount();
	time_t t_now,t_update;
	bool haveDtuflag=false;
	//get option param from mainframe 应该要让我获取
	OPTION option= ((CMainFrame *)AfxGetMainWnd())->m_option;
	//long m_iOffLineTime=120;
	//m_chooseDtuCombo.Clear();
	//暂存当前的 comboxStr
	CString currentComboxStrTemp;
	this->m_chooseDtuCombo.GetWindowText(currentComboxStrTemp);
	int currentComboxStrIndexTemp=-1;
	int addToComboxNumber=-1;			//0 represent 1 num, because used for index (it based 0)
	m_chooseDtuCombo.ResetContent();
	
	for (i=0;i<iMaxDTUAmount;i++)
	{
		ui.m_status=0;
		//我的理解 调用get_user_at 并不能判断dtu是否真的在线，只有时间前后和设定的阈值才能判断
		get_user_at(i,&ui);
		if(1==ui.m_status)// 1 在线；0 不在线
		{
			//取当前系统时间
			t_now=time(NULL);
			//取m_update_time的时间
			t_update=*((time_t *)ui.m_update_time);
			//用当前时间比较最后一次更新的时间
			if ((t_now-t_update)>=option.m_iOffLineTime)//m_iOffLineTime
			{
				//比较的结果大于指定的时间，则认为该DTU已经不在线，执行do_close_one_user
			//	AfxMessageBox("the current dtu has m_iOffLineTime");
			//	do_close_one_user((unsigned char *)ui.m_userid,NULL);
			}                		
			else
			{
				addToComboxNumber++;
				
				if (0!=currentComboxStrTemp.GetLength()&&strcmp(ui.m_userid,currentComboxStrTemp)==0)
				{
					currentComboxStrIndexTemp=addToComboxNumber;
				}
				this->m_chooseDtuCombo.AddString(ui.m_userid);
				haveDtuflag=true;
			}
		}
	}
	this->m_chooseDtuCombo.SetCurSel(currentComboxStrIndexTemp);
	if (-1==addToComboxNumber)
	{
		//AfxMessageBox("no one add to combo");
		return false;
	}
	else		//随时会断开网络 所以延时找DTU	//找到一次就行了
	{
		//this->KillTimer(m_timenEvent);
		return true;
	}
	
}
void CRemoteDtuSettingDlg::showItem()
{
	int index=this->m_tab.GetCurSel();

	this->m_panel1.ShowWindow(FALSE);
	this->m_panel2.ShowWindow(FALSE);
	this->m_panel3.ShowWindow(FALSE);
	this->m_panel4.ShowWindow(FALSE);
	switch(index)
	{
	case 0:
		this->m_panel1.ShowWindow(TRUE);
		break;
	case 1:
		this->m_panel2.ShowWindow(TRUE);
		break;
	case 2:
		this->m_panel3.ShowWindow(TRUE);
		break;
	case 3:
		this->m_panel4.ShowWindow(TRUE);
		break;
	default:
		break;
	}
}

void CRemoteDtuSettingDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	this->showItem();
	*pResult = 0;
}

void CRemoteDtuSettingDlg::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(TRUE==m_timeoutCheck)
	{
		m_timeoutEdit.EnableWindow(TRUE);
	}
	else
	{
		m_timeoutEdit.EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}
void CRemoteDtuSettingDlg::doQueryDtuParam(char userId[12])
{
// 	if (-1==this->m_chooseDtuCombo.GetCurSel())
// 	{
// 		return;
// 	}
	CString dtuUserId;
	this->m_chooseDtuCombo.GetWindowText(dtuUserId);
	if (strcmp(userId,dtuUserId)==0)		//用于处理多机器的情况
	{
		this->m_panel1.QueryDtuParamFromRemoteDtu(userId);
		this->m_panel2.QueryDtuParamFromRemoteDtu(userId);
		this->m_panel3.QueryDtuParamFromRemoteDtu(userId);
		this->m_panel4.QueryDtuParamFromRemoteDtu(userId);
		//查询成功后关闭查询参数的计时器
		KillTimer(timer_nEvent2);
	}


}
void CRemoteDtuSettingDlg::sendQueryCommand()
{
	if (-1==this->m_chooseDtuCombo.GetCurSel())
	{
		return;
	}
	CString dtuUserId;
	this->m_chooseDtuCombo.GetWindowText(dtuUserId);
	if (0==dtuUserId.GetLength())
	{
		return;
	}
	//SOcket连接不通
	if (0!=do_read_param((unsigned char*)(LPCTSTR)dtuUserId,NULL,NULL))
	{
		//检查SOCKET连接是否正常
		AfxMessageBox("无网络连接 ");
		return;
	}
	else
	{
		//AfxMessageBox("do_read_param success");
	}
	//如果需要，可以开启一个定时器，检查DTU回应是否超时 has realized
	if (TRUE==m_timeoutCheck)
	{
		//DTU包更新时间，DSC接收到该DTU最近一个包的时间，使用前四字
		CString timeoutStr;
		//char m_update_time[20];
		m_timeoutEdit.GetWindowText(timeoutStr);
		if (0L==atol(timeoutStr))			//number error
		{
			AfxMessageBox("请输入合法字符");//("set timeout error,not number");
			return ;
		}
		//strcpy(m_update_time,timeoutStr);
		//get from the timeoutEdit
		m_timeout=(time_t)atol(timeoutStr);//(time_t)m_update_time;
		//开启查询参数的计时器
		this->SetTimer(timer_nEvent2,m_timeout,NULL);
	}
	
}

void CRemoteDtuSettingDlg::doDefaultParam()
{
	this->m_panel1.m_serviceEdit.SetWindowText("*99***1#");
	this->m_panel1.m_accessEdit.SetWindowText("CMNET");
	
	this->m_panel2.m_localPortEdit.SetWindowText("5001");
	this->m_panel2.m_onlineEdit.SetWindowText("40");
	this->m_panel2.m_maxPackageEdit.SetWindowText("256");
	this->m_panel2.m_reconnectIntervalEdit.SetWindowText("0");
	this->m_panel2.m_debugStateEdit.SetWindowText("1");
	this->m_panel2.m_lastPackageIntervalEdit.SetWindowText("600");
	this->m_panel2.m_packageSplitEdit.SetWindowText("0");
	
	//this->m_panel3.mainDsc.SetWindowText("0.0.0.0");
	this->m_panel3.m_mainDscAddressCtrl.SetAddress(0,0,0,0);
	this->m_panel3.m_dscDomainEdit.SetWindowText("www.mdtu.com");
	this->m_panel3.m_domainIntervalEdit.SetWindowText("0");
	this->m_panel3.m_mainDscPortEdit.SetWindowText("5002");
	
	this->m_panel3.m_domainServerAddressCombox.SetWindowText("0.0.0.0");
	this->m_panel3.m_domainServerAddressCombox.AddString("202.96.133.134");



	this->m_panel4.m_baudRateCombox.SetCurSel(6);//57600
	
	
	this->m_panel4.m_dataBitsCombox.SetCurSel(2);
	

	this->m_panel4.m_stopBitsCombox.SetCurSel(0);


	this->m_panel4.m_verifyBitsCombox.SetCurSel(0);


	this->m_panel4.m_streamControlCombox.SetCurSel(2);

}
void CRemoteDtuSettingDlg::doSettingParam()
{
	vector<SETTINGPARAM> *vec=new vector<SETTINGPARAM>;
	vector<SETTINGPARAM> *selectVec=new vector<SETTINGPARAM>;
	vector<SETTINGPARAM>::iterator ite;
	this->m_panel1.GetSettingParamToRemoteDtu(vec);
	this->m_panel2.GetSettingParamToRemoteDtu(vec);
	this->m_panel3.GetSettingParamToRemoteDtu(vec);
	
	if(false==this->m_panel4.GetSettingParamToRemoteDtu(vec))
		return;
	int i=0;

	for (ite=vec->begin();ite<vec->end();ite++)
	{
		if (0!=ite->value.GetLength())
		{
			selectVec->push_back(*ite);
		}
	}
// 	int count=vec->size();
// 	for (int i=0;i<count;i++)
// 	{
// 		if (0==vec->at(i).value.GetLength())
// 		{
// 			vec->erase(vec->begin()+i);
// 		}
// 	}

	CSendSettingParamDlg sendParamDlg;
	sendParamDlg.setSendSettingData(selectVec);
	CString user;
	this->m_chooseDtuCombo.GetWindowText(user);
	sendParamDlg.setUserId((unsigned char*)(LPSTR)(LPCTSTR)user);
	sendParamDlg.DoModal();

	vector<SETTINGPARAM>().swap(*vec);
	vector<SETTINGPARAM>().swap(*selectVec);
//	vec.clear();

	//vec->clear();
//	selectVec->clear();
	//vec->release();
}
void CRemoteDtuSettingDlg::OnButtonClicked(UINT uId)
{
	// only cancel and default button is useful when it is -1;
	if (-1==this->m_chooseDtuCombo.GetCurSel())
	{
		
		if (uId==IDC_CANCELBTN||uId==IDC_DEFAULTBTN)
		{
		//	GetDlgItem(IDC)
		}
		else
		{
	//		return;
		}

	}
	switch(uId)
	{
	case IDC_CHECKBTN:
		sendQueryCommand();
		break;
	case IDC_SETBTN:
		doSettingParam();
		break;
	case IDC_CANCELBTN:
		CRemoteDtuSettingDlg::OnCancel();
		break;
	case IDC_DEFAULTBTN:
		doDefaultParam();
		break;
	case IDC_REBOOTBTN:
	
		if (-1!=this->m_chooseDtuCombo.GetCurSel())
		{
			CString UserIdstr;
			m_chooseDtuCombo.GetWindowText(UserIdstr);
			if(-1==do_close_one_user2((unsigned char *)(LPCTSTR)UserIdstr,NULL))
			{
				AfxMessageBox("cannot find the special dtu");
			}
		}

		break;
	default:
		AfxMessageBox("btn have a mistake");
		break;
	}
}

void CRemoteDtuSettingDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(timer_nEvent1==nIDEvent)
		AddDtuInformationToSelectCombo();
	else if (timer_nEvent2==nIDEvent)
	{
		if (TRUE==this->m_timeoutCheck)
		{
			//int i;//,iMaxDTUAmount;
			user_info ui;
			time_t t_now,t_update;
			CString comboxStr;
			m_chooseDtuCombo.GetWindowText(comboxStr);
			if(0==get_user_info((unsigned char *)(LPSTR)(LPCTSTR)comboxStr,&ui))//成功查询
			{
				//获取指定位置的DTU信息
				if (1==ui.m_status)
				{			
					//取当前系统时间
					t_now=time(NULL);
					//取m_update_time的时间
					t_update=*((time_t *)ui.m_update_time);
					//用当前时间比较最后一次更新的时间
					if ((t_now-t_update)>=m_timeout)
					{
						KillTimer(timer_nEvent2);
						//比较的结果大于指定的时间，则认为该DTU已经不在线，执行do_close_one_user
						AfxMessageBox("查询超时");//current dtu has query timeout,so I show it");
						//do_close_one_user((unsigned char *)ui.m_userid,NULL);

					}                		
				}
			}
			else
			{
				AfxMessageBox("查询失败");//cuttent dtu get_user_info failed,so I show combox reffered");
				//do_close_one_user((unsigned char *)comboxStr,NULL);
			}
			
	}
	}

	CDialog::OnTimer(nIDEvent);
}


void CRemoteDtuSettingDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(timer_nEvent1);
	KillTimer(timer_nEvent2);
	CDialog::OnClose();
}
void CRemoteDtuSettingDlg::OnSelendokChoosecombo() 
{
	// TODO: Add your control notification handler code here
// 	CString str;
// 	str.Format("%d",	this->m_chooseDtuCombo.GetCurSel());
// 	
// 	AfxMessageBox(str);
	//有效的被选中
	((CButton *)GetDlgItem(IDC_SETBTN))->EnableWindow(TRUE);
	((CButton *)GetDlgItem(IDC_CHECKBTN))->EnableWindow(TRUE);
	((CButton *)GetDlgItem(IDC_REBOOTBTN))->EnableWindow(TRUE);

}