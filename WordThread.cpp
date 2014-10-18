// WordThread.cpp : implementation file
//

#include "stdafx.h"
#include "DSC.h"
#include "WordThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWordThread

IMPLEMENT_DYNCREATE(CWordThread, CWinThread)

CWordThread::CWordThread()
{
}

CWordThread::~CWordThread()
{
}

BOOL CWordThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CWordThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CWordThread, CWinThread)
	//{{AFX_MSG_MAP(CWordThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWordThread message handlers

int  CWordThread::Run()
{
	//this thread is used to write received data to file
	return CWinThread::Run();
}

