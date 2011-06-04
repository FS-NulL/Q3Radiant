//-----------------------------------------------------------------------------
//
// $LogFile$
// $Revision: 1.4 $
// $Author: ttimo $
// $Date: 2000/06/23 00:26:26 $
// $Log: TexTool.cpp,v $
// Revision 1.4  2000/06/23 00:26:26  ttimo
// fixed some error messages
//
// Revision 1.3  2000/03/16 03:18:00  ttimo
// Released version 03 16 2000
//
// Revision 1.2  2000/03/15 01:22:51  ttimo
// added close button to textool window
//
// Revision 1.1.1.1  2000/01/07 17:15:33  ttimo
// initial import of Q3Plugin module
//
// Revision 1.4  1999/12/02 16:09:45  Timo & Christine
// commit before e-brain dump
// TexTool v1.1 and Docs
//
// Revision 1.3  1999/11/22 17:44:25  Timo & Christine
// bug fixing old brushes / brush primitive
//
// Revision 1.2  1999/11/20 12:10:29  Timo & Christine
// first release to Wolfen and Spog
// see TexTool\changelog.txt
//
// Revision 1.1.1.1  1999/10/27 08:46:27  Timo & Christine
// initial import of Q3Plugin module, preview version of TexTool plugin, see PluginExtensions2 in Q3Radiant module
//
//
// DESCRIPTION:
// main plugin implementation
// texturing tools for Q3Radiant
//

#include "stdafx.h"

HINSTANCE g_hModule;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			g_hModule = reinterpret_cast<HINSTANCE>(hModule);
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

// Radiant function table
_QERFuncTable_1 g_FuncTable;
// plugin name
const char *PLUGIN_NAME = "Q3 Texture Tools";
// commands in the menu
const char *PLUGIN_COMMANDS = "About...;Help...;-;Clone texture...;-;Go...";
// globals
HMODULE g_hApp;
HWND	g_hwndMain;
// handle to the tool window
HWND	g_hToolWnd = NULL;
HDC		g_hToolDC;
HGLRC	g_hGLRC;
// plugin interfaces ---------------------------
bool					g_bQglInitDone = false;
_QERQglTable			g_QglTable;
bool					g_bSelectedFaceInitDone = false;
_QERSelectedFaceTable	g_SelectedFaceTable;
// selected face -------------------------------
// we use this one to commit / read with Radiant
_QERFaceData			g_SelectedFaceData;
// g_pSelectedFaceWindings gets allocated with MAX_POINTS_ON_WINDING at plugin startup ( QERPlug_Init )
winding_t				*g_pSelectedFaceWinding = NULL;
const float				g_ViewportRatio = 1.2f;
// usefull class to manage the 2D view
C2DView					g_2DView;
// control points to move the polygon
CControlPointsManagerBFace	g_ControlPointsBFace;
// tells if a face is selected and we have something to render in the TexWindow
bool					g_bTexViewReady = false;
// data for texture work
int						g_NumPoints;
CtrlPts_t				g_WorkWinding;
// reference _QERFaceData we use on Cancel, and for Commit
_QERFaceData			g_CancelFaceData;
// patches -------------------------------------
bool					g_bPatch = false;
//++timo we use this one to grab selected patchMesh_t
// FIXME: update when there's a real interface to read/write patches
bool					g_bSurfaceTableInitDone = false;
_QERAppSurfaceTable		g_SurfaceTable;
CControlPointsManagerPatch	g_ControlPointsPatch;
// data for texture work
patchMesh_t*			g_pPatch;
// we only use ctrl[][].st in this one
patchMesh_t				g_WorkPatch;
// copy of initial g_pPatch for Cancel situation
patchMesh_t				g_CancelPatch;
// ---------------------------------------------
// holds the manager we are currently using
CControlPointsManager	*g_pManager = NULL;
// ---------------------------------------------
// globals flags for user preferences
//++timo TODO: this should be retrieved from the Editor's .INI prefs in a dedicated interface
// update camera view during manipulation ?
bool					g_bPrefsUpdateCameraView = true;
// misc ----------------------------------------
bool					g_bHelp = false;
//++timo FIXME: used to close the plugin window if InitTexView fails
// it's dirty, only use is to prevent infinite loop in DialogProc
bool					g_bClosing = false;

const char *PLUGIN_ABOUT = "Texture Tools for Q3Radiant\nhttp://plugins.qeradiant.com\nWritten by TTimo";

