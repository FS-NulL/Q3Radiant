#if !defined(AFX_LIGHTPROMPT_H__0D957C4C_30D4_11D4_A423_0004AC96D4C3__INCLUDED_)
#define AFX_LIGHTPROMPT_H__0D957C4C_30D4_11D4_A423_0004AC96D4C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LightPrompt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLightPrompt dialog

class CLightPrompt : public CDialog
{
// Construction
public:
	BOOL OnInitDialog();
//  int DoModal();
	CLightPrompt(CWnd* pParent = NULL);   // standard constructor


// Dialog Data
	//{{AFX_DATA(CLightPrompt)
	enum { IDD = IDD_DIALOG_LIGHTPROMPT };
	int		m_iIntensity;
	//}}AFX_DATA
//	afx_msg void OnEnterIdle( UINT nWhy, CWnd* pWho );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLightPrompt)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLightPrompt)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIGHTPROMPT_H__0D957C4C_30D4_11D4_A423_0004AC96D4C3__INCLUDED_)
