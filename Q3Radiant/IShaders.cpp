//-----------------------------------------------------------------------------
//
// $LogFile$
// $Revision: 1.8 $
// $Author: ttimo $
// $Date: 2000/07/24 04:47:26 $
// $Log: IShaders.cpp,v $
// Revision 1.8  2000/07/24 04:47:26  ttimo
// 202beta3, released version
//
// Revision 1.7  2000/07/22 22:52:27  ttimo
// beta 2 sent to PJ
//
// Revision 1.6  2000/07/21 05:24:54  ttimo
// hash table in QERApp_Try_Texture_ForName, more to come
//
// Revision 1.5  2000/07/20 04:49:17  ttimo
// major speedup in Map_BuildBrushData by fixing a silly bug
//
// Revision 1.4  2000/07/09 21:54:17  ttimo
// synced it with GtkRadiant
//
// Revision 1.3  2000/06/24 20:55:42  ttimo
// build 201
//
// Revision 1.1.2.1.2.30  2000/06/23 22:25:37  ttimo
// 201! texture submenu bug fixes...
//
// Revision 1.1.2.1.2.29  2000/06/11 00:17:50  ttimo
// more fixes
//
// Revision 1.1.2.1.2.28  2000/06/05 22:59:08  ttimo
// G_Dewan's fixes
//
// Revision 1.1.2.1.2.27  2000/05/25 19:20:30  ttimo
// fixed common shaders load at startup
//
// Revision 1.1.2.1.2.26  2000/05/22 03:40:50  ttimo
// 199RC1
//
// Revision 1.1.2.1.2.25  2000/05/15 06:19:39  ttimo
// towards RC
//
// Revision 1.1.2.1.2.24  2000/05/11 22:07:14  ttimo
// fixing case sensitivity bugs
//
// Revision 1.1.2.1.2.23  2000/05/11 03:44:13  ttimo
// sorted textures
//
// Revision 1.1.2.1.2.22  2000/05/10 05:07:58  ttimo
// texture browser is better
//
// Revision 1.1.2.1.2.21  2000/05/09 22:27:01  ttimo
// working on texture browser
//
// Revision 1.1.2.1.2.20  2000/04/29 03:25:55  ttimo
// 198 beta 3
//
// Revision 1.1.2.1.2.19  2000/04/25 18:40:30  ttimo
// better alpha channel support
//
// Revision 1.1.2.1.2.18  2000/04/25 03:15:06  ttimo
// Curry QERApp_ReloadShaderFile ..
//
// Revision 1.1.2.1.2.17  2000/04/24 22:34:13  ttimo
// no message
//
// Revision 1.1.2.1.2.16  2000/04/24 05:00:25  ttimo
// no message
//
// Revision 1.1.2.1.2.15  2000/04/22 16:53:00  ttimo
// proper GL context for plugins
// CSG Merge shortcut
//
// Revision 1.1.2.1.2.14  2000/04/18 06:24:23  ttimo
// no message
//
// Revision 1.1.2.1.2.13  2000/04/17 02:38:27  ttimo
// no message
//
// Revision 1.1.2.1.2.12  2000/04/16 22:51:04  ttimo
// no message
//
// Revision 1.1.2.1.2.11  2000/04/14 04:09:32  ttimo
// ain't ya feelin better now?
//
// Revision 1.1.2.1.2.10  2000/04/13 05:00:22  ttimo
// crashes, texture fixes on the way!
//
// Revision 1.1.2.1.2.9  2000/04/12 04:17:27  ttimo
// no message
//
// Revision 1.1.2.1.2.8  2000/04/10 22:51:40  ttimo
// 198beta ( released version, fixed stuff )
//
// Revision 1.1.2.1.2.7  2000/04/10 06:03:52  ttimo
// 198 beta
//
// Revision 1.1.2.1.2.6  2000/04/08 22:26:11  ttimo
// nearing beta
//
// Revision 1.1.2.1.2.5  2000/04/07 04:27:36  ttimo
// fixing on the way!
//
// Revision 1.1.2.1.2.4  2000/03/23 23:41:46  ttimo
// nearly ready for merge with Release197
//
// Revision 1.1.2.1.2.3  2000/02/22 00:30:44  ttimo
// starting to look good
//
// Revision 1.1.2.1.2.2  2000/02/16 14:44:14  ttimo
// CShader loading .. the IShader API is getting shape
//
// Revision 1.1.2.1.2.1  2000/02/13 22:05:14  ttimo
// no message
//
// Revision 1.1.2.1  2000/02/11 03:52:30  ttimo
// working on the IShader interface
//
//
// DESCRIPTION:
// implementation of the shaders / textures interface
//