LPVOID __declspec(dllexport) QERPlug_GetFuncTable()
{
  return &g_FuncTable;
}

LPCSTR __declspec(dllexport) QERPlug_Init(HMODULE hApp, HWND hwndMain)
{
	int size;

	g_hApp = hApp;
	g_hwndMain = hwndMain;
	memset(&g_FuncTable, 0, sizeof(_QERFuncTable_1));
	g_FuncTable.m_fVersion = QER_PLUG_VERSION;
	g_FuncTable.m_nSize = sizeof(_QERFuncTable_1);
	size = (int)((winding_t *)0)->points[MAX_POINTS_ON_WINDING];
	g_pSelectedFaceWinding = (winding_t *)malloc( size );
	memset( g_pSelectedFaceWinding, 0, size );
	return "Texture tools for Q3Radiant";
}

LPCSTR __declspec(dllexport) QERPlug_GetName()
{
  return PLUGIN_NAME;
}

LPCSTR __declspec(dllexport) QERPlug_GetCommandList()
{
  return PLUGIN_COMMANDS;
}

char	*TranslateString (char *buf)
{
	static	char	buf2[32768];
	int		i, l;
	char	*out;

	l = strlen(buf);
	out = buf2;
	for (i=0 ; i<l ; i++)
	{
		if (buf[i] == '\n')
		{
			*out++ = '\r';
			*out++ = '\n';
		}
		else
			*out++ = buf[i];
	}
	*out++ = 0;

	return buf2;
}

extern "C" void Sys_Printf (char *text, ...)
{
	va_list argptr;
	char	buf[32768];
	char	*out;

	va_start (argptr,text);
	vsprintf (buf, text,argptr);
	va_end (argptr);

	out = TranslateString (buf);

	g_FuncTable.m_pfnSysMsg ( out );
}

// called by InitTexView to fit the view against the bounding box of control points
void FitView( HWND hwndDlg, int TexSize[2] )
{
	// apply a ratio to get the area we'll draw
	g_2DView.m_Center[0] = 0.5f * ( g_2DView.m_Mins[0] + g_2DView.m_Maxs[0] );
	g_2DView.m_Center[1] = 0.5f * ( g_2DView.m_Mins[1] + g_2DView.m_Maxs[1] );
	g_2DView.m_Mins[0] = g_2DView.m_Center[0] + g_ViewportRatio*( g_2DView.m_Mins[0] - g_2DView.m_Center[0] );
	g_2DView.m_Mins[1] = g_2DView.m_Center[1] + g_ViewportRatio*( g_2DView.m_Mins[1] - g_2DView.m_Center[1] );
	g_2DView.m_Maxs[0] = g_2DView.m_Center[0] + g_ViewportRatio*( g_2DView.m_Maxs[0] - g_2DView.m_Center[0] );
	g_2DView.m_Maxs[1] = g_2DView.m_Center[1] + g_ViewportRatio*( g_2DView.m_Maxs[1] - g_2DView.m_Center[1] );
	if ( !GetClientRect( hwndDlg, &g_2DView.m_rect ) )
	{
		Sys_Printf("TexTool: GetClientRect failed\n");
		return;
	}
	// we need to draw this area, now compute a bigger area so the texture scale is the same along X and Y
	// compute box shape in XY space, let's say X <-> S we'll get a ratio for Y: 
	if (!g_bPatch)
	{
		g_SelectedFaceTable.m_pfnGetTextureSize( TexSize );
	}
	else
	{
		TexSize[0] = g_pPatch->d_texture->width;
		TexSize[1] = g_pPatch->d_texture->height;
	}
	// we want a texture with the same X / Y ratio
	// compute XY space / window size ratio
	float SSize = (float)fabs( g_2DView.m_Maxs[0] - g_2DView.m_Mins[0] );
	float TSize = (float)fabs( g_2DView.m_Maxs[1] - g_2DView.m_Mins[1] );
	float XSize = TexSize[0] * SSize;
	float YSize = TexSize[1] * TSize;
	float RatioX = XSize / (float)abs( g_2DView.m_rect.left - g_2DView.m_rect.right );
	float RatioY = YSize / (float)abs( g_2DView.m_rect.top - g_2DView.m_rect.bottom );
	if ( RatioX > RatioY )
	{
		YSize = (float)abs( g_2DView.m_rect.top - g_2DView.m_rect.bottom ) * RatioX;
		TSize = YSize / (float)TexSize[1];
	}
	else
	{
		XSize = (float)abs( g_2DView.m_rect.left - g_2DView.m_rect.right ) * RatioY;
		SSize = XSize / (float)TexSize[0];
	}
	g_2DView.m_Mins[0] = g_2DView.m_Center[0] - 0.5f * SSize;
	g_2DView.m_Maxs[0] = g_2DView.m_Center[0] + 0.5f * SSize;
	g_2DView.m_Mins[1] = g_2DView.m_Center[1] - 0.5f * TSize;
	g_2DView.m_Maxs[1] = g_2DView.m_Center[1] + 0.5f * TSize;
}

