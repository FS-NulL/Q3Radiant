// ZView.cpp : implementation file
//

#include "stdafx.h"
#include "Radiant.h"
#include "ZView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZView

IMPLEMENT_DYNCREATE(CZView, CView)

CZView::CZView()
{
}

CZView::~CZView()
{
}


BEGIN_MESSAGE_MAP(CZView, CView)
	//{{AFX_MSG_MAP(CZView)
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZView drawing

void CZView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CZView diagnostics

#ifdef _DEBUG
void CZView::AssertValid() const
{
	CView::AssertValid();
}

void CZView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CZView message handlers

int CZView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	
	return 0;
}

void CZView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CZView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

void CZView::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CZView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CView::OnLButtonUp(nFlags, point);
}

void CZView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CView::OnLButtonDown(nFlags, point);
}

void CZView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CView::OnRButtonUp(nFlags, point);
}

void CZView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CView::OnRButtonDown(nFlags, point);
}

void CZView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CView::OnMouseMove(nFlags, point);
}

void CZView::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	
	CView::OnGetMinMaxInfo(lpMMI);
}