#include "stdafx.h"

//++timo TODO: use stl::map !! (I tried having a look to CMap but it obviously sucks)
CShaderArray	g_Shaders;
// whenever a shader gets activated / deactivated this list is updated
// NOTE: make sure you don't add a shader that's already in
// NOTE: all shaders in this array are in the main g_Shaders
CShaderArray	g_ActiveShaders;

// clean a texture name to the qtexture_t name format we use internally
// NOTE: there are so many cases .. this may need to get updated to cover all of them
// we expect a "textures/" path on top, except if bAddTexture is set to true .. in case we add in needed
// NOTE: case sensitivity: the engine is case sensitive. we store the shader name with case information and save with case
// information as well. but we assume there won't be any case conflict and so when doing lookups based on shader name,
// we compare as case insensitive. That is Radiant is case insensitive, but knows that the engine is case sensitive.
//++timo FIXME: we need to put code somewhere to detect when two shaders that are case insensitive equal are present
char* CleanTextureName(const char* name, bool bAddTexture)
{
  static char stdName[QER_MAX_NAMELEN];
  strcpy( stdName, name );
  QE_ConvertDOSToUnixName( stdName, stdName );
  if (stdName[strlen(name)-4] == '.')
  // strip extension
  stdName[strlen(stdName)-4] = '\0';
  // check we have "textures/"
  char *cut = strstr( stdName, "textures/" );
  if (!cut)
	{
		if (bAddTexture)
		{
#if 0
			Sys_Printf("CleanTextureName: adding \"texture/\" to %s\n", stdName );
#endif
			char aux[QER_MAX_NAMELEN];
			sprintf( aux, "textures/%s", stdName );
			strcpy( stdName, aux );
		}
#if 0
		else
		{
			Sys_Printf("WARNING: \"textures/\" not found in CleanTextureName\n");
		}
#endif
	}
  return stdName;
}

int	WINAPI QERApp_GetActiveShaderCount	()
{
	return g_ActiveShaders.GetSize();
}

IShader* WINAPI QERApp_ActiveShader_ForIndex (int i)
{
	return static_cast<CShader *>(g_ActiveShaders.GetAt(i));
}

void CShaderArray::SortShaders()
{
	CPtrArray aux;
	int i,icount;
	int j,jcount;
	CShader *pSort;
	const char *sSort;
	// dumb sort .. would it ever grow big enough so we would have to do something clever? noooo
  icount = CPtrArray::GetSize();
	for( i = 0; i < icount ; i++ )
	{
		pSort = static_cast<CShader *>(GetAt(i));
		sSort = pSort->getName();
		jcount = aux.GetSize();
		for( j = 0; j < jcount ; j++ )
		{
			if (strcmp(sSort, static_cast<CShader *>(aux.GetAt(j))->getName()) < 0)
				break;
		}
		aux.InsertAt(j, pSort);
	}
  CPtrArray::RemoveAll();
  CPtrArray::InsertAt(0, &aux);
}

// will sort the active shaders list by name
// NOTE: it would be easier if the thing would stay sorted by using a map<name,CShader> thing
//++timo FIXME: would need to export that to allow external override?
void WINAPI QERApp_SortActiveShaders()
{
	g_ActiveShaders.SortShaders();
}

// NOTE: case sensitivity
// although we store shader names with case information, Radiant does case insensitive searches
// (we assume there's no case conflict with the names)
CShader* CShaderArray::Shader_ForName( const char * name) const
{
	int i;
  for ( i = 0; i < CPtrArray::GetSize(); i++ )
	{
    CShader *pShader = static_cast<CShader*>(CPtrArray::GetAt(i));
		if (_stricmp(pShader->getName(), name) == 0)
			return pShader;
	}
	return NULL;
}

