// ===========================================================================
//	ColorRefs				(C)2002 Mark F. Coniglio. All rights reserved.
// ===========================================================================
//
// This file contains handy macros for converting between ColorRefs (consisting
// of four bytes of color information, MS Byte to LSB: left to right, alpha, red,
// green, blue) to Mac RGBColor structs, as well as macros for converting to and 
// from ColorRefs to the float colors used in Isadora.

#ifndef _H_ColorRefs
#define _H_ColorRefs

#include "VidFormats.h"

typedef	UInt32		ColorRef;

// PIXEL ORDER MSB -> LSB = ARGB
#if kGWorldPixelComponentOrder_ARGB == 1

	#define	RGBToColorRef(r, g, b)  	( (((UInt32) ((UInt8)(r))) << 16) | (((UInt32) ((UInt8)(g))) << 8) | (((UInt32)(b))) )
	#define	ARGBToColorRef(a, r, g, b)  ( (((UInt32) ((UInt8)(a))) << 24) | (((UInt32) ((UInt8)(r))) << 16) | (((UInt32) ((UInt8)(g))) << 8) | (((UInt32)(b))) )
	#define	RGBColorToColorRef(rgb) 	( ((((UInt32)(rgb.red)) << 8) & 0x00FF0000) | ((((UInt32)(rgb.green)) << 0) & 0x0000FF00) | ((((UInt32)(rgb.blue)) >> 8) & 0x000000FF) )

	#define	FloatToColorRef(inFloat)	( ((UInt32)(( (inFloat) * (double) 0x00FFFFFF) / 100.0)) << 0)
	#define	ColorRefToFloat(inColorRef)	( ( ((double) ((UInt32)(inColorRef) >> 0)) * 100.0) / (double) 0x00FFFFFF)

	#define ColorRefToRGBColor(inColorRef, outRGB)	(	((outRGB).red		= ((((inColorRef) >> 8) & 0x0000FF00) | (((inColorRef) >> 16) & 0x000000FF))),	\
														((outRGB).green		= ((((inColorRef) >> 0) & 0x0000FF00) | (((inColorRef) >> 8) & 0x000000FF))),		\
														((outRGB).blue		= ((((inColorRef) << 8) & 0x0000FF00) | (((inColorRef) >> 0) & 0x000000FF))) )
	
// PIXEL ORDER MSB -> LSB = BGRA
#else

	#define	RGBToColorRef(r, g, b)  	( (((UInt32) ((UInt8)(b))) << 24) | (((UInt32) ((UInt8)(g))) << 16) | (((UInt32)(r)) << 8) )
	#define	ARGBToColorRef(a, r, g, b)  ( (((UInt32) ((UInt8)(b))) << 24) | (((UInt32) ((UInt8)(g))) << 16) | (((UInt32)(r)) << 8) | (((UInt32)(a)) << 0) )
	#define	RGBColorToColorRef(rgb)		(((((UInt32)(rgb.blue)) << 16) & 0xFF000000) | ((((UInt32)(rgb.green)) << 8) & 0x00FF0000) | ((((UInt32)(rgb.red)) >> 0) & 0x0000FF00))

	#define	FloatToColorRef(inFloat)	( ((UInt32)(( (inFloat) * (double) 0x00FFFFFF) / 100.0)) << 8)
	#define	ColorRefToFloat(inColorRef)	( (((double) ((UInt32)(inColorRef) >> 8)) * 100.0) / (double) 0x00FFFFFF)

	#define ColorRefToRGBColor(inColorRef, outRGB)	(	((outRGB).blue		= ((((inColorRef) >> 8) & 0x0000FF00) | (((inColorRef) >> 16) & 0x000000FF))),	\
														((outRGB).green		= ((((inColorRef) >> 0) & 0x0000FF00) | (((inColorRef) >> 8) & 0x000000FF))),		\
														((outRGB).red		= ((((inColorRef) << 8) & 0x0000FF00) | (((inColorRef) >> 0) & 0x000000FF))) )
	
#endif

#endif
