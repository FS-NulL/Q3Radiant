//-----------------------------------------------------------------------------
//
// $LogFile$
// $Revision: 1.3 $
// $Author: ttimo $
// $Date: 2000/06/24 20:55:42 $
// $Log: Messaging.h,v $
// Revision 1.3  2000/06/24 20:55:42  ttimo
// build 201
//
// Revision 1.1.2.1  2000/02/04 22:59:34  ttimo
// messaging API preview
//
//
// DESCRIPTION:
// headers for internal classes used in Messaging.cpp
// 

#ifndef __MESSAGING_H_
#define __MESSAGING_H_

class CXYWndWrapper : public IXYWndWrapper
{
public:
	void SnapToGrid( int x1, int y1, vec3_t pt );
};

#endif