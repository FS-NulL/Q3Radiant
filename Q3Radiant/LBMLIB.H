// piclib.h


void LoadLBM (char *filename, byte **picture, byte **palette);
void WriteLBMfile (char *filename, byte *data, int width, int height
	, byte *palette);
void LoadPCX (char *filename, byte **picture, byte **palette, int *width, int *height);
void WritePCXfile (char *filename, byte *data, int width, int height
	, byte *palette);

// loads / saves either lbm or pcx, depending on extension
void Load256Image (char *name, byte **pixels, byte **palette,
				   int *width, int *height);
void Save256Image (char *name, byte *pixels, byte *palette,
				   int width, int height);


void LoadTGA (char *filename, byte **pixels, int *width, int *height);
// LoadImage will rely on file extension to call LoadTGA LoadPCX32 LoadBMP LoadJPG
void LoadImage( const char *name, byte **pic, int *width, int *height );