void CShader::CreateDefault(const char* name)
{
	char *stdName = CleanTextureName( name, true );
  m_strTextureName = stdName;
	setName( name );
}

CShader* CShaderArray::Shader_ForTextureName( const char * name) const
{
#ifdef _DEBUG
  // check we were given a texture name that fits the qtexture_t naming conventions
  if (strcmp( name, CleanTextureName( name ) ) != 0)
    Sys_Printf("WARNING: texture name %s doesn't fit qtexture_t conventions in CShaderArray::Shader_ForTextureName\n", name);
#endif
  int i;
  for ( i = 0; i < CPtrArray::GetSize(); i++ )
  {
    CShader *pShader = static_cast<CShader*>(CPtrArray::GetAt(i));
    if (strcmp( name, CleanTextureName( pShader->getTextureName() ) ) == 0)
      return pShader;
  }
  return NULL;
}

void CShaderArray::AddSingle(LPVOID lp)
{
	int i;
  for ( i = 0; i < CPtrArray::GetSize(); i++ )
	{
    if (CPtrArray::GetAt(i) == lp)
			return;
	}
  CPtrArray::Add(lp);
}

void CShaderArray::operator = (const class CShaderArray & src)
{
	int i;

#ifdef _DEBUG
  if (CPtrArray::GetSize()!=0)
		Sys_Printf("WARNING: CShaderArray::operator = expects an empty array\n");
#endif
	Copy(src);
	// now go through and IncRef
  for(i=0; i<CPtrArray::GetSize(); i++)
    static_cast<IShader*>(CPtrArray::GetAt(i))->IncRef();
}

//++timo NOTE: for debugging we may need to keep track and tell wether everything has been properly unloaded
void CShaderArray::ReleaseAll()
{
	int i;
  int count = CPtrArray::GetSize();
	// decref
	for(i=0; i<count; i++)
    static_cast<IShader*>(CPtrArray::GetAt(i))->DecRef();
	// get rid
  CPtrArray::RemoveAll();
}

//++timo clean when bug 104655 fixed
typedef struct patchEntry_s
{
	char name[QER_MAX_NAMELEN];
	patchMesh_t *p;
} patchEntry_t;

CPtrArray PatchShaders;

void PushPatch(patchMesh_t *patch)
{
	patchEntry_t *pEntry = new patchEntry_s;
	pEntry->p = patch;
	strcpy( pEntry->name, patch->pShader->getName() );
	PatchShaders.Add(pEntry);
}

char *ShaderNameLookup(patchMesh_t *patch)
{
	int i;
	int count = PatchShaders.GetSize();
	for(i=0; i<count; i++)
	{
		if (static_cast<patchEntry_t*>(PatchShaders.GetAt(i))->p == patch)
			return static_cast<patchEntry_t*>(PatchShaders.GetAt(i))->name;
	}
	Sys_Printf("ERROR: failed to lookup name in ShaderNameLookup??\n");
	return "radiant/notex";
}
//++timo end clean

