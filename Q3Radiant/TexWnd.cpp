// TexWnd.cpp : implementation file
//

#include "stdafx.h"
#include <assert.h>
#include "Radiant.h"
#include "TexWnd.h"
#include "qe3.h"
#include "io.h"
#include "PrefsDlg.h"
#include "shaderinfo.h"
#include "pakstuff.h"
#include "str.h"
#include "PrefsDlg.h"

Str m_gStr;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FOLDER_CONTRACT_SIZE 65

#define	TYP_MIPTEX	68
static unsigned	tex_palette[256];

//++timo: this is a remain from old texture code, maybe should add a noshader pointing to "radiant/notex"
// anyway, this notexture* is supposed to == QERApp_Shader_ForName("radiant/notex")->getTexture();
qtexture_t	*notexture = NULL;
qtexture_t	*g_pluginTexture = NULL;

static qboolean	nomips = false;

#define	FONT_HEIGHT	10

HGLRC s_hglrcTexture = NULL;
HDC	 s_hdcTexture = NULL;

//int		texture_mode = GL_NEAREST;
//int		texture_mode = GL_NEAREST_MIPMAP_NEAREST;
//int		texture_mode = GL_NEAREST_MIPMAP_LINEAR;
//int		texture_mode = GL_LINEAR;
//int		texture_mode = GL_LINEAR_MIPMAP_NEAREST;
int		texture_mode = GL_LINEAR_MIPMAP_LINEAR;

// this is the global counter for GL bind numbers
int		texture_extension_number = 1;
int g_nCurrentTextureMenuName;

int g_nTextureOffset = 0;

// current active texture directory
//++timo FIXME: I'm not sure this is used anymore
char		texture_directory[128];
// if true, the texture window will only display in-use shaders
// if false, all the shaders in memory are displayed
qboolean g_bShowAllShaders;

bool g_bFilterEnabled = false;
CString g_strFilter;

// texture layout functions
// TTimo: now based on shaders
int			nActiveShadersCount;
int			nCurrentShader;
IShader*	pCurrentShader;
qtexture_t	*current_texture = NULL;
int			current_x, current_y, current_row;

// globals for textures
int			  texture_nummenus;
char		  texture_menunames[MAX_TEXTUREDIRS][128];
CPtrArray lstSubMenuHandles;

void SelectTexture (int mx, int my, bool bShift, bool bFitScale=false);

void	Texture_MouseDown (int x, int y, int buttons);
void	Texture_MouseUp (int x, int y, int buttons);
void	Texture_MouseMoved (int x, int y, int buttons);

CPtrArray g_lstSkinCache;

// TTimo: modifed to add a qtexture_t, Texture_LoadSkin loads using the shader API / QERApp_TryTexture_ForName
// m_strName is a copy of qtex->name
struct SkinInfo
{
  CString m_strName;
  int m_nTextureBind;
	qtexture_t *m_qtex;
  SkinInfo(const char *pName, int n, qtexture_t *qtex)
  {
    m_strName = pName;
    m_nTextureBind = n;
		m_qtex = qtex;
  };
  SkinInfo(){};
};

#if 0
// checks wether a qtexture_t exists for a given name
//++timo FIXME: is this really any use? redundant.
bool ShaderQTextureExists(const char *pName)
{
  for (qtexture_t *q=g_qeglobals.d_qtextures ; q ; q=q->next)
  {
    if (!strcmp(q->name,  pName))
    {
      return true;
    }
  }
  return false;

}
#endif

// gets active texture extension
// 
// FIXME: fix this to be generic from project file
//
int GetTextureExtensionCount()
{
  return 2;
}

const char* GetTextureExtension(int nIndex)
{
  if ( nIndex == 0)
  {
    _QERTextureInfo *pInfo = g_pParentWnd->GetPlugInMgr().GetTextureInfo();
    const char *pTex = (pInfo != NULL) ? pInfo->m_TextureExtension : NULL;
    return (pTex == NULL) ? (g_PrefsDlg.m_bHiColorTextures == FALSE) ? "wal" : "tga" : pTex;
  }
  // return jpg for 2nd extension
  return "jpg";
}

/*
==============
Texture_InitPalette
==============
*/
void Texture_InitPalette (byte *pal)
{
    int		r,g,b,v;
    int		i;
	int		inf;
	byte	gammatable[256];
	float	gamma;

	gamma = g_qeglobals.d_savedinfo.fGamma;

	if (gamma == 1.0)
	{
		for (i=0 ; i<256 ; i++)
			gammatable[i] = i;
	}
	else
	{
		for (i=0 ; i<256 ; i++)
		{
			inf = 255 * pow ( (float)( (((float)i)+0.5)/255.5 ), (float)gamma ) + 0.5;
			if (inf < 0)
				inf = 0;
			if (inf > 255)
				inf = 255;
			gammatable[i] = inf;
		}
	}

    for (i=0 ; i<256 ; i++)
    {
		  r = gammatable[pal[0]];
		  g = gammatable[pal[1]];
		  b = gammatable[pal[2]];
		  pal += 3;
		
		  v = (r<<24) + (g<<16) + (b<<8) + 255;
		  v = BigLong (v);
		
		  tex_palette[i] = v;
    }
}

void SetTexParameters (void)
{
	qglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_mode );
	
	switch ( texture_mode )
	{
	case GL_NEAREST:
	case GL_NEAREST_MIPMAP_NEAREST:
	case GL_NEAREST_MIPMAP_LINEAR:
		qglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		break;
	case GL_LINEAR:
	case GL_LINEAR_MIPMAP_NEAREST:
	case GL_LINEAR_MIPMAP_LINEAR:
		qglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		break;
	}
}

/*
============
Texture_SetMode
============
*/
void Texture_SetMode(int iMenu)
{
	int	i, iMode;
	HMENU hMenu;
	qboolean texturing = true;

	hMenu = GetMenu(g_qeglobals.d_hwndMain);

	switch(iMenu) {
	case ID_VIEW_NEAREST:					
		iMode = GL_NEAREST;
		break;
	case ID_VIEW_NEARESTMIPMAP:
		iMode = GL_NEAREST_MIPMAP_NEAREST;
		break;
	case ID_VIEW_LINEAR:
		iMode = GL_NEAREST_MIPMAP_LINEAR;
		break;
	case ID_VIEW_BILINEAR:
		iMode = GL_LINEAR;
		break;
	case ID_VIEW_BILINEARMIPMAP:
		iMode = GL_LINEAR_MIPMAP_NEAREST;
		break;
	case ID_VIEW_TRILINEAR:
		iMode = GL_LINEAR_MIPMAP_LINEAR;
		break;

	case ID_TEXTURES_WIREFRAME:
		iMode = 0;
		texturing = false;
		break;

	case ID_TEXTURES_FLATSHADE:
		iMode = 0;
		texturing = false;
		break;

	}

	CheckMenuItem(hMenu, ID_VIEW_NEAREST, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_VIEW_NEARESTMIPMAP, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_VIEW_LINEAR, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_VIEW_BILINEARMIPMAP, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_VIEW_BILINEAR, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_VIEW_TRILINEAR, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_TEXTURES_WIREFRAME, MF_BYCOMMAND | MF_UNCHECKED);
	CheckMenuItem(hMenu, ID_TEXTURES_FLATSHADE, MF_BYCOMMAND | MF_UNCHECKED);

	CheckMenuItem(hMenu, iMenu, MF_BYCOMMAND | MF_CHECKED);

	g_qeglobals.d_savedinfo.iTexMenu = iMenu;
	texture_mode = iMode;

  if (g_PrefsDlg.m_bSGIOpenGL)
  {
    if (s_hdcTexture && s_hglrcTexture)
    {
      //if (!qwglMakeCurrent(g_qeglobals.d_hdcBase, g_qeglobals.d_hglrcBase))
      if (!qwglMakeCurrent(s_hdcTexture, s_hglrcTexture))
		    Error ("wglMakeCurrent in LoadTexture failed");
    }
    else
      return;
  }

	if ( texturing )
		SetTexParameters ();

	if ( !texturing && iMenu == ID_TEXTURES_WIREFRAME)
	{
		g_pParentWnd->GetCamera()->Camera().draw_mode = cd_wire;
		Map_BuildBrushData();
		Sys_UpdateWindows (W_ALL);
		return;

	} else if ( !texturing && iMenu == ID_TEXTURES_FLATSHADE) {

		g_pParentWnd->GetCamera()->Camera().draw_mode = cd_solid;
		Map_BuildBrushData();
		Sys_UpdateWindows (W_ALL);
		return;
	}

	for (i=1 ; i<texture_extension_number ; i++)
	{
		qglBindTexture( GL_TEXTURE_2D, i );
		SetTexParameters ();
	}

	// select the default texture
	qglBindTexture( GL_TEXTURE_2D, 0 );

	qglFinish();

	if (g_pParentWnd->GetCamera()->Camera().draw_mode != cd_texture)
	{
		g_pParentWnd->GetCamera()->Camera().draw_mode = cd_texture;
		Map_BuildBrushData();
	}

	Sys_UpdateWindows (W_ALL);
}

/*
================
R_MipMap

Operates in place, quartering the size of the texture
================
*/
void R_MipMap (byte *in, int &width, int &height)
{
	int		i, j;
	byte	*out;
	int		row;
	
	row = width * 4;
	width >>= 1;
	height >>= 1;
	out = in;
	for (i=0 ; i<height ; i++, in+=row)
	{
		for (j=0 ; j<width ; j++, out+=4, in+=8)
		{
			out[0] = (in[0] + in[4] + in[row+0] + in[row+4])>>2;
			out[1] = (in[1] + in[5] + in[row+1] + in[row+5])>>2;
			out[2] = (in[2] + in[6] + in[row+2] + in[row+6])>>2;
			out[3] = (in[3] + in[7] + in[row+3] + in[row+7])>>2;
		}
	}
}

