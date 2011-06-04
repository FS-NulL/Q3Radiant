//-----------------------------------------------------------------------------
//
// $LogFile$
// $Revision: 1.5 $
// $Author: ttimo $
// $Date: 2000/07/30 17:43:50 $
// $log$
// Revision 1.1.1.3  1999/12/29 18:31:26  TBesset
// Q3Radiant public version
//
// Revision 1.2  1999/11/22 17:46:45  Timo & Christine
// merged EARadiant into the main tree
// bug fixes for Q3Plugin / EAPlugin
// export for Robert
//
// Revision 1.1.4.1  1999/11/03 20:37:59  Timo & Christine
// MEAN plugin for Q3Radiant, alpha version
//
// Revision 1.1.2.1  1999/10/27 08:34:26  Timo & Christine
// preview version of the texture tools plugin is ready
// ( TexTool.dll plugin is in Q3Plugin module )
// plugins can draw in their own window using Radiant's qgl bindings
//
//
// DESCRIPTION:
// Quick interface hack for selected face interface
// this one really needs more work, but I'm in a hurry with TexTool

#include "stdafx.h"

// stores objects that want to be hooked into drawing in the XY window or Camera view
//++timo TODO: add support for Z view ... (texture view?)
CPtrArray l_GL2DWindows;
CPtrArray l_GL3DWindows;

float qer_cam_x = 0.0f;
float qer_cam_y = 0.0f;
float qer_cam_z = 0.0f;
float qer_cam_yaw = 0.0f;
float qer_cam_pitch = 0.0f;

void WINAPI QERApp_GetCameraInfo(float *cam_info)
{
	cam_info[0] = qer_cam_x;
	cam_info[1] = qer_cam_y;
	cam_info[2] = qer_cam_z;
	cam_info[3] = qer_cam_yaw;
	cam_info[4] = qer_cam_pitch;
}

int WINAPI QERApp_ISelectedFace_GetTextureNumber()
{
	if (g_ptrSelectedFaces.GetSize() > 0)
	{
		face_t *selFace = reinterpret_cast<face_t*>(g_ptrSelectedFaces.GetAt(0));
		return selFace->d_texture->texture_number;
	}
	//++timo hu ? find the appropriate gl bind number
	return 0;
}

void WINAPI QERApp_HookGL2DWindow(IGL2DWindow* pGLW)
{
	l_GL2DWindows.Add( pGLW );
	pGLW->IncRef();
}

void WINAPI QERApp_UnHookGL2DWindow(IGL2DWindow* pGLW)
{
	for( int i = 0; i < l_GL2DWindows.GetSize(); i++ )
	{
		if (l_GL2DWindows.GetAt(i) == pGLW)
		{
			l_GL2DWindows.RemoveAt(i);
			pGLW->DecRef();
			return;
		}
	}
#ifdef _DEBUG
	Sys_Printf("ERROR: IGL2DWindow* not found in QERApp_UnHookGL2DWindow\n");
#endif
}

void Draw2DPluginEntities( VIEWTYPE vt )
{
	for(int i = 0; i<l_GL2DWindows.GetSize(); i++ )
		static_cast<IGL2DWindow*>(l_GL2DWindows.GetAt(i))->Draw2D( vt );
}

void WINAPI QERApp_HookGL3DWindow(IGL3DWindow* pGLW)
{
  l_GL3DWindows.Add( pGLW );
  pGLW->IncRef();
}

void WINAPI QERApp_UnHookGL3DWindow(IGL3DWindow* pGLW)
{
	for( int i = 0; i < l_GL3DWindows.GetSize(); i++ )
	{
		if (l_GL3DWindows.GetAt(i) == pGLW)
		{
			l_GL3DWindows.RemoveAt(i);
			pGLW->DecRef();
			return;
		}
	}
#ifdef _DEBUG
	Sys_Printf("ERROR: IGL3DWindow* not found in QERApp_UnHookGL3DWindow\n");
#endif
}

void Draw3DPluginEntities()
{
	for(int i = 0; i<l_GL3DWindows.GetSize(); i++ )
		static_cast<IGL3DWindow*>(l_GL3DWindows.GetAt(i))->Draw3D();
}