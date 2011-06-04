// TextureLoad.cpp : implementation file
//

#include "stdafx.h"
#include "Radiant.h"
#include "TextureLoad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextureLoad dialog


CTextureLoad::CTextureLoad(CWnd* pParent /*=NULL*/)
	: CDialog(CTextureLoad::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTextureLoad)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTextureLoad::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextureLoad)
	DDX_Control(pDX, IDC_LIST_TEXTURES, m_wndList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTextureLoad, CDialog)
	//{{AFX_MSG_MAP(CTextureLoad)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextureLoad message handlers

BOOL CTextureLoad::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTextureLoad::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