// will free all GL binded qtextures and shaders
// NOTE: doesn't make much sense out of Radiant exit or called during a reload
void WINAPI QERApp_FreeShaders()
{
	int i;
	brush_t	*b;

	// store the shader names used by the patches
	for(i=0; i<PatchShaders.GetSize(); i++)
		delete PatchShaders.GetAt(i);
	PatchShaders.RemoveAll();
	for (b=active_brushes.next ; b != NULL && b != &active_brushes ; b=b->next)
  {
    if (b->patchBrush)
			PushPatch(b->pPatch);
  }
	for (b=selected_brushes.next ; b != NULL && b != &selected_brushes ; b=b->next)
  {
    if (b->patchBrush)
			PushPatch(b->pPatch);
  }

	// reload shaders
	// empty the actives shaders list
	g_ActiveShaders.ReleaseAll();
	g_Shaders.ReleaseAll();
	// empty the main g_qeglobals.d_qtextures list
	// FIXME: when we reload later on, we need to have the shader names
	// for brushes it's stored in the texdef
	// but patches don't have texdef
	// see bug 104655 for details
	// so the solution, build an array of patchMesh_t* and their shader names
#ifdef _DEBUG
	Sys_Printf("FIXME: bug 104655 workaround\n");
#endif

	// NOTE: maybe before we'd like to set all qtexture_t in the shaders list to notex?
	// NOTE: maybe there are some qtexture_t we don't want to erase? For plain color faces maybe?
	// NOTE: the GL textures are freed later on
  if (g_qeglobals.d_qtextures)
  {
	  qtexture_t* pTex = g_qeglobals.d_qtextures->next;
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
  		  if ( pTex->pData )
	  		  GETPLUGINTEXDEF(pTex)->DecRef();
  	  }
	    free(pTex);
      pTex = pNextTex;
    }
  }
	// free GL bindings
  GLuint* pGln = new GLuint[texture_extension_number-1];
  qglGenTextures(texture_extension_number-1, pGln);
  QE_CheckOpenGLForErrors();
  qglDeleteTextures(texture_extension_number-1, pGln);
  QE_CheckOpenGLForErrors();
  delete []pGln;
  texture_extension_number = 1;
	g_qeglobals.d_qtextures = NULL;
  // free the map
  g_qeglobals.d_qtexmap->RemoveAll();
}

// NOTE: maybe this needs to be exported in the func_table??
void SetShader(patchMesh_t * patch)
{
	// unhook current shader
	patch->pShader->DecRef();
	// don't access this one! it has been deleted .. it's DEAD
	patch->d_texture = NULL;
	// hook the new one, increment the refcount
	patch->pShader = QERApp_Shader_ForName(ShaderNameLookup(patch));
	patch->pShader->IncRef();
	patch->d_texture = patch->pShader->getTexture();
}

void SetShader(face_t * f)
{
	// unhook current shader
	f->pShader->DecRef();
	// don't access the texdef! it's DEAD
	f->d_texture = NULL;
	// hook and increment refcount
	f->pShader = QERApp_Shader_ForName(f->texdef.name);
	f->pShader->IncRef();
	f->d_texture = f->pShader->getTexture();
}

void WINAPI QERApp_ReloadShaders()
{
	brush_t *b;
	face_t *f;

	QERApp_FreeShaders();
	// now we must reload the shader information from shaderfiles
	LoadShaders();
	// refresh the map visuals: replace our old shader objects by the new ones
	// on brush faces we have the shader name in texdef.name
	// on patches we have the shader name in PatchShaders
	// while we walk through the map data, we DecRef the old shaders and push the new ones in
	// if all goes well, most of our old shaders will get deleted on the way

	// FIXME: bug 104655, when we come accross a patch, we use the above array since the d_texture is lost
	// NOTE: both face_t and patchMesh_t store pointers to the shader and qtexture_t
	// in an ideal world they would only store shader and access the qtexture_t through it
	// reassign all current shaders
	for (b=active_brushes.next ; b != NULL && b != &active_brushes ; b=b->next)
  {
    if (b->patchBrush)
			SetShader(b->pPatch);
    else
		  for (f=b->brush_faces ; f ; f=f->next)
				SetShader(f);
  }
	for (b=selected_brushes.next ; b != NULL && b != &selected_brushes ; b=b->next)
  {
    if (b->patchBrush)
			SetShader(b->pPatch);
    else
		  for (f=b->brush_faces ; f ; f=f->next)
				SetShader(f);
  }
	// current shader
	// NOTE: we are kinda making it loop on itself, it will update the pShader and scroll the texture window
	Texture_SetTexture( &g_qeglobals.d_texturewin.texdef, &g_qeglobals.d_texturewin.brushprimit_texdef );

}

