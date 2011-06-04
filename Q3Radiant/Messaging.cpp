//-----------------------------------------------------------------------------
//
// $LogFile$
// $Revision: 1.3 $
// $Author: ttimo $
// $Date: 2000/06/24 20:55:42 $
// $Log: Messaging.cpp,v $
// Revision 1.3  2000/06/24 20:55:42  ttimo
// build 201
//
// Revision 1.1.2.1  2000/02/04 22:59:34  ttimo
// messaging API preview
//
//
// DESCRIPTION:
// implementation of IMessaging specific interface
// 

#include "stdafx.h"

CPtrArray l_Listeners[RADIANT_MSGCOUNT];
CPtrArray l_WindowListeners;
CXYWndWrapper l_XYWndWrapper;

void WINAPI QERApp_HookWindow(IWindowListener* pListen)
{
	l_WindowListeners.Add( pListen );
	pListen->IncRef();
}

void WINAPI QERApp_UnHookWindow(IWindowListener* pListen)
{
	for ( int i = 0; i < l_WindowListeners.GetSize(); i++ )
	{
		if (l_WindowListeners.GetAt(i) == pListen)
		{
			l_WindowListeners.RemoveAt(i);
			pListen->DecRef();
			return;
		}
	}
#ifdef _DEBUG
	Sys_Printf("WARNING: IWindowListener not found in QERApp_UnHookWindow\n");
#endif
}

void DispatchOnMouseMove(UINT nFlags, int x, int y)
{
	for( int i = 0; i < l_WindowListeners.GetSize(); i++ )
		static_cast<IWindowListener*>(l_WindowListeners.GetAt(i))->OnMouseMove( nFlags, x, y );
}

bool DispatchOnLButtonDown(UINT nFlags, int x, int y)
{
	for( int i = 0; i < l_WindowListeners.GetSize(); i++ )
		if (static_cast<IWindowListener*>(l_WindowListeners.GetAt(i))->OnLButtonDown( nFlags, x, y ))
			return true;
	return false;
}

bool DispatchOnLButtonUp(UINT nFlags, int x, int y)
{
	for( int i = 0; i < l_WindowListeners.GetSize(); i++ )
		if (static_cast<IWindowListener*>(l_WindowListeners.GetAt(i))->OnLButtonUp( nFlags, x, y ))
			return true;
	return false;
}

void WINAPI QERApp_HookListener(IListener* pListen, int Msg)
{
#ifdef _DEBUG
	if (Msg >= RADIANT_MSGCOUNT)
	{
		Sys_Printf("ERROR: bad index in QERApp_HookListener\n");
		return;
	}
#endif
	l_Listeners[Msg].Add( pListen );
	pListen->IncRef();
}

int  WINAPI QERApp_UnHookListener(IListener* pListen)
{
	int count = 0;
	for( int i = 0; i<RADIANT_MSGCOUNT; i++ )
		for( int j = 0; j<l_Listeners[i].GetSize(); j++ )
			if (l_Listeners[i].GetAt(j) == pListen)
			{
				l_Listeners[i].RemoveAt(j);
				pListen->DecRef();
				count++;
			}
	return count;
}

void DispatchRadiantMsg( int Msg )
{
#ifdef _DEBUG
	if (Msg >= RADIANT_MSGCOUNT)
	{
		Sys_Printf("ERROR: bad index in DispatchRadiantMsg\n");
		return;
	}
#endif
	for(int i = 0; i<l_Listeners[Msg].GetSize(); i++)
		static_cast<IListener *>(l_Listeners[Msg].GetAt(i))->DispatchRadiantMsg(Msg);
}

void CXYWndWrapper::SnapToGrid( int x1, int y1, vec3_t pt )
{
	CRect rctZ;
	g_pParentWnd->ActiveXY()->GetClientRect( rctZ );
	g_pParentWnd->ActiveXY()->SnapToPoint( x1, rctZ.Height() - 1 - y1, pt );
}

IXYWndWrapper* WINAPI QERApp_GetXYWndWrapper()
{
	return &l_XYWndWrapper;
}
