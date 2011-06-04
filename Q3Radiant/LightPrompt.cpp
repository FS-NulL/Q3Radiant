// LightPrompt.cpp : implementation file
//

#include "stdafx.h"
#include "radiant.h"
#include "LightPrompt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLightPrompt dialog


CLightPrompt::CLightPrompt(CWnd* pParent /*=NULL*/)
	: CDialog(CLightPrompt::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLightPrompt)
	m_iIntensity = 0;
	//}}AFX_DATA_INIT
}


void CLightPrompt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLightPrompt)
	DDX_Text(pDX, IDC_EDIT1, m_iIntensity);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLightPrompt, CDialog)
	//{{AFX_MSG_MAP(CLightPrompt)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLightPrompt message handlers

BOOL CLightPrompt::OnInitDialog()
{
  CDialog::OnInitDialog();
  // set the focus on the edit box to go faster
  CWnd *pWnd;
  pWnd = GetDlgItem( IDC_EDIT1 );
  GotoDlgCtrl( pWnd );
  return FALSE;
}
