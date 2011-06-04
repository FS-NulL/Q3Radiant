#if !defined(AFX_NAMEDLG_H__624F3290_976A_48A9_AE89_50CD1E6E2F9A__INCLUDED_)
#define AFX_NAMEDLG_H__624F3290_976A_48A9_AE89_50CD1E6E2F9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NameDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNameDlg dialog

class CNameDlg : public CDialog
{
  CString m_strCaption;
// Construction
public:
	CNameDlg(const char *pName, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNameDlg)
	enum { IDD = IDD_NEWNAME };
	CString	m_strName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNameDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNameDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NAMEDLG_H__624F3290_976A_48A9_AE89_50CD1E6E2F9A__INCLUDED_)