// call this one each time we need to re-init
//++timo TODO: re-init objects state, g_2DView and g_ControlPointsManager
void InitTexView( HWND hwndDlg )
{
	// size of the texture we are working on
	int TexSize[2];
	g_bTexViewReady = false;
	if (g_SelectedFaceTable.m_pfnGetFaceInfo( &g_SelectedFaceData, g_pSelectedFaceWinding ))
	{
		g_bPatch = false;
		int i;
		// we have something selected
		// setup: compute BBox for the winding ( in ST space )
		//++timo FIXME: move this in a C2DView member ? used as well for patches
		g_2DView.m_Mins[0] = +9999.0f; g_2DView.m_Mins[1] = +9999.0f; g_2DView.m_Maxs[0] = -9999.0f; g_2DView.m_Maxs[1] = -9999.0f;
		for ( i=0; i<g_pSelectedFaceWinding->numpoints; i++ )
		{
			if ( g_pSelectedFaceWinding->points[i][3] < g_2DView.m_Mins[0] )
				g_2DView.m_Mins[0] = g_pSelectedFaceWinding->points[i][3];
			if ( g_pSelectedFaceWinding->points[i][3] > g_2DView.m_Maxs[0] )
				g_2DView.m_Maxs[0] = g_pSelectedFaceWinding->points[i][3];
			if ( g_pSelectedFaceWinding->points[i][4] < g_2DView.m_Mins[1] )
				g_2DView.m_Mins[1] = g_pSelectedFaceWinding->points[i][4];
			if ( g_pSelectedFaceWinding->points[i][4] > g_2DView.m_Maxs[1] )
				g_2DView.m_Maxs[1] = g_pSelectedFaceWinding->points[i][4];
		}
		// NOTE: FitView will read and init TexSize
		FitView( hwndDlg, TexSize );
		// now init the work tables
		g_NumPoints = g_pSelectedFaceWinding->numpoints;
		for ( i=0; i<g_NumPoints; i++ )
		{
			g_WorkWinding.data[i][0] = g_pSelectedFaceWinding->points[i][3];
			g_WorkWinding.data[i][1] = g_pSelectedFaceWinding->points[i][4];
		}
		g_ControlPointsBFace.Init( g_NumPoints, &g_WorkWinding, &g_2DView, TexSize, &g_SelectedFaceData, &g_QglTable );
		// init snap-to-grid
		float fTexStep[2];
		fTexStep[0] = 1.0f / float(TexSize[0]);
		fTexStep[1] = 1.0f / float(TexSize[1]);
		g_2DView.SetGrid( fTexStep[0], fTexStep[1] );
		g_pManager = &g_ControlPointsBFace;
		// prepare the "Cancel" data
		memcpy( &g_CancelFaceData, &g_SelectedFaceData, sizeof(_QERFaceData) );
		// we are done
		g_bTexViewReady = true;
	}
	else if ( g_SurfaceTable.m_pfnAnyPatchesSelected())
	{
		g_pPatch = g_SurfaceTable.m_pfnGetSelectedPatch();
		g_bPatch = true;
		int i,j;
		// compute BBox for all patch points
		g_2DView.m_Mins[0] = +9999.0f; g_2DView.m_Mins[1] = +9999.0f; g_2DView.m_Maxs[0] = -9999.0f; g_2DView.m_Maxs[1] = -9999.0f;
		for ( i=0; i<g_pPatch->width; i++ )
		{
			for ( j=0; j<g_pPatch->height; j++ )
			{
				if ( g_pPatch->ctrl[i][j].st[0] < g_2DView.m_Mins[0] )
					g_2DView.m_Mins[0] = g_pPatch->ctrl[i][j].st[0];
				if ( g_pPatch->ctrl[i][j].st[0] > g_2DView.m_Maxs[0] )
					g_2DView.m_Maxs[0] = g_pPatch->ctrl[i][j].st[0];
				if ( g_pPatch->ctrl[i][j].st[1] < g_2DView.m_Mins[1] )
					g_2DView.m_Mins[1] = g_pPatch->ctrl[i][j].st[1];
				if ( g_pPatch->ctrl[i][j].st[1] > g_2DView.m_Maxs[1] )
					g_2DView.m_Maxs[1] = g_pPatch->ctrl[i][j].st[1];
			}
		}
		FitView( hwndDlg, TexSize);
		// init the work tables
		g_WorkPatch = *g_pPatch;
		g_ControlPointsPatch.Init( &g_WorkPatch, &g_2DView, &g_QglTable, g_pPatch );
		// init snap-to-grid
		float fTexStep[2];
		fTexStep[0] = 1.0f / float(TexSize[0]);
		fTexStep[1] = 1.0f / float(TexSize[1]);
		g_2DView.SetGrid( fTexStep[0], fTexStep[1] );
		g_pManager = &g_ControlPointsPatch;
		// prepare the "cancel" data
		g_CancelPatch = *g_pPatch;
		// we are done
		g_bTexViewReady = true;
	}
}

