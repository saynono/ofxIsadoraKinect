// ===========================================================================
//	CFloatXYZ					(C)2009 Mark F. Coniglio. All rights reserved.
// ===========================================================================

#ifndef _H_CFLOATXYZ
#define _H_CFLOATXYZ

#include "CPlusPlusMacros.h"

// ---------------------------------------------------------------------------
//		CFloatXYZ
// ---------------------------------------------------------------------------
CLASSHDR CFloatXYZ
{
PUBLIC

#ifdef __cplusplus

	CFloatXYZ()
	{
		x = y = z = 0.0f;
	}
	
	CFloatXYZ(
		const CFloatXYZ&		inXYZ)
	{
		x = inXYZ.x;
		y = inXYZ.y;
		z = inXYZ.z;
	}
	
	CFloatXYZ(
		float	inX,
		float	inY,
		float	inZ)
	{
		x = inX;
		y = inY;
		z = inZ;
	}
#endif

	float	x;
	float	y;
	float	z;
};



#endif
