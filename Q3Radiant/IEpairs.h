//-----------------------------------------------------------------------------
//
// $LogFile$
// $Revision: 1.1.1.4 $
// $Author: ttimo $
// $Date: 2000/01/18 00:17:12 $
// $Log: IEpairs.h,v $
// Revision 1.1.1.4  2000/01/18 00:17:12  ttimo
// merging in for RC
//
// Revision 1.3  2000/01/17 23:53:42  TBesset
// ready for merge in sourceforge (RC candidate)
//
// Revision 1.2  2000/01/07 16:40:10  TBesset
// merged from BSP frontend
//
// Revision 1.1.1.3  1999/12/29 18:31:26  TBesset
// Q3Radiant public version
//
// Revision 1.1.1.1.2.1  1999/12/29 21:39:35  TBesset
// updated to update3 from Robert
//
// Revision 1.1.1.3  1999/12/29 18:31:26  TBesset
// Q3Radiant public version
//
// Revision 1.1.1.3  1999/12/29 18:31:26  TBesset
// Q3Radiant public version
//
//
// Revision 1.2  1999/11/22 17:46:45  Timo & Christine
// merged EARadiant into the main tree
// bug fixes for Q3Plugin / EAPlugin
// export for Robert
//
// Revision 1.1.2.1  1999/11/03 20:37:59  Timo & Christine
// MEAN plugin for Q3Radiant, alpha version
//
//
// DESCRIPTION:
// virtual class to allow plugin operations on entities epairs
//

#ifndef _IEPAIRS_H_
#define _IEPAIRS_H_

class IEpair
{
public:
	// Increment the number of references to this object
	virtual void IncRef () = 0;
	// Decrement the reference count
	virtual void DecRef () = 0;
	virtual void GetVectorForKey( char* key, vec3_t vec ) = 0;
	virtual float FloatForKey( char *key ) = 0;
	virtual char* ValueForKey( char *key ) = 0;
	virtual void SetKeyValue( char *key, char *value ) = 0;
	virtual void GetEntityOrigin( vec3_t vec ) = 0;
	// compute the rotated bounds of the BBox based on "angle" and "angles" keys
	virtual void CalculateRotatedBounds( vec3_t mins, vec3_t maxs ) = 0;
};

#endif 