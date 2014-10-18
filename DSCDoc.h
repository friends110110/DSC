// DSCDoc.h : interface of the CDSCDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSCDOC_H__6642D2B6_24ED_4ED2_B282_84D0DECDCF59__INCLUDED_)
#define AFX_DSCDOC_H__6642D2B6_24ED_4ED2_B282_84D0DECDCF59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDSCDoc : public CDocument
{
protected: // create from serialization only
	CDSCDoc();
	DECLARE_DYNCREATE(CDSCDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDSCDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDSCDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDSCDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSCDOC_H__6642D2B6_24ED_4ED2_B282_84D0DECDCF59__INCLUDED_)
