//-----------------------------------------------------------------------------
//
// $LogFile$
// $Revision: 1.3 $
// $Author: ttimo $
// $Date: 2000/06/24 20:55:42 $
// $Log: IShaders.h,v $
// Revision 1.3  2000/06/24 20:55:42  ttimo
// build 201
//
// Revision 1.1.2.2.2.16  2000/06/11 00:17:50  ttimo
// more fixes
//
// Revision 1.1.2.2.2.15  2000/05/22 03:40:50  ttimo
// 199RC1
//
// Revision 1.1.2.2.2.14  2000/05/10 05:07:58  ttimo
// texture browser is better
//
// Revision 1.1.2.2.2.13  2000/05/09 22:27:01  ttimo
// working on texture browser
//
// Revision 1.1.2.2.2.12  2000/04/29 03:25:55  ttimo
// 198 beta 3
//
// Revision 1.1.2.2.2.11  2000/04/25 18:40:30  ttimo
// better alpha channel support
//
// Revision 1.1.2.2.2.10  2000/04/24 22:34:13  ttimo
// no message
//
// Revision 1.1.2.2.2.9  2000/04/24 05:00:25  ttimo
// no message
//
// Revision 1.1.2.2.2.8  2000/04/18 06:24:23  ttimo
// no message
//
// Revision 1.1.2.2.2.7  2000/04/13 05:00:22  ttimo
// crashes, texture fixes on the way!
//
// Revision 1.1.2.2.2.6  2000/04/10 22:51:40  ttimo
// 198beta ( released version, fixed stuff )
//
// Revision 1.1.2.2.2.5  2000/04/08 22:26:11  ttimo
// nearing beta
//
// Revision 1.1.2.2.2.4  2000/03/23 23:41:46  ttimo
// nearly ready for merge with Release197
//
// Revision 1.1.2.2.2.3  2000/02/22 00:30:44  ttimo
// starting to look good
//
// Revision 1.1.2.2.2.2  2000/02/16 14:44:14  ttimo
// CShader loading .. the IShader API is getting shape
//
// Revision 1.1.2.2.2.1  2000/02/13 22:05:14  ttimo
// no message
//
// Revision 1.1.2.2  2000/02/11 03:52:30  ttimo
// working on the IShader interface
//
// Revision 1.1.2.1  2000/02/10 03:00:20  ttimo
// adding IShaders.h
//
//
// DESCRIPTION:
// a set of functions to manipulate textures in Radiant
// 

#ifndef __ISHADERS_H_
#define __ISHADERS_H_

// define a GUID for this interface so plugins can access and reference it
// {D42F798A-DF57-11d3-A3EE-0004AC96D4C3}
static const GUID QERShadersTable_GUID = 
{ 0xd42f798a, 0xdf57, 0x11d3, { 0xa3, 0xee, 0x0, 0x4, 0xac, 0x96, 0xd4, 0xc3 } };

// NOTES ABOUT SYNTAX:
// if a function starts by 'Try' it means that if the requested thing could not be found / loaded it will return nothing / NULL
// otherwise a default object will be created
// the _QERShadersTable is also used by shader code inside Radiant. but for speed and "keep it simple" consideration you
// can get the static equivalent of the func pointers by adding 'QERApp_' (access to _QERShadersTable is better thought ..
// see the note to move all the shader language out of Radiant below)

class IShader
{
public:
	// Increment the number of references to this object
	virtual void IncRef () = 0;
	// Decrement the reference count
	virtual void DecRef () = 0;
	// get/set the qtexture_t* Radiant uses to represent this shader object
	virtual qtexture_t* getTexture() const = 0;
	virtual void setTexture(qtexture_t *pTex) = 0;
	// get shader name
	virtual const char* getName() const = 0;
	// is this shader in use?
	// NOTE: this flag can mean this shader has been in use at least once since the last rescan of in-use stuff
  // (rescan of in-use happens in several cases, user command or during a texture directory load)
  // NOTE: this is used to draw the green outline in the texture window
	// NOTE: when does Radiant set the InUse flag? Whenever Select_SetTexture is called (well that doesn't necessarily means the texture actually gets in use, but that's close enough)
	virtual bool IsInUse() const = 0;
	virtual void SetInUse(bool) = 0;
  // is this shader displayed in the texture browser?
  // NOTE: if IsInUse() == true, the shader will always be displayed in the texture window and this flag ingored
  virtual bool IsDisplayed() const = 0;
  virtual void SetDisplayed(bool) = 0;
  // get the editor flags (QER_NOCARVE QER_TRANS)
  virtual int getFlags() = 0;
  // get the transparency value
  virtual float getTrans() = 0;
	// test if it's a true shader, or a default shader created to wrap around a texture
	virtual bool IsDefault() = 0;
	// test if it's a plain color shader, i.e. a shader we use on plain color stuff (like info_playerstart)
	virtual bool IsColor() = 0;
	// get the related color then!
	virtual void getColor(vec3_t v) = 0;
  // get shader file name (ie the file where this one is defined)
  virtual const char* getShaderFileName() const = 0;
};