int WINAPI QERApp_LoadShadersFromDir (const char* path)
{
  int count = 0;
	// scan g_Shaders, and call QERApp_Shader_ForName for each in the given path
	// this will load the texture if needed and will set it in use..
	int nSize = g_Shaders.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		CShader *pShader = reinterpret_cast<CShader*>(g_Shaders.ElementAt(i));
		if (strstr(pShader->getShaderFileName(), path) || strstr(pShader->getName(), path))
		{
      count++;
			// request the shader, this will load the texture if needed and set "inuse"
			//++timo FIXME: should we put an Activate member on CShader?
			// this QERApp_Shader_ForName call is a kind of hack
			IShader *pFoo = QERApp_Shader_ForName(pShader->getName());
#ifdef _DEBUG
			// check we activated the right shader
			// NOTE: if there was something else loaded, the size of g_Shaders may have changed and strange behaviours are to be expected
			if (pFoo != pShader)
				Sys_Printf("WARNING: unexpected pFoo != pShader in QERApp_LoadShadersFromDir\n");
#endif
		}
  }
  return count;
}

bool CShader::Parse()
{
	// token is shader name (full path with a "textures\")
	// we remove the "textures\" part
	setName((char *)&(token[9]));
	// name of the qtexture_t we'll use to represent this shader (this one has the "textures\" before)
	char *stdName = CleanTextureName( token );
	m_strTextureName = stdName;
	GetToken(true);
	if (strcmp(token, "{"))
		return false;
	else
	{
		// we need to read until we hit a balanced }
		int nMatch = 1;
		while (nMatch > 0 && GetToken(true))
		{
			if (strcmp(token, "{") == 0)
			{
				nMatch++;
			}
			else if (strcmp(token, "}") == 0)
			{
				nMatch--;
			}
			else if (strcmpi(token, "qer_nocarve") == 0)
			{
				m_nFlags |= QER_NOCARVE;
			}
			else if (strcmpi(token, "qer_trans") == 0)
			{
				if (GetToken(true))
				{
					m_fTrans = atof(token);
				}
				m_nFlags |= QER_TRANS;
			}
			else if (strcmpi(token, "qer_editorimage") == 0)
			{
				if (GetToken(true))
				{
					m_strTextureName = CleanTextureName( token, true );
				}
			}
			else if (strcmpi(token, "surfaceparm") == 0)
			{
				if (GetToken(true))
				{
					if (strcmpi(token, "fog") == 0)
					{
						if (m_fTrans == 1.0f) // has not been explicitly set by qer_trans
						{
							m_fTrans = 0.35f;
						}
					}
				}
			}
		}
		if (nMatch != 0)
			return false;
	}
	return true;
}

void CShader::RegisterActivate()
{
	// fill the qtexture_t with shader information
	//++timo FIXME: a lot of that won't be necessary, will be stored at IShader* level
    strcpy(m_pTexture->shadername, m_Name);
    // this flag is set only if we have a shaderfile name
    if (m_ShaderFileName[0] != '\0')
      m_pTexture->bFromShader = true;
    else
      m_pTexture->bFromShader = false;
	//++timo FIXME: what do we do with that?
    //m_pTexture->fTrans = pInfo->m_fTransValue;
	m_pTexture->fTrans = 1.0f; // if != 1.0 it's ot getting drawn in Cam_Draw
    m_pTexture->nShaderFlags = m_nFlags;
	// store in the active shaders list (if necessary)
	g_ActiveShaders.AddSingle( this );
  // when you activate a shader, it gets displayed in the texture browser
  m_bDisplayed = true;
	IncRef();
}

void CShader::Try_Activate()
{
	m_pTexture = QERApp_Try_Texture_ForName( m_strTextureName );
	if (m_pTexture)
		RegisterActivate();
}

void CShader::Activate()
{
	Try_Activate();
	if (!m_pTexture)
	{
		// use the shadernotex.jpg
		//++timo TODO: detect shadernotex.jpg not found and give an error message!
		m_pTexture = QERApp_Texture_ForName2( "textures/radiant/shadernotex" );
		RegisterActivate();
	}
}

