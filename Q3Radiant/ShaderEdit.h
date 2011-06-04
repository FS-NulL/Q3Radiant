#if !defined(AFX_SHADEREDIT_H__CFE2CBF1_E980_11D2_A509_0020AFEB881A__INCLUDED_)
#define AFX_SHADEREDIT_H__CFE2CBF1_E980_11D2_A509_0020AFEB881A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShaderEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShaderEdit frame

class CShaderEdit : public CFrameWnd
{
	DECLARE_DYNCREATE(CShaderEdit)
protected:
	CShaderEdit();           // protected constructor used by dynamic creation

  CStatusBar  m_StatusBar;
	CToolBar    m_ToolBar;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShaderEdit)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CShaderEdit();

	// Generated message map functions
	//{{AFX_MSG(CShaderEdit)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHADEREDIT_H__CFE2CBF1_E980_11D2_A509_0020AFEB881A__INCLUDED_)
