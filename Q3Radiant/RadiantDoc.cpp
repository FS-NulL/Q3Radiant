// RadiantDoc.cpp : implementation of the CRadiantDoc class
//

#include "stdafx.h"
#include "Radiant.h"

#include "RadiantDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRadiantDoc

IMPLEMENT_DYNCREATE(CRadiantDoc, CDocument)

BEGIN_MESSAGE_MAP(CRadiantDoc, CDocument)
	//{{AFX_MSG_MAP(CRadiantDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRadiantDoc construction/destruction

CRadiantDoc::CRadiantDoc()
{
	// TODO: add one-time construction code here

}

CRadiantDoc::~CRadiantDoc()
{
}

BOOL CRadiantDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CRadiantDoc serialization

void CRadiantDoc::Serialize(CArchive& ar)
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
// CRadiantDoc diagnostics

#ifdef _DEBUG
void CRadiantDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRadiantDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRadiantDoc commands