void WINAPI QERApp_LoadShaderFile (const char* filename)
{
	char* pBuff = NULL;
	int nSize = LoadFile(filename, reinterpret_cast<void**>(&pBuff));
	if (nSize == -1)
		nSize = PakLoadAnyFile(filename, reinterpret_cast<void**>(&pBuff));
	if (nSize > 0)
	{
		Sys_Printf("Parsing shaderfile %s\n", filename);
		StartTokenParsing(pBuff);
		while (GetToken(true))
		{
			// first token should be the path + name.. (from base)
			CShader *pShader = new CShader();
			// we want the relative filename only, it's easier for later lookup .. see QERApp_ReloadShaderFile
			char cTmp[1024];
			QE_ConvertDOSToUnixName( cTmp, filename );
			pShader->setShaderFileName( filename + strlen(ValueForKey(g_qeglobals.d_project_entity, "basepath")) + 1);
			if (pShader->Parse())
			{
				// do we already have this shader?
				//++timo NOTE: this may a bit slow, we may need to use a map instead of a dumb list
				if (g_Shaders.Shader_ForName( pShader->getName() ) != NULL)
				{
					Sys_Printf( "WARNING: shader %s is already in memory, definition in %s ignored.\n", pShader->getName(), filename );
					delete pShader;
				}
				else
				{
					pShader->IncRef();
					g_Shaders.Add( (LPVOID)pShader );
					int n = g_PrefsDlg.m_nShader;
					if ((g_PrefsDlg.m_nShader == CPrefsDlg::SHADER_ALL) || (g_PrefsDlg.m_nShader == CPrefsDlg::SHADER_COMMON && strstr(pShader->getName(), "common" )))
						// load the qtexture and display in tex wnd
						pShader->Activate();
				}
			}
			else
			{
				Sys_Printf("Error parsing shader %s\n", pShader->getName());
				delete pShader;
			}			
		}
		free (pBuff);
	}
	else
	{
		Sys_Printf("Unabled to read shaderfile %s\n", filename);
	}	
}

IShader* WINAPI QERApp_Try_Shader_ForName(const char* name)
{
	// look for the shader
	CShader* pShader = g_Shaders.Shader_ForName( name );
	if (!pShader)
		// not found
		return NULL;
	// we may need to load the texture or use the "shader without texture" one
	pShader->Activate();
  pShader->SetDisplayed( true );
	return pShader;
}

IShader* WINAPI QERApp_Shader_ForName(const char* name)
{
	//++timo FIXME: do we allow NULL and "" calling?
	// how does it deal with notexture? can we simply replace by "textures/radiant/notex"?
	if (name == NULL || strlen(name) == 0)
	{
		Sys_Printf("FIXME: name == NULL || strlen(name) == 0 in QERApp_Shader_ForName\n");
		return QERApp_Shader_ForName("radiant/notex"); //++timo ???
	}

	// entities that should be represented with plain colors instead of textures
	// request a texture name with (r g b) (it's stored in their class_t)
	if (name[0]=='(')
	{
		return QERApp_ColorShader_ForName(name);
	}

	CShader* pShader = static_cast<CShader*>(QERApp_Try_Shader_ForName( name ));
	if (pShader)
	{
		pShader->SetDisplayed( true );
		return pShader;
	}
	// we don't know this shader, maybe it's a straight texture 
	pShader = new CShader;
	pShader->CreateDefault( name );
  // hook it into the shader list
	g_Shaders.Add( (LPVOID)pShader );
	pShader->IncRef();
	// if it can't find the texture, "textures/radiant/notex" will be used
  pShader->Activate();
  pShader->SetDisplayed( true );
	return pShader;
}

//++timo TODO: to be removed and replace by our own stuff soon
qtexture_t *Texture_LoadTGATexture (unsigned char* pPixels, int nWidth, int nHeight, char* pPath, int nFlags, int nContents, int nValue );

