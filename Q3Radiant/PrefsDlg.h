#if !defined(AFX_PREFSDLG_H__DC829122_812D_11D1_B548_00AA00A410FC__INCLUDED_)
#define AFX_PREFSDLG_H__DC829122_812D_11D1_B548_00AA00A410FC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PrefsDlg.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CPrefsDlg dialog

#define MAX_TEXTURE_QUALITY 3

class CPrefsDlg : public CDialog
{
// Construction
public:
  // last light intensity used in the CLightPrompt dialog, stored in registry
	int m_iLastLightIntensity;
	// these mirror what goes in the combo box
	// see CPrefDlg::m_nShader, tells wether to load NONE / COMMON or ALL shaders at parsing stage
	enum {SHADER_NONE = 0, SHADER_COMMON, SHADER_ALL};
	void LoadPrefs();
	void SavePrefs();
	void SetGamePrefs();
	CPrefsDlg(CWnd* pParent = NULL);   // standard constructor
  void InitEnginePath();

// Dialog Data
	//{{AFX_DATA(CPrefsDlg)
	enum { IDD = IDD_DLG_PREFS };
	CSpinButtonCtrl	m_wndUndoSpin;
	CSpinButtonCtrl	m_wndFontSpin;
	CSliderCtrl	m_wndTexturequality;
	CComboBox	m_wndWhatGame;
	CSliderCtrl	m_wndCamSpeed;
	CSpinButtonCtrl	m_wndSpin;
	CString	m_strQuake2;
	int		m_nMouse;
	int		m_nView;
  BOOL   m_bTextureLock;
	BOOL	m_bLoadLast;
	BOOL	m_bRunBefore;
	CString	m_strLastProject;
	CString	m_strLastMap;
	BOOL	m_bFace;
	BOOL	m_bInternalBSP;
	BOOL	m_bRightClick;
	BOOL	m_bRunQuake;
	BOOL	m_bSetGame;
	BOOL	m_bAutoSave;
  BOOL  m_bNewApplyHandling;
	CString	m_strAutoSave;
	BOOL	m_bPAK;
	BOOL	m_bLoadLastMap;
	BOOL	m_bGatewayHack;
	BOOL	m_bTextureWindow;
	BOOL	m_bSnapShots;
  float m_fTinySize;
  BOOL  m_bCleanTiny;
	CString	m_strPAKFile;
	int		m_nStatusSize;
	BOOL	m_bCamXYUpdate;
	BOOL	m_bNewLightDraw;
	CString	m_strPrefabPath;
	int		m_nWhatGame;
	CString	m_strWhatGame;
	BOOL	m_bALTEdge;
	BOOL	m_bTextureBar;
	BOOL	m_bFaceColors;
	BOOL	m_bQE4Painting;
	BOOL	m_bSnapTToGrid;
	BOOL	m_bXZVis;
	BOOL	m_bYZVis;
	BOOL	m_bZVis;
	BOOL	m_bSizePaint;
	BOOL	m_bDLLEntities;
  BOOL  m_bRotateLock;
	BOOL	m_bWideToolbar;
	BOOL	m_bNoClamp;
	CString	m_strUserPath;
	int		m_nRotation;
	BOOL	m_bSGIOpenGL;
	BOOL	m_bBuggyICD;
	BOOL	m_bHiColorTextures;
	BOOL	m_bChaseMouse;
	BOOL	m_bTextureScrollbar;
	BOOL	m_bDisplayLists;
	BOOL	m_bShowShaders;
	int		m_nShader;
	BOOL	m_bNoStipple;
	int		m_nUndoLevels;
	BOOL	m_bVertexSplit;
	BOOL	m_bLogConsole;
	BOOL	m_bDisableAlphaChannel;
	//}}AFX_DATA
	int	m_nMouseButtons;
  int m_nAngleSpeed;
  int m_nMoveSpeed;
  int m_nAutoSave;
  bool m_bCubicClipping;
  int m_nCubicScale;
  BOOL  m_bSelectCurves;
	BOOL	m_bSelectModels;
  int m_nEntityShowState;
  int m_nTextureScale;
  BOOL m_bNormalizeColors;
  BOOL m_bSwitchClip;
  BOOL m_bSelectWholeEntities;
  int m_nTextureQuality;
	BOOL  m_bGLLighting;
  BOOL  m_bTexturesShaderlistOnly;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrefsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPrefsDlg)
	afx_msg void OnBtnBrowse();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBtnBrowsepak();
	afx_msg void OnBtnBrowseprefab();
	afx_msg void OnBtnBrowseuserini();
	afx_msg void OnSelchangeComboWhatgame();
	afx_msg void OnButtonClean();
	afx_msg void OnNoalpha();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFSDLG_H__DC829122_812D_11D1_B548_00AA00A410FC__INCLUDED_)
