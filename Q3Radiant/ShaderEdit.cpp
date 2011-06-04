// ShaderEdit.cpp : implementation file
//

#include "stdafx.h"
#include "Radiant.h"
#include "ShaderEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShaderEdit

IMPLEMENT_DYNCREATE(CShaderEdit, CFrameWnd)

CShaderEdit::CShaderEdit()
{
}

CShaderEdit::~CShaderEdit()
{
}


BEGIN_MESSAGE_MAP(CShaderEdit, CFrameWnd)
	//{{AFX_MSG_MAP(CShaderEdit)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShaderEdit message handlers

int CShaderEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}
