#if !defined(AFX_TEXTURELOAD_H__E53749E6_AAE3_47B8_B909_84C7982E35C9__INCLUDED_)
#define AFX_TEXTURELOAD_H__E53749E6_AAE3_47B8_B909_84C7982E35C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextureLoad.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextureLoad dialog

class CTextureLoad : public CDialog
{
// Construction
public:
	CTextureLoad(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTextureLoad)
	enum { IDD = IDD_TEXLIST };
	CListBox	m_wndList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextureLoad)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTextureLoad)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTURELOAD_H__E53749E6_AAE3_47B8_B909_84C7982E35C9__INCLUDED_)
