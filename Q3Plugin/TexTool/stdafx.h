// stdafx.h
// precompiled headers

// standard headers
#include <windows.h>
#include <stdio.h>
#include "resource.h"

// plugin
extern "C" void Sys_Printf (char *text, ...);
extern HINSTANCE g_hModule;
#include "qerplugin.h"
#include "mathlib.h"
#include "igl.h"
#include "ISelectedFace.h"
#include "isurfaceplugin.h"

// internals
#include "2DView.h"
typedef struct
{
	float	data[MAX_POINTS_ON_WINDING][2];
} CtrlPts_t;
#include "ControlPointsManager.h"

extern _QERFuncTable_1			g_FuncTable;
// prefs globals
// NOTE: these are used by the CControlPointsManager classes, not very C++ish
extern bool						g_bPrefsUpdateCameraView;
extern _QERSelectedFaceTable	g_SelectedFaceTable;
extern _QERFaceData				g_CancelFaceData;

// call to validate the current changes into the editor
extern void Textool_Validate();
extern void Textool_Cancel();
extern HWND	g_hToolWnd;