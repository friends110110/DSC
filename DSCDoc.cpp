// DSCDoc.cpp : implementation of the CDSCDoc class
//

#include "stdafx.h"
#include "DSC.h"

#include "DSCDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDSCDoc

IMPLEMENT_DYNCREATE(CDSCDoc, CDocument)

BEGIN_MESSAGE_MAP(CDSCDoc, CDocument)
	//{{AFX_MSG_MAP(CDSCDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDSCDoc construction/destruction

CDSCDoc::CDSCDoc()
{
	// TODO: add one-time construction code here

}

CDSCDoc::~CDSCDoc()
{
}

BOOL CDSCDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDSCDoc serialization

void CDSCDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDSCDoc diagnostics

#ifdef _DEBUG
void CDSCDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDSCDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDSCDoc commands