/*
=================
Texture_LoadTexture
=================
*/
//++timo FIXME: this is dead .WAL low-res textures support, remove it!
// NOTE: miptex_t is used only for .WAL format .. a bit outdated
qtexture_t *Texture_LoadTexture (miptex_t *qtex)
{
  byte		*source;
  unsigned	char *dest;
  int			width, height, i, count;
	int			total[3];
  qtexture_t	*q;
      
  width = LittleLong(qtex->width);
  height = LittleLong(qtex->height);

  q = (qtexture_t*)qmalloc(sizeof(*q));

  q->width = width;
  q->height = height;

	q->flags = qtex->flags;
	q->value = qtex->value;
	q->contents = qtex->contents;

	dest = (unsigned char*)qmalloc (width*height*4);

  count = width*height;
  source = (byte *)qtex + LittleLong(qtex->offsets[0]);

	// The dib is upside down so we want to copy it into 
	// the buffer bottom up.

	total[0] = total[1] = total[2] = 0;
  for (i=0 ; i<count ; i++)
	{
		dest[i] = tex_palette[source[i]];

		total[0] += ((byte *)(dest+i))[0];
		total[1] += ((byte *)(dest+i))[1];
		total[2] += ((byte *)(dest+i))[2];
	}

	q->color[0] = (float)total[0]/(count*255);
	q->color[1] = (float)total[1]/(count*255);
	q->color[2] = (float)total[2]/(count*255);

  q->texture_number = texture_extension_number++;

  if (g_qeglobals.bSurfacePropertiesPlugin)
  {
	  // Timo
	  // Surface properties plugins can store their own data in an IPluginQTexture
	  q->pData = g_SurfaceTable.m_pfnQTextureAlloc( q );
	  GETPLUGINTEXDEF(q)->InitForMiptex( qtex );
  }

  //++timo is the m_bSGIOpenGL parameter still taken into account?
  if (g_PrefsDlg.m_bSGIOpenGL)
  {
    //if (!qwglMakeCurrent(g_qeglobals.d_hdcBase, g_qeglobals.d_hglrcBase))
    if (!qwglMakeCurrent(s_hdcTexture, s_hglrcTexture))
		  Error ("wglMakeCurrent in LoadTexture failed");
  }

  qglBindTexture( GL_TEXTURE_2D, q->texture_number );

  //Handle3DfxTexturing(q, width, height, dest);

  SetTexParameters ();

  int nCount = MAX_TEXTURE_QUALITY - g_PrefsDlg.m_nTextureQuality;
  while (nCount-- > 0)
  {
    if (width > 16 && height > 16)
    {
      R_MipMap(dest, width, height);
    }
    else
    {
      break;
    }
  }

  if (g_PrefsDlg.m_bSGIOpenGL)
  {
	  if (nomips)
    {
		  qglTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dest);
    }
	  else
		  qgluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height,GL_RGBA, GL_UNSIGNED_BYTE, dest);
  }
  else
  {
	  if (nomips)
		  qglTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dest);
	  else
		  qgluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height,GL_RGBA, GL_UNSIGNED_BYTE, dest);
  }

	free (dest);

	qglBindTexture( GL_TEXTURE_2D, 0 );

  return q;
}




/*
=================
Texture_LoadTexture
=================
*/
qtexture_t *Texture_LoadTGATexture (unsigned char* pPixels, int nWidth, int nHeight, char* pPath, int nFlags, int nContents, int nValue )
{
  int i, j, inf;
	byte	gammatable[256];
	float fGamma = g_qeglobals.d_savedinfo.fGamma;


  qtexture_t* q = (qtexture_t*)qmalloc(sizeof(*q));
  q->width = nWidth;
  q->height = nHeight;
	q->flags = nFlags;
	q->value = nValue;
	q->contents = nContents;

  int nCount = nWidth * nHeight;
  float total[3];
  total[0] = total[1] = total[2] = 0.0f;

  //++timo FIXME: move gamma table initialization somewhere else!
	if (fGamma == 1.0)
	{
		for (i=0 ; i<256 ; i++)
			gammatable[i] = i;
	}
	else
	{
		for (i=0 ; i<256 ; i++)
		{
			inf = 255 * pow ((float) ( (((float)i)+0.5)/255.5 ), (float) fGamma ) + 0.5;
			if (inf < 0)
				inf = 0;
			if (inf > 255)
				inf = 255;
			gammatable[i] = inf;
		}
	}


  // all targas are stored internally as 32bit so rgba = 4 bytes
  for (i = 0 ; i < (nCount * 4) ; i += 4)
	{
    for (j = 0; j < 3; j++)
    {
	    total[j] += (pPixels+i)[j];
      byte b = (pPixels+i)[j];
      (pPixels+i)[j] = gammatable[b];
              
    }
	}

	q->color[0] = total[0] / (nCount * 255);
	q->color[1] = total[1] / (nCount * 255);
	q->color[2] = total[2] / (nCount * 255);


  q->texture_number = texture_extension_number++;

  if (g_qeglobals.bSurfacePropertiesPlugin)
  {
	  // Timo
	  // Surface properties plugins can store their own data in an IPluginQTexture
	  q->pData = g_SurfaceTable.m_pfnQTextureAlloc( q );
	  GETPLUGINTEXDEF(q)->SetDefaultTexdef();
  }

  if (g_PrefsDlg.m_bSGIOpenGL)
  {
    //if (!qwglMakeCurrent(g_qeglobals.d_hdcBase, g_qeglobals.d_hglrcBase))
    if (!qwglMakeCurrent(s_hdcTexture, s_hglrcTexture))
		  Error ("wglMakeCurrent in LoadTexture failed");
  }

  qglBindTexture( GL_TEXTURE_2D, q->texture_number );

  SetTexParameters();

  nCount = MAX_TEXTURE_QUALITY - g_PrefsDlg.m_nTextureQuality;
  while (nCount-- > 0)
  {
    if (nWidth > 16 && nHeight > 16)
    {
      R_MipMap(pPixels, nWidth, nHeight);
    }
    else
    {
      break;
    }
  }

  if (nomips)
		qglTexImage2D(GL_TEXTURE_2D, 0, 4, nWidth, nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pPixels);
  else
		qgluBuild2DMipmaps(GL_TEXTURE_2D, 4, nWidth, nHeight,GL_RGBA, GL_UNSIGNED_BYTE, pPixels);
  
  qglBindTexture( GL_TEXTURE_2D, 0 );

  return q;
}


qtexture_t *Texture_LoadTGATexture (unsigned char* pPixels, int nWidth, int nHeight, char *pPath)
{
  CString strName;
  CString strPath;
  ExtractPath_and_Filename(pPath, strPath, strName);
  AddSlash(strPath);
  strPath += "textureinfo.ini";
  strName.MakeLower();
  StripExtension (strName.GetBuffer(0));
  strName.ReleaseBuffer();
  
  int nFlags = GetPrivateProfileInt(strName, "Flags", 0, strPath);
  int nValue = GetPrivateProfileInt(strName, "Value", 0, strPath);
  int nContents = GetPrivateProfileInt(strName, "Contents", 0, strPath);
  return Texture_LoadTGATexture(pPixels, nWidth, nHeight, pPath, nFlags, nValue, nContents);
}


void Texture_LoadFromPlugIn(LPVOID vp)
{
  g_pluginTexture = notexture;
  _QERTextureLoad *pLoad = reinterpret_cast<_QERTextureLoad*>(vp);
  if (pLoad != NULL)
  {
	  qtexture_t	*q;
    q = Texture_LoadTGATexture(pLoad->m_pRGBA, pLoad->m_nWidth, pLoad->m_nHeight, NULL, pLoad->m_nFlags, pLoad->m_nContents, pLoad->m_nValue);
    if (q != NULL)
    {
      g_pluginTexture = q;
    }
  }
}


