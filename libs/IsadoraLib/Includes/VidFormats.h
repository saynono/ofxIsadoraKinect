// ===========================================================================
//	VidFormats					  (C)2007 Mark F. Coniglio. All rights reserved.
// ===========================================================================
// Defines video formats used throughout Isadora and constants related to
// then. Notably kIzzyRGBFormat or kIzzyRGBFormat can be found in the mFormat
// member of the ImageBuffer struct.

#ifndef _H_VIDFORMATS
#define _H_VIDFORMATS

	#ifndef __APPLE_CC__
	#include <TargetConditionals.h>
	#endif

	#ifndef TARGET_CPU_PPC
		#error "TARGET_CPU_PPC must be defined here"
	#endif

	#ifndef TARGET_CPU_X86
		#error "TARGET_CPU_X86 must be defined here"
	#endif

	#define	kIzzyInvalidVidFormat				0						// an invalid format

	#if TARGET_CPU_PPC
		#define	kIzzyRGBFormat					k32ARGBPixelFormat		// gworld format: ARGB
		#define	kIzzyYUVFormat					k2vuyPixelFormat		// mem layout: CbY0CrY1
		#define	kGWorldPixelComponentOrder_ARGB	1						// Set to 1 when RGB GWorlds are organized in the ARGB (BigEndian) memory layout
		#define kGWorldYUV_CbY0CrY1				1						// Set to 1 when YUV GWorlds are organized in CbY0CrY1 (BigEndian) memory layout 
		#define kGWorldYUV_CrY1CbY0				0						// Set to 1 when YUV GWorlds are organized in CrY1CbY0 (LittleEndian) memory layout 
	#elif TARGET_CPU_X86
		#define	kIzzyRGBFormat					k32BGRAPixelFormat		// gworld format: BGRA
		#define	kIzzyYUVFormat					kYUVSPixelFormat		// mem layout: CrY1CbY0
		#define	kGWorldPixelComponentOrder_ARGB	1						// Set to 1 when RGB GWorlds are organized in the ARGB (BigEndian) memory layout
		#define kGWorldYUV_CbY0CrY1				0						// Set to 1 when YUV GWorlds are organized in CbY0CrY1 (BigEndian) memory layout
		#define kGWorldYUV_CrY1CbY0				1						// Set to 1 when YUV GWorlds are organized in CrY1CbY0 (LittleEndian) memory layout 
	#endif

#endif
