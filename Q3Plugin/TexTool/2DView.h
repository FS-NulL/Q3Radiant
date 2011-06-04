//-----------------------------------------------------------------------------
//
// $LogFile$
// $Revision: 1.2 $
// $Author: ttimo $
// $Date: 2000/03/16 03:18:00 $
// $Log: 2DView.h,v $
// Revision 1.2  2000/03/16 03:18:00  ttimo
// Released version 03 16 2000
//
// Revision 1.1.1.1  2000/01/07 17:15:29  ttimo
// initial import of Q3Plugin module
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
// a class to provide basic services for 2D view of a world
// window <-> local 2D space transforms
// snap to grid
// TODO: this one could be placed under an interface, and provided to the editor as a service

#ifndef _2DVIEW_H_
#define _2DVIEW_H_

class C2DView
{
	enum		E2DViewState { View_Idle, View_Move } ViewState;
	int			m_xPosMove, m_yPosMove;
	float		m_MinsMove[2], m_MaxsMove[2];
	qboolean	m_bDoGrid;
	float		m_GridStep[2];
  qboolean m_bPopup;
public:
	RECT m_rect;
	float m_Mins[2],m_Maxs[2],m_Center[2];
	C2DView() { 
		ViewState = View_Idle; 
		m_bDoGrid = false;
    m_bPopup = false;
	}
	~C2DView() { }
	void SetGrid( float xGridStep, float yGridStep )
	{	m_bDoGrid = true; m_GridStep[0] = xGridStep; m_GridStep[1] = yGridStep;	}
	// get window coordinates for space coordinates
	void WindowForSpace( int &x, int &y, const float c[2]);
	void SpaceForWindow( float c[2], int x, int y);
	void GridForWindow( float c[2], int x, int y);
	qboolean DoesSelect( int x, int y, float c[2] );
	void PreparePaint( const _QERQglTable & QglTable );
	// this one must be called in the DialogProc loop of the window
  // it also handles drop-down menu (WM_RBUTTONDOWN + WM_RBUTTONUP)
	BOOL DialogProc(  HWND hwndDlg,  // handle to dialog box
					  UINT uMsg,     // message
					  WPARAM wParam, // first message parameter
					  LPARAM lParam  // second message parameter
					  );
	void ZoomIn();
  void ZoomOut();
};

#endif