/*
===============
Texture_CreateSolid

Create a single pixel texture of the apropriate color
===============
*/
qtexture_t *Texture_CreateSolid (const char *name)
{
	byte	data[4];
	qtexture_t	*q;

  q = (qtexture_t*)qmalloc(sizeof(*q));

  if (g_qeglobals.bSurfacePropertiesPlugin)
  {
	  // Timo
	  // Surface properties plugins can store their own data in an IPluginQTexture
	  q->pData = g_SurfaceTable.m_pfnQTextureAlloc( q );
	  GETPLUGINTEXDEF(q)->SetDefaultTexdef();
  }
	
	sscanf (name, "(%f %f %f)", &q->color[0], &q->color[1], &q->color[2]);

	data[0] = q->color[0]*255;
	data[1] = q->color[1]*255;
	data[2] = q->color[2]*255;
	data[3] = 255;

	q->width = q->height = 1;
	//q->width = q->height = 2;
  q->texture_number = texture_extension_number++;
	qglBindTexture( GL_TEXTURE_2D, q->texture_number );
	SetTexParameters ();

  if (g_PrefsDlg.m_bSGIOpenGL)
  {
		qglTexImage2D(GL_TEXTURE_2D, 0, 3, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  }
  else
  {
	  if (nomips)
		  qglTexImage2D(GL_TEXTURE_2D, 0, 3, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	  else
		  qgluBuild2DMipmaps(GL_TEXTURE_2D, 3, 1, 1,GL_RGBA, GL_UNSIGNED_BYTE, data);
  }
	qglBindTexture( GL_TEXTURE_2D, 0 );

	return q;
}


/*
=================
Texture_MakeDefault
=================
*/
qtexture_t* Texture_MakeDefault (void)
{
  qtexture_t	*q;
  byte		data[4][4];

  if (g_PrefsDlg.m_bSGIOpenGL)
  {
    if (s_hdcTexture && s_hglrcTexture)
    { 
       if (!qwglMakeCurrent(s_hdcTexture, s_hglrcTexture))
		     Error ("wglMakeCurrent in Texture_MakeDefault failed");
    }
    else
      return NULL;
  }

  q = (qtexture_t*)qmalloc(sizeof(*q));
  
  strcpy (q->name, "radiant/notex");
  q->width = q->height = 64;
  
  memset (data, 0, sizeof(data));
  data[0][2] = data[3][2] = 255;
  
  q->color[0] = 0;
  q->color[1] = 0;
  q->color[2] = 0.5;

  q->texture_number = texture_extension_number++;
  qglBindTexture( GL_TEXTURE_2D, q->texture_number );
  SetTexParameters ();

	if (nomips)
		qglTexImage2D(GL_TEXTURE_2D, 0, 3, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else
		VERIFY(qgluBuild2DMipmaps(GL_TEXTURE_2D, 3, 2, 2,GL_RGBA, GL_UNSIGNED_BYTE, data) == 0);

  qglBindTexture( GL_TEXTURE_2D, 0 );
  return q;
}


/*
=================
Texture_MakeNotexture
=================
*/
void Texture_MakeNotexture (void)
{
  //++timo FIXME: this is ugly, we create a qtexture_t from scratch without going through the IShader interface
  // then in Texture_Init we store something else as current texture ...
  notexture = Texture_MakeDefault();
  // Timo
  // Surface properties plugins can store their own data in an IPluginQTexture
  if (g_qeglobals.bSurfacePropertiesPlugin)
  {
	  notexture->pData = g_SurfaceTable.m_pfnQTextureAlloc( notexture );
	  GETPLUGINTEXDEF(notexture)->SetDefaultTexdef();
  }
}

#if 0
void Texture_Remove(qtexture_t *q)
{
  qtexture_t* pTex = g_qeglobals.d_qtextures->next;
  if (q == g_qeglobals.d_qtextures)   // it is the head
  {
    g_qeglobals.d_qtextures->next = q->next->next;
    g_qeglobals.d_qtextures = q->next;
  }
  else
  {
    qtexture_t* pLast = g_qeglobals.d_qtextures;
    while (pTex != NULL && pTex != g_qeglobals.d_qtextures)
    {
      if (pTex == q)
      {
        pLast->next = q->next;
        break;
      }
      pLast = pTex;
      pTex = pTex->next;
    }
  }
  qglDeleteTextures(1, reinterpret_cast<const unsigned int*>(&q->texture_number));

  if (g_qeglobals.bSurfacePropertiesPlugin)
  {
	  // Timo
	  // Surface properties plugin
#ifdef _DEBUG
	  if ( !q->pData )
		  Sys_Printf("WARNING: found a qtexture_t* with no IPluginQTexture\n");
#endif
	  if ( q->pData )
		  GETPLUGINTEXDEF(q)->DecRef();
  }

  free(q);

}
#endif

/*
=================
Texture_MakeNoShadertexture

Make a default black/red check pattern texture
=================
*/
qtexture_t * Texture_MakeNoshadertexture( const char *name )
{
	qtexture_t	*q;
	byte		data[4][4];

	notexture = q = (qtexture_t*)qmalloc(sizeof(*q));
	q->width = q->height = 64;
	q->fTrans = 1;

	q = (qtexture_t*)qmalloc(sizeof(*q));
  strcpy (q->name, name);

	q->width = q->height = 64;
	q->fTrans = 1;

	memset (data, 0, sizeof(data));
	data[0][0] = data[3][0] = 255;

	q->color[0] = 0;
	q->color[1] = 0;
	q->color[2] = 0.5;

	q->texture_number = texture_extension_number++;
	qglBindTexture( GL_TEXTURE_2D, q->texture_number );
	SetTexParameters ();

	if (nomips)
		qglTexImage2D(GL_TEXTURE_2D, 0, 3, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else
		VERIFY(qgluBuild2DMipmaps(GL_TEXTURE_2D, 3, 2, 2,GL_RGBA, GL_UNSIGNED_BYTE, data) == 0);

	qglBindTexture( GL_TEXTURE_2D, 0 );

	return q;
}

/*
==================
DumpUnreferencedShaders
usefull function: dumps the list of .shader files that are not referenced to the console
==================
*/
void DumpUnreferencedShaders(const class CStringList & shaders )
{
  char    dirstring[_MAX_PATH];
	char	  *path;
	struct  _finddata_t fileinfo;
	int		  handle;
  bool    bFound = false;
  path = ValueForKey (g_qeglobals.d_project_entity, "basepath");
  sprintf(dirstring, "%s/scripts/*.shader", path);
  handle = _findfirst (dirstring, &fileinfo);
  if (handle != -1)
  {
    if (!shaders.Find( fileinfo.name ))
    {
      if (!bFound)
      {
        bFound = true;
        Sys_Printf("Following shader files are not referenced in shaderlist.txt:\n");
      }
      Sys_Printf( "%s\n", fileinfo.name );
    }
    _findclose (handle);
  }
}

/*
==================
BuildShaderList
build a CStringList of shader names
==================
*/
void BuildShaderList(class CStringList & lst)
{
	char	dirstring[1024];
	char	*path;
  path = ValueForKey (g_qeglobals.d_project_entity, "basepath");
  sprintf (dirstring, "%s/scripts/shaderlist.txt", path);
  char *pBuff = NULL;
  
  int nLen = LoadFile(dirstring, reinterpret_cast<void**>(&pBuff));
  if (nLen == -1)
  {
    nLen = PakLoadAnyFile(dirstring, reinterpret_cast<void**>(&pBuff));
  }
  if (nLen > 0)
  {
    StartTokenParsing(pBuff);
    nLen = 0;
    while (GetToken(true))
    {
      // each token should be a shader filename
      sprintf(dirstring, "%s/scripts/%s.shader", path, token);
      lst.AddTail(dirstring);
      nLen++;
    }
    free(pBuff);
  }
  else
  {
    Sys_Printf("Unable to load %s, shaders not loaded!\n", dirstring);
  }
}

void MenuLetter2String(int l,char* s)
{
	if (l==0) // special case for 'other' group
	{
		strcpy(s,"0");
		return;
	}

	l=l+64;
	s[0] = (char) l;
	s[1] = 0;
}

HMENU MenuLetter2Menu(char l,HMENU* list)
{
	if ((l >='a' && l<= 'z') || (l >='A' && l<= 'Z'))
	{
		l = l & ~32;
		l=l-64;
		return (list[l]);
	}
	else return list[0];
}

/*
==================
FillTextureMenu

==================
*/
// JN: Added alphabetic list as base then normal list within (for last folder lists)
void FillTextureMenu (CStringArray* pArray)
{
	HMENU	hmenu;
	int		i, j, icount;
	struct _finddata_t fileinfo;
	int		handle;
	char	dirstring[1024];
	char	*path;
	DIRLIST	*list = NULL, *temp;

  HMENU hSubMenu;
  char  dirRoot[1024];
	MENUITEMINFO info;

  if (g_pParentWnd->GetPlugInMgr().GetTextureInfo() != NULL)
  {
    if (g_pParentWnd->GetPlugInMgr().GetTextureInfo()->m_bWadStyle)
      return;
  }

	hmenu = GetSubMenu (GetMenu(g_qeglobals.d_hwndMain), MENU_TEXTURE);

	// delete everything
	for (i=0 ; i<texture_nummenus ; i++)
		DeleteMenu (hmenu, CMD_TEXTUREWAD+i, MF_BYCOMMAND);
	// delete the submenu things as well
	// we'll need to scan and search for them in the menu
	info.cbSize = sizeof(MENUITEMINFO);
	info.fMask = MIIM_SUBMENU;
	icount = lstSubMenuHandles.GetSize();
	for (i=0; i<icount; i++)
	{
		for(j=0; j<GetMenuItemCount( hmenu ); j++ )
		{
			GetMenuItemInfo( hmenu, j, TRUE, &info );
			if (info.hSubMenu == static_cast<HMENU>(lstSubMenuHandles.GetAt(i)))
				DeleteMenu( hmenu, j, MF_BYPOSITION );
		}
	}
	lstSubMenuHandles.RemoveAll();

  texture_nummenus = 0;

	// add everything
  if (g_qeglobals.d_project_entity)
  {
    // scan texture dirs and pak files only if not restricting to shaderlist
    if (!g_PrefsDlg.m_bTexturesShaderlistOnly)
    {
      path = ValueForKey (g_qeglobals.d_project_entity, "texturepath");
      sprintf (dirstring, "%s/*.*", path);
      
      handle = _findfirst (dirstring, &fileinfo);
      if (handle != -1)
      {
        do
        {
          if (!(fileinfo.attrib & _A_SUBDIR))
            continue;
          if (fileinfo.name[0] == '.')
            continue;
          AddToDirListAlphabetized(&list, fileinfo.name, FROMDISK);
        } while (_findnext( handle, &fileinfo ) != -1);
        _findclose (handle);
      }
      
      GetPackTextureDirs(&list);
    }

    // scan the shaders in shaderlist.txt
    CStringList shaders;
    BuildShaderList( shaders );
    DumpUnreferencedShaders( shaders );
    POSITION pos = shaders.GetHeadPosition();
    while (pos != NULL)
    {
      char shaderfile[_MAX_PATH];
      ExtractFileName( shaders.GetAt(pos).GetBuffer(0), shaderfile );
      StripExtension( shaderfile );
      AddToDirListAlphabetized(&list, shaderfile, FROMDISK );
      shaders.GetNext(pos);
    }

	// JN ADD ALPHABET LIST HERE
	bool alphabet[27];
	for (int j=0;j<27;j++)                                //123456789012345678901234567
		alphabet[j] = false; // Hide each letter by default 0ABCDEFGHIJKLMNOPQRSTUVWXYZ

	temp = list;
	int foldercount=0;
	while(temp)
	{
		foldercount++;
		// Check first letter
		char letter = temp->dirname[0];
		letter = tolower(letter);
		switch(letter)
		{
		case 'a':
			alphabet[1] = true;
			break;
		case 'b':
			alphabet[2] = true;
			break;
		case 'c':
			alphabet[3] = true;
			break;
		case 'd':
			alphabet[4] = true;
			break;
		case 'e':
			alphabet[5] = true;
			break;
		case 'f':
			alphabet[6] = true;
			break;
		case 'g':
			alphabet[7] = true;
			break;
		case 'h':
			alphabet[8] = true;
			break;
		case 'i':
			alphabet[9] = true;
			break;
		case 'j':
			alphabet[10] = true;
			break;
		case 'k':
			alphabet[11] = true;
			break;
		case 'l':
			alphabet[12] = true;
			break;
		case 'm':
			alphabet[13] = true;
			break;
		case 'n':
			alphabet[14] = true;
			break;
		case 'o':
			alphabet[15] = true;
			break;
		case 'p':
			alphabet[16] = true;
			break;
		case 'q':
			alphabet[17] = true;
			break;
		case 'r':
			alphabet[18] = true;
			break;
		case 's':
			alphabet[19] = true;
			break;
		case 't':
			alphabet[20] = true;
			break;
		case 'u':
			alphabet[21] = true;
			break;
		case 'v':
			alphabet[22] = true;
			break;
		case 'w':
			alphabet[23] = true;
			break;
		case 'x':
			alphabet[24] = true;
			break;
		case 'y':
			alphabet[25] = true;
			break;
		case 'z':
			alphabet[26] = true;
			break;
				
		default:
			alphabet[0] = true;
		}
		temp=temp->next;
	}

	char tempstr[10];
	HMENU alphamenus[27];

	if (foldercount > FOLDER_CONTRACT_SIZE)
	{
		for (int j=0;j<27;j++)
		{
			if(alphabet[j] == true)
			{
				//AppendMenu(hmenu, MF_POPUP, (UINT_PTR)hSubMenu, dirRoot);
				hSubMenu = CreateMenu();
				lstSubMenuHandles.Add(hSubMenu);
				MenuLetter2String(j,tempstr);
				AppendMenu(hmenu, MF_POPUP, (UINT_PTR)hSubMenu, tempstr);
				alphamenus[j] = hSubMenu;
			}
		}
	}

	

    temp = list;
    while (temp)
	  {
      // do we shrink the menus?
      if (strchr(temp->dirname, '_'))
      {
        // extract the root
        strcpy(dirRoot, temp->dirname);
        dirRoot[strchr(temp->dirname, '_')-temp->dirname] = 0;
        // we shrink only if we have at least two things to shrink :-)
        if (temp->next && (strstr(temp->next->dirname, dirRoot)==temp->next->dirname))
        {
          hSubMenu = CreateMenu();
		  lstSubMenuHandles.Add(hSubMenu);
          // keep going...
          do
          {
            AppendMenu( hSubMenu, MF_ENABLED, CMD_TEXTUREWAD+texture_nummenus, (LPCTSTR)temp->dirname);
            //++timo FIXME: merge with below
      		  strcpy (texture_menunames[texture_nummenus], temp->dirname);
            strcat (texture_menunames[texture_nummenus], "/");
            if (pArray)
              pArray->Add(temp->dirname);
            if (++texture_nummenus == MAX_TEXTUREDIRS)
            {
              char strMsg[1024];
              int lost = 0;
              while (temp)
              {
                lost++;
                temp = temp->next;
              }
              sprintf(strMsg, "maximum textures directories count reached, dropping %d dirs", lost );
              MessageBox( NULL, strMsg, "Maximum texture count reached", MB_OK | MB_ICONERROR );
              strcat( strMsg, "\n" );
              Sys_Printf( strMsg );
              // push submenu and get out
			  AppendMenu( (foldercount > FOLDER_CONTRACT_SIZE) ? MenuLetter2Menu(dirRoot[0],alphamenus) : hmenu, MF_POPUP, (UINT_PTR)hSubMenu, dirRoot);
              ClearDirList(&list);
              return;
            }
            temp = temp->next;
          } while (temp && (strstr(temp->dirname, dirRoot)==temp->dirname));
		  AppendMenu((foldercount > FOLDER_CONTRACT_SIZE) ? MenuLetter2Menu(dirRoot[0],alphamenus) : hmenu, MF_POPUP, (UINT_PTR)hSubMenu, dirRoot);
		  //MenuLetter2Menu(dirRoot[0],alphamenus);
          continue;
        }
      }

	  AppendMenu ((foldercount > FOLDER_CONTRACT_SIZE) ? MenuLetter2Menu(temp->dirname[0],alphamenus) : hmenu, MF_ENABLED|MF_STRING, CMD_TEXTUREWAD+texture_nummenus, (LPCTSTR)temp->dirname);
		  strcpy (texture_menunames[texture_nummenus], temp->dirname);
		  strcat (texture_menunames[texture_nummenus], "/");
      if (pArray)
        pArray->Add(temp->dirname);
		  if (++texture_nummenus == MAX_TEXTUREDIRS)
      {
        char strMsg[1024];
        int lost = 0;
        while (temp)
        {
          lost++;
          temp = temp->next;
        }
        sprintf(strMsg, "maximum textures directories count reached, dropping %d dirs", lost );
        MessageBox( NULL, strMsg, "Maximum texture count reached", MB_OK | MB_ICONERROR );
        strcat( strMsg, "\n" );
        Sys_Printf( strMsg );
        ClearDirList(&list);
        return;
      }

      temp = temp->next;
	  }
	  ClearDirList(&list);
  }
}

/*
==============
Texture_ShowDirectory
relies on texture_directory global for the directory to use
called by
  void Texture_ShowDirectory (int menunum, bool bLinked)
	void	Texture_ShowDirectory (char* pPath, bool bLinked)
1) Load the shaders for the given directory
2) Scan the remaining texture, load them and assign them a default shader (the "noshader" shader)
NOTE: when writing a texture plugin, or some texture extensions, this function may need to be overriden, and made
  available through the IShaders interface
NOTE: for texture window layout:
  all shaders are stored with alphabetical order after load
  previously loaded and displayed stuff is hidden, only in-use and newly loaded is shown
  ( the GL textures are not flushed though)
==============
*/
void Texture_ShowDirectory ()
{
	struct _finddata_t fileinfo;
	int		handle;
	char	name[1024];
	char	dirstring[1024];
  FILELIST			*list = NULL, *temp;
  CString strTemp;
  int shaders_count = 0;
  int textures_count = 0;

	// refresh the in-use textures: that will clear the IsDisplayed flag on unused stuff
	// and leave it on in-use so they'll still be displayed
	Texture_ShowInuse();
	// and textures loaded in the following lines will be displayed as well...
	// NOTE: shaders that are not in use but have been loaded previously are still in memory. But they don't get displayed.
	
	g_qeglobals.d_texturewin.originy = 0;
  // load texture_directory.shader
  // NOTE: because of above call to Texture_ClearInuse, g_ActiveShaders will have the newly loaded shaders only
  // we'll use that later to check if textures have a shader associated or not
  // NOTE: all shaders loaded through QERApp_LoadShadersFromDir will get their InUse flag to True, we'll need a call to Texture_ShowInUse for later cleanup/adjustment
  // NOTE: QERApp_LoadShadersFromDir has two criterions for loading a shader:
  //   the shaderfile is texture_directory (like "museum" will load everything in museum.shader)
  //   the shader name contains texture_directory (like "base_floor" will load museum.shader::base_floor/concfloor_rain)
	shaders_count = QERApp_LoadShadersFromDir(texture_directory);
  // load remaining texture files
  // if a texture is already in use to represent a shader, ignore it
  for (int nExt = 0; nExt < GetTextureExtensionCount(); nExt++)
  {
    sprintf (dirstring, "%s/textures/%s*.%s", ValueForKey (g_qeglobals.d_project_entity, "basepath"), texture_directory, GetTextureExtension(nExt));
    Sys_Printf ("Scanning %s\n", dirstring);
    handle = _findfirst (dirstring, &fileinfo);
    if (handle == -1)
    {
      sprintf(dirstring, "%s/%s*.%s", ValueForKey (g_qeglobals.d_project_entity, "texturepath"), texture_directory, GetTextureExtension(nExt));
      handle = _findfirst (dirstring, &fileinfo);
    }
    if (handle != -1)
    {
		    do
        {
          sprintf (name, "%s%s", texture_directory, fileinfo.name);
          AddToFileListAlphabetized(&list, name, FROMDISK, 0, false);
        } while (_findnext( handle, &fileinfo ) != -1);
        _findclose (handle);
    }
    else
    {
      sprintf (dirstring, "%s*.%s", texture_directory, GetTextureExtension(nExt));
  		GetPackFileList(&list, dirstring);
    }
  }

  for(temp = list; temp; temp = temp->next)
  {
		  if(temp->offset == -1)
        sprintf(name, "%s", temp->filename);
      else
        sprintf(name, "%s%s", texture_directory, temp->filename);
      StripExtension (name);
      strTemp = name;
      strTemp.MakeLower();
      if ( strTemp.Find(".specular") >= 0 ||
        strTemp.Find(".glow") >= 0 ||
        strTemp.Find(".bump") >= 0 ||
        strTemp.Find(".diffuse") >= 0 ||
        strTemp.Find(".blend") >= 0 ||
        strTemp.Find(".alpha") >= 0
        )
        continue;
      else
      {
        // build a texture name that fits the conventions for qtexture_t::name
        char stdName[1024];
        sprintf( stdName, "textures/%s", name );
        // check if this texture doesn't have a shader
        if (!g_ActiveShaders.Shader_ForTextureName( stdName ))
        {
          //++timo TODO: we need to move that into a function that creates a default shader
          // the same code is already used in QERApp_ShaderForName???
          // CAN WE MERGE OR NOT?
          // the texture isn't represented by a shader, create a default shader for it (flagged "not a real shader")
					// CreateDefault expects a texture / shader name reletive to the "textures" directory (cause shader names are reletive to "textures/")
          CShader* pShader = new CShader;
          pShader->CreateDefault(name);
          // hook it into the shader list
					pShader->IncRef();
					g_Shaders.Add( (LPVOID)pShader );
          pShader->Activate();
          pShader->SetDisplayed(true);
          textures_count++;
        }
      }
  }

  Sys_Printf("Loaded %d shaders and created default shader for %d orphan textures.\n", shaders_count, textures_count );

	ClearFileList(&list);

	// we'll display the newly loaded textures + all the ones already in use
	// this will rescan the in use textures. setting texture_showinuse to false makes sure we draw the green outline
	// (cause there's no green outline if texture_showinuse=true, since we are only showing in use stuff)
	//++timo FIXME: texture_showinuse is old crap and should be removed
//  Texture_ShowInuse();
//	g_bShowAllShaders = false;
	
	// sort for displaying
	QERApp_SortActiveShaders();
  
  sprintf (name, "Textures: %s", texture_directory);
  SetWindowText(g_qeglobals.d_hwndEntity, name);
  
  // select the first texture in the list
  if (!g_qeglobals.d_texturewin.texdef.name[0])
		  SelectTexture (16, g_qeglobals.d_texturewin.height -16, false);

	Sys_UpdateWindows (W_TEXTURE);  
}

/*
==============
Texture_ShowDirectory
1) Load the shaders for the given directory
2) Scan the remaining texture, load them and assign them a default shader (the "noshader" shader)
NOTE: when writing a texture plugin, or some texture extensions, this function may need to be overriden, and made
  available through the IShaders interface
==============
*/
//++timo FIXME: bLinked is dead and not used?
void Texture_ShowDirectory (int menunum, bool bLinked)
{
	strcpy (texture_directory, texture_menunames[menunum-CMD_TEXTUREWAD]);
	Texture_ShowDirectory();
}

// called straight when user selects a directory to import
// NOTE: it's both a real directory and the shaders directory that get loaded
//++timo FIXME: bLinked is dead and not used?
void	Texture_ShowDirectory (char* pPath, bool bLinked)
{
	// it's a full path, we want reletive path to texture directory
	// pPath is supposed to start with "texturepath" ??
	// yeah I'm freakin paranoid
#ifdef _DEBUG
  if ( strstr( pPath, ValueForKey (g_qeglobals.d_project_entity, "texturepath") ) != 0)
		Sys_Printf("ERROR: unexpected texturepath not found in Texture_ShowDirectory\n");
#endif
	strcpy (texture_directory, pPath + strlen(ValueForKey (g_qeglobals.d_project_entity, "texturepath")) + 1 );
#ifdef _DEBUG
	if (texture_directory[strlen(texture_directory)-1] != '\\')
		Sys_Printf("ERROR: unexpected no \\ at the end of texture_directory in Texture_ShowDirectory\n");
#endif
	texture_directory[strlen(texture_directory)-1] = '/';
	Texture_ShowDirectory();
}

void Texture_ResetPosition()
{
  SelectTexture (16, g_qeglobals.d_texturewin.height -16 ,false);
  g_qeglobals.d_texturewin.originy = 0;
}

/*
==============
Texture_ShowAll
will set the IsDisplayed flag on all the active shaders, so we see everything that's currently in memory
==============
*/
void	Texture_ShowAll()
{
	char name[1024];

#ifdef _DEBUG
	if (g_bShowAllShaders)
		Sys_Printf("WARNING: already showing all shaders\n");
#endif
	QERApp_ActiveShaders_SetDisplayed(true);
	g_bShowAllShaders = true;
	// put some information in the texture window title?
	sprintf (name, "Textures: in use");
	SetWindowText(g_qeglobals.d_hwndEntity, name);
/*	if (texture_showinuse)
	{
		texture_showinuse = false;
		Sys_Printf("Showing all textures...\n");
	}
	else
		Sys_Printf("Already showing all textures\n"); */
	Sys_UpdateWindows (W_TEXTURE);
}

/*
==============
Texture_ShowInuse
clear all IsDisplayed flags
scan the map, set IsInUse (will set IsDisplayed on the way)
NOTE: don't sort the textures, don't update the windows (it's used in several contexts, not always necessary to do either)
==============
*/
void	Texture_ShowInuse (void)
{
	face_t	*f;
	brush_t	*b;
	char	name[1024];

//	texture_showinuse = true;
//	g_dontuse = false;

	g_qeglobals.d_texturewin.originy = 0;	

//	Texture_ClearInuse();
	
	// purge
	QERApp_ActiveShaders_SetDisplayed(false);
	// scan and only display in-use stuff
	Sys_Status("Selecting active textures\n", 0);
	for (b=active_brushes.next ; b != NULL && b != &active_brushes ; b=b->next)
  {
    if (b->patchBrush)
    {
      b->pPatch->pShader->SetInUse(true);
    }
    else
    {
		  for (f=b->brush_faces ; f ; f=f->next)
      {
        f->pShader->SetInUse(true);
      }
    }
  }
	for (b=selected_brushes.next ; b != NULL && b != &selected_brushes ; b=b->next)
  {
    if (b->patchBrush)
    {
      b->pPatch->pShader->SetInUse(true);
    }
    else
    {
		  for (f=b->brush_faces ; f ; f=f->next)
      {
        f->pShader->SetInUse(true);
      }
    }
  }
	// we are no longer showing everything
	g_bShowAllShaders = false;
	// put some information in the texture window title?
	sprintf (name, "Textures: in use");
	SetWindowText(g_qeglobals.d_hwndEntity, name);


	// select the first texture in the list
	if (!g_qeglobals.d_texturewin.texdef.name[0])
  {
		SelectTexture (16, g_qeglobals.d_texturewin.height -16, false);
  }
}

/*
============================================================================

TEXTURE LAYOUT

TTimo: now based on a rundown through all the shaders
nActiveShadersCount: number of shader that have a qtexture_t and may be displayed in the tex window
nCurrentShader: index of active shader that has the current_texture
pCurrentShader: IShader* for current shader
NOTE: we expect the Active shaders count doesn't change during a Texture_StartPos .. Texture_NextPos cycle
  otherwise we may need to rely on a list instead of an array storage
============================================================================
*/

void Texture_StartPos (void)
{
	//++timo TODO: check use of current_texture and current_row?
	current_x = 8;
	current_y = -8;
	current_row = 0;
	nActiveShadersCount = QERApp_GetActiveShaderCount();
	nCurrentShader = -1;
	current_texture = NULL;
	pCurrentShader = NULL;
}


// if texture_showinuse jump over non in-use textures
// it's not very clear what should be done here and what in Texture_Draw .. maybe merging the two would do good
IShader* Texture_NextPos (int *x, int *y)
{
	qtexture_t* q;
	while (1)
	{
		if (nCurrentShader >= nActiveShadersCount - 1)
		{
			// no more shaders
			current_texture = NULL;
			pCurrentShader = NULL;
			return NULL;
		}
		nCurrentShader++;
		pCurrentShader = QERApp_ActiveShader_ForIndex(nCurrentShader);

#ifdef _DEBUG
		if (pCurrentShader == NULL)
		{
			Sys_Printf("ERROR: unexpected pCurrentShader == NULL in Texture_NextPos\n");
			return NULL;
		}
#endif

		current_texture = pCurrentShader->getTexture();
		q = current_texture;
		
#ifdef _DEBUG
		if (!q)
		{
			Sys_Printf("WARNING: found an IShader without qtexture_t in Texture_NextPos\n");
			return NULL;
		}
#endif

		// don't show shaders?
		if (!(g_PrefsDlg.m_bShowShaders || pCurrentShader->IsDefault()))
			continue;

		if (g_PrefsDlg.m_bTextureWindow)
		{
			// some basic filtering
			if (!g_pParentWnd->GetTexWnd()->CheckFilter( pCurrentShader->getName() ))
				continue;
		}

		//++timo FIXME: texture_showinuse is useless? with the menu and reload we just refresh the IsDisplayed flag
		// but the IsInUse is only relevant to draw the green outline
		if (pCurrentShader->IsDisplayed())
			break;

		continue;
	}

	// NOTE: this supposes we remove the g_PrefsDlg.m_bHiColorTextures
	int nWidth = q->width * ((float)g_PrefsDlg.m_nTextureScale / 100);
	int nHeight = q->height * ((float)g_PrefsDlg.m_nTextureScale / 100);
	if (current_x + nWidth > g_qeglobals.d_texturewin.width-8 && current_row)
	{	// go to the next row unless the texture is the first on the row
		current_x = 8;
		current_y -= current_row + FONT_HEIGHT + 4;
		current_row = 0;
	}
	
	*x = current_x;
	*y = current_y;
	
	// Is our texture larger than the row? If so, grow the 
	// row height to match it
	
  if (current_row < nHeight)
		current_row = nHeight;
	
	// never go less than 64, or the names get all crunched up
	current_x += nWidth < 64 ? 64 : nWidth;
	current_x += 8;

	return pCurrentShader;
}

/*
============================================================================

  MOUSE ACTIONS

============================================================================
*/

static	int	textures_cursorx, textures_cursory;

/*
============
Texture_SetTexture

brushprimit_texdef must be understood as a qtexture_t with width=2 height=2 ( the default one )
============
*/
void Texture_SetTexture (texdef_t *texdef, brushprimit_texdef_t *brushprimit_texdef, bool bFitScale, IPluginTexdef *pTexdef, bool bSetSelection )
{
	qtexture_t	*q;
	int			x,y;

	if (!(texdef->name))
	{
		Sys_Status("Can't select a model's texture\n", 0);
		return;
	}
	if (texdef->name[0] == '(')
	{
		Sys_Status("Can't select an entity's texture\n", 0);
		return;
	}
	g_qeglobals.d_texturewin.texdef = *texdef;
	g_qeglobals.d_texturewin.texdef.flags &= ~SURF_KEEP;
	g_qeglobals.d_texturewin.texdef.contents &= ~CONTENTS_KEEP;
	// store the shader pointer
	// NOTE: maybe passing the shader pointer would help?
	g_qeglobals.d_texturewin.pShader->DecRef();
	g_qeglobals.d_texturewin.pShader = QERApp_Shader_ForName(texdef->name);
	g_qeglobals.d_texturewin.pShader->IncRef();
	// set this shader as in use
	g_qeglobals.d_texturewin.pShader->SetInUse( true );
	// store the texture coordinates for new brush primitive mode
	// be sure that all the callers are using the default 2x2 texture
	if (g_qeglobals.m_bBrushPrimitMode)
	{
		g_qeglobals.d_texturewin.brushprimit_texdef = *brushprimit_texdef;
	}
	// surface properties plugin
	if (g_qeglobals.bSurfacePropertiesPlugin)
	{
		if (g_qeglobals.d_texturewin.pTexdef)
		{
			// decrement reference count
			static_cast<IPluginTexdef *>(g_qeglobals.d_texturewin.pTexdef)->DecRef();
			g_qeglobals.d_texturewin.pTexdef = NULL;
		}
		if (pTexdef)
		{
			g_qeglobals.d_texturewin.pTexdef = pTexdef->Copy();
		}
	}

  g_dlgFind.updateTextures(texdef->name);
  if (!g_dlgFind.isOpen() && bSetSelection)
  {
    Select_SetTexture(texdef,brushprimit_texdef,bFitScale);
  }

	//plugins: send a message telling that the selected texture may have changed
	DispatchRadiantMsg( RADIANT_TEXTURE );

	// scroll origin so the texture is completely on screen
	Texture_StartPos ();
	while (1)
	{
		// NOTE: return value is == pCurrentShader and pCurrentShader->getTexture == current_texture
		Texture_NextPos (&x, &y);
		q = current_texture;
		if (!q)
			break;

    int nWidth = (g_PrefsDlg.m_bHiColorTextures == TRUE) ? q->width * ((float)g_PrefsDlg.m_nTextureScale / 100) : q->width;
    int nHeight = (g_PrefsDlg.m_bHiColorTextures == TRUE) ? q->height * ((float)g_PrefsDlg.m_nTextureScale / 100) : q->height;
		// we have found when texdef->name and the shader name match
		// NOTE: as everywhere else for our comparisons, we are not case sensitive
		if (!strcmpi( texdef->name, pCurrentShader->getName() ))
		{
			if (y > g_qeglobals.d_texturewin.originy)
			{
				g_qeglobals.d_texturewin.originy = y;
				Sys_UpdateWindows (W_TEXTURE);
				return;
			}

			if (y-nHeight-2*FONT_HEIGHT < g_qeglobals.d_texturewin.originy-g_qeglobals.d_texturewin.height)
			{
				g_qeglobals.d_texturewin.originy = y-nHeight-2*FONT_HEIGHT+g_qeglobals.d_texturewin.height;
				Sys_UpdateWindows (W_TEXTURE);
				return;
			}

			Sys_UpdateWindows (W_TEXTURE);
			return;
		}
	}
}


HWND FindEditWindow()
{
  HWND hwnd = FindWindow("TEditPadForm", NULL);
  HWND hwndEdit = NULL;
  if (hwnd != NULL)
  {
    HWND hwndTab = FindWindowEx(hwnd, NULL, "TTabControl", NULL);
    if (hwndTab != NULL)
    {
      hwndEdit = FindWindowEx(hwndTab, NULL, "TRicherEdit", NULL);
    }
  }
  return hwndEdit;
}

void Delay(float fSeconds)
{
  DWORD dw = ::GetTickCount();
  DWORD dwTil = dw + (fSeconds * 1000);
  while (::GetTickCount() < dwTil)
  {
    MSG msg;
    if (::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE )) 
    { 
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
}


void ViewShader(const char *pFile, const char *pName)
{
	// we load the .shader file to find where it actually is
  CString fullName = ValueForKey( g_qeglobals.d_project_entity, "basepath" );
  fullName += '/';
  fullName += pFile;
  char* pBuff = NULL;
  int nSize = LoadFile(fullName.GetBuffer(0), reinterpret_cast<void**>(&pBuff));
  if (nSize == -1)
  {
    Sys_Printf("Failed to load shader file %s ... check your project settings!\n", pFile );
    return;
  }
  // look for the shader declaration
  int nStart;
  CString strFind = pName;
  CString strLook = pBuff;
  strLook.MakeLower();
  strFind.MakeLower();
  // offset used when jumping over commented out definitions
  int nOffset = 0;
  while (true)
  {
    nStart = strLook.Find(strFind, nOffset);
    if (nStart == -1)
      break;
    // we have found something, maybe it's a commented out shader name?
    char *strCheck = new char[strLook.GetLength()+1];
    strcpy( strCheck, strLook.GetBuffer(0) );
    strCheck[nStart] = 0;
    char *pCheck = strrchr( strCheck, '\n' );
    // if there's a commentary sign in-between we'll continue
    if (pCheck && strstr( pCheck, "//" ))
    {
      delete[] strCheck;
      nOffset = nStart + 1;
      continue;
    }
    delete[] strCheck;
    break;
  }
  // now close the file
  free(pBuff);

  CString s= "editpad ";
	// build the full shader name
	s += ValueForKey(g_qeglobals.d_project_entity, "basepath");
	s += '\\';
  s += pFile;
  WinExec(s, SW_SHOWNORMAL);

  // TTimo: we used to call Delay here, to continue processing messages. But it seems to induce a lot of instabilities.
  // so now the user will simply have to wait.
  Sleep( 1500 );

	// now grab the edit window and scroll to the shader we want to edit
  HWND hwndEdit = FindEditWindow();

  if (hwndEdit != NULL)
  {
    PostMessage(hwndEdit, EM_SETSEL, nStart, nStart);
  }
  else
  {
    Sys_Printf("Unable to load shader editor.\n");
  }
}

/*
==============
SelectTexture

  By mouse click
==============
*/
void SelectTexture (int mx, int my, bool bShift, bool bFitScale)
{
	int		x, y;
	qtexture_t	*q;
	texdef_t	tex;
	brushprimit_texdef_t brushprimit_tex;

	my += g_qeglobals.d_texturewin.originy-g_qeglobals.d_texturewin.height;
	
	Texture_StartPos ();
	while (1)
	{
		// NOTE: return value is == pCurrentShader and pCurrentShader->getTexture == current_texture
		Texture_NextPos (&x, &y);
		q = current_texture;
		if (!q)
			break;
		int nWidth = (g_PrefsDlg.m_bHiColorTextures == TRUE) ? q->width * ((float)g_PrefsDlg.m_nTextureScale / 100) : q->width;
		int nHeight = (g_PrefsDlg.m_bHiColorTextures == TRUE) ? q->height * ((float)g_PrefsDlg.m_nTextureScale / 100) : q->height;
		if (mx > x && mx - x < nWidth
			&& my < y && y - my < nHeight + FONT_HEIGHT)
		{
#if 0
			if (bShift)
				if (g_PrefsDlg.m_bHiColorTextures && q->shadername[0] != 0)
					ViewShader(pCurrentShader->getShaderFileName(), pCurrentShader->getName());
//					ViewShader(q->shadername, q->name);				
#endif
			if (bShift)
				if (pCurrentShader->IsDefault())
					Sys_Printf("ERROR: %s is not a shader, it's a texture.\n", pCurrentShader->getName() );
				else
					ViewShader( pCurrentShader->getShaderFileName(), pCurrentShader->getName() );
			memset (&tex, 0, sizeof(tex));
			memset (&brushprimit_tex, 0, sizeof(brushprimit_tex));
			if (g_qeglobals.m_bBrushPrimitMode)
			{
				// brushprimit fitted to a 2x2 texture
				brushprimit_tex.coords[0][0] = 1.0f;
				brushprimit_tex.coords[1][1] = 1.0f;
			}
			else
			{
				tex.scale[0] = (g_PrefsDlg.m_bHiColorTextures) ? 0.5 : 1;
				tex.scale[1] = (g_PrefsDlg.m_bHiColorTextures) ? 0.5 : 1;
			}
			tex.flags = q->flags;
			tex.value = q->value;
			tex.contents = q->contents;
			// TTimo - shader code cleanup
			// texdef.name is the name of the shader, not the name of the actual texture file
			tex.SetName(pCurrentShader->getName());
			// NOTE WARNING: Texture_SetTexture uses Texture_NextPos stuff to move the window position on to the texture
			// if there's some kind of fuckup in Texture_SetTexture you may end up with different pCurrentShader or even pCurrentShader == NULL
			// so we just consider pCurrentShader and current_texture are not valid after this point
			IShader *pAuxShader = pCurrentShader;
			Texture_SetTexture ( &tex, &brushprimit_tex, bFitScale, GETPLUGINTEXDEF(q));
			CString strTex;
			CString strName;
			// if shader, print shader name, otherwise texture name
			//++timo FIXME: maybe CShader needs some properties between color / default / actual shader
#ifdef _DEBUG
			// this one is never supposed to be set as current one
			if (pAuxShader->IsColor())
				Sys_Printf("ERROR: unexpected pCurrentShader->IsColor() in SelectTexture\n");
#endif
			// NOTE: IsColor is false, IsDefault the only remaining property
			if (pAuxShader->IsDefault())
			{
				strName = q->name;
				// remove the "textures/" if needed
				if (strName.Find("textures/")!=-1)
					strName = strName.Mid(9);
			}
			else
			{
				strName = pAuxShader->getName();
			}
			strTex.Format("%s W: %i H: %i", strName.GetBuffer(0), q->width, q->height);
			g_pParentWnd->SetStatusText(3, strTex);
			return;
		}
	}

	Sys_Status("Did not select a texture\n", 0);
}

/*
==============
Texture_MouseDown
==============
*/
void Texture_MouseDown (int x, int y, int buttons)
{
	Sys_GetCursorPos (&textures_cursorx, &textures_cursory);

	// lbutton = select texture
	if (buttons == MK_LBUTTON || buttons == (MK_LBUTTON | MK_SHIFT) || buttons == (MK_LBUTTON | MK_CONTROL))
	{
    SelectTexture (x, g_qeglobals.d_texturewin.height - 1 - y, buttons & MK_SHIFT, buttons & MK_CONTROL);
    UpdateSurfaceDialog();
    UpdatePatchInspector();
	}
}

/*
==============
Texture_MouseUp
==============
*/
void Texture_MouseUp (int x, int y, int buttons)
{
}

/*
==============
Texture_MouseMoved
==============
*/
void Texture_MouseMoved (int x, int y, int buttons)
{
	int scale = 1;

	if ( buttons & MK_SHIFT )
		scale = 4;

	// rbutton = drag texture origin
	if (buttons & MK_RBUTTON)
	{
		Sys_GetCursorPos (&x, &y);
		if ( y != textures_cursory)
		{
			g_qeglobals.d_texturewin.originy += ( y-textures_cursory) * scale;
			if (g_qeglobals.d_texturewin.originy > 0)
				g_qeglobals.d_texturewin.originy = 0;
			Sys_SetCursorPos (textures_cursorx, textures_cursory);
      CWnd *pWnd = CWnd::FromHandle(g_qeglobals.d_hwndTexture);
      if (g_PrefsDlg.m_bTextureScrollbar && pWnd != NULL)
      {
        pWnd->SetScrollPos(SB_VERT, abs(g_qeglobals.d_texturewin.originy));
      }
		  InvalidateRect(g_qeglobals.d_hwndTexture, NULL, false);
		  UpdateWindow (g_qeglobals.d_hwndTexture);
		}
		return;
	}
}


/*
============================================================================

DRAWING

============================================================================
*/

int imax(int iFloor, int i) { if (i>iFloor) return iFloor; return i; }
HFONT ghFont = NULL;

/*
============
Texture_Draw
TTimo: relying on the shaders list to display the textures
we must query all qtexture_t* to manage and display through the IShaders interface
this allows a plugin to completely override the texture system
============
*/
void Texture_Draw (int width, int height)
{
	qtexture_t	*q;
	int			x, y;
	char		*name;
	
	qglClearColor (
		g_qeglobals.d_savedinfo.colors[COLOR_TEXTUREBACK][0],
		g_qeglobals.d_savedinfo.colors[COLOR_TEXTUREBACK][1],
		g_qeglobals.d_savedinfo.colors[COLOR_TEXTUREBACK][2],
		0);
	qglViewport (0,0,width,height);
	qglMatrixMode(GL_PROJECTION);
	qglLoadIdentity ();
	
	qglClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	qglDisable (GL_DEPTH_TEST);
	qglDisable(GL_BLEND);
	qglOrtho (0, width, g_qeglobals.d_texturewin.originy-height, g_qeglobals.d_texturewin.originy, -100, 100);
	qglEnable (GL_TEXTURE_2D);
	
	qglPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
	g_qeglobals.d_texturewin.width = width;
	g_qeglobals.d_texturewin.height = height;
	
	Texture_StartPos();
	while (1)
	{

		// NOTE: return value is == pCurrentShader and pCurrentShader->getTexture == current_texture
		Texture_NextPos (&x, &y);
		q = current_texture;
		if (!q)
			break;

		int nWidth = (g_PrefsDlg.m_bHiColorTextures == TRUE) ? q->width * ((float)g_PrefsDlg.m_nTextureScale / 100) : q->width;
		int nHeight = (g_PrefsDlg.m_bHiColorTextures == TRUE) ? q->height * ((float)g_PrefsDlg.m_nTextureScale / 100) : q->height;
		// Is this texture visible?
		if ( (y-nHeight-FONT_HEIGHT < g_qeglobals.d_texturewin.originy)
			&& (y > g_qeglobals.d_texturewin.originy - height) )
		{
			// borders rules:
			// if it's the current texture, draw a thick red line, else:
			// shaders have a white border, simple textures don't
			// if !texture_showinuse: (some textures displayed may not be in use)
			// draw an additional square around with 0.5 1 0.5 color

			if (!strcmpi(g_qeglobals.d_texturewin.texdef.name, pCurrentShader->getName()))
			{
				qglLineWidth (3);
				qglColor3f (1,0,0);
				qglDisable (GL_TEXTURE_2D);
				
				qglBegin (GL_LINE_LOOP);
				qglVertex2f (x-4,y-FONT_HEIGHT+4);
				qglVertex2f (x-4,y-FONT_HEIGHT-nHeight-4);
				qglVertex2f (x+4+nWidth,y-FONT_HEIGHT-nHeight-4);
				qglVertex2f (x+4+nWidth,y-FONT_HEIGHT+4);
				qglEnd ();
				
				qglEnable (GL_TEXTURE_2D);
				qglLineWidth (1);
			}
			else
			{
				qglLineWidth (1);
				// shader border:
				if (!pCurrentShader->IsDefault())
				{
					qglColor3f (1,1,1);
					qglDisable (GL_TEXTURE_2D);				
					qglBegin (GL_LINE_LOOP);
					qglVertex2f (x-1,y+1-FONT_HEIGHT);
					qglVertex2f (x-1,y-nHeight-1-FONT_HEIGHT);
					qglVertex2f (x+1+nWidth,y-nHeight-1-FONT_HEIGHT);
					qglVertex2f (x+1+nWidth,y+1-FONT_HEIGHT);
					qglEnd ();				
					qglEnable (GL_TEXTURE_2D);
				}
				// highlight in-use textures
				if (pCurrentShader->IsInUse())
				{
					qglColor3f (0.5,1,0.5);
					qglDisable (GL_TEXTURE_2D);				
					qglBegin (GL_LINE_LOOP);
					qglVertex2f (x-3,y+3-FONT_HEIGHT);
					qglVertex2f (x-3,y-nHeight-3-FONT_HEIGHT);
					qglVertex2f (x+3+nWidth,y-nHeight-3-FONT_HEIGHT);
					qglVertex2f (x+3+nWidth,y+3-FONT_HEIGHT);
					qglEnd ();
					qglEnable (GL_TEXTURE_2D);
				}
			}

			// Draw the texture
			float fScale = (g_PrefsDlg.m_bHiColorTextures == TRUE) ? ((float)g_PrefsDlg.m_nTextureScale / 100) : 1.0;
			
			qglBindTexture( GL_TEXTURE_2D, q->texture_number );
			QE_CheckOpenGLForErrors();
			qglColor3f (1,1,1);
			qglBegin (GL_QUADS);
			qglTexCoord2f (0,0);
			qglVertex2f (x,y-FONT_HEIGHT);
			qglTexCoord2f (1,0);
			qglVertex2f (x+nWidth,y-FONT_HEIGHT);
			qglTexCoord2f (1,1);
			qglVertex2f (x+nWidth,y-FONT_HEIGHT-nHeight);
			qglTexCoord2f (0,1);
			qglVertex2f (x,y-FONT_HEIGHT-nHeight);
			qglEnd ();

			// draw the texture name
			qglDisable (GL_TEXTURE_2D);				
			qglColor3f (1,1,1);
			
			qglRasterPos2f (x, y-FONT_HEIGHT+2);
			
			// don't draw the directory name
			for (name = (char*)pCurrentShader->getName() ; *name && *name != '/' && *name != '\\' ; name++)
				;
			if (!*name)
				name = (char*)pCurrentShader->getName();
			else
				name++;
			
			if (!pCurrentShader->IsDefault())
			{
				// slow as shit
				CString s = "[";
				s += name;
				s += "]";
				qglCallLists (s.GetLength(), GL_UNSIGNED_BYTE, s.GetBuffer(0));
			}
			else
			{
				qglCallLists (strlen(name), GL_UNSIGNED_BYTE, name);
			}
			qglEnable (GL_TEXTURE_2D);

		}
	}
	
	g_qeglobals.d_texturewin.m_nTotalHeight = abs(y) + 100;
	// reset the current texture
	qglBindTexture( GL_TEXTURE_2D, 0 );
	qglFinish();
}

//++timo seems we only know hard inits now..
//void Texture_Init (bool bHardInit)
void Texture_Init()
{
	char	name[1024];
	byte	*pal = NULL;

  if (g_PrefsDlg.m_bHiColorTextures == FALSE)
  {
	  // load the palette
	  sprintf (name, "%s/pics/colormap.pcx", ValueForKey (g_qeglobals.d_project_entity, "basepath"));

	  Load256Image (name, NULL, &pal, NULL, NULL);
	  if (!pal)
    {
      // before dropping out, try to load it from the QERadiant directory
      CString strFile = g_strAppPath;
      AddSlash(strFile);
      strFile += "colormap.pcx";
	    Load256Image (strFile.GetBuffer(0), NULL, &pal, NULL, NULL);
	    if (!pal)
		    Sys_Printf ("Couldn't load %s or %s", name, strFile);
    }
    else
    {
	    Texture_InitPalette (pal);
	    free (pal);
    }
  }

	// create the fallback texture

//  if (bHardInit)
//  {
	  Texture_MakeNotexture();
	  g_qeglobals.d_qtextures = NULL;
    // initialize the qtexture map
#ifdef QTEXMAP_DEBUG
    if (g_qeglobals.d_qtexmap)
      Sys_Printf("WARNING: unexpected d_qtexmap is not empty in Texture_Init\n");
#endif
    g_qeglobals.d_qtexmap = new CMapStringToPtr;
		// initialize .. in some cases if no default texture / project loaded it crashes
		memset( &g_qeglobals.d_texturewin.texdef, 0, sizeof(g_qeglobals.d_texturewin.texdef) );
		g_qeglobals.d_texturewin.texdef.SetName("radiant/notex");
		g_qeglobals.d_texturewin.pShader = QERApp_Shader_ForName("radiant/notex");
//  }
  LoadShaders();

}

// somewhat replaced by QERApp_ReloadShaders
#if 0
void Texture_FlushUnused()
{
  CWaitCursor cursor;
  Texture_ShowInuse();
  if (g_qeglobals.d_qtextures)
  {
	  qtexture_t* pTex = g_qeglobals.d_qtextures->next;
    qtexture_t *pPrev = g_qeglobals.d_qtextures;
    while (pTex != NULL && pTex != g_qeglobals.d_qtextures)
    {
      qtexture_t* pNextTex = pTex->next;
  	  if (g_qeglobals.bSurfacePropertiesPlugin)
	    {
		    // Timo
		    // Surface properties plugin
#ifdef _DEBUG
  		  if ( !pTex->pData )
	  		  Sys_Printf("WARNING: found a qtexture_t* with no IPluginQTexture\n");
#endif
  		  if ( pTex->pData && pTex->inuse )
	  		  GETPLUGINTEXDEF(pTex)->DecRef();
  	  }

      if (!pTex->inuse)
      {
        unsigned int nTexture = pTex->texture_number;
        qglDeleteTextures(1, &nTexture);
        pPrev->next = pNextTex;
	      free(pTex);
      }
      else
      {
        pPrev = pTex;
      }
      pTex = pNextTex;
    }
  }
//	Texture_SortTextures();
	QERApp_SortActiveShaders();
	Sys_UpdateWindows (W_TEXTURE);
}
#endif

#if 0
void Texture_Cleanup(CStringList *pList)
{
  if (g_qeglobals.d_qtextures)
  {
	  qtexture_t* pTex = g_qeglobals.d_qtextures->next;
    while (pTex != NULL && pTex != g_qeglobals.d_qtextures)
    {
      qtexture_t* pNextTex = pTex->next;
      if (pList)
      {
        if (pTex->name[0] != '(')
        {
          pList->AddTail(pTex->name);
        }
      }

  	  if (g_qeglobals.bSurfacePropertiesPlugin)
	    {
		    // Timo
		    // Surface properties plugin
#ifdef _DEBUG
  		  if ( !pTex->pData )
	  		  Sys_Printf("WARNING: found a qtexture_t* with no IPluginQTexture\n");
#endif
  		  if ( pTex->pData )
	  		  GETPLUGINTEXDEF(pTex)->DecRef();
  	  }
	    free(pTex);
      pTex = pNextTex;
    }
  }

  int nSize = g_lstSkinCache.GetSize();
  for (int i = 0; i < nSize; i++)
  {
    SkinInfo *pInfo = reinterpret_cast<SkinInfo*>(g_lstSkinCache.GetAt(i));
		Sys_Printf("TODO: free skin qtexture_t\n");
    delete pInfo;
  }

}
#endif

//++timo replaced the IShaders API
#if 0
/*
==================
Texture_Flush
==================
*/
void Texture_Flush (bool bReload)
{
  if (!ConfirmModified())
    return;

  Map_New ();

  CWaitCursor cursor;
  CStringList strList;
  Texture_Init(false);
  Texture_Cleanup(&strList);

  GLuint* pGln = new GLuint[texture_extension_number-1];
  qglGenTextures(texture_extension_number-1, pGln);
  QE_CheckOpenGLForErrors();
  qglDeleteTextures(texture_extension_number-1, pGln);
  QE_CheckOpenGLForErrors();
  delete []pGln;
  texture_extension_number = 1;
	g_qeglobals.d_qtextures = NULL;

  if (bReload)
  {
    POSITION pos = strList.GetHeadPosition();
    while (pos)
    {
      CString strTex = strList.GetNext(pos);
		  QERApp_Shader_ForName(strTex.GetBuffer(0))->getTexture();
    }
  }

}
#endif

/////////////////////////////////////////////////////////////////////////////
// CTexWnd
IMPLEMENT_DYNCREATE(CTexWnd, CWnd);

CTexWnd::CTexWnd()
{
  m_bNeedRange = true;
}

CTexWnd::~CTexWnd()
{
}


BEGIN_MESSAGE_MAP(CTexWnd, CWnd)
	//{{AFX_MSG_MAP(CTexWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PARENTNOTIFY()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
  ON_BN_CLICKED(1200, OnShaderClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTexWnd message handlers

/*
============
WTexWndProc
============
*/
LONG WINAPI TexWndProc (
    HWND    hWnd,
    UINT    uMsg,
    WPARAM  wParam,
    LPARAM  lParam)
{
	int		xPos, yPos;
    RECT	rect;

    GetClientRect(hWnd, &rect);

    switch (uMsg)
    {
	case WM_CREATE:
    s_hdcTexture = GetDC(hWnd);
		QEW_SetupPixelFormat(s_hdcTexture, false);

		if ( ( s_hglrcTexture = qwglCreateContext( s_hdcTexture ) ) == 0 )
			Error( "wglCreateContext in WTex_WndProc failed" );

		if (!qwglShareLists( g_qeglobals.d_hglrcBase, s_hglrcTexture ) )
			Error( "wglShareLists in WTex_WndProc failed" );

    if (!qwglMakeCurrent( s_hdcTexture, s_hglrcTexture ))
		  Error ("wglMakeCurrent in WTex_WndProc failed");

	  g_qeglobals.d_hwndTexture = hWnd;
		return 0;

	case WM_DESTROY:
		//wglMakeCurrent( NULL, NULL );
		//wglDeleteContext( s_hglrcTexture );
		 ReleaseDC( hWnd, s_hdcTexture );
		return 0;
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN:
		SetCapture( g_qeglobals.d_hwndTexture );
		xPos = (short)LOWORD(lParam);  // horizontal position of cursor 
		yPos = (short)HIWORD(lParam);  // vertical position of cursor 
		
		Texture_MouseDown (xPos, yPos - g_nTextureOffset, wParam);
		return 0;

	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_LBUTTONUP:
		xPos = (short)LOWORD(lParam);  // horizontal position of cursor 
		yPos = (short)HIWORD(lParam);  // vertical position of cursor 
		
		Texture_MouseUp (xPos, yPos - g_nTextureOffset, wParam);
		if (! (wParam & (MK_LBUTTON|MK_RBUTTON|MK_MBUTTON)))
			ReleaseCapture ();
		return 0;

	case WM_MOUSEMOVE:
		xPos = (short)LOWORD(lParam);  // horizontal position of cursor 
		yPos = (short)HIWORD(lParam);  // vertical position of cursor 
		
		Texture_MouseMoved (xPos, yPos - g_nTextureOffset, wParam);
		return 0;
    }

    return DefWindowProc (hWnd, uMsg, wParam, lParam);
}



BOOL CTexWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
  WNDCLASS wc;
  HINSTANCE hInstance = AfxGetInstanceHandle();
  if (::GetClassInfo(hInstance, TEXTURE_WINDOW_CLASS, &wc) == FALSE)
  {
    // Register a new class
  	memset (&wc, 0, sizeof(wc));
    wc.style         = CS_NOCLOSE | CS_OWNDC;
    wc.lpszClassName = TEXTURE_WINDOW_CLASS;
    wc.hCursor       = LoadCursor (NULL,IDC_ARROW);
    wc.lpfnWndProc = TexWndProc;
    if (AfxRegisterClass(&wc) == FALSE)
      Error ("CZWnd RegisterClass: failed");
  }

  cs.lpszClass = TEXTURE_WINDOW_CLASS;
  cs.lpszName = "TEX";
  if (cs.style != QE3_CHILDSTYLE && cs.style != QE3_STYLE)
    cs.style = QE3_SPLITTER_STYLE;

	return CWnd::PreCreateWindow(cs);
}

int CTexWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

  CRect rctEdit(8, 5, 20, 20);
  g_nTextureOffset = 0;

/*
  if (g_PrefsDlg.m_bShaderTest)
  {
    m_wndShaders.Create("Show Shaders", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, rctEdit, this, 1200);
    m_wndShaders.ModifyStyleEx(0, WS_EX_CLIENTEDGE, 0);
    m_wndShaders.SetCheck(g_PrefsDlg.m_bShowShaders);
    g_nTextureOffset = 25;
  }
*/
  rctEdit.SetRect(8, g_nTextureOffset, 20, 20);
  m_wndFilter.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_LEFT, rctEdit, this, 1201);
  m_wndFilter.ModifyStyleEx(0, WS_EX_CLIENTEDGE, 0);
  m_wndFilter.SetTexWnd(this);

  g_nTextureOffset += 25;
  if (!g_PrefsDlg.m_bTextureWindow)
  {
    m_wndFilter.ShowWindow(SW_HIDE);
    g_nTextureOffset -= 25;
  }

  ShowScrollBar(SB_VERT, g_PrefsDlg.m_bTextureScrollbar);
  m_bNeedRange = true;

	return 0;
}

void CTexWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
  CRect rctClient;
  GetClientRect(rctClient);
/*
  if (g_PrefsDlg.m_bShaderTest && m_wndShaders.GetSafeHwnd())
  {
    m_wndShaders.SetWindowPos(NULL, rctClient.left + 8, rctClient.top + 5, rctClient.right - 16, 20, 0);
  }
*/
  m_wndFilter.SetWindowPos(NULL, rctClient.left + 8, rctClient.top + 25, rctClient.right - 16, 20, 0);
  m_bNeedRange = true;
}

void CTexWnd::OnShaderClick()
{
  g_PrefsDlg.m_bShowShaders = (m_wndShaders.GetCheck() != 0);
  g_PrefsDlg.SavePrefs();
	RedrawWindow();
}

void CTexWnd::OnParentNotify(UINT message, LPARAM lParam) 
{
	CWnd::OnParentNotify(message, lParam);
}

int g_nLastLen = 0;
int g_nTimerHandle = -1;
char g_cLastChar;

void CTexWnd::UpdateFilter(const char* pFilter)
{
  if (g_nTimerHandle > 0)
    KillTimer(1);
  g_bFilterEnabled = false;
  if (pFilter)
  {
    g_strFilter = pFilter;
    if (g_strFilter.GetLength() > 0)
    {
      g_bFilterEnabled = true;
      if (g_pParentWnd->CurrentStyle() == QR_QE4 || g_pParentWnd->CurrentStyle() == QR_4WAY)
      {
        if (g_strFilter.GetLength() > g_nLastLen)
        {
          g_cLastChar = toupper(g_strFilter.GetAt(g_strFilter.GetLength()-1));
          if (g_cLastChar == 'N' || g_cLastChar == 'O') // one of the other popups
          {
            g_nTimerHandle = SetTimer(1, 800, NULL);   // half second timer
          }
        }
      }
    }
    g_nLastLen = g_strFilter.GetLength();
//	  Texture_SortTextures();
		QERApp_SortActiveShaders();
  }
  Sys_UpdateWindows (W_TEXTURE);
}

void CTexWnd::UpdatePrefs()
{
  if (!g_PrefsDlg.m_bTextureWindow)
  {
    m_wndFilter.ShowWindow(SW_HIDE);
    g_nTextureOffset = 0;
  }
  else
  {
    m_wndFilter.ShowWindow(SW_SHOW);
    g_nTextureOffset = 25;
  }
  ShowScrollBar(SB_VERT, g_PrefsDlg.m_bTextureScrollbar);
  m_bNeedRange = true;
  Invalidate();
  UpdateWindow();
}

void CTexWnd::FocusEdit()
{
  if (m_wndFilter.IsWindowVisible())
    m_wndFilter.SetFocus();
}

void CTexWnd::OnTimer(UINT nIDEvent) 
{
  KillTimer(1);
  g_nLastLen = 0;
  g_nTimerHandle = -1;
  ::SetFocus(g_qeglobals.d_hwndEntity);
  ::PostMessage(g_qeglobals.d_hwndEntity, WM_CHAR, g_cLastChar, 0);
}

void CTexWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
  g_pParentWnd->HandleKey(nChar, nRepCnt, nFlags);
}

void CTexWnd::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
  g_pParentWnd->HandleKey(nChar, nRepCnt, nFlags, false);
}

void CTexWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
  CRect rctClient;
  GetClientRect(rctClient);
  int nOld = g_qeglobals.d_texturewin.m_nTotalHeight;
  if (!qwglMakeCurrent(s_hdcTexture, s_hglrcTexture))
  //if ( !qwglMakeCurrent(dc.m_hDC, s_hglrcTexture ) )
  {
    Sys_Printf("ERROR: wglMakeCurrent failed..\n ");
    Sys_Printf("Please restart Q3Radiant if the Texture view is not working\n");
  }
  else
  {
    Texture_Draw (rctClient.right-rctClient.left, rctClient.bottom-rctClient.top - g_nTextureOffset);
		qwglSwapBuffers(s_hdcTexture);
    TRACE("Texture Paint\n");
  }
  if (g_PrefsDlg.m_bTextureScrollbar && (m_bNeedRange || g_qeglobals.d_texturewin.m_nTotalHeight != nOld))
  {
    m_bNeedRange = false;
    SetScrollRange(SB_VERT, 0, g_qeglobals.d_texturewin.m_nTotalHeight, TRUE);
  }
}

void CTexWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);

  int n = GetScrollPos(SB_VERT);;
  switch (nSBCode)
  {
    case SB_LINEUP :
    {
      n = (n - 15 >  0) ? n - 15 : 0; 
      break;
    }
    case SB_LINEDOWN :
    {
      n = (n + 15 < g_qeglobals.d_texturewin.m_nTotalHeight) ? n + 15 : n; 
      break;
    }
    case SB_PAGEUP :
    {
      n = (n - g_qeglobals.d_texturewin.height >  0) ? n - g_qeglobals.d_texturewin.height : 0; 
      break;
    }
    case SB_PAGEDOWN :
    {
      n = (n + g_qeglobals.d_texturewin.height < g_qeglobals.d_texturewin.m_nTotalHeight) ? n + g_qeglobals.d_texturewin.height : n; 
      break;
    }
    case SB_THUMBPOSITION :
    {
      n = nPos;
      break;
    }
    case SB_THUMBTRACK :
    {
      n = nPos;
      break;
    }
  }
  SetScrollPos(SB_VERT, n);
	g_qeglobals.d_texturewin.originy = -((int)n);
  Invalidate();
  UpdateWindow();
  //Sys_UpdateWindows(W_TEXTURE);
}

void LoadShaders()
{
  CStringList lst;
  BuildShaderList( lst );
  if (lst.GetCount() == 0)
    return;
  POSITION pos = lst.GetHeadPosition();
  while (pos != NULL)
  {
    QERApp_LoadShaderFile( lst.GetAt(pos).GetBuffer(0) );
    lst.GetNext(pos);
  }

  //++timo clean
#if 0
	char	dirstring[1024];
	char	*path;
  path = ValueForKey (g_qeglobals.d_project_entity, "basepath");
  sprintf (dirstring, "%s/scripts/shaderlist.txt", path);
  char *pBuff = NULL;
  
  int nLen = LoadFile(dirstring, reinterpret_cast<void**>(&pBuff));
  if (nLen == -1)
  {
    nLen = PakLoadAnyFile(dirstring, reinterpret_cast<void**>(&pBuff));
  }
  if (nLen > 0)
  {
    CStringList lst;
    StartTokenParsing(pBuff);
    nLen = 0;
    while (GetToken(true))
    {
      // each token should be a shader filename
      sprintf(dirstring, "%s/scripts/%s.shader", path, token);
      lst.AddTail(dirstring);
      nLen++;
    }
    POSITION pos = lst.GetHeadPosition();
    while (pos != NULL)
    {
      QERApp_LoadShaderFile( lst.GetAt(pos).GetBuffer(0) );
      lst.GetNext(pos);
    }
    free(pBuff);
  }
  else
  {
    Sys_Printf("Unable to load %s, shaders not loaded!\n", dirstring);
  }
#endif
}

