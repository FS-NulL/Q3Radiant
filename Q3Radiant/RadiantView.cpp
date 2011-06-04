// RadiantView.cpp : implementation of the CRadiantView class
//

#include "stdafx.h"
#include "Radiant.h"

#include "RadiantDoc.h"
#include "RadiantView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRadiantView

IMPLEMENT_DYNCREATE(CRadiantView, CView)

BEGIN_MESSAGE_MAP(CRadiantView, CView)
	//{{AFX_MSG_MAP(CRadiantView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRadiantView construction/destruction

CRadiantView::CRadiantView()
{
	// TODO: add construction code here

}

CRadiantView::~CRadiantView()
{
}

BOOL CRadiantView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CRadiantView drawing

void CRadiantView::OnDraw(CDC* pDC)
{
	CRadiantDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CRadiantView printing

BOOL CRadiantView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRadiantView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRadiantView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CRadiantView diagnostics

#ifdef _DEBUG
void CRadiantView::AssertValid() const
{
	CView::AssertValid();
}

void CRadiantView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRadiantDoc* CRadiantView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRadiantDoc)));
	return (CRadiantDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRadiantView message handlers
