// ===========================================================================
//	ImageBufferCommon			 (C)2000 Mark F. Coniglio. All rights reserved.
// ===========================================================================
//
// ImageBuffers define a bit mapped image.

#ifndef _H_ImageBufferCommon
#define _H_ImageBufferCommon

#ifdef __APPLE_CC__
	#include "QuickTime/ImageCompression.h"
#else
	#include "ImageCompression.h"
#endif

#include "IsadoraTypes.h"
#include "ValueCommon.h"

#include "CoreImageCommon.h"
#if COMPILE_CORE_VIDEO
	#include "CoreImageCommonTypes.h"
#endif

// Magic Value to identify valid image buffers...
#define kImageBufferMagic		0x12345678

// ===========================================================================
// ImageBufferFormatInfo struct

struct IBFormatInfo
{
	SInt32			mBitDepth;				// bit depth of this image
	SInt32			mWidth;					// width of this image in pixels
	SInt32			mHeight;				// height of this image in pixels
	OSType			mFormat;				// four char code for format
	UInt32			mUniqueID;				// Image Buffer's Unique ID
};

typedef struct IBFormatInfo	IBFormatInfo, *IBFormatInfoPtr;

// ===========================================================================
// ImageBuffer struct

struct ImageBuffer
{
	DataValueInfo		mInfo;					// our data value info struct - required
	
	SInt32				mBitDepth;				// bit depth of this image
	SInt32				mWidth;					// width of this image in pixels
	SInt32				mHeight;				// height of this image in pixels
	OSType				mFormat;				// four char code for format
	
	void*				mBaseAddress;			// Ptr to the raw bytes of the image,
												// if BitDepth is 16 each line should 
												// be seen ad gWidth unsigned shorts,
												// if BitDepth is 32 each line is 
												// gWidth unsigned ints.
									
	SInt32				mRowBytes;				// byte offset between lines -- 
												// mBaseAddress + mRowBytes -> start of line 2;
												// mBaseAddress + 2 * mRowBytes -> start of third line, etc.

	CGrafPtr 			mGWorldPtr;				// MacOS/QTWin GWorldPtr to the Image data.
#if COMPILE_CORE_VIDEO
	CI_Image			mCIImage;				// CIImage - uses data provider to give a representation
												// of the data inside the GWorldPtr
#else
	void*				mCIImage;				// for targets not compiling core vide
#endif

	
	UInt32				mUniqueID;				// this value will be unique for each ImageBuffer
												// created within a given Isadora session
	
	Boolean				mHasAlpha;				// true if there is alpha channel info in this image buffer
	
	UInt32				mMagic;					// magic values - always = kImageBufferMagic
};

typedef struct ImageBuffer	ImageBuffer, *ImageBufferPtr;

// ===========================================================================
// Image Buffer Functions

void			ImageBuffer_InitImageBuffer(
					ImageBuffer*	inBuffer);
						
ImageBufferPtr	ImageBuffer_MakeImageBuffer(
					SInt16				inWidth,
					SInt16				inHeight,
					SInt16				inPixelDepth,
					OSType				inFormat = 0);
					
void			ImageBuffer_DisposeImageBuffer(
					ImageBufferPtr		inImageBuffer);

void			ImageBuffer_SetGWorld(
					ImageBuffer*		mImageBuffer,
					GWorldPtr			inGWorldPtr);

OSErr			ImageBuffer_CreateGWorld(
					ImageBuffer*		inBuffer,
					SInt16				inWidth,
					SInt16				inHeight,
					SInt16				inPixelDepth,
					OSType				inFormat = 0);
					
void			ImageBuffer_DisposeGWorld(
					ImageBuffer*	inBuffer);
					
void			ImageBuffer_EraseToColor(
					ImageBuffer*	inBuffer,
					UInt32			inARGBColor);

void			ImageBuffer_IncrementFrameCount(
					ImageBuffer*	inBuffer);

void			ImageBuffer_Copy(
					ImageBufferPtr				inImageBufferSrc,
					ImageBufferPtr				inImageBufferDst,
					const Rect*					inSrcRect,
					const Rect*					inDstRect,
					SInt16						inXFerModeCode,
					RGBColor*					inRGBBlendColor = nil,
					Boolean						inDontAccelerate = false);