// NOTE: how to move all the shader language out of Radiant in a plugin?
// -> change this _QERShadersTable into an IShadersManager
// -> let the plugin create an instance of IShadersManager
// -> make sure Radiant uses this IShadersManager to load / query the shaders

// NOTE: shader and texture names used must be full path, ie. most often with "textures/" prefix
// (since shaders are defined in .shader files with textures/)

// free all shaders
// free the shaders, will not free the qtexture_t*
typedef void		(WINAPI* PFN_FREESHADERS)		();
// reload all the shaders
// this will free everything (shaders and their textures), then reload all in use stuff
typedef void		(WINAPI* PFN_RELOADSHADERS)		();
// load all shaders in a given directory
// this will scan the list of in-memory shaders, and load the related qtexture_t if needed
typedef void		(WINAPI* PFN_LOADSHADERSFROMDIR)(const char* path);
// load a shader file (ie a set of shaders)
// after LoadShaderFile shaders will be in memory, next step is to load the qtexture_t Radiant uses to represent them
// if a shader with the same name exists, new one will not be loaded - don't use this to refresh the shaders!
typedef void		(WINAPI* PFN_LOADSHADERFILE)	(const char* filename);
// tell if a given shader exists in our shader table
// NOTE: this doesn't tell wether it's corresponding qtexture is loaded 
typedef int			(WINAPI* PFN_HASSHADER)			(const char* name);
// return the shader for a given name
// if the qtexture is not already in memory, will try loading it
// if the qtexture could not be found, will use default
// will return NULL on shader not found
typedef IShader*	(WINAPI* PFN_TRYSHADERFORNAME)	(const char* name);
// return the shader for a given name
// if the qtexture is not already in memory, will try loading it
// will create a default shader if not found (will use a default texture)
typedef IShader*	(WINAPI* PFN_SHADERFORNAME)		(const char* name);
// query / load a texture
// will not try loading a shader, will look for the actual image file ..
// returns NULL on file not found
// NOTE: strategy for file lookup:
//   paths must be relative, ie. textures/me/myfile
//   if a 3-letters filename extension (such as .jpg or .tga) is provided, it will get loaded first
//   if not found or no extension, will try loading after adding .tga and .jpg (in this order)
typedef qtexture_t* (WINAPI* PFN_TRYTEXTUREFORNAME) (const char* filename);
// query / load a texture
// will not try loading a shader, will look for the actual image file ..
// on file not found will use the "texture not found"
typedef qtexture_t* (WINAPI* PFN_TEXTUREFORNAME)	(const char* filename);
// get the number of active shaders
// these are the shaders currently loaded, that have an associated qtexture_t*
typedef int			(WINAPI* PFN_GETACTIVESHADERCOUNT)	();
// for stuff that needs to be represented by a plain texture
// the shader will get a "color" name, use GetColor to get the actual color
typedef IShader*	(WINAPI* PFN_COLORSHADERFORNAME)	(const char* name);
// reload a shaderfile - update shaders and their display properties/qtexture_t if needed
// will not reload the texture files
// will switch to "show in use" atfer use
// filename must be reletive path of the shader, ex. scripts/gothic_wall.shader
typedef void		(WINAPI* PFN_RELOADSHADERFILE)(const char* filename);
// retrieve a shader if exists, without loading the textures for it etc.
// use this function if you want special info on a shader
typedef IShader* (WINAPI* PFN_SHADERFORNAMENOLOAD) (const char* name);

struct _QERShadersTable
{
	int		m_nSize;
	PFN_FREESHADERS			              m_pfnFreeShaders;
	PFN_RELOADSHADERS		              m_pfnReloadShaders;
	PFN_LOADSHADERFILE		            m_pfnLoadShaderFile;
	PFN_RELOADSHADERFILE              m_pfnReloadShaderFile;
	PFN_HASSHADER			                m_pfnHasShader;
	PFN_TRYSHADERFORNAME	            m_pfnTry_Shader_ForName;
	PFN_SHADERFORNAME		              m_pfnShader_ForName;
	PFN_TRYTEXTUREFORNAME	            m_pfnTry_Texture_ForName;
	PFN_TEXTUREFORNAME		            m_pfnTexture_ForName;
	PFN_GETACTIVESHADERCOUNT			    m_pfnGetActiveShaderCount;
	PFN_COLORSHADERFORNAME						m_pfnColorShader_ForName;
	PFN_SHADERFORNAMENOLOAD						m_pfnShader_ForName_NoLoad;
};

#endif