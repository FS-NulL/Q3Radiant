//-----------------------------------------------------------------------------
//
// $LogFile$
// $Revision: 1.12 $
// $Author: ttimo $
// $Date: 2000/07/30 17:43:50 $
// $Log: igl.h,v $
// Revision 1.12  2000/07/30 17:43:50  ttimo
// G_Dewan additions and bug fixing
//
// Revision 1.11  2000/06/24 20:55:42  ttimo
// build 201
//
// Revision 1.7.2.1.2.5  2000/05/08 22:59:09  ttimo
// start of 3D drawing in the plugin API
//
// Revision 1.7.2.1.2.4  2000/05/03 23:17:29  ttimo
// started adding the Cam drawing hooks
//
// Revision 1.7.2.1.2.3  2000/05/03 05:14:30  ttimo
// commit after merge from HEAD - MGSPlugin - RC32Branch - Id198Import
//
//
// Revision 1.7.2.1.2.2  2000/04/28 23:54:45  maj
// Adding glMaterialfv, glMaterialf and glLightfv to the plugin gl interface
//
// Revision 1.7.2.1.2.1  2000/04/06 06:21:28  ttimo
// under work .. merged from q3r 197
//

// Revision 1.7.2.1  2000/02/04 22:59:34  ttimo
// messaging API preview
//

// Revision 1.9  2000/03/26 09:37:24  maj
// Adding glColor3/4fv, glDepthFunc, glDepthMask, glHint, glTexCoord2fv and glVertex3fv to plugin interface
//
// Revision 1.8  2000/03/24 00:10:31  ttimo
// Release 197 source code, everything merged from MGSPlugin branch (except ShadersCleanup)
//
// Revision 1.7.2.4  2000/03/20 22:54:03  ttimo
// some new GL bindings, brush primit format in brushes in /out
//
// Revision 1.7.2.3  2000/02/16 15:43:14  ttimo
// Curry bug #101996: added glTexParameterf and glNormal3f
//
// Revision 1.7.2.2  2000/02/13 17:48:07  ttimo
// Curry bug #101874 - added glTexGenf to GL interface
//
// Revision 1.7.2.1  2000/02/04 22:59:34  ttimo
// messaging API preview
//
// Revision 1.10  2000/05/03 04:13:43  ttimo
// commit after merge from MGSPlugin - RC32Branch - Id198Import
//
// Revision 1.9  2000/03/26 09:37:24  maj
// Adding glColor3/4fv, glDepthFunc, glDepthMask, glHint, glTexCoord2fv and glVertex3fv to plugin interface
//
// Revision 1.8  2000/03/24 00:10:31  ttimo
// Release 197 source code, everything merged from MGSPlugin branch (except ShadersCleanup)
//
// Revision 1.7.2.4  2000/03/20 22:54:03  ttimo
// some new GL bindings, brush primit format in brushes in /out
//
// Revision 1.7.2.3  2000/02/16 15:43:14  ttimo
// Curry bug #101996: added glTexParameterf and glNormal3f
//
// Revision 1.7.2.2  2000/02/13 17:48:07  ttimo
// Curry bug #101874 - added glTexGenf to GL interface
//
// Revision 1.7.2.1  2000/02/04 22:59:34  ttimo
// messaging API preview
//
// Revision 1.7.2.4  2000/03/20 22:54:03  ttimo
// some new GL bindings, brush primit format in brushes in /out
//
// Revision 1.7.2.3  2000/02/16 15:43:14  ttimo
// Curry bug #101996: added glTexParameterf and glNormal3f
//
// Revision 1.7.2.2  2000/02/13 17:48:07  ttimo
// Curry bug #101874 - added glTexGenf to GL interface
//
// Revision 1.7.2.1  2000/02/04 22:59:34  ttimo
// messaging API preview
//
// Revision 1.7  2000/01/18 00:43:59  ttimo
// RC
//
// Revision 1.6  2000/01/13 00:46:41  ttimo
// Merged in patches in / out
// Revision 1.1.1.4  2000/01/18 00:17:12  ttimo
// merging in for RC
//
// Revision 1.5  2000/01/11 22:14:48  ttimo
// cleaning up after merging and thinks badly fucked up
//
// Revision 1.2  2000/01/09 04:39:14  ttimo
// added some bindings to the GL table
//
// Revision 1.1.1.3  2000/01/11 16:43:15  ttimo
// merged version from old reposit and Robert update
//
// Revision 1.1.1.1  2000/01/07 17:17:30  ttimo
// initial import of Q3Radiant module
//
// Revision 1.1.1.1  2000/01/07 17:17:30  ttimo
// initial import of Q3Radiant module
//
// Revision 1.3  2000/01/17 23:53:44  TBesset
// ready for merge in sourceforge (RC candidate)
//
// Revision 1.2  2000/01/07 16:40:13  TBesset
// merged from BSP frontend
//
// Revision 1.1.1.3  1999/12/29 18:31:27  TBesset
// Q3Radiant public version
//
// Revision 1.1.1.1.2.1  1999/12/29 21:39:45  TBesset
// updated to update3 from Robert
//
// Revision 1.1.1.3  1999/12/29 18:31:27  TBesset
// Q3Radiant public version
// Revision 1.1.1.3  1999/12/29 18:31:27  TBesset
// Q3Radiant public version
//
// Revision 1.2  1999/11/22 17:46:48  Timo & Christine
// merged EARadiant into the main tree
// bug fixes for Q3Plugin / EAPlugin
// export for Robert
//
// Revision 1.1.2.2  1999/11/03 20:38:04  Timo & Christine
// MEAN plugin for Q3Radiant, alpha version
//
// Revision 1.1.2.1.2.1  1999/10/27 08:34:29  Timo & Christine
// preview version of the texture tools plugin is ready
// ( TexTool.dll plugin is in Q3Plugin module )
// plugins can draw in their own window using Radiant's qgl bindings
//
// Revision 1.1.2.1  1999/10/08 16:28:14  Timo & Christine
// started plugin extensions for EA features support in Q3Radiant
// MEAN files plugin, and Surface Properties plugin
//
//
// DESCRIPTION:
// all purpose OpenGL interface for Q3Radiant plugins
//

