#if !defined(AFX_GROUPDLG_H__92368487_9E05_454E_A66F_23A9A94A753B__INCLUDED_)
#define AFX_GROUPDLG_H__92368487_9E05_454E_A66F_23A9A94A753B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GroupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGroupDlg dialog

class CGroupDlg : public CDialog
{
  CImageList m_imgList;
// Construction
public:
	virtual void OnOK();
	virtual void OnCancel();
	void InitGroups();
	CGroupDlg(CWnd* pParent = NULL);   // standard constructor

  HTREEITEM m_hWorld;

// Dialog Data
	//{{AFX_DATA(CGroupDlg)
	enum { IDD = IDD_DLG_GROUP };
	CTreeCtrl	m_wndTree;
	CButton	m_wndEdit;
	CButton	m_wndDel;
	CButton	m_wndAdd;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGroupDlg)
	public:
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGroupDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnDel();
	afx_msg void OnBtnEdit();
	afx_msg void OnRclickTreeGroup(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditTreeGroup(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickTreeGroup(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetdispinfoTreeGroup(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindragTreeGroup(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

extern CGroupDlg *g_pGroupDlg;


#endif // !defined(AFX_GROUPDLG_H__92368487_9E05_454E_A66F_23A9A94A753B__INCLUDED_)
