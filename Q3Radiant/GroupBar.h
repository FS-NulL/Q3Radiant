#if !defined(AFX_GROUPBAR_H__926379A9_F46B_4E81_AD23_71BDDF55BDB3__INCLUDED_)
#define AFX_GROUPBAR_H__926379A9_F46B_4E81_AD23_71BDDF55BDB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GroupBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGroupBar dialog

class CGroupBar : public CDialogBar
{
// Construction
public:
	CGroupBar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGroupBar)
	enum { IDD = IDD_GROUPBAR };
	CComboBox	m_wndGroupList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGroupBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGroupBar)
	afx_msg void OnBtnAddgroup();
	afx_msg void OnBtnListgroups();
	afx_msg void OnBtnRemovegroup();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GROUPBAR_H__926379A9_F46B_4E81_AD23_71BDDF55BDB3__INCLUDED_)