// draw an Image Buffer into the current port using CopyBits
#ifdef __ISADORA_APP__
void			ImageBuffer_DrawInCurrentGrafPort_CB(
					ImageBufferPtr				inImageBufferSrc,
					const Rect*					inSrcRect,		// can be nil
					const Rect*					inDstRect,		// can be nil
					SInt16						inXFerModeCode = srcCopy,
					RGBColor*					inRGBBlendColor = nil);
#endif

// draw an Image Buffer into the current port using Decompress Image
#ifdef __ISADORA_APP__
void			ImageBuffer_DrawInCurrentGrafPort_DI(
					ImageBufferPtr				inImageBufferSrc,
					const Rect*					inSrcRect,		// can be nil
					const Rect*					inDstRect,		// can be nil
					SInt16						inXFerModeCode = srcCopy,
					RGBColor*					inRGBBlendColor = nil);
#endif
					
// returns true if the specified image buffer contains a YUV bitmap 
Boolean			ImageBuffer_IsYUV(
					const ImageBufferPtr		inImageBuffer);

// ensures that the alpha byte for all pixels is set to 0xFF
void			ImageBuffer_FixAlpha(
					ImageBufferPtr		inImageBuffer);

void			ImageBuffer_AnalyzeBuffer(
					const ImageBufferPtr	inImageBuffer,
					bool*					outAllBlack,
					bool*					outAlphaAllZeroes,
					bool*					outAlphaAllOnes,
					UInt32*					outCenterPixel);
					
// if any alpha byte in the specified image buffer is not 0xFF
// then the mHasAlpha flag is set for the specified image buffer
void			ImageBuffer_CheckForAlpha(
					ImageBufferPtr		inImageBuffer);

// the caller is responsible for calling DisposeHandle on
// the returned result
ImageDescriptionHandle	ImageBuffer_CreateImageDescription(
							ImageBufferPtr		inImageBuffer);


#ifdef __cplusplus
	inline void		ImageBuffer_StoreFormatInfo(
						ImageBufferPtr		inImageBuffer,
						IBFormatInfoPtr		inFormatInfo)
					{
						inFormatInfo->mWidth = inImageBuffer->mWidth;
						inFormatInfo->mHeight = inImageBuffer->mHeight;
						inFormatInfo->mBitDepth = inImageBuffer->mBitDepth;
						inFormatInfo->mFormat = inImageBuffer->mFormat;
						inFormatInfo->mUniqueID = inImageBuffer->mUniqueID;
					}


	inline Boolean	ImageBuffer_AreFormatsDifferent(
						const ImageBufferPtr		inImageBuffer1,
						const ImageBufferPtr		inImageBuffer2)
					{
						return (inImageBuffer1->mWidth != inImageBuffer2->mWidth
							|| inImageBuffer1->mHeight != inImageBuffer2->mHeight
							|| inImageBuffer1->mBitDepth != inImageBuffer2->mBitDepth
							|| inImageBuffer1->mFormat != inImageBuffer2->mFormat);
					}
	
	inline Boolean	ImageBuffer_AreFormatsDifferent(
						const IBFormatInfoPtr		inFormatInfo1,
						const IBFormatInfoPtr		inFormatInfo2)
					{
						return (inFormatInfo1->mWidth != inFormatInfo2->mWidth
							|| inFormatInfo1->mHeight != inFormatInfo2->mHeight
							|| inFormatInfo1->mBitDepth != inFormatInfo2->mBitDepth
							|| inFormatInfo1->mFormat != inFormatInfo2->mFormat);
					}
	
	inline Boolean	ImageBuffer_HasFormatChanged(
						const ImageBufferPtr		inImageBuffer,
						const IBFormatInfoPtr		inFormatInfo)
					{
						return (inImageBuffer->mWidth != inFormatInfo->mWidth
							|| inImageBuffer->mHeight != inFormatInfo->mHeight
							|| inImageBuffer->mBitDepth != inFormatInfo->mBitDepth
							|| inImageBuffer->mFormat != inFormatInfo->mFormat);
					}
	
	inline Boolean	ImageBuffer_HasFormatOrUniqueIDChanged(
						const ImageBufferPtr		inImageBuffer,
						const IBFormatInfoPtr		inFormatInfo)
					{
						return (inImageBuffer->mWidth != inFormatInfo->mWidth
							|| inImageBuffer->mHeight != inFormatInfo->mHeight
							|| inImageBuffer->mBitDepth != inFormatInfo->mBitDepth
							|| inImageBuffer->mFormat != inFormatInfo->mFormat
							|| inImageBuffer->mUniqueID != inFormatInfo->mUniqueID);
					}
	
	inline Boolean	ImageBuffer_HasFormatChanged_ForceFormat(
						const ImageBufferPtr		inImageBuffer,
						const IBFormatInfoPtr		inFormatInfo,
						const OSType				inForceFormat,
						const SInt32				inForceBitDepth)
					{
						if (inForceFormat != 0) {
							return (inImageBuffer->mWidth != inFormatInfo->mWidth
								|| inImageBuffer->mHeight != inFormatInfo->mHeight
								|| inImageBuffer->mBitDepth != inForceBitDepth
								|| inImageBuffer->mFormat != inForceFormat);
						} else {
							return (inImageBuffer->mWidth != inFormatInfo->mWidth
								|| inImageBuffer->mHeight != inFormatInfo->mHeight
								|| inImageBuffer->mBitDepth != inFormatInfo->mBitDepth
								|| inImageBuffer->mFormat != inFormatInfo->mFormat);
						}
					}
	
	inline void		ImageBuffer_ClearFormatInfo(
						IBFormatInfoPtr				inFormatInfo)
					{
						inFormatInfo->mWidth = 0;
						inFormatInfo->mHeight = 0;
						inFormatInfo->mBitDepth = 0;
						inFormatInfo->mFormat = 0;
						inFormatInfo->mUniqueID = 0;
					}

