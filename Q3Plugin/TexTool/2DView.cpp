//-----------------------------------------------------------------------------
//
// $LogFile$
// $Revision: 1.2 $
// $Author: ttimo $
// $Date: 2000/03/16 03:18:00 $
// $Log: 2DView.cpp,v $
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
// TODO: this one can be placed under an interface, and provided to the editor as a service

#include "stdafx.h"

void C2DView::PreparePaint( const _QERQglTable & QglTable )
{
	QglTable.m_pfn_qglClearColor( 0, 0, 0, 0 );
	QglTable.m_pfn_qglViewport( 0, 0, m_rect.right, m_rect.bottom );
	QglTable.m_pfn_qglMatrixMode( GL_PROJECTION );
	QglTable.m_pfn_qglLoadIdentity();
	QglTable.m_pfn_qglOrtho( m_Mins[0], m_Maxs[0], m_Maxs[1], m_Mins[1], -1, 1 );	
}

void C2DView::SpaceForWindow( float c[2], int x, int y)
{
	c[0] = ((float)(x))/((float)(m_rect.right-m_rect.left))*(m_Maxs[0]-m_Mins[0])+m_Mins[0];
	c[1] = ((float)(y))/((float)(m_rect.bottom-m_rect.top))*(m_Maxs[1]-m_Mins[1])+m_Mins[1];
}

void C2DView::GridForWindow( float c[2], int x, int y)
{
	SpaceForWindow( c, x, y );
	if ( !m_bDoGrid )
		return;
	c[0] /= m_GridStep[0];
	c[1] /= m_GridStep[1];
	c[0] = (float)floor( c[0] + 0.5f );
	c[1] = (float)floor( c[1] + 0.5f );
	c[0] *= m_GridStep[0];
	c[1] *= m_GridStep[1];
}

void C2DView::WindowForSpace( int &x, int &y, const float c[2] )
{
	x = m_rect.left + (int)( ((float)(m_rect.right-m_rect.left))*(c[0]-m_Mins[0])/(m_Maxs[0]-m_Mins[0]) );
	y = m_rect.top + (int)( ((float)(m_rect.bottom-m_rect.top))*(c[1]-m_Mins[1])/(m_Maxs[1]-m_Mins[1]) );
}

qboolean C2DView::DoesSelect( int x, int y, float c[2] )
{
	int xc,yc;
	WindowForSpace( xc, yc, c );
	if ( abs(xc-x)<=3 && abs(yc-y)<=3 )
		return true;
	return false;
}

void C2DView::ZoomIn()
{
  m_Mins[0] = 0.5f * ( m_Mins[0] - m_Center[0] ) + m_Center[0];
  m_Mins[1] = 0.5f * ( m_Mins[1] - m_Center[1] ) + m_Center[1];
  m_Maxs[0] = 0.5f * ( m_Maxs[0] - m_Center[0] ) + m_Center[0];
  m_Maxs[1] = 0.5f * ( m_Maxs[1] - m_Center[1] ) + m_Center[1];
  // redraw
  SendMessage( g_hToolWnd, WM_PAINT, 0, 0 );
}

void C2DView::ZoomOut()
{
  m_Mins[0] = 2.0f * ( m_Mins[0] - m_Center[0] ) + m_Center[0];
  m_Mins[1] = 2.0f * ( m_Mins[1] - m_Center[1] ) + m_Center[1];
  m_Maxs[0] = 2.0f * ( m_Maxs[0] - m_Center[0] ) + m_Center[0];
  m_Maxs[1] = 2.0f * ( m_Maxs[1] - m_Center[1] ) + m_Center[1];
  // redraw
  SendMessage( g_hToolWnd, WM_PAINT, 0, 0 );
}

BOOL C2DView::DialogProc(  HWND hwndDlg,  // handle to dialog box
					  UINT uMsg,     // message
					  WPARAM wParam, // first message parameter
					  LPARAM lParam  // second message parameter
					  )
{
	switch (ViewState)
	{
	case View_Idle:
		switch (uMsg)
		{
		case WM_RBUTTONDOWN:
			m_xPosMove = LOWORD(lParam); // horizontal position of cursor
			m_yPosMove = HIWORD(lParam); // vertical position of cursor
			// store
			m_MinsMove[0] = m_Mins[0]; m_MinsMove[1] = m_Mins[1];
			m_MaxsMove[0] = m_Maxs[0]; m_MaxsMove[1] = m_Maxs[1];
			ViewState = View_Move;
      // set popup to true
      m_bPopup = true;
			return TRUE;
			break;
		case WM_KEYDOWN:
			switch ((int) wParam)
			{
			case VK_INSERT:
        ZoomOut();
				break;
			case VK_DELETE:
        ZoomIn();
				break;
			}
			break;
		}
		break;
	case View_Move:
		switch (uMsg)
		{
		case WM_MOUSEMOVE:
			int xPos,yPos;
			float V[2];
			xPos = LOWORD(lParam);  // horizontal position of cursor
			yPos = HIWORD(lParam);  // vertical position of cursor
			// V is the offset
			V[0] = ((float)( xPos - m_xPosMove )) * ( m_MaxsMove[0] - m_MinsMove[0] ) / ((float)( m_rect.left - m_rect.right ));
			V[1] = ((float)( yPos - m_yPosMove )) * ( m_MaxsMove[1] - m_MinsMove[1] ) / ((float)( m_rect.top - m_rect.bottom ));
			// update m_Mins m_Maxs and m_Center
			m_Mins[0] = m_MinsMove[0] + V[0];
			m_Mins[1] = m_MinsMove[1] + V[1];
			m_Maxs[0] = m_MaxsMove[0] + V[0];
			m_Maxs[1] = m_MaxsMove[1] + V[1];
			m_Center[0] = 0.5f * ( m_Mins[0] + m_Maxs[0] );
			m_Center[1] = 0.5f * ( m_Mins[1] + m_Maxs[1] );
      // no popup menu if we moved
      m_bPopup = false;
			// send a repaint message
			SendMessage( hwndDlg, WM_PAINT, 0, 0 );
			return TRUE;
			break;
		case WM_RBUTTONUP:
      // maybe it's time for popup menu
      if (m_bPopup)
      {
        HMENU menu = LoadMenu( g_hModule, MAKEINTRESOURCE(IDR_DROP_MENU) );
        HMENU popup=GetSubMenu(menu,0);
        POINT p;
        p.x = LOWORD(lParam);
        p.y = HIWORD(lParam);
        ClientToScreen( hwndDlg, &p );
        int ret = TrackPopupMenu( popup, TPM_LEFTALIGN | TPM_RETURNCMD, p.x, p.y, 0, hwndDlg, NULL );
        switch (ret)
        {
          case ID_DROP_VALIDATE:
            Textool_Validate();
            break;
          case ID_DROP_ZOOMIN:
            ZoomIn();
            break;
          case ID_DROP_ZOOMOUT:
            ZoomOut();
            break;
          case ID_DROP_CANCEL:
            Textool_Cancel();
            break;
        }
      }
			// back to Idle mode
			ViewState = View_Idle;
			return TRUE;
			break;
		}
		break;
	}
	return FALSE;
}
