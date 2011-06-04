// GroupBar.cpp : implementation file
//

#include "stdafx.h"
#include "Radiant.h"
#include "GroupBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGroupBar dialog


CGroupBar::CGroupBar(CWnd* pParent /*=NULL*/)
	: CDialogBar()
{
	//{{AFX_DATA_INIT(CGroupBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CGroupBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGroupBar)
	DDX_Control(pDX, IDC_COMBO_GROUPS, m_wndGroupList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGroupBar, CDialogBar)
	//{{AFX_MSG_MAP(CGroupBar)
	ON_BN_CLICKED(IDC_BTN_ADDGROUP, OnBtnAddgroup)
	ON_BN_CLICKED(IDC_BTN_LISTGROUPS, OnBtnListgroups)
	ON_BN_CLICKED(IDC_BTN_REMOVEGROUP, OnBtnRemovegroup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGroupBar message handlers

void CGroupBar::OnBtnAddgroup() 
{
	// TODO: Add your control notification handler code here
	
}

void CGroupBar::OnBtnListgroups() 
{
	// TODO: Add your control notification handler code here
	
}

void CGroupBar::OnBtnRemovegroup() 
{
	// TODO: Add your control notification handler code here
	
}
