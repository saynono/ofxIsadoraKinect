// ===========================================================================
//	CFLoatXY					 (C)2009 Mark F. Coniglio. All rights reserved.
// ===========================================================================

#ifndef _H_CFLOATXY
#define _H_CFLOATXY

#include "CPlusPlusMacros.h"
#include "CFloatXYZ.h"

#include <math.h>
// ---------------------------------------------------------------------------
//		CFloatXY
// ---------------------------------------------------------------------------
CLASSHDR CFloatXY
{
PUBLIC

#ifdef __cplusplus

				CFloatXY()
				{
					x = y = 0.0f;
				}
				
				CFloatXY(
					const CFloatXYZ&		inXYZ)
				{
					x = inXYZ.x;
					y = inXYZ.y;
				}
	
				CFloatXY(
					float	inX,
					float	inY)
				{
					x = inX;
					y = inY;
				}
				
	CFloatXY	operator +(
					float inValue)
				{
					return CFloatXY(x + inValue, y + inValue);
				}
				
	
	CFloatXY	operator +(
					const CFloatXY&	inValue)
				{
					return CFloatXY(x + inValue.x, y + inValue.y);
				}
				
	
	CFloatXY	operator +=(
					float inValue)
				{
					x += inValue;
					y += inValue;
					return (*this);
				}
				
	
	CFloatXY	operator +=(
					const CFloatXY&	inValue)
				{
					x += inValue.x;
					y += inValue.y;
					return (*this);
				}
				
	
	CFloatXY	operator -(
					float inValue)
				{
					return CFloatXY(x + inValue, y + inValue);
				}
				
	
	CFloatXY	operator -(
					const CFloatXY&	inValue)
				{
					return CFloatXY(x - inValue.x, y - inValue.y);
				}
				
	
	CFloatXY	operator -=(
					float inValue)
				{
					x -= inValue;
					y -= inValue;
					return (*this);
				}
				
	
	CFloatXY	operator -=(
					const CFloatXY&	inValue)
				{
					x -= inValue.x;
					y -= inValue.y;
					return (*this);
				}
				
	
	CFloatXY	operator *(
					float inValue)
				{
					return CFloatXY(x * inValue, y * inValue);
				}
				
	CFloatXY	operator *(
					const CFloatXY&	inValue)
				{
					return CFloatXY(x * inValue.x, y * inValue.y);
				}
				
	CFloatXY&	operator *=(
					float inValue)
				{
					x *= inValue;
					y *= inValue;
					return (*this);
				}
				
	CFloatXY&	operator *=(
					const CFloatXY&	inValue)
				{
					x *= inValue.x;
					y *= inValue.y;
					return (*this);
				}
				
	CFloatXY	operator /(
					float inValue)
				{
					return CFloatXY(x / inValue, y / inValue);
				}
				
	CFloatXY	operator /(
					const CFloatXY&	inValue)
				{
					return CFloatXY(x / inValue.x, y / inValue.y);
				}
				
	CFloatXY&	operator /=(
					float inValue)
				{
					x /= inValue;
					y /= inValue;
					return (*this);
				}
				
	CFloatXY&	operator /=(
					const CFloatXY&	inValue)
				{
					x /= inValue.x;
					y /= inValue.y;
					return (*this);
				}
				
	float		Distance(
					const CFloatXY&	inValue)
				{
					float deltaX = x - inValue.x;
					float deltaY = y - inValue.y;
					return sqrt(deltaX * deltaX + deltaY * deltaY);
				}
				
	inline bool	operator ==(
					const CFloatXY&	inValue)
				{
					return x == inValue.x && y == inValue.y;
				}
	
	inline bool	operator !=(
					const CFloatXY&	inValue)
				{
					return x != inValue.x || y != inValue.y;
				}
#endif

	float	x;
	float	y;
};



#endif
