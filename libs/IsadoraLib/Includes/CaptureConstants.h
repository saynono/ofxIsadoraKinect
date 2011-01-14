// ===========================================================================
//	CaptureConstants			  (C)2002 Mark F. Coniglio. All rights reserved.
// ===========================================================================
// Defines constants used with the SetCaptureMode_ function defined in
// IsadoraCallbacks.h

#ifndef _H_CaptureConstants
#define _H_CaptureConstants

typedef UInt8	CaptureEnable;

enum {
	kCaptureEnableDefault,
	kCaptureEnabled,
	kCaptureDisabled
};
	
typedef	UInt8	CaptureSize;

enum
{
	kCaptureSizeDefault = 0,
	kCaptureSizeNormal,			// Full Size Frame
	kCaptureSizeThird,			// 1/3 Size Frame
	kCaptureSizeHalf,			// 1/2 Size Frame
	kCaptureSizeQuarter,		// 1/4 Size Frame
	kCaptureSize640x480,		// Full 4x3 NTSC
	kCaptureSize320x240,		// Half 4x3 NTSC
	kCaptureSize160x120,		// Quarter 4x3 NTSC
	kCaptureSize768x576,		// Full 4x3 PAL
	kCaptureSize384x288, 		// Half 4x3 PAL
	kCaptureSize192x144,		// Quarter 4x3 PAL
	kCaptureSizeCustom			// Custom Size
};

typedef	UInt8	CaptureQuality;

enum
{
	kCaptureQualityInvalid = 0,
	kCaptureQualityMaximum,
	kCaptureQualityHigh,
	kCaptureQualityNormal,
	kCaptureQualityLow,
	kCaptureQualityMinimum
};

#endif