void Textool_Validate()
{
  // validate current situation into the main view
  g_pManager->Commit( );
  // for a brush face we have an aditionnal step
  if (!g_bPatch)
  {
		// tell Radiant to update (will also send update windows messages )
		g_SelectedFaceTable.m_pfnSetFaceInfo( &g_SelectedFaceData );
  }
  else
  {
		// ask to rebuild the patch display data
		g_pPatch->bDirty = true;
    // send a repaint to the camera window as well
    g_FuncTable.m_pfnSysUpdateWindows( W_CAMERA );
  }
  // we'll need to update after that as well:
  g_bTexViewReady = false;
  // send a repaint message
  SendMessage( g_hToolWnd, WM_PAINT, 0, 0 );
}

void Textool_Cancel()
{
  if (!g_bPatch)
  {
	  // tell Radiant to update (will also send update windows messages )
	  g_SelectedFaceTable.m_pfnSetFaceInfo( &g_CancelFaceData );
  }
  else
  {
	  *g_pPatch = g_CancelPatch;
    g_pPatch->bDirty = true;
    g_FuncTable.m_pfnSysUpdateWindows( W_CAMERA );
  }
  EndDialog( g_hToolWnd, TRUE );
}

BOOL CALLBACK DialogProc(
  HWND hwndDlg,  // handle to dialog box
  UINT uMsg,     // message
  WPARAM wParam, // first message parameter
  LPARAM lParam  // second message parameter
)
{

	if (!g_bTexViewReady && !g_bClosing)
	{
		InitTexView( hwndDlg );
		if (!g_bTexViewReady)
		{
			g_bClosing = true;
			MessageBox( g_hwndMain, "You must use brush primitives and have a patch or a single face selected for TexTool plugin.\nSee in your project settings for brush primitives and in plugins/TexToolHelp for documentation", "TexTool plugin", MB_ICONERROR | MB_OK );
			return 0;
		}
		else
			g_bClosing = false;
	}
	else if (!g_bTexViewReady && g_bClosing)
	{
		if ( uMsg == WM_KEYDOWN && LOWORD(wParam) == VK_ESCAPE )
		{
			EndDialog( hwndDlg, TRUE );
		}
		return 0;
	}

	if ( g_2DView.DialogProc( hwndDlg, uMsg, wParam, lParam ) )
		return TRUE;

	if ( g_pManager->DialogProc( hwndDlg, uMsg, wParam, lParam ) )
		return TRUE;

  // handle the two closing commands from outside
  if ((uMsg == WM_SYSCOMMAND && wParam == SC_CLOSE) || (uMsg == WM_KEYDOWN && LOWORD(wParam) == VK_ESCAPE))
  {
    Textool_Cancel();
  }

	switch (uMsg)
	{
		//++timo remove
	case WM_DESTROY:
		//++timo handle the WM_DESTROY to release OpenGL context
		Sys_Printf("WARNING: TexTool plugin needs code to release OpenGL context on WM_DESTROY\n");
		break;
	case WM_KEYDOWN:
    if (LOWORD(wParam) == VK_RETURN)
      Textool_Validate();
		break;
	case WM_SIZE:
	case WM_PAINT:
		if (g_bTexViewReady)
		{
			PAINTSTRUCT ps;
			int i,j;
			BeginPaint( hwndDlg, &ps );
			if ( !g_QglTable.m_pfn_qwglMakeCurrent( g_hToolDC, g_hGLRC ) )
			{
				Sys_Printf("TexTool: qwglMakeCurrent failed\n");
				return 0;
			}
			g_2DView.PreparePaint( g_QglTable );		
			g_QglTable.m_pfn_qglColor3f(1, 1, 1);
			// draw the texture background
			g_QglTable.m_pfn_qglPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			if (!g_bPatch)
			{
				g_QglTable.m_pfn_qglBindTexture( GL_TEXTURE_2D, g_SelectedFaceTable.m_pfnGetTextureNumber() );
			}
			else
			{
				g_QglTable.m_pfn_qglBindTexture( GL_TEXTURE_2D, g_pPatch->d_texture->texture_number );
			}
			g_QglTable.m_pfn_qglEnable( GL_TEXTURE_2D );
			g_QglTable.m_pfn_qglBegin( GL_QUADS );
			g_QglTable.m_pfn_qglTexCoord2f( g_2DView.m_Mins[0], g_2DView.m_Mins[1] );
			g_QglTable.m_pfn_qglVertex2f( g_2DView.m_Mins[0], g_2DView.m_Mins[1] );
			g_QglTable.m_pfn_qglTexCoord2f( g_2DView.m_Maxs[0], g_2DView.m_Mins[1] );
			g_QglTable.m_pfn_qglVertex2f( g_2DView.m_Maxs[0], g_2DView.m_Mins[1] );
			g_QglTable.m_pfn_qglTexCoord2f( g_2DView.m_Maxs[0], g_2DView.m_Maxs[1] );
			g_QglTable.m_pfn_qglVertex2f( g_2DView.m_Maxs[0], g_2DView.m_Maxs[1] );
			g_QglTable.m_pfn_qglTexCoord2f( g_2DView.m_Mins[0], g_2DView.m_Maxs[1] );
			g_QglTable.m_pfn_qglVertex2f( g_2DView.m_Mins[0], g_2DView.m_Maxs[1] );
			g_QglTable.m_pfn_qglEnd();
			g_QglTable.m_pfn_qglDisable( GL_TEXTURE_2D );
			if (!g_bPatch)
			{
				g_QglTable.m_pfn_qglBegin( GL_LINE_LOOP );
				for ( i=0; i<g_NumPoints; i++ )
				{
					g_QglTable.m_pfn_qglVertex2f( g_WorkWinding.data[i][0], g_WorkWinding.data[i][1] );
				}
				g_QglTable.m_pfn_qglEnd();
			}
			else
			{
				g_QglTable.m_pfn_qglBegin( GL_LINES );
				for ( i=0; i<g_pPatch->width; i++ )
					for ( j=0; j<g_pPatch->height; j++ )
					{
						if ( i < g_pPatch->width-1 )
						{
							g_QglTable.m_pfn_qglVertex2f( g_WorkPatch.ctrl[i][j].st[0], g_WorkPatch.ctrl[i][j].st[1] );
							g_QglTable.m_pfn_qglVertex2f( g_WorkPatch.ctrl[i+1][j].st[0], g_WorkPatch.ctrl[i+1][j].st[1] );
						}
						if ( j < g_pPatch->height-1 )
						{
							g_QglTable.m_pfn_qglVertex2f( g_WorkPatch.ctrl[i][j].st[0], g_WorkPatch.ctrl[i][j].st[1] );
							g_QglTable.m_pfn_qglVertex2f( g_WorkPatch.ctrl[i][j+1].st[0], g_WorkPatch.ctrl[i][j+1].st[1] );
						}
					}
				g_QglTable.m_pfn_qglEnd();
			}
			// let the control points manager render
			g_pManager->Render( );
			g_QglTable.m_pfn_qwglSwapBuffers( g_hToolDC );
			g_QglTable.m_pfn_QE_CheckOpenGLForErrors();
			EndPaint( hwndDlg, &ps);
		}
		break;
	}
	return 0;
}

