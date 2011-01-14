// ---------------------------------------------------------------------------
//		DrawPictureWithAlpha
// ---------------------------------------------------------------------------
//	DrawPictureWithAlpha

#ifndef _H_DrawPictureWithAlpha
#define _H_DrawPictureWithAlpha

#define kAlphaBitmapMagic		0x12345678L

#ifdef __APPLE_CC__
	#include <Quicktime/ImageCompression.h>
#else
	#include <ImageCompression.h>
#endif

#include "ImageBufferCommon.h"
#include "VidFormats.h"

// ---------------------------------------------------------------------------------
//		AlphaBitmap [STRUCT]
// ---------------------------------------------------------------------------------
struct AlphaBitmap {
	ImageBufferPtr			mImageBuffer;	// Image Buffer Pointer with the image data
	ImageDescriptionHandle	mImgDesc;		// an image description handle for the data
	Rect					mImgBounds;		// the image bounds
	UInt32					mMagic;			// magic value that indicates that the GWorld data is valid
};

typedef AlphaBitmap AlphaBitmap;

// ---------------------------------------------------------------------------------
//		CreateAlphaBitmap
// ---------------------------------------------------------------------------------
// Given a PicHandle, create a GWorld with an alpha channel component
OSStatus
CreateAlphaBitmap(
	PicHandle		inPicHandle,
	AlphaBitmap*	mImgDesc,
	OSType			inPixelFormat = kIzzyRGBFormat);
	
// ---------------------------------------------------------------------------------
//		DisposeAlphaBitmap
// ---------------------------------------------------------------------------------
// Disposes an AlphaBitmap created with CreateAlphaBitmap
void
DisposeAlphaBitmap(
	AlphaBitmap*	inAlphaBitmap);
	
// ---------------------------------------------------------------------------------
//		DrawAlphaBitmap
// ---------------------------------------------------------------------------------
// Given a valid AlphaBitmap, draws the image using the alpha channel
OSStatus
DrawAlphaBitmap(
	AlphaBitmap*	inAlphaBitmap,
	const Rect*		inDestRect,
	const Rect*		inSrcRect = nil);

// ---------------------------------------------------------------------------------
//		DrawPictureWithAlpha
// ---------------------------------------------------------------------------------
// Draws a PicHandle using its embedded alpha channel information. This convenience
// function calls CreateAlphaBitmap, DrawAlphaBitmap, and DisposeAlphaBitmap in 
// sequence. Note, however, that if you will be drawing the picture repeatedly, it is
// faster to create the AlphaBitmap, use DrawAlphaBitmap to draw the image, and then
// dispose the AlphaBitmap when you are done.

void
DrawPictureWithAlpha(
	PicHandle		inPicHandle,
	const Rect*		inRect);

#endif
