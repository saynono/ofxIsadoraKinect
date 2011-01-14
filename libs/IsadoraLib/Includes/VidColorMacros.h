// ===========================================================================
//	VidColorMacros				  (C)2007 Mark F. Coniglio. All rights reserved.
// ===========================================================================
// These macros allow you to pull the individual components (A/R/G/B or
// Cb/Y0/Cr/Y1) from a color. They are conditionally compiled depending
// on the platform endian settings in VidFormats.h

#ifndef _H_VIDCOLORMACROS
#define _H_VIDCOLORMACROS

#include "VidFormats.h"

// PIXEL ORDER MSB -> LSB = ARGB
#if kGWorldPixelComponentOrder_ARGB == 1
	#define	RGB_(r, g, b)				( (((UInt32)(r & 0xFF) << 16)) | (((UInt32)(g & 0xFF) << 8)) | ((UInt32)(b & 0xFF)) )
	#define	ARGB_(a, r, g, b)			( (((UInt32)(a & 0xFF) << 24)) | (((UInt32)(r & 0xFF) << 16)) | (((UInt32)(g & 0xFF) << 8)) | ((UInt32)(b & 0xFF)) )

	#define MASK_ONLY_ALPHA				0xFF000000
	#define MASK_NO_ALPHA				0x00FFFFFF
	
	#define REMOVE_ALPHA(rgb)			(0x00FFFFFF & (rgb))
	#define	SET_ALPHA(rgb)				(0xFF000000 | (rgb))
	#define	ALPHA_MASK_(argb)			( ((UInt32) argb) & MASK_ONLY_ALPHA)

	#define	ALPHA_(argb)				((UInt8)((argb >> 24) & 0xFF))
	#define	RED_(argb)					((UInt8)((argb >> 16) & 0xFF))
	#define	GREEN_(argb)				((UInt8)((argb >> 8) & 0xFF))
	#define	BLUE_(argb)					((UInt8)((argb >> 0) & 0xFF))

// PIXEL ORDER MSB -> LSB = BGRA
#else

	#define	RGB_(r, g, b)				((((UInt32)(b & 0xFF) << 24)) | (((UInt32)(g & 0xFF) << 16)) | (((UInt32)(r & 0xFF) << 8)))
	#define	ARGB_(a, r, g, b)			((((UInt32)(b & 0xFF) << 24)) | (((UInt32)(g & 0xFF) << 16)) | (((UInt32)(r & 0xFF) << 8)) | (((UInt32)(a & 0xFF) << 0)))

	#define MASK_ONLY_ALPHA				0x000000FF
	#define MASK_NO_ALPHA				0xFFFFFF00

	#define REMOVE_ALPHA(rgb)			(0xFFFFFF00 & (rgb))
	#define	SET_ALPHA(rgb)				(0x000000FF | (rgb))
	#define	ALPHA_MASK_(bgra)			( ((UInt32) bgra) & 0x000000FF)
	#define	ALPHA_(bgra)				((UInt8)((bgra) & 0xFF))
	#define	RED_(bgra)					((UInt8)((bgra >> 8) & 0xFF))
	#define	GREEN_(bgra)				((UInt8)((bgra >> 16) & 0xFF))
	#define	BLUE_(bgra)					((UInt8)((bgra >> 24) & 0xFF))

#endif

#if kGWorldYUV_CbY0CrY1 == 1

	#define	Cb_(c)						((UInt32) (((c) >> 24) & 0xFF))
	#define	Y0_(c)						((UInt32) (((c) >> 16) & 0xFF))
	#define	Cr_(c)						((UInt32) (((c) >> 8 ) & 0xFF))
	#define	Y1_(c)						((UInt32) (((c)      ) & 0xFF))
	
	#define	YUV_(y0, Cb, y1, Cr)		( (((UInt32)((Cb) & 0xFF)) << 24) | (((UInt32)((y0) & 0xFF)) << 16) | (((UInt32)((Cr) & 0xFF)) << 8) | ((UInt32)((y1) & 0xFF)) )
	
#elif kGWorldYUV_CrY1CbY0 == 1

	#define	Cb_(c)						((UInt32) (((c) >> 8 ) & 0xFF))
	#define	Y0_(c)						((UInt32) (((c)      ) & 0xFF))
	#define	Cr_(c)						((UInt32) (((c) >> 24) & 0xFF))
	#define	Y1_(c)						((UInt32) (((c) >> 16) & 0xFF))
	
	#define	YUV_(y0, Cb, y1, Cr)		( (((UInt32)((Cb) & 0xFF)) << 8) | (((UInt32)((y0) & 0xFF))) | (((UInt32)((Cr) & 0xFF)) << 24) | (((UInt32)((y1) & 0xFF)) << 16) )

#endif

#endif
