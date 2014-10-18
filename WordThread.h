#if !defined(AFX_WORDTHREAD_H__E6C481C6_44FF_4969_9BF1_DF29EF960DB1__INCLUDED_)
#define AFX_WORDTHREAD_H__E6C481C6_44FF_4969_9BF1_DF29EF960DB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WordThread.h : header file
//

#include "MainFrm.h"

/////////////////////////////////////////////////////////////////////////////
// CWordThread thread

class CWordThread : public CWinThread
{
	DECLARE_DYNCREATE(CWordThread)
protected:
	CWordThread();           // protected constructor used by dynamic creation
// Attributes
public:
// Operations
public:
 
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWordThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CWordThread();

	// Generated message map functions
	//{{AFX_MSG(CWordThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORDTHREAD_H__E6C481C6_44FF_4969_9BF1_DF29EF960DB1__INCLUDED_)
