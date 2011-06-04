// parse.h -- text file parsing routines

#define	MAXTOKEN	1024

extern	char	token[MAXTOKEN];
extern	int		scriptline;

// NOTE: added WINAPI call syntax to export these for plugins in _QERScripLibTable
void	StartTokenParsing (char *data);
qboolean WINAPI GetToken (qboolean crossline);
void WINAPI UngetToken (void);
qboolean TokenAvailable (void);