#else
	void			ImageBuffer_StoreFormatInfo(
						ImageBufferPtr		inImageBuffer,
						IBFormatInfoPtr		inFormatInfo);
						
	Boolean			ImageBuffer_AreFormatsDifferent(
						const IBFormatInfoPtr		inFormatInfo1,
						const IBFormatInfoPtr		inFormatInfo2);
						
	void			ImageBuffer_ClearFormatInfo(
						IBFormatInfoPtr		inFormatInfo);

	Boolean			ImageBuffer_HasFormatChanged(
						const ImageBufferPtr		inImageBuffer,
						const IBFormatInfoPtr		inFormatInfo);

	Boolean			ImageBuffer_HasFormatOrUniqueIDChanged(
						const ImageBufferPtr		inImageBuffer,
						const IBFormatInfoPtr		inFormatInfo);

	Boolean			ImageBuffer_HasFormatChanged_ForceFormat(
						const ImageBufferPtr		inImageBuffer,
						const IBFormatInfoPtr		inFormatInfo,
						const OSType				inForceFormat,
						const SInt32				inForceBitDepth);

	void			ImageBuffer_ClearFormatInfo(
						IBFormatInfoPtr				inFormatInfo);
#endif
					
void			ImageBuffer_Copy_ARGB32_to_2yuv(
					ImageBufferPtr	inSrcBuffer,
					ImageBufferPtr	inDstBuffer,
					const Rect*		inSrcRect = nil,
					const Rect*		inDstRect = nil);
					
void			ImageBuffer_Copy_ARGB32_to_2yuv_2to1(
					ImageBufferPtr	inSrcBuffer,
					ImageBufferPtr	inDstBuffer,
					const Rect*		inSrcRect = nil,
					const Rect*		inDstRect = nil);

void			ImageBuffer_Copy_2yuv_to_ARGB32(
					ImageBufferPtr	inSrcBuffer,
					PixMapHandle	inDstMap,
					const Rect*		inSrcRect = nil,
					const Rect*		inDstRect = nil);

void			ImageBuffer_Copy_2yuv_to_ARGB32(
					ImageBufferPtr	inSrcBuffer,
					ImageBufferPtr	inDstBuffer,
					const Rect*		inSrcRect = nil,
					const Rect*		inDstRect = nil);
					
void			ImageBuffer_Copy_2yuv_to_ARGB32_1to2(
					ImageBufferPtr	inSrcBuffer,
					PixMapHandle	inDstMap,
					const Rect*		inSrcRect /* = nil */,
					const Rect*		inDstRect /* = nil */);
					
void			ImageBuffer_Copy_2yuv_to_ARGB32_1to2(
					ImageBufferPtr	inSrcBuffer,
					ImageBufferPtr	inDstBuffer,
					const Rect*		inSrcRect /* = nil */,
					const Rect*		inDstRect /* = nil */);

#if (DEBUG || CHECK_IMAGE_BUFFER_MAGIC)
	#define ImageBuffer_CheckMagic(ib)	Assert_(ib == nil || (ib)->mMagic == kImageBufferMagic)

#else
	#define ImageBuffer_CheckMagic(ib)	0

#endif

						
#endif
