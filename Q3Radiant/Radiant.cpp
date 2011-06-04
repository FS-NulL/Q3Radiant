// Radiant.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Radiant.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "RadiantDoc.h"
#include "RadiantView.h"
#include "PrefsDlg.h"

#include "shlwapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRadiantApp

BEGIN_MESSAGE_MAP(CRadiantApp, CWinApp)
	//{{AFX_MSG_MAP(CRadiantApp)
	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_HELP_LINKS, OnHelpLinks)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRadiantApp construction

CRadiantApp::CRadiantApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CRadiantApp object

CRadiantApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CRadiantApp initialization

HINSTANCE g_hOpenGL32 = NULL;
HINSTANCE g_hOpenGL = NULL;
bool g_bBuildList = false;

BOOL CRadiantApp::InitInstance()
{
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

  g_qeglobals.disable_ini = false;

	// If there's a .INI file in the directory use it instead of registry
	char RadiantPath[_MAX_PATH];
	GetModuleFileName( NULL, RadiantPath, _MAX_PATH );
	// search for exe
	CFileFind Finder;
	Finder.FindFile( RadiantPath );
	Finder.FindNextFile();
	// extract root
	CString Root = Finder.GetRoot();
	// build root\*.ini
	CString IniPath = Root + "\\REGISTRY.INI";
	// search for ini file
	Finder.FindNextFile();
	if (Finder.FindFile( IniPath ))
	{
    m_sIniPath = IniPath;
		Finder.FindNextFile();
		// use the .ini file instead of the registry
		free((void*)m_pszProfileName);
		m_pszProfileName=_tcsdup(_T(Finder.GetFilePath()));
		// look for the registry key for void* buffers storage ( these can't go into .INI files )
		int i=0;
		CString key;
		HKEY hkResult;
		DWORD dwDisp;
		DWORD type;
		char iBuf[3];
		do
		{
			sprintf( iBuf, "%d", i );
			key = "Software\\Q3Radiant\\IniPrefs" + CString(iBuf);
			// does this key exists ?
			if ( RegOpenKeyEx( HKEY_CURRENT_USER, key, 0, KEY_ALL_ACCESS, &hkResult ) != ERROR_SUCCESS )
			{
				// this key doesn't exist, so it's the one we'll use
				strcpy( g_qeglobals.use_ini_registry, key.GetBuffer(0) );
				RegCreateKeyEx( HKEY_CURRENT_USER, key, 0, NULL, 
					REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkResult, &dwDisp );
				RegSetValueEx( hkResult, "RadiantName", 0, REG_SZ, reinterpret_cast<CONST BYTE *>(RadiantPath), strlen( RadiantPath )+1 );
				RegCloseKey( hkResult );
				break;
			}
			else
			{
				char RadiantAux[ _MAX_PATH ];
				unsigned long size = _MAX_PATH;
				// the key exists, is it the one we are looking for ?
				RegQueryValueEx( hkResult, "RadiantName", 0, &type, reinterpret_cast<BYTE *>(RadiantAux), &size );
				RegCloseKey( hkResult );
				if ( !strcmp( RadiantAux, RadiantPath ) )
				{
					// got it !
					strcpy( g_qeglobals.use_ini_registry, key.GetBuffer(0) );
					break;
				}
			}
			i++;
		} while (1);
    m_sIniSubkey = "IniPrefs" + CString(iBuf);
		g_qeglobals.use_ini = true;
	}
	else
	{
		// Change the registry key under which our settings are stored.
		// You should modify this string to be something appropriate
		// such as the name of your company or organization.
		SetRegistryKey("Q3Radiant");
		g_qeglobals.use_ini = false;
	}

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// create main MDI Frame window

  // initialize g_strAppPath, we need it in LoadPrefs if InitEnginePath gets called
  char* pBuffer = g_strAppPath.GetBufferSetLength(_MAX_PATH + 1);
  int nResult = ::GetModuleFileName(NULL, pBuffer, _MAX_PATH);
  ASSERT(nResult != 0);
  pBuffer[g_strAppPath.ReverseFind('\\') + 1] = '\0';
  g_strAppPath.ReleaseBuffer();

	// before loading prefs and initializing GL, check for a Radiant.pid file
	// that would mean we crashed somewhere during startup last time
	// so this time, prompt the user about cleaning the registry .. maybe we'll be more lucky next startup
	g_pidFile = g_strAppPath + "Radiant.pid";
  if (Finder.FindFile( g_pidFile ))
	{
    CString Msg = "I found a " + g_pidFile + " file. That often means last Radiant startup failed (if not remove it).\nDo you want me to reset the registry settings?\n(answer no to produce a log of your startup)";
    if (MessageBox( NULL, Msg, "Radiant", MB_YESNO | MB_ICONQUESTION ) == IDYES)
    {
      // remove the file
      CFile::Remove( g_pidFile );	
      ResetRegistry();
      exit(-1);
    }
    MessageBox( NULL, "I'm turning console logging on, see Radiant.log file in Radiant's directory if startup fails again.", "Console logging", MB_OK );
    // force console logging on! (will go in prefs too)
    g_PrefsDlg.m_bLogConsole = true;
    Sys_LogFile();
    g_PrefsDlg.LoadPrefs();
	}
  else
	{
    // create one, will remove right after entering message loop
    CFile File;
    File.Open( g_pidFile, CFile::modeCreate | CFile::modeWrite, NULL);
    File.Close();
    g_PrefsDlg.LoadPrefs();
    // if console logging is on in the prefs, warn about performance hit
    if (g_PrefsDlg.m_bLogConsole)
      if (MessageBox( NULL, "Preferences indicate that console logging is on. This might affect performances. Turn it off?", "Radiant", MB_YESNO | MB_ICONQUESTION ) == IDYES) 
        g_PrefsDlg.m_bLogConsole = false;
    // toggle console logging if necessary
    Sys_LogFile();
	}

  int nMenu = IDR_MENU1;

  CString strOpenGL = (g_PrefsDlg.m_bSGIOpenGL) ? "opengl.dll" : "opengl32.dll";
  CString strGLU = (g_PrefsDlg.m_bSGIOpenGL) ? "glu.dll" : "glu32.dll";
  
  if (!QGL_Init(strOpenGL, strGLU))
  {
    g_PrefsDlg.m_bSGIOpenGL ^= 1;
    strOpenGL = (g_PrefsDlg.m_bSGIOpenGL) ? "opengl.dll" : "opengl32.dll";
    strGLU = (g_PrefsDlg.m_bSGIOpenGL) ? "glu.dll" : "glu32.dll";
    if (!QGL_Init(strOpenGL, strGLU))
    {
      AfxMessageBox("Failed to load OpenGL libraries. \"OPENGL32.DLL\" and \"OPENGL.DLL\" were tried");
      return FALSE;
    }
    g_PrefsDlg.SavePrefs();
  }

	CString strTemp = m_lpCmdLine;
  strTemp.MakeLower();
  if (strTemp.Find("builddefs") >= 0)
    g_bBuildList = true;

	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(nMenu))
		return FALSE;

  if (pMainFrame->m_hAccelTable)
    ::DestroyAcceleratorTable(pMainFrame->m_hAccelTable);
  
  pMainFrame->LoadAccelTable(MAKEINTRESOURCE(IDR_MINIACCEL));

	m_pMainWnd = pMainFrame;

  // Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	//if (!ProcessShellCommand(cmdInfo))
	//	return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

  free((void*)m_pszHelpFilePath);
  CString strHelp = g_strAppPath;
  AddSlash(strHelp);
  strHelp += "Q3RManual.chm";
  m_pszHelpFilePath= _tcsdup(strHelp);

	// m_bShowShader and m_bTextureShaderlistOnly have a menu checkbox, update it now
	//++timo FIXME: I really don't know if there's an official place for that .. so I did it here
  CheckMenuItem ( ::GetMenu(g_qeglobals.d_hwndMain), ID_TEXTURES_SHADERS_SHOW, MF_BYCOMMAND | ((g_PrefsDlg.m_bShowShaders) ? MF_CHECKED : MF_UNCHECKED ));
  CheckMenuItem ( ::GetMenu(g_qeglobals.d_hwndMain), ID_TEXTURES_SHADERLISTONLY, MF_BYCOMMAND | ((g_PrefsDlg.m_bTexturesShaderlistOnly) ? MF_CHECKED : MF_UNCHECKED ));

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CRadiantApp commands

int CRadiantApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
  //::FreeLibrary(g_hOpenGL32);
	QGL_Shutdown();

  g_PrefsDlg.m_bLogConsole = false;
	Sys_LogFile();
	return CWinApp::ExitInstance();
}

BOOL CRadiantApp::OnIdle(LONG lCount) 
{
	if (g_pParentWnd)
    g_pParentWnd->RoutineProcessing();
	return CWinApp::OnIdle(lCount);
}

void CRadiantApp::OnHelp() 
{
  ShellExecute(m_pMainWnd->GetSafeHwnd(), "open", m_pszHelpFilePath, NULL, NULL, SW_SHOW);
}

// NOTE: the application MUST exit after this step
// depending from where we call we may not have to exit the same way
void CRadiantApp::ResetRegistry()
{
  g_qeglobals.disable_ini = true;
  // this will reset registry entries and exit Radiant
  if (g_qeglobals.use_ini)
  {
    // empty m_sIniPath, don't remove it!
    CFile IniFile( m_sIniPath.GetBuffer(0), CFile::modeReadWrite );
    IniFile.SetLength(0);
    IniFile.Close();
    // now clear the section in the registry
    HKEY key;
    RegOpenKey( HKEY_CURRENT_USER, "Software\\Q3Radiant", &key );
    RegDeleteKey( key, m_sIniSubkey.GetBuffer(0) );
  }
  else
  {
    HKEY key;
    RegOpenKey( HKEY_CURRENT_USER, "Software\\Q3Radiant", &key );
    SHDeleteKey( key, "Radiant" );
    SHDeleteKey( key, "Q3Radiant" );
  }
}

void CRadiantApp::OnHelpLinks() 
{
	// launch default web browser, display the links.html in Radiant directory
	CString links = g_strAppPath;
	links += "links.htm";
	ShellExecute(NULL, "open", links, NULL, NULL, 1);
}