qtexture_t* WINAPI QERApp_Try_Texture_ForName(const char* name)
{
	qtexture_t *q;
	char f1[1024],f2[1024];
	unsigned char *pPixels = NULL;
	int nWidth,nHeight;

  // convert the texture name to the standard format we use in qtexture_t
  char *stdName = CleanTextureName(name);

  // use the hash table
  q = NULL;
  g_qeglobals.d_qtexmap->Lookup( stdName, (void *&)q );
  if (q)
    return q;
#ifdef QTEXMAP_DEBUG
  for (q=g_qeglobals.d_qtextures ; q ; q=q->next)
	{
		if (!strcmp(stdName,  q->name))
    {
      Sys_Printf("ERROR: %s is not in texture map, but was found in texture list\n");
			return q;
    }
	}
#endif
#if 0
  for (q=g_qeglobals.d_qtextures ; q ; q=q->next)
	{
		if (!strcmp(stdName,  q->name))
				return q;
	}
#endif
	//++timo TODO: say something about loading the file?
	// try loading the texture
	//++timo FIXME: "texturepath" is no use now?
	sprintf(f1, "%s/%s", ValueForKey (g_qeglobals.d_project_entity, "basepath"), name);
	QE_ConvertDOSToUnixName( f2, f1 );
	// NOTE: we may need a global strategy to support default extensions etc.
	strcpy(f1,f2);
	// check wether a filename extension was provided
	// NOTE: only works for 3 letters extensions ( .tga .jpg ... )
	if (f1[strlen(f1)-4] == '.')
	{
		// try straight loading
		LoadImage( f1, &pPixels, &nWidth, &nHeight );
	}
	if (!pPixels)
	{
		// try adding extensions, .tga first
		sprintf(f2,"%s.tga",f1);
		LoadImage( f2, &pPixels, &nWidth, &nHeight );
		if (!pPixels)
		{
			// .jpg
			sprintf(f2,"%s.jpg",f1);
			LoadImage( f2, &pPixels, &nWidth, &nHeight );
		}
	}
	if (!pPixels)
		// we failed
		return NULL;
	else
	{
		// TODO: display .pk3 file name if loaded from .pk3 (needs to write a VFS .. sort of)
		Sys_Printf("LOADED: %s\n", f2 );
	}
	// instanciate a new qtexture_t
	// NOTE: when called by a plugin we must make sure we have set Radiant's GL context before binding the texture

	// we'll be binding the GL texture now
	// need to check we are using a right GL context
	// with GL plugins that have their own window, the GL context may be the plugin's, in which case loading textures will bug
	HDC currentHDC = qwglGetCurrentDC();
	HGLRC currentHGLRC = qwglGetCurrentContext();
	//++timo FIXME: this may duplicate with qtexture_t* WINAPI QERApp_Texture_ForName (const char *name)
	//++timo FIXME: we need a list of lawfull GL contexts or something?
	// I'd rather always use the same GL context for binding...
	if (currentHDC != g_qeglobals.d_hdcBase || currentHGLRC != g_qeglobals.d_hglrcBase)
	{
#ifdef _DEBUG
		Sys_Printf("Switching context!\n");
#endif
		qwglMakeCurrent( g_qeglobals.d_hdcBase, g_qeglobals.d_hglrcBase );
	}

	//++timo TODO: remove that and use our own implementation?
	q = Texture_LoadTGATexture( pPixels, nWidth, nHeight, NULL, 0, 0, 0);
	free(pPixels);
	// fill qtexture_t information
	//++timo FIXME: filename will be removed .. name for qtexture_t is actually the filename now
  // NOTE: see qtexture_s::name for naming conventions, must remove filename extension
	strcpy( q->filename, name );
	strcpy( q->name, name );
	// only strip extension if extension there is!
	if (q->name[strlen(q->name)-4] == '.')
		q->name[strlen(q->name)-4]='\0';
	// hook into the main qtexture_t list
	q->next = g_qeglobals.d_qtextures;
	g_qeglobals.d_qtextures = q;
  // push it in the map
  g_qeglobals.d_qtexmap->SetAt( q->name, q );

	return q;
}

int WINAPI QERApp_HasShader( const char *pName )
{
 	//  mickey check the global shader array for existense of pName
 	CShader* pShader = g_Shaders.Shader_ForName( pName );
 	if (pShader)
 		return 1;
	return 0;
}

IShader* WINAPI QERApp_Shader_ForName_NoLoad( const char *pName )
{
	CShader* pShader = g_Shaders.Shader_ForName( pName );
	return pShader;
}

//++timo rename as QERApp_Texture_ForName after getting rid of the old one
qtexture_t* WINAPI QERApp_Texture_ForName2( const char *filename)
{
	qtexture_t* q;
	q = QERApp_Try_Texture_ForName(filename);
	if (q)
		return q;
	// not found? use "texture not found"
	q = QERApp_Try_Texture_ForName( "textures/radiant/notex" );
	if (q)
		return q;
	// still not found?
	return notexture;
}