// TTimo: modified to expect the reletive path to the skin as input
// will look into pak files if necessary
// uses the shader code to load the texture Try_Texture_ForName
// modified SkinInfo accordingly to store the qtexture_t and shader name (reletive version)
// the .md3 have bundled filtype extension, but they don't fit with the actual data
//   ex: models/mapobjects/gargoyle.tga doesn't exist, but models/mapobjects/gargoyle.jpg can be used instead
//   so we remove the extension before load attempt
int WINAPI Texture_LoadSkin(char *pName, int *pnWidth, int *pnHeight)
{
  byte *pic = NULL;
  byte *pic32 = NULL;
  int nTex = -1;
	qtexture_t *qtex;
	SkinInfo *pInfo;
	char *pCleanName;

  int nSize = g_lstSkinCache.GetSize();
	pCleanName = CleanTextureName( pName );
  for (int i = 0; i < nSize; i++)
  {
    SkinInfo *pInfo = reinterpret_cast<SkinInfo*>(g_lstSkinCache.GetAt(i));
    if (pInfo)
    {
      if (stricmp(pCleanName, pInfo->m_strName) == 0)
      {
        return pInfo->m_nTextureBind;
      }
    }
  }

	// if the load is successfull, we get back a qtexture_t
	// we don't need to free it, it's in g_qeglobals.d_qtextures
	qtex = QERApp_Try_Texture_ForName( pCleanName );
	if (qtex)
	{
		nTex = qtex->texture_number;
		pInfo = new SkinInfo(qtex->name, nTex, qtex);
	}
	else
	{
		pInfo = new SkinInfo(pCleanName, -1, NULL);
	}
  g_lstSkinCache.Add(pInfo);

  return nTex;

}

bool CTexWnd::CheckFilter( const char* name )
{
	char buf[1024];
	short int *pBuf = (short int *)&buf[0];
	*pBuf = 1024;
	m_wndFilter.GetLine( 0, buf );
	if (strstr( name, buf ) != 0)
		return true;
	return false;
}