void __declspec(dllexport) QERPlug_Dispatch(LPCSTR p, vec3_t vMin, vec3_t vMax, BOOL bSingleBrush)
{
	// if it's the first call, perhaps we need some additional init steps
	if (!g_bQglInitDone)
	{
		g_QglTable.m_nSize = sizeof(_QERQglTable);
		if ( g_FuncTable.m_pfnRequestInterface( QERQglTable_GUID, static_cast<LPVOID>(&g_QglTable) ) )
		{
			g_bQglInitDone = true;
		}
		else
		{
			Sys_Printf("TexTool plugin: _QERQglTable interface request failed\n");
			return;
		}
	}
	if (!g_bSelectedFaceInitDone)
	{
		g_SelectedFaceTable.m_nSize = sizeof(_QERSelectedFaceTable);
		if ( g_FuncTable.m_pfnRequestInterface( QERSelectedFaceTable_GUID, static_cast<LPVOID>(&g_SelectedFaceTable) ) )
		{
			g_bSelectedFaceInitDone = true;
		}
		else
		{
			Sys_Printf("TexTool plugin: _QERSelectedFaceTable interface request failed\n");
			return;
		}
	}
	if (!g_bSurfaceTableInitDone)
	{
		g_SurfaceTable.m_nSize = sizeof(_QERAppSurfaceTable);
		if ( g_FuncTable.m_pfnRequestInterface( QERAppSurfaceTable_GUID, static_cast<LPVOID>(&g_SurfaceTable) ) )
		{
			g_bSurfaceTableInitDone = true;
		}
		else
		{
			Sys_Printf("TexTool plugin: _QERAppSurfaceTable interface request failed\n");
			return;
		}
	}
	if (!strcmp(p, "About..."))
	{
		MessageBox( g_hwndMain, PLUGIN_ABOUT, "About ...", MB_OK );
	}
	else if (!strcmp(p, "Go..."))
	{
		if (!g_hToolWnd)
		{
			if ( !(g_hToolWnd = CreateDialog( g_hModule, MAKEINTRESOURCE( IDD_DIALOG2 ), g_hwndMain, DialogProc )) )
			{
				::MessageBox( NULL, "Error creating texture tools window!", "TexTool plugin", MB_ICONERROR | MB_OK );
				return;
			}
			g_hToolDC = ::GetDC( g_hToolWnd );		
			g_QglTable.m_pfn_QEW_SetupPixelFormat( g_hToolDC, 0 );
			if ( !(g_hGLRC = g_QglTable.m_pfn_qwglCreateContext( g_hToolDC )) )
			{
				::MessageBox( NULL, "Error creating OpenGL context!", "TexTool plugin", MB_ICONERROR | MB_OK );
				return;
			}
			// share the OpenGL display lists
			if (!g_QglTable.m_pfn_qwglShareLists( g_QglTable.m_pfnGetQeglobalsHGLRC(), g_hGLRC ))
			{
				::MessageBox( NULL, "Error sharing lists with new context!", "TexTool plugin", MB_ICONERROR | MB_OK );
				return;
			}
		}
		g_bTexViewReady = false;
		g_bClosing = false;
		ShowWindow( g_hToolWnd, SW_SHOW );
	}
	else if (!strcmp(p, "Clone texture..." ))
	{
		patchMesh_t *p1,*p2;
		g_SurfaceTable.m_pfnGetTwoSelectedPatch( &p1, &p2 );
		if (!p1 || !p2)
			Sys_Printf("Textool plugin: you must have two patches selected\n");
		else if ( p1->height != p2->height || p1->width != p2->width )
			Sys_Printf("Textool plugin: patches must be same size\n");
		else
		{
			p1->contents = p2->contents;
			p1->flags = p2->flags;
			p1->value = p2->value;
			p1->type = p2->type;
			p1->d_texture = p2->d_texture;
			int i,j;
			for ( i=0; i<p1->width; i++ )
				for ( j=0; j<p1->height; j++ )
				{
					p1->ctrl[i][j].st[0] = p2->ctrl[i][j].st[0];
					p1->ctrl[i][j].st[1] = p2->ctrl[i][j].st[1];
				}
			p1->bDirty = true;
		}
	}
	else if (!strcmp(p, "Help..."))
	{
		if (!g_bHelp)
			::MessageBox( NULL, "Select a brush face (ctrl+shift+left mouse) or a patch, and hit Go...\nSee tutorials for more", "TexTool plugin", MB_OK );
		else
			::MessageBox( NULL, "Are you kidding me ?", "TexTool plugin", MB_OK );
		g_bHelp = true;
	}
}
