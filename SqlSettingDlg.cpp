// SqlSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DSC.h"
#include "SqlSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSqlSettingDlg dialog


CSqlSettingDlg::CSqlSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSqlSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSqlSettingDlg)
	m_strDataBaseName = _T("");
	m_strPassword = _T("");
	m_strUserName = _T("");
	//}}AFX_DATA_INIT
}


void CSqlSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSqlSettingDlg)
	DDX_Text(pDX, IDC_DATABASENAME, m_strDataBaseName);
	DDX_Text(pDX, IDC_PASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_USERNAME, m_strUserName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSqlSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CSqlSettingDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSqlSettingDlg message handlers
