#if !defined(AFX_WAVEOPEN_H__0FB9DA11_EB02_11D2_A50A_0020AFEB881A__INCLUDED_)
#define AFX_WAVEOPEN_H__0FB9DA11_EB02_11D2_A50A_0020AFEB881A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WaveOpen.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWaveOpen dialog

class CWaveOpen : public CFileDialog
{
	DECLARE_DYNAMIC(CWaveOpen)

public:
	CWaveOpen(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);

  virtual void OnFileNameChange( );
protected:
	//{{AFX_MSG(CWaveOpen)
	afx_msg void OnBtnPlay();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAVEOPEN_H__0FB9DA11_EB02_11D2_A50A_0020AFEB881A__INCLUDED_)