#ifndef __IGL_H__
#define __IGL_H__

// we use these classes to let plugins draw inside the Radiant windows
// 2D window like YZ XZ XY
class IGL2DWindow
{
public:
	// Increment the number of references to this object
	virtual void IncRef () = 0;
	// Decrement the reference count
	virtual void DecRef () = 0;
	virtual void Draw2D( VIEWTYPE vt ) = 0;
};
// 3D window
class IGL3DWindow
{
public:
	// Increment the number of references to this object
	virtual void IncRef () = 0;
	// Decrement the reference count
	virtual void DecRef () = 0;
  virtual void Draw3D() = 0;
};

// define a GUID for this interface so plugins can access and reference it
// {0F237620-854B-11d3-8EF3-A2DFBD53251B}
static const GUID QERQglTable_GUID = 
{ 0xf237620, 0x854b, 0x11d3, { 0x8e, 0xf3, 0xa2, 0xdf, 0xbd, 0x53, 0x25, 0x1b } };

#include <gl/gl.h>
#include <gl/glu.h>

typedef void (APIENTRY* PFN_QGLALPHAFUNC)		(GLenum func, GLclampf ref);
typedef void (APIENTRY* PFN_QGLBEGIN)			(GLenum);
typedef void (APIENTRY* PFN_QGLBINDTEXTURE)		(GLenum target, GLuint texture);
typedef void (APIENTRY* PFN_QGLBLENDFUNC)       (GLenum sfactor, GLenum dfactor);
typedef void (APIENTRY* PFN_QGLCALLLIST)        (GLuint list);
typedef void (APIENTRY* PFN_QGLCALLLISTS)       (GLsizei n, GLenum type, const GLvoid *lists);
typedef void (APIENTRY* PFN_QGLCLEAR)			(GLbitfield mask);
typedef void (APIENTRY* PFN_QGLCLEARCOLOR)		(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void (APIENTRY* PFN_QGLCLEARDEPTH)      (GLclampd depth);
typedef void (APIENTRY* PFN_QGLCOLOR3F)			(GLfloat red, GLfloat green, GLfloat blue);
typedef void (APIENTRY* PFN_QGLCOLOR3FV)        (const GLfloat *v);
typedef void (APIENTRY* PFN_QGLCOLOR4F)			(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (APIENTRY* PFN_QGLCOLOR4FV)        (const GLfloat *v);
typedef void (APIENTRY* PFN_QGLCULLFACE)        (GLenum mode);
typedef void (APIENTRY* PFN_QGLDELETELISTS)     (GLuint list, GLsizei range);
typedef void (APIENTRY* PFN_QGLDEPTHFUNC)       (GLenum func);
typedef void (APIENTRY* PFN_QGLDEPTHMASK)       (GLboolean flag);
typedef void (APIENTRY* PFN_QGLDISABLE)			(GLenum cap);
typedef void (APIENTRY* PFN_QGLENABLE)			(GLenum cap);
typedef void (APIENTRY* PFN_QGLEND)				();
typedef void (APIENTRY* PFN_QGLENDLIST)         (void);
typedef GLuint (APIENTRY* PFN_QGLGENLISTS)      (GLsizei range);
typedef void (APIENTRY* PFN_QGLHINT)            (GLenum target, GLenum mode);
typedef void (APIENTRY* PFN_QGLLIGHTFV)			(GLenum light, GLenum pname, const GLfloat *params);
typedef void (APIENTRY* PFN_QGLLINEWIDTH)       (GLfloat size);
typedef void (APIENTRY* PFN_QGLLISTBASE)        (GLuint base);
typedef void (APIENTRY* PFN_QGLLOADIDENTITY)	();
typedef void (APIENTRY* PFN_QGLMATERIALF)		(GLenum face, GLenum pname, GLfloat param);
typedef void (APIENTRY* PFN_QGLMATERIALFV)		(GLenum face, GLenum pname, const GLfloat *params);
typedef void (APIENTRY* PFN_QGLMATRIXMODE)		(GLenum mode);
typedef void (APIENTRY* PFN_QGLNEWLIST)         (GLuint list, GLenum mode);
typedef void (APIENTRY* PFN_QGLNORMAL3F)		(GLfloat nx, GLfloat ny, GLfloat nz);
typedef void (APIENTRY* PFN_QGLORTHO)			(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
typedef void (APIENTRY*	PFN_QGLPOINTSIZE)		(GLfloat size);
typedef void (APIENTRY* PFN_QGLPOLYGONMODE)		(GLenum face, GLenum mode);
typedef void (APIENTRY* PFN_QGLPOPMATRIX)		();
typedef void (APIENTRY* PFN_QGLPUSHMATRIX)		();
typedef void (APIENTRY* PFN_QGLROTATED)         (GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
typedef void (APIENTRY* PFN_QGLROTATEF)			(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY* PFN_QGLSCALEF)			(GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY* PFN_QGLTEXCOORD2F)		(GLfloat s, GLfloat t);
typedef void (APIENTRY* PFN_QGLTEXCOORD2FV)     (const GLfloat *v);
typedef void (APIENTRY* PFN_QGLTEXENVF)			(GLenum target, GLenum pname, GLfloat param);
typedef void (APIENTRY* PFN_QGLTEXGENF)			(GLenum coord, GLenum pname, GLfloat param);
typedef void (APIENTRY* PFN_QGLTEXPARAMETERF)	(GLenum target, GLenum pname, GLfloat param);
typedef void (APIENTRY* PFN_QGLTRANSLATED)      (GLdouble x, GLdouble y, GLdouble z);
typedef void (APIENTRY* PFN_QGLTRANSLATEF)		(GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY* PFN_QGLVERTEX2F)		(GLfloat x, GLfloat y);
typedef void (APIENTRY* PFN_QGLVERTEX3F)		(GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY* PFN_QGLVERTEX3FV)       (const GLfloat *v);
typedef void (APIENTRY* PFN_QGLVIEWPORT)		(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (APIENTRY* PFN_QGLPUSHATTRIB)		(GLbitfield mask); // GDD: added line
typedef void (APIENTRY* PFN_QGLPOPATTRIB)		(); // GDD: added line
typedef void (APIENTRY* PFN_QGLGETDOUBLEV)		(GLenum pname, GLdouble * params); // GDD: added line
typedef void (APIENTRY* PFN_QGLGETINTEGERV)		(GLenum pname, GLint * params); // GDD: added line
typedef void (APIENTRY* PFN_QGLSHADEMODEL)		(GLenum mode); // GDD: added line
typedef void (APIENTRY* PFN_QGLFOGI)			(GLenum pname, GLint param); // GDD: added line
typedef void (APIENTRY* PFN_QGLFOGF)			(GLenum pname, GLfloat param); // GDD: added line
typedef void (APIENTRY* PFN_QGLFOGFV)			(GLenum pname, const GLfloat *param); // GDD: added line
 
typedef void	(WINAPI* PFN_QE_CHECKOPENGLFORERRORS)	();
typedef int		(WINAPI* PFN_QEW_SETUPPIXELFORMAT)		(HDC hDC, qboolean zbuffer );
typedef HGLRC	(WINAPI* PFN_QWGL_CREATECONTEXT)		(HDC);
typedef BOOL	(WINAPI* PFN_QWGL_DELETECONTEXT)        (HGLRC);
typedef BOOL	(WINAPI* PFN_QWGL_MAKECURRENT)			(HDC, HGLRC);
typedef BOOL	(WINAPI* PFN_QWGL_SHARELISTS)			(HGLRC, HGLRC);
typedef BOOL	(WINAPI* PFN_QWGL_SWAPBUFFERS)			(HDC);
typedef BOOL    (WINAPI* PFN_QWGL_USEFONTBITMAPS)       (HDC, DWORD, DWORD, DWORD);

// glu stuff
typedef void (APIENTRY * PFN_QGLUPERSPECTIVE) (GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
typedef void (APIENTRY * PFN_QGLULOOKAT)(
  GLdouble eyex,
  GLdouble eyey,
  GLdouble eyez,
  GLdouble centerx,
  GLdouble centery,
  GLdouble centerz,
  GLdouble upx,
  GLdouble upy,
  GLdouble upz);


// specially provided by the editor for plugins
typedef void (WINAPI* PFN_QERAPP_GETCAMERAINFO) (float *cam_info); // X, Y, Z, yaw, pitch
typedef HGLRC	(WINAPI* PFN_QERAPP_GETQEGLOBALSHGLRC)	();

// plugins drawing inside the GL windows
//++timo TODO: add hooking into other windows (Z and .. texture??)
//+timo NOTE: this could be moved to the messaging system instead of having a dedicated interface <- yet I don't know how
typedef void (WINAPI* PFN_QERAPP_HOOKGL2DWINDOW) (IGL2DWindow *);
typedef void (WINAPI* PFN_QERAPP_UNHOOKGL2DWINDOW) (IGL2DWindow *);
typedef void (WINAPI* PFN_QERAPP_HOOKGL3DWINDOW) (IGL3DWindow *);
typedef void (WINAPI* PFN_QERAPP_UNHOOKGL3DWINDOW) (IGL3DWindow *);

struct _QERQglTable
{
	//++timo do we really wanna play with versions ?
//	float m_fVersion;
	int m_nSize;
	PFN_QGLALPHAFUNC		m_pfn_qglAlphaFunc;
	PFN_QGLBEGIN			m_pfn_qglBegin;
	PFN_QGLBINDTEXTURE		m_pfn_qglBindTexture;
	PFN_QGLBLENDFUNC		m_pfn_qglBlendFunc;
	PFN_QGLCALLLIST			m_pfn_qglCallList;
	PFN_QGLCLEAR			m_pfn_qglClear;
	PFN_QGLCLEARCOLOR		m_pfn_qglClearColor;
	PFN_QGLCALLLISTS		m_pfn_qglCallLists;
	PFN_QGLCLEARDEPTH		m_pfn_qglClearDepth;
	PFN_QGLCOLOR3F			m_pfn_qglColor3f;
	PFN_QGLCOLOR3FV         m_pfn_qglColor3fv;
	PFN_QGLCOLOR4F			m_pfn_qglColor4f;
	PFN_QGLCOLOR4FV         m_pfn_qglColor4fv;
	PFN_QGLCULLFACE			m_pfn_qglCullFace;
	PFN_QGLDELETELISTS		m_pfn_qglDeleteLists;
	PFN_QGLDEPTHFUNC        m_pfn_qglDepthFunc;
	PFN_QGLDEPTHMASK        m_pfn_qglDepthMask;
	PFN_QGLDISABLE			m_pfn_qglDisable;
	PFN_QGLENABLE			m_pfn_qglEnable;
	PFN_QGLEND				m_pfn_qglEnd;
	PFN_QGLENDLIST			m_pfn_qglEndList;
	PFN_QGLGENLISTS			m_pfn_qglGenLists;
	PFN_QGLHINT             m_pfn_qglHint;
	PFN_QGLLIGHTFV          m_pfn_qglLightfv;
	PFN_QGLLINEWIDTH        m_pfn_qglLineWidth;
	PFN_QGLLISTBASE			m_pfn_qglListBase;
	PFN_QGLLOADIDENTITY		m_pfn_qglLoadIdentity;
	PFN_QGLMATERIALF		m_pfn_qglMaterialf;
	PFN_QGLMATERIALFV       m_pfn_qglMaterialfv;
	PFN_QGLMATRIXMODE		m_pfn_qglMatrixMode;
	PFN_QGLNEWLIST			m_pfn_qglNewList;
	PFN_QGLNORMAL3F			m_pfn_qglNormal3f;
	PFN_QGLORTHO			m_pfn_qglOrtho;
	PFN_QGLPOINTSIZE		m_pfn_qglPointSize;
	PFN_QGLPOLYGONMODE		m_pfn_qglPolygonMode;
	PFN_QGLPOPMATRIX		m_pfn_qglPopMatrix;
	PFN_QGLPUSHMATRIX		m_pfn_qglPushMatrix;
	PFN_QGLROTATED			m_pfn_qglRotated;
	PFN_QGLROTATEF			m_pfn_qglRotatef;
	PFN_QGLSCALEF			m_pfn_qglScalef;
	PFN_QGLTEXCOORD2F		m_pfn_qglTexCoord2f;
	PFN_QGLTEXCOORD2FV      m_pfn_qglTexCoord2fv;
	PFN_QGLTEXENVF			m_pfn_qglTexEnvf;
	PFN_QGLTEXGENF			m_pfn_qglTexGenf;
	PFN_QGLTEXPARAMETERF	m_pfn_qglTexParameterf;
	PFN_QGLTRANSLATED		m_pfn_qglTranslated;
	PFN_QGLTRANSLATEF		m_pfn_qglTranslatef;
	PFN_QGLVERTEX2F			m_pfn_qglVertex2f;
	PFN_QGLVERTEX3F			m_pfn_qglVertex3f;
	PFN_QGLVERTEX3FV        m_pfn_qglVertex3fv;
	PFN_QGLVIEWPORT			m_pfn_qglViewport;
	PFN_QGLPUSHATTRIB		m_pfn_qglPushAttrib; // GDD: added line
	PFN_QGLPOPATTRIB		m_pfn_qglPopAttrib; // GDD: added line
	PFN_QGLGETDOUBLEV		m_pfn_qglGetDoublev; // GDD: added line
	PFN_QGLGETINTEGERV		m_pfn_qglGetIntegerv; // GDD: added line
	PFN_QGLSHADEMODEL		m_pfn_qglShadeModel; // GDD: added line
	PFN_QGLFOGI				m_pfn_qglFogi; // GDD: added line
	PFN_QGLFOGF				m_pfn_qglFogf; // GDD: added line
	PFN_QGLFOGFV			m_pfn_qglFogfv; // GDD: added line
 
	PFN_QE_CHECKOPENGLFORERRORS	m_pfn_QE_CheckOpenGLForErrors;
	PFN_QEW_SETUPPIXELFORMAT	m_pfn_QEW_SetupPixelFormat;
	PFN_QWGL_CREATECONTEXT		m_pfn_qwglCreateContext;
	PFN_QWGL_DELETECONTEXT      m_pfn_qwglDeleteContext;
	PFN_QWGL_MAKECURRENT		m_pfn_qwglMakeCurrent;
	PFN_QWGL_SHARELISTS			m_pfn_qwglShareLists;
	PFN_QWGL_SWAPBUFFERS		m_pfn_qwglSwapBuffers;
	PFN_QWGL_USEFONTBITMAPS		m_pfn_qwglUseFontBitmaps;

	PFN_QERAPP_GETQEGLOBALSHGLRC	m_pfnGetQeglobalsHGLRC;
	PFN_QERAPP_GETCAMERAINFO		m_pfnGetCameraInfo;

	// glu stuff
	PFN_QGLUPERSPECTIVE			m_pfn_qgluPerspective;
	PFN_QGLULOOKAT				m_pfn_qgluLookAt;

	// plugin entities drawing inside Radiant windows
	PFN_QERAPP_HOOKGL2DWINDOW	m_pfnHookGL2DWindow;
	PFN_QERAPP_UNHOOKGL2DWINDOW	m_pfnUnHookGL2DWindow;
  PFN_QERAPP_HOOKGL3DWINDOW m_pfnHookGL3DWindow;
  PFN_QERAPP_UNHOOKGL3DWINDOW m_pfnUnHookGL3DWindow;
};

#endif