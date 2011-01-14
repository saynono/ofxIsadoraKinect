// ===========================================================================
//	VidProcessCommon			  Â(C)2004 Mark F. Coniglio. All rights reserved.
// ===========================================================================
// Inline functions for showing an assertion dialog, limiting video values
// and for converting between rgb and yuv.

#ifndef _H_VIDPROCESSCOMMON
#define _H_VIDPROCESSCOMMON

#include "VidFormats.h"
#include "VidColorMacros.h"
#include "ImageBufferCommon.h"
#include "ArchitectureFlags.h"

// ---------------------------------------------------------------------------------
//		VidAssert_
// ---------------------------------------------------------------------------------
// Allows the client to place an assertion test in their code. If the DEBUG constant
// is defined, and if the expression 'cond' evaluates to false, then an assertion
// error dialog box will be shown to the user

#if DEBUG
	#define	VidAssert_(cond)	if (!(cond)) VidAssert(#cond, __FILE__, __LINE__)
#else
	// MFC 10/28/07: changed to { } to prevent warning errors
	#define	VidAssert_(cond)	{ }
#endif

// extern RGBColor	sVidProcessWhite;
// extern RGBColor	sVidProcessBlack;

 // ---------------------------------------------------------------------------------
// FUNCTIONS

#ifdef __cplusplus
extern "C" {
#endif

void
InitVidProcessCommon();

// ---------------------------------------------------------------------------------
//		VidAssert
// ---------------------------------------------------------------------------------
// Allows video processing routines to throw an assertion error -- clients should use
// the VidAssert_ macro instead of calling this function directly

void
VidAssert(
	const char*	inCondition,
	const char*	inFile,
	int			inLine);


// value used when converting yuv to rgb and vice versa
const int kYUVRGBConvFactor		= 32768;

// ---------------------------------------------------------------------------------
//		LimitToUInt8
// ---------------------------------------------------------------------------------
// Limits the incoming value to the range 255, inclusive

inline int LimitToUInt8(
	int inValue)
{
	int mask_ge0	= (0-(inValue >= 0));		// if >= 0 then mask = (0 - 1) = 0xFFFF, else mask = (0 - 0) = 0x0000
	int mask_lt255	= (0-(inValue <= 255));		// if <= 255 then mask = (0 - 1) = 0xFFFF, else mask = (0 - 0) = 0x0000
	
	// if the value is within the range 0-255, then the
	// original value is returned -- otherwise, if it is
	// greater than 255, then 255 is returned; if it is 
	// less than 0, 0 is returned
	return (inValue & mask_ge0 & mask_lt255) | (~mask_lt255 & 255);
}

// ---------------------------------------------------------------------------------
//		ClampY
// ---------------------------------------------------------------------------------
// Limits the incoming value to the range 16-235, inclusive

inline int ClampY(
	int inValue)
{
	int mask_ge16	= (0-(inValue >= 16));		// if >= 16 then mask = (0 - 1) = 0xFFFF, else mask = (0 - 0) = 0x0000
	int mask_lt235	= (0-(inValue <= 235));		// if <= 235 then mask = (0 - 1) = 0xFFFF, else mask = (0 - 0) = 0x0000
	
	return (inValue & mask_ge16 & mask_lt235) | (~mask_ge16 & 16) | (~mask_lt235 & 235);
}

// ---------------------------------------------------------------------------------
//		ClampY_219
// ---------------------------------------------------------------------------------
// Limits the incoming value to the range 0-219, inclusive

inline int ClampY_219(
	int inValue)
{
	int mask_ge0	= (0-(inValue >= 0));		// if >= 0 then mask = (0 - 1) = 0xFFFF, else mask = (0 - 0) = 0x0000
	int mask_lt219	= (0-(inValue <= 219));		// if <= 219 then mask = (0 - 1) = 0xFFFF, else mask = (0 - 0) = 0x0000
	
	return (inValue & mask_ge0 & mask_lt219) | (~mask_lt219 & 219);
}

// ---------------------------------------------------------------------------------
//		ClampY_438
// ---------------------------------------------------------------------------------
// Limits the incoming value to the range 0-438, inclusive

inline int ClampY_438(
	int inValue)
{
	int mask_ge0	= (0-(inValue >= 0));		// if >= 0 then mask = (0 - 1) = 0xFFFF, else mask = (0 - 0) = 0x0000
	int mask_lt438	= (0-(inValue <= 438));		// if <= 438 then mask = (0 - 1) = 0xFFFF, else mask = (0 - 0) = 0x0000
	
	return (inValue & mask_ge0 & mask_lt438) | (~mask_lt438 & 438);
}

// ---------------------------------------------------------------------------------
//		ClampY_1_219
// ---------------------------------------------------------------------------------
// Limits the incoming value to the range 0-219, inclusive

inline int ClampY_1_219(
	int inValue)
{
	int mask_ge1	= (0-(inValue >= 1));		// if >= 1 then mask = (0 - 1) = 0xFFFF, else mask = (0 - 0) = 0x0000
	int mask_lt219	= (0-(inValue <= 219));		// if <= 219 then mask = (0 - 1) = 0xFFFF, else mask = (0 - 0) = 0x0000
	
	return (inValue & mask_ge1 & mask_lt219) | (~mask_ge1 & 1) | (~mask_lt219 & 219);
}

// ---------------------------------------------------------------------------------
//		ClampY_1_438
// ---------------------------------------------------------------------------------
// Limits the incoming value to the range 0-438, inclusive

inline int ClampY_1_438(
	int inValue)
{
	int mask_ge1	= (0-(inValue >= 1));		// if >= 1 then mask = (0 - 1) = 0xFFFF, else mask = (0 - 0) = 0x0000
	int mask_lt438	= (0-(inValue <= 438));		// if <= 438 then mask = (0 - 1) = 0xFFFF, else mask = (0 - 0) = 0x0000
	
	return (inValue & mask_ge1 & mask_lt438) | (~mask_ge1 & 1) | (~mask_lt438 & 438);
}

// ---------------------------------------------------------------------------------
//		ClampCbCr
// ---------------------------------------------------------------------------------
// Limits the incoming value to the range 16-240, inclusive

inline int ClampCbCr(
	int inValue)
{
	int mask_ge16	= (0-(inValue >= 16));		// if >= 16 then mask = (0 - 1) = 0xFFFF, else mask = (0 - 0) = 0x0000
	int mask_lt240	= (0-(inValue <= 240));		// if <= 240 then mask = (0 - 1) = 0xFFFF, else mask = (0 - 0) = 0x0000
	
	return (inValue & mask_ge16 & mask_lt240) | (~mask_ge16 & 16) | (~mask_lt240 & 240);
}

// ---------------------------------------------------------------------------------
//		byte_clamp
// ---------------------------------------------------------------------------------
// used by both the yuv_to_rgb and rgb_to_yuv routines
inline UInt8
byte_clamp(
	int				x)
{
	return (UInt8) LimitToUInt8(x / kYUVRGBConvFactor);
}
	 
// ---------------------------------------------------------------------------------
//		yuv_to_rgb
// ---------------------------------------------------------------------------------
// YUV to RGB Conversion
//
// 		R = (Y-16) x 1.164384	+ (U-128) x 0.000000	+ (V-128) x 1.596027
// 		G = (Y-16) x 1.164384	+ (U-128) x -0.391762	+ (V-128) x -0.812967
// 		B = (Y-16) x 1.164384	+ (U-128) x 2.017232	+ (V-128) x 0.000000

// OUPTUT FORMAT: 00RRGGBB

inline UInt32
yuv_to_rgb(
	UInt8	y,
	UInt8	u,
	UInt8	v)
{
	const int yFactor = int(1.164384 * kYUVRGBConvFactor);
	
	// const int uFactor1 = int( 0.000000 * kYUVRGBConvFactor);
	const int uFactor2 = int(-0.391762 * kYUVRGBConvFactor);
	const int uFactor3 = int( 2.017232 * kYUVRGBConvFactor);
	
	const int vFactor1 = int( 1.596027 * kYUVRGBConvFactor);
	const int vFactor2 = int(-0.812967 * kYUVRGBConvFactor);
	// const int vFactor3 = int( 0.000000 * kYUVRGBConvFactor);
	
	const int roundAmt = kYUVRGBConvFactor/2;

	SInt32 yMultFactor = (y-16) * yFactor;
	SInt32 um128 = (u-128);
	SInt32 vm128 = (v-128);
	
	UInt32 rgb =       byte_clamp(yMultFactor +                    vFactor1 * vm128 + roundAmt);
	rgb = (rgb << 8) | byte_clamp(yMultFactor + uFactor2 * um128 + vFactor2 * vm128 + roundAmt);
	rgb = (rgb << 8) | byte_clamp(yMultFactor + uFactor3 * um128                    + roundAmt);
	
	return rgb;
}
	 
// ---------------------------------------------------------------------------------
//		rgb_to_yuv
// ---------------------------------------------------------------------------------
// RGB to YUV Conversion
//
//		Y  =     0.299000R	+ 0.587000G	+  0.114000B	+ 16
//		Cb =	-0.168736R	- 0.331264G	+  0.500000B	+ 128 
//		Cr =     0.500000R	- 0.418688G	-  0.081312B	+ 128
//
//		Y  =	R *  0.256788	+ G *  0.504129	+ B *  0.097906 + 16
//		Cb =	R * -0.148223	+ G * -0.290993	+ B *  0.439216 + 128
//		Cr =	R *  0.439216	+ G * -0.367789	+ B * -0.071427 + 128


// OUPTUT FORMAT: 00YYUUVV

inline UInt32
rgb_to_yuv(
	UInt8	r,
	UInt8	g,
	UInt8	b)
{
	const int YR = int( 0.256788 * kYUVRGBConvFactor);
	const int YG = int( 0.504129 * kYUVRGBConvFactor);
	const int YB = int( 0.097906 * kYUVRGBConvFactor);
	
	const int CbR = int(-0.148223 * kYUVRGBConvFactor);
	const int CbG = int(-0.290993 * kYUVRGBConvFactor);
	const int CbB = int( 0.439216 * kYUVRGBConvFactor);

	const int CrR = int( 0.439216 * kYUVRGBConvFactor);
	const int CrG = int(-0.367789 * kYUVRGBConvFactor);
	const int CrB = int(-0.071427 * kYUVRGBConvFactor);
	
	const int _16xYUVGBFactor = int(16 * kYUVRGBConvFactor);
	const int _128xYUVRGBFactor = int(128 * kYUVRGBConvFactor);

	const int roundAmt = kYUVRGBConvFactor/2;

	UInt32 yuv =       byte_clamp(YR  * r  +  YG  * g  +  YB  * b  +  _16xYUVGBFactor + roundAmt);
	yuv = (yuv << 8) | byte_clamp(CbR * r  +  CbG * g  +  CbB * b  +  _128xYUVRGBFactor + roundAmt);
	yuv = (yuv << 8) | byte_clamp(CrR * r  +  CrG * g  +  CrB * b  +  _128xYUVRGBFactor + roundAmt);
	
	return yuv;
}

// ---------------------------------------------------------------------------------
//		IsFormatRGB
// ---------------------------------------------------------------------------------
//	Returns true if the specified video format is a RGB format

inline Boolean
IsFormatRGB(
	OSType	inFormat) 
{
	return (inFormat == kIzzyRGBFormat);
}

// ---------------------------------------------------------------------------------
//		IsFormatYUV
// ---------------------------------------------------------------------------------
//	Returns true if the specified video format is a YUV format

inline Boolean
IsFormatYUV(
	OSType	inFormat) 
{
	return (inFormat == kIzzyYUVFormat);
}

// ---------------------------------------------------------------------------------
//		GetWhiteRGBColor
// ---------------------------------------------------------------------------------
//	Returns a pointer to an RGBColor that is white

/*
inline const RGBColor*
GetWhiteRGBColor() 
{
	return &sVidProcessWhite;
}

// ---------------------------------------------------------------------------------
//		GetBlackRGBColor
// ---------------------------------------------------------------------------------
//	Returns a pointer to an RGBColor that is black

inline const RGBColor*
GetBlackRGBColor() 
{
	return &sVidProcessBlack;
}
*/

// ---------------------------------------------------------------------------------
//		RGB_To_IzzyYUV
// ---------------------------------------------------------------------------------
// Convert an RGB color to a CbY0CrY1 yuv color

inline UInt32
RGB_To_IzzyYUV(UInt32 rgb) 
{
	// memory layout = 00Y0CbCr
	UInt32 yuv = rgb_to_yuv((UInt8)(rgb >> 16), (UInt8)(rgb >> 8), (UInt8)rgb);
	
	#if kGWorldYUV_CbY0CrY1

		// memory layout CbY0CrY1
		//                      Cb					   Y0							 Cr								Y0
		return ((yuv << 16) & 0xFF000000) | (yuv & 0x00FF0000) | ((yuv << 8) & 0x0000FF00) | ((yuv >> 16) & 0x000000FF);
	
	#elif kGWorldYUV_CrY1CbY0

		// memory layout CrY1CbY0
		//                      Cr                     Y0Cb                             Y0
		return ((yuv << 24) & 0xFF000000) | (yuv & 0x00FFFF00) | ((yuv >> 16) & 0x000000FF);

	#endif
}

// ---------------------------------------------------------------------------------
//		CalculateDSTFlags
// ---------------------------------------------------------------------------------
// Used only with Altivec processing routines on MacOS - this function will calculate
// the prefetch constant used in the vec_dst series of instructions.

UInt32
CalculateDSTPrefetchConstant(
	SInt32		inRowBytes);


// ---------------------------------------------------------------------------------
//		FixAlpha
// ---------------------------------------------------------------------------------
// This function will take an RGB ImageBuffer and ensure that the alpha byte is set
// to 255 (hex FF).

void
FixAlpha(
	Boolean						inHasFastMath,
	ImageBufferPtr				inBuffer,
	const Rect*					dstRect);
	
// ---------------------------------------------------------------------------------
//		CheckForAlpha
// ---------------------------------------------------------------------------------
// If there is any alpha value that is not 0xFF (255) then the mHasAlpha flag will be
// set for this ImageBuffer

void
CheckForAlpha(
	ImageBufferPtr				inBuffer);

#ifdef __cplusplus
}
#endif

#endif
