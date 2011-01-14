//============================================================================
//	CoreImageCommonTypes		  (C)2007 Mark F. Coniglio. All rights reserved.
// ===========================================================================

#ifndef _H_CoreImageCommonTypes
#define	_H_CoreImageCommonTypes
#pragma once

#include "IsadoraTypes.h"

#include "ValueCommon.h"

#include "CoreImageCommon.h"

	#ifdef _OBJC_OBJC_H_
	
		#import <QuartzCore/QuartzCore.h>
		#import <QuartzCore/CIVector.h>
		#import <QuartzCore/CIFilter.h>
		#import <Quartz/Quartz.h>
		
		typedef NSBitmapImageRep*				NS_BitmapImageRep;
		typedef NSGraphicsContext*				NS_GraphicsContext;
		typedef CIContext*						CI_Context;
		typedef CIImage*						CI_Image;
		typedef NSImage*						NS_Image;
		typedef struct VideoView*				CI_VideoView;
		typedef CIFilter*						CI_Filter;
		typedef CI_Filter						CI_ColorMatrixFilter;
		typedef QCRenderer*						QC_Renderer;
		typedef NSOpenGLContext*				NS_OpenGLContext;
		typedef NSOpenGLPixelBuffer*			NS_OpenGLPixelBuffer;
	#else
		typedef struct OpaqueNSBitmapImageRep*	NS_BitmapImageRep;
		typedef struct OpaqueNSGraphicsContext*	NS_GraphicsContext;
		typedef struct OpaqueCIContext*			CI_Context;
		typedef struct OpaqueCIImage*			CI_Image;
		typedef struct OpaqueNSImage*			NS_Image;
		typedef struct OpaqueVideoVidew*		CI_VideoView;
		typedef struct OpaqueCIFilter*			CI_Filter;
		typedef CI_Filter						CI_ColorMatrixFilter;
		typedef struct OpaqueQCRenderer*		QC_Renderer;
		typedef struct OpaqueNSOpenGLContext*	NS_OpenGLContext;
		typedef struct OpaqueNSOpenGLPixelBuffer*	NS_OpenGLPixelBuffer;
	#endif

#define	CICONV_USE_OPENGL					1
#define	CICONV_USE_CG_CONTEXT				1

// ===========================================================================
// CV_ImageBufferRef struct

#if COMPILE_CORE_VIDEO
	typedef CVImageBufferRef					CV_ImageBufferRef;
#else
	typedef struct OpaqueCVImageBufferRef*		CV_ImageBufferRef;
#endif

// ===========================================================================
// CoreImageBuffer struct

// Magic Value to identify valid image buffers...
#define kCoreImageBufferMagic		0x12345678

struct CoreImageBuffer
{
	DataValueInfo	mInfo;					// our data value info struct - required
	UInt32			mMagic;
	CI_Image		mImage;
	StageIndexT		mTargetStageIndex0;		// destination stage for the buffer - type should match StageIndexT
};

typedef struct CoreImageBuffer	CoreImageBuffer, *CoreImageBufferPtr;


#endif	// #ifndef _H_CoreImageCommonTypes
