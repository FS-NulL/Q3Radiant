// RADEditView.cpp : implementation file
//

#include "stdafx.h"
#include "Radiant.h"
#include "RADEditView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRADEditView

IMPLEMENT_DYNCREATE(CRADEditView, CEditView)

CRADEditView::CRADEditView()
{
}

CRADEditView::~CRADEditView()
{
}


BEGIN_MESSAGE_MAP(CRADEditView, CEditView)
	//{{AFX_MSG_MAP(CRADEditView)
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRADEditView drawing

void CRADEditView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CRADEditView diagnostics

#ifdef _DEBUG
void CRADEditView::AssertValid() const
{
	CEditView::AssertValid();
}

void CRADEditView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRADEditView message handlers

void CRADEditView::OnChange() 
{
}
