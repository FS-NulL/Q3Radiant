
//--typedef struct texdef_s
//--{
//--	char	name[32];
//--	float	shift[2];
//--	float	rotate;
//--	float	scale[2];
//--	int		contents;
//--	int		flags;
//--	int		value;
//--} texdef_t;
//--
//--typedef struct
//--{
//--	int			width, height;
//--	int			originy;
//--	texdef_t	texdef;
//--  int m_nTotalHeight;
//--} texturewin_t;
//--
//--#define QER_TRANS     0x00000001
//--#define QER_NOCARVE   0x00000002
//--
//--typedef struct qtexture_s
//--{
//--	struct	qtexture_s *next;
//--	char	name[64];		// includes partial directory and extension
//--  int		width,  height;
//--	int		contents;
//--	int		flags;
//--	int		value;
//--	int		texture_number;	// gl bind number
//--  
//--  char  shadername[1024]; // old shader stuff
//--  qboolean bFromShader;   // created from a shader
//--  float fTrans;           // amount of transparency
//--  int   nShaderFlags;     // qer_ shader flags
//--	vec3_t	color;			    // for flat shade mode
//--	qboolean	inuse;		    // true = is present on the level
//--} qtexture_t;
//--

// a texturename of the form (0 0 0) will
// create a solid color texture

//void	Texture_Init (bool bHardInit = true);
void Texture_Init();
//void	Texture_FlushUnused ();
//void	Texture_Flush (bool bReload = false);
void	Texture_ShowDirectory (int menunum, bool bLinked = false);
void	Texture_ShowAll();
void	Texture_ShowInuse();
//void	Texture_Cleanup(CStringList *pList = NULL);

// Timo
// added an optional IPluginTexdef when one is available
// we need a forward declaration, this is crap
class IPluginTexdef;
//++timo clean
void	Texture_SetTexture2 (IShader *pShader, texdef_t *texdef, brushprimit_texdef_t *brushprimit_texdef, bool bFitScale = false, IPluginTexdef *pTexdef = NULL, bool bSetSelection = true);
void	Texture_SetTexture (texdef_t *texdef, brushprimit_texdef_t *brushprimit_texdef, bool bFitScale = false, IPluginTexdef *pTexdef = NULL, bool bSetSelection = true);

void	Texture_SetMode(int iMenu);	// GL_TEXTURE_NEAREST, etc..
void Texture_ResetPosition();

void LoadShaders();
int  WINAPI Texture_LoadSkin(char *pName, int *pnWidth, int *pnHeight);
void Texture_LoadFromPlugIn(LPVOID vp);
void Texture_StartPos (void);
IShader* Texture_NextPos (int *x, int *y);
