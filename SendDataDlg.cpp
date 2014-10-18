// SendDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DSC.h"
#include "SendDataDlg.h"
#include "wcomm_dll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSendDataDlg dialog

CSendDataDlg::CSendDataDlg(CWnd* pParent,CTreeCtrl* ptc)
	: CDialog(CSendDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSendDataDlg)
	m_hex = FALSE;
	//}}AFX_DATA_INIT
	m_pTree=NULL;
	if (ptc)
		m_pTree=ptc;
}


void CSendDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSendDataDlg)
	DDX_Control(pDX, IDC_CMUSERLIST, m_cmuserlist);
	DDX_Check(pDX, IDC_CKHEX, m_hex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSendDataDlg, CDialog)
	//{{AFX_MSG_MAP(CSendDataDlg)
	ON_BN_CLICKED(IDC_BQUIT, OnBquit)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_CKHEX, OnCkhex)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BSEND, OnBsend)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CKAUTOSEND, OnCkautosend)
	ON_BN_CLICKED(IDC_BCR, OnBcr)
	ON_BN_CLICKED(IDC_BLF, OnBlf)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSendDataDlg message handlers

void CSendDataDlg::AddUserId()
{	
	HTREEITEM hti;
	CString str;

	if ((m_pTree) && (m_pTree->GetFirstVisibleItem()))
	{
		hti=m_pTree->GetChildItem(m_pTree->GetFirstVisibleItem());
	    while (hti)
		{
    		str=m_pTree->GetItemText(hti);
	        m_cmuserlist.AddString(str.GetBuffer(0));
		    hti=m_pTree->GetNextItem(hti,TVGN_NEXT);
		}
	}

	m_cmuserlist.SetWindowText(m_userid);
}

void CSendDataDlg::SetUserId(char *userid)
{
	m_userid=userid;
}

void CSendDataDlg::OnBquit() 
{
	
	CButton* pButton=(CButton *)GetDlgItem(IDC_CKAUTOSEND);
	if (1==pButton->GetCheck())
	{
		//this->SetTimer(1,interval,NULL);
		this->KillTimer(1);
	//	AfxMessageBox("killtimer");
	}

    this->OnCancel();	
}

void CSendDataDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
    this->AddUserId();	
	this->SendMessage(WM_SIZE);
	((CEdit *)GetDlgItem(IDC_INTERVAL))->SetWindowText("200");
}

int  CSendDataDlg::StrToHex(char *str,char *hex)
{
	int i,slen;

	slen=strlen(str);
	for (i=0;i<slen;i++)
	{
		sprintf(hex+i*3,"%02X ",str[i]&0xFF);
	}
	return 0;
}

int  CSendDataDlg::HexToStr(char *hex,char *str)
{
	int i,hlen,iData,iFlag;
	char ch;

	iData=0;
	iFlag=0;
	hlen=strlen(hex);

	for (i=0;i<hlen;i++)
	{
		if (' '==hex[i]) 
			continue;

		ch=hex[i];

		if ((ch>='0') && (ch<='9'))
		{
			ch-=0x30;
		}
		else if ((ch>='a') && (ch<='f'))
		{
			ch-=0x57;
		}
		else if ((ch>='A') && (ch<='F'))
		{
			ch-=0x37;
		}
		else
		{
			*str='\0';
		    return -1;
		}

		if (0==iFlag)
		{
			iData=ch;
			iFlag=1;
		}
		else
		{
			iData=(iData<<4) +ch;
			iFlag=0;
			*str++=iData;
		}
	}
	*str='\0';
	return 0;
}

void CSendDataDlg::OnCkhex() 
{
	char *hexbuf,*strbuf;
	int  iLen;
	CEdit *pEdit=(CEdit *)GetDlgItem(IDC_EDSHOW);

    this->UpdateData(TRUE);

	//-----------------------------------
	//判断编辑框是否为空，空则直接返回
	CString txt;
	pEdit->GetWindowText(txt);
	
	if(txt == "")
	{
		//AfxMessageBox(txt);
		return;
	}
	//-----------------------------------

	hexbuf=new char[3096];
	strbuf=new char[1024];
	
	

	if (m_hex)
	{
		iLen=pEdit->GetWindowText(strbuf,1024);
		strbuf[iLen]='\0';
		if (0==this->StrToHex(strbuf,hexbuf))
			pEdit->SetWindowText(hexbuf);
		else
		{
			m_hex=FALSE;
			UpdateData(FALSE);
		}
	}
	else
	{
		iLen=pEdit->GetWindowText(hexbuf,3096);
		hexbuf[iLen]='\0';
		if (0==this->HexToStr(hexbuf,strbuf))
			pEdit->SetWindowText(strbuf);
		else
		{
			m_hex=TRUE;
			UpdateData(FALSE);
		}
	}

	delete[] hexbuf;
	delete[] strbuf;
}

void CSendDataDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if (!this->IsWindowVisible())
		return;

	CRect rt;
	CEdit *pEdit;
	CButton *pButton;

    pEdit=(CEdit *)GetDlgItem(IDC_EDSHOW);
	pEdit->GetWindowRect(&rt);	
    pEdit->MoveWindow(11,36,cx-25,cy-77);

	pEdit=(CEdit *)GetDlgItem(IDC_EDSENDCOUNT);
	pEdit->GetWindowRect(&rt);
	pEdit->MoveWindow(cx-104,9,rt.Width(),rt.Height());

    CStatic *pStatic=(CStatic *)GetDlgItem(IDC_STATIC1);
	pStatic->GetWindowRect(&rt);
	pStatic->MoveWindow(cx-164,13,rt.Width(),rt.Height());

	pButton=(CButton *)GetDlgItem(IDC_CKHEX);
	pButton->GetWindowRect(&rt);
	pButton->MoveWindow(12,cy-rt.Height()-10,rt.Width(),rt.Height());
	
	pButton=(CButton *)GetDlgItem(IDC_BCR);
	pButton->GetWindowRect(&rt);
	pButton->MoveWindow(60,cy-rt.Height()-10,rt.Width(),rt.Height());

	pButton=(CButton *)GetDlgItem(IDC_BLF);
	pButton->GetWindowRect(&rt);
	pButton->MoveWindow(90,cy-rt.Height()-10,rt.Width(),rt.Height());

	pButton=(CButton *)GetDlgItem(IDC_CKAUTOSEND);
	pButton->GetWindowRect(&rt);
	pButton->MoveWindow(120,cy-rt.Height()-10,rt.Width(),rt.Height());

	pButton=(CButton *)GetDlgItem(IDC_INTERVAL);
	pButton->GetWindowRect(&rt);
	pButton->MoveWindow(180,cy-rt.Height()-10,rt.Width(),rt.Height());

	pButton=(CButton *)GetDlgItem(IDC_BQUIT);
	pButton->GetWindowRect(&rt);
	pButton->MoveWindow(cx-74,cy-rt.Height()-10,rt.Width(),rt.Height());

	pButton=(CButton *)GetDlgItem(IDC_BSEND);
	pButton->GetWindowRect(&rt);
	pButton->MoveWindow(cx-144,cy-rt.Height()-10,rt.Width(),rt.Height());
}

void CSendDataDlg::OnBsend() 
{	
    char *strbuf,*hexbuf;
	char userid[12];

	this->UpdateData(TRUE);
	hexbuf=new char[3200];   //3072+128
	strbuf=new char[1152];   //1024+128

	if (m_hex)
	{
		((CEdit *)GetDlgItem(IDC_EDSHOW))->GetWindowText(hexbuf,3200);
		this->HexToStr(hexbuf,strbuf);
	}
	else
		((CEdit *)GetDlgItem(IDC_EDSHOW))->GetWindowText(strbuf,1152);

	this->m_cmuserlist.GetWindowText(userid,12);
	userid[11]='\0';
	do_send_user_data((unsigned char *)userid,
		              (unsigned char *)strbuf,
					  strlen(strbuf),
					  NULL);

	CString text;
	((CEdit *)GetDlgItem(IDC_EDSENDCOUNT))->GetWindowText(text);
	int count=atoi(text.GetBuffer(0));
	char pbuf[32];
	sprintf(pbuf,"%d",count+strlen(strbuf));
	((CEdit *)GetDlgItem(IDC_EDSENDCOUNT))->SetWindowText(pbuf);
	
	delete[] strbuf;
	delete[] hexbuf;
}

void CSendDataDlg::OnTimer(UINT nIDEvent) 
{
	if (1==nIDEvent)
		this->OnBsend();
	
	CDialog::OnTimer(nIDEvent);
}

void CSendDataDlg::OnCkautosend() 
{
    CButton *pButton;
	CEdit   *pEdit;
	char    buf[32];
	int     interval;

	pEdit=(CEdit *)GetDlgItem(IDC_INTERVAL);
	pEdit->GetWindowText(buf,32);
	interval=atoi(buf);

	if (interval<=0) 
		return;

	pButton=(CButton *)GetDlgItem(IDC_CKAUTOSEND);
	if (1==pButton->GetCheck())
	{
		this->SetTimer(1,interval,NULL);
	}
	else
	{
		this->KillTimer(1);
	}
}

void CSendDataDlg::OnBcr() 
{
	CString text;
	CEdit *pEdit;

	pEdit=(CEdit *)GetDlgItem(IDC_EDSHOW);
	pEdit->GetWindowText(text);
    UpdateData(TRUE);
	if (m_hex)
	{
		pEdit->SetWindowText(text+"0D ");
	}
	else
	{
		pEdit->SetWindowText(text+"\r");
	}
}

void CSendDataDlg::OnBlf() 
{
	CString text;
	CEdit *pEdit;

	pEdit=(CEdit *)GetDlgItem(IDC_EDSHOW);
	pEdit->GetWindowText(text);
    UpdateData(TRUE);
	if (m_hex)
	{
		pEdit->SetWindowText(text+"0A ");
	}
	else
	{
		pEdit->SetWindowText(text+"\n");
	}	
}
