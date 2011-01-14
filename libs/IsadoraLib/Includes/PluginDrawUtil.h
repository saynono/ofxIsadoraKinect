// =================================================================================
//	PluginDrawUtil				  ©2002 Mark F. Coniglio -- All Rights Reserved.
// =================================================================================
//
// Handy functions to help plugins with drawing and color manipulation

#ifndef _H_PluginDrawUtil
#define _H_PluginDrawUtil

#include "3DDrawFlags.h"
#include "ColorRefs.h"
#include "VidColorMacros.h"

// ----------------------------------------------------------------------
// DEFINES

// return the min of three values
#define MIN3(a,b,c) ((a)<=(b) ? (a)<=(c)?(a):(c) : (b)<=(c)?(b):(c) )

// return the max of three values
#define MAX3(a,b,c) ((a)>=(b) ? (a)>=(c)?(a):(c) : (b)>=(c)?(b):(c) )


// ----------------------------------------------------------------------
// FUNCTIONS

enum {
	kIzzyColorGray,
	kIzzyColorRed,
	kIzzyColorOrange,
	kIzzyColorYellow,
	kIzzyColorGreen,
	kIzzyColorAqua,
	kIzzyColorBlue,
	kIzzyColorPurple
};

typedef	SInt32	IzzyColorIndex;

ColorRef
IsadoraColorIndexToColorRef(
	IzzyColorIndex	inColorIndex);
	
void
RGBtoHLS(
	const UInt32 	rgb, 
	double*			H,
	double*			L,
	double*			S);

ColorRef
HLStoRGB(
	const double	H,
	const double	L,
	const double	S);
	
void
RGBtoHSV (
	double			r,
	double			g,
	double			b,
	double*			H,
	double*			S,
	double*			V);

void
HSVtoRGB (
	double			H,
	double			S,
	double			V,
	double*			r,
	double*			g,
	double*			b);
	
// ---------------------------------------------------------------------------------
//		¥ MixPixels32
// ---------------------------------------------------------------------------------
// inline function mixes two ARGB pixels.
// inMinValue should be between 0 and inMixMax
// lower values of inMixValue show more of inPixel1
// higher values of inMixValue show more of inPixel2

inline UInt32
MixPixels32(
	UInt32	inPixel1,		// 32 bit pixel in format 00RRGGBB
	UInt32	inPixel2,		// 32 bit pixel in format 00RRGGBB
	SInt32	inMixValue,		// higher values show more of pixel1, lower values show more of pixel 2
	SInt32	inMixMax)		// maximum possible value of inMixValue	
{
	SInt32 c1, c2;
	
	c1 = ALPHA_(inPixel1);
	c2 = ALPHA_(inPixel2);
	UInt32 alp = c2 + ((c1-c2) * inMixValue)/inMixMax;
	
	c1 = RED_(inPixel1);
	c2 = RED_(inPixel2);
	UInt32 red = c2 + ((c1-c2) * inMixValue)/inMixMax;
	
	c1 = GREEN_(inPixel1);
	c2 = GREEN_(inPixel2);
	UInt32 green = c2 + ((c1-c2) * inMixValue)/inMixMax;
	
	c1 = BLUE_(inPixel1);
	c2 = BLUE_(inPixel2);
	UInt32 blue = c2 + ((c1-c2) * inMixValue)/inMixMax;
	
	return ARGB_(alp, red, green, blue);
}
	
// ---------------------------------------------------------------------------------
//		¥ MixPixels24
// ---------------------------------------------------------------------------------
// inline function mixes two RGB pixels, ignoring their alpha and setting
// the resulting RGB alpha value to full (0xFF)
// inMinValue should be between 0 and inMixMax
// lower values of inMixValue show more of inPixel1
// higher values of inMixValue show more of inPixel2

inline UInt32
MixPixels24(
	UInt32	inPixel1,		// 32 bit pixel in format 00RRGGBB
	UInt32	inPixel2,		// 32 bit pixel in format 00RRGGBB
	SInt32	inMixValue,		// higher values show more of pixel1, lower values show more of pixel 2
	SInt32	inMixMax)		// maximum possible value of inMixValue	
{
	SInt32 c1, c2;

	c1 = RED_(inPixel1);
	c2 = RED_(inPixel2);
	UInt32 red = c2 + ((c1-c2) * inMixValue)/inMixMax;
	
	c1 = GREEN_(inPixel1);
	c2 = GREEN_(inPixel2);
	UInt32 green = c2 + ((c1-c2) * inMixValue)/inMixMax;
	
	c1 = BLUE_(inPixel1);
	c2 = BLUE_(inPixel2);
	UInt32 blue = c2 + ((c1-c2) * inMixValue)/inMixMax;
	
	return ARGB_(0xFF, red, green, blue);
}
	
// ---------------------------------------------------------------------------------
//		¥ AvgPixels32
// ---------------------------------------------------------------------------------
// inline function mixes two RGB pixels, ignoring their alpha and setting
// the resulting RGB alpha value to off (0x00)
// inMinValue should be between 0 and inMixMax
// lower values of inMixValue show more of inPixel1
// higher values of inMixValue show more of inPixel2

inline UInt32
AvgPixels32(
	UInt32	inPixel1,		// 32 bit pixel in format 00RRGGBB
	UInt32	inPixel2,		// 32 bit pixel in format 00RRGGBB
	SInt32	inMixValue,		// higher values show more of pixel1, lower values show more of pixel 2
	SInt32	inMixMax)		// maximum possible value of inMixValue	
{
	SInt32 c1, c2;
	
	c1 = RED_(inPixel1);
	c2 = RED_(inPixel2);
	UInt32 red = (c1 * inMixValue) / inMixMax + (c2 * inMixValue) / inMixMax;
	
	c1 = GREEN_(inPixel1);
	c2 = GREEN_(inPixel2);
	UInt32 green = (c1 * inMixValue) / inMixMax + (c2 * inMixValue) / inMixMax;
	
	c1 = BLUE_(inPixel1);
	c2 = BLUE_(inPixel2);
	UInt32 blue = (c1 * inMixValue) / inMixMax + (c2 * inMixValue) / inMixMax;
	
	
	return RGB_(red, green, blue);
}


#endif
