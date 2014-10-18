// DSC.h : main header file for the DSC application
//

#if !defined(AFX_DSC_H__408B9528_9212_4EC9_BB51_A77FC12FAEDA__INCLUDED_)
#define AFX_DSC_H__408B9528_9212_4EC9_BB51_A77FC12FAEDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDSCApp:
// See DSC.cpp for the implementation of this class
//

class CDSCApp : public CWinApp
{
public:
	CDSCApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDSCApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CDSCApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSC_H__408B9528_9212_4EC9_BB51_A77FC12FAEDA__INCLUDED_)
