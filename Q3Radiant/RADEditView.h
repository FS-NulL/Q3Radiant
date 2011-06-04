#if !defined(AFX_RADEDITVIEW_H__DC829123_812D_11D1_B548_00AA00A410FC__INCLUDED_)
#define AFX_RADEDITVIEW_H__DC829123_812D_11D1_B548_00AA00A410FC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// RADEditView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRADEditView view

class CRADEditView : public CEditView
{
public:
	CRADEditView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CRADEditView)
protected:

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRADEditView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CRADEditView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CRADEditView)
	afx_msg void OnChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RADEDITVIEW_H__DC829123_812D_11D1_B548_00AA00A410FC__INCLUDED_)
