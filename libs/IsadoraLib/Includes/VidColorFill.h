// ===========================================================================
//	VidColorFill			 	  (C)2004 Mark F. Coniglio. All rights reserved.
// ===========================================================================
// Routines for filling an image buffer with a solid color. The image buffer
// can be a 32 bit ARGB buffer, or a YUV buffer. If the inHasFastMath flag is
// true, then routines accelerated for the current processor will be used

#ifndef _H_VidColorFill
#define _H_VidColorFill

#include "ImageBufferCommon.h"
#include "VidProcessCommon.h"

#ifndef _H_ArchitectureFlags
	#error "ArchitectureFlags.h must be included by this file"
#endif

#ifdef __cplusplus
extern "C" {
#endif

	Boolean
	VidColorFillAlpha(
		Boolean				inHasFastMath,
		ImageBufferPtr		img1,
		UInt32				inRGBColor,			// 00RRGGBB (even for YUV Buffers)
		UInt8				inAlpha);			// 0 to 255

	// NOTE: inRGBColor is in the format 00RRGGBB, even for YUV ImageBuffers
	Boolean
	VidColorFill(
		Boolean				inHasFastMath,
		ImageBufferPtr		img1,
		UInt32				inRGBColor);		// 00RRGGBB (even for YUV Buffers)

	#if ALTIVEC_EXTENSIONS
	Boolean
	VidColorFill_AV(
		ImageBufferPtr		img1,
		UInt32				inRGBColor);
	#endif

	#if SSE_EXTENSIONS
	Boolean
	VidColorFill_MMX(
		ImageBufferPtr		img1,
		UInt32				inRGBColor);
	#endif

	void
	VidColorFill_C(
		ImageBufferPtr		img1,
		UInt32				inRGBColor);

#ifdef __cplusplus
}
#endif

#endif