void CShader::CreateColor(const char* name)
{
	// parse
	sscanf(name, "(%g %g %g)", m_vColor, m_vColor+1, m_vColor+2 ); 
  m_strTextureName = name;
	setName( "color" );
	// create the qtexture_t
	qtexture_t *q1 = QERApp_Texture_ForName2( "textures/radiant/notex" );
	// copy this one
	qtexture_t *q2 = new qtexture_t;
	memcpy( q2, q1, sizeof(qtexture_t) );
	strcpy( q2->name, m_strTextureName.GetBuffer(0) );
	VectorCopy( m_vColor, q2->color );
	m_pTexture = q2;
}

IShader* WINAPI QERApp_ColorShader_ForName( const char* name )
{
	CShader *pShader = new CShader();
	pShader->CreateColor(name);
  // hook it into the shader list
	pShader->IncRef();
	g_Shaders.Add( (LPVOID)pShader );
	return pShader;
}

void CShaderArray::ReleaseForShaderFile( const char *name )
{
	int i;
	// decref
  for(i=0; i<CPtrArray::GetSize(); i++)
  {
    IShader* pShader = static_cast<IShader*>(CPtrArray::GetAt(i));
    if (!strcmp(name, pShader->getShaderFileName()))
    {
      pShader->DecRef();
      CPtrArray::RemoveAt(i);
      i--; // get ready for next loop
    }
  }
}

void WINAPI QERApp_ReloadShaderFile( const char* name )
{
	brush_t *b;
	face_t *f;

#ifdef _DEBUG
	// check the shader name is a reletive path
	// I hacked together a few quick tests to make sure :-)
	if (strstr( name, ":\\" ) || !strstr( name, "scripts" ))
		Sys_Printf("WARNING: is %s a reletive path to a shader file? (QERApp_ReloadShaderFile\n");
#endif

  // in the actives and global shaders lists, decref and unhook the shaders
	//++timo NOTE: maybe we'd like to keep track of the shaders we are unhooking?
	g_ActiveShaders.ReleaseForShaderFile( name );
	g_Shaders.ReleaseForShaderFile( name );
	// go through a reload of the shader file
	QERApp_LoadShaderFile( name );
	// scan all the brushes, replace all the old ones by refs to their new equivalents
	for (b=active_brushes.next ; b != NULL && b != &active_brushes ; b=b->next)
  {
    if (b->patchBrush && !strcmp(b->pPatch->pShader->getShaderFileName(), name))
			SetShader(b->pPatch);
    else
		  for (f=b->brush_faces ; f ; f=f->next)
				if (!strcmp(f->pShader->getShaderFileName(), name))
					SetShader(f);
  }
	for (b=selected_brushes.next ; b != NULL && b != &selected_brushes ; b=b->next)
  {
    if (b->patchBrush && !strcmp(b->pPatch->pShader->getShaderFileName(), name))
			SetShader(b->pPatch);
    else
		  for (f=b->brush_faces ; f ; f=f->next)
				if (!strcmp(f->pShader->getShaderFileName(), name))
					SetShader(f);
  }
	// call Texture_ShowInUse to clean and display only what's required
	Texture_ShowInuse();
//	Texture_SortTextures();
	QERApp_SortActiveShaders();
	Sys_UpdateWindows (W_TEXTURE);
}

void CShaderArray::SetDisplayed(bool b)
{
	int i,count;
	count = CPtrArray::GetSize();
	for(i=0; i<count; i++)
		static_cast<IShader*>(CPtrArray::GetAt(i))->SetDisplayed(b);
}

void CShaderArray::SetInUse(bool b)
{
	int i,count;
	count = CPtrArray::GetSize();
	for(i=0; i<count; i++)
		static_cast<IShader*>(CPtrArray::GetAt(i))->SetInUse(b);
}

// Set the IsDisplayed flag on all active shaders
void WINAPI QERApp_ActiveShaders_SetDisplayed(bool b)
{
	g_ActiveShaders.SetDisplayed(b);
}

void WINAPI QERApp_ActiveShaders_SetInUse(bool b)
{
	g_ActiveShaders.SetInUse(b);
}