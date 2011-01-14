// ===========================================================================
//	ImageBufferUtil				(C)2000 Mark F. Coniglio. All rights reserved.
// ===========================================================================
//
// It is common for video processing plugins to have two (or more) inputs and
// a video output. It may be that the two image buffer inputs are of different
// dimensions and bit depths, which often requires that the one of the two
// inputs be mapped to a common size and bit depth so that they can be 
// manipulated. These utilities facilitate this process.
//
// The caller starts by creating an ImageBufferMap, which contains information
// about the inputs and the output. Initially, all values in this struct can be
// set to nil.
//
// When a new image buffer input is connected, or sends a new value, the user
// should call SetImageBufferValue with a pointer to the input image buffer,
// and an index indicating which input buffer this refers to in the input map.
// SetImageBufferValue will set flags in preparation for a call to
// UpdateImageBufferMap
//
// When the user is ready to render an output image, it should always call
// UpdateImageBufferMap. This function will allocate any intermediary image
// buffers (based on the Isadora Preference settings in the IsadoraParameters
// struct) and also allocate an output buffer. All of this is done on an as
// needed basis, i.e., if the size and bit depth of the inputs remains constant,
// and if the output buffer already exists, then no action will be taken. 
// UpdateImageBufferMap is a low-cost call as long as no new buffers need to
// be allocated.
//
// The final step is to call GetImageBufferPtr for the inputs. This function 
// takes care of copying the true input buffer into the intermediary buffer
// (if necessary) and returns an ImageBufferPtr to the source data for the
// specified input. At that point, the user can manipulate the data from these
// input points with assurance that they are of the same bit depth and dimensions.
//
// Needless to say, none of these functions can be called at Interrupt Time.
//

#ifndef _H_ImageBufferUtil
#define _H_ImageBufferUtil

#include "IsadoraCallbacks.h"
#include "ImageBufferCommon.h"

#define	kMaxImageBufferMapBuffers	2

// -----------------------------------------------------------------------------
// ImageBufferInfo STRUCT
// -----------------------------------------------------------------------------
// This struct keeps information about an image buffer (not owned by this struct)
// and, potentially, an intermediary buffer associated with that image buffer.
// It keeps status information about the input image buffer property so that the
// existence of the intermediary buffer can be kept in sync with the current
// input image buffer. It also keeps track of the last data change count when
// the intermediary buffer was updated.

struct ImageBufferInfo {
	ImageBufferPtr		mSrcBuffer;					// the source image buffer
	UInt32				mLastDataChangeCount;		// keeps track of last data changed value
	
	ImageBufferPtr		mIntermediaryBufferPtr;		// the intermediary buffer, if needed
	
	Boolean				mImageBufferNeedsRemap;		// set true when image buffer needs remapping
	Boolean				mBlackBufReplacesNothing;	// if nil input, use the appropriate black buffer
	
	ImageBufferPtr		mProcessBuffer;				// for internal use only
};
typedef struct ImageBufferInfo	ImageBufferInfo;


// -----------------------------------------------------------------------------
// ImageBufferMap STRUCT
// -----------------------------------------------------------------------------
// Holds a pair of ImageBufferInfo structs, and an output buffer associated with
// those two inputs.
struct ImageBufferMap {
	UInt32				mImageBufferMagic;		// set to 0x12345678 after calling CreateImageBufferMap
	OSType				mDefaultPixelFormat;	// default format for video processing
	IBFormatInfo		mFormat;				// preferred format for data in this buffer
	ImageBufferPtr		mBlackBuffer;			// a black image buffer in the format specified by mFormat
	
	Boolean				mMissingInputBuffersOK;	// if true, it is OK if one or more input buffers are nil
	UInt32				mInputBufferCount;		// number of input buffers
	ImageBufferInfo*	mInputBuffers;			// an array of ImageBufferInfo's, one for each input buffer
	
	Boolean				mOutputBuffersValid;	// true when output buffers exist
	UInt32				mOutputBufferCount;		// number of output buffers in this map
	ImageBufferPtr*		mOutputBufferPtrs;		// array of pointers to all of the output buffers
};
typedef struct ImageBufferMap	ImageBufferMap;

#define	kImageBufferMagic	0x12345678

// -----------------------------------------------------------------------------
// CreateOutputBufferArray
// -----------------------------------------------------------------------------
Boolean
CreateOutputBufferArray(
	IsadoraParameters*	ip,
	ImageBufferMap*		inBufferMap);

// -----------------------------------------------------------------------------
// CreateImageBufferMap
// -----------------------------------------------------------------------------
// This function should be called to initialize an empty ImageBufferMap
// structure. The mInputBufferCount and mOutputBufferCount members determine the
// number of input and output buffers, so these should be initialized before
// calling this function.

void
CreateImageBufferMap(
	IsadoraParameters*	ip,
	ImageBufferMap*		inBufferMap);

// -----------------------------------------------------------------------------
// DisposeImageBufferMap
// -----------------------------------------------------------------------------
// This function disposes memory allocated by the CreateImageBufferMap function.
// It should be called after calling DisposeOwnedImageBuffers, after you are
// finished using an ImageBufferMap

void
DisposeImageBufferMap(
	IsadoraParameters*	ip,
	ImageBufferMap*		inBufferMap);

// -----------------------------------------------------------------------------
// SetImageBufferValue
// -----------------------------------------------------------------------------
// this function should be called whenever one of the input buffer ptrs changes
// its value (including when it is set to nil) and whenever it has new data. If
// there is an intermediary buffer, the data from the source will be copied to
// that buffer.
void
SetImageBufferValue(
	IsadoraParameters*	ip,
	ImageBufferMap*		inBufferMap,
	UInt32				inIndex0,
	ImageBufferPtr		inImageBufferPtr);


// -----------------------------------------------------------------------------
// ClearSourceBuffers
// -----------------------------------------------------------------------------
// Sets all of the source buffers to nil

void
ClearSourceBuffers(
	IsadoraParameters*	ip,
	ImageBufferMap*		inBufferMap);

// -----------------------------------------------------------------------------
// DisposeOwnedImageBuffers
// -----------------------------------------------------------------------------
// Disposes all intermediary buffers associated with this ImageBufferMap's
// input buffers, and all this ImageBufferMap's output image buffers.
void
DisposeOwnedImageBuffers(
	IsadoraParameters*	ip,
	ImageBufferMap*		inBufferMap);


// -----------------------------------------------------------------------------
// DisposeOwnedInputBuffers
// -----------------------------------------------------------------------------
// Disposes all intermediary buffers associated with this ImageBufferMap's
// input buffers.
void
DisposeOwnedInputBuffers(
	IsadoraParameters*	ip,
	ImageBufferMap*		inBufferMap);


// -----------------------------------------------------------------------------
// DisposeOwnedOutputBuffers
// -----------------------------------------------------------------------------
// Disposes all of this ImageBufferMap's output image buffers.
void
DisposeOwnedOutputBuffers(
	IsadoraParameters*	ip,
	ImageBufferMap*		inBufferMap);


// -----------------------------------------------------------------------------
// UpdateImageBufferMap
// -----------------------------------------------------------------------------
// Updates the image buffer map based on the current status of the two input
// image maps. If needed, this function will create intermediary image maps based
// on the current Isadora preference settings. It will also create an output
// buffer that matches the characteristics of the two input (and/or intermediary
// buffers)

Boolean
UpdateImageBufferMap(
	IsadoraParameters*	ip,
	ImageBufferMap*		inBufferMap);

// -----------------------------------------------------------------------------
// UpdateImageBufferMapNotifyRemap
// -----------------------------------------------------------------------------
// Same as UpdateImageBufferMap, but returns a flag whether or not a remap
// of the buffers.

Boolean
UpdateImageBufferMapNotifyRemap(
	IsadoraParameters*	ip,
	ImageBufferMap*		inBufferMap,
	Boolean*			outRemapped);			// CAN BE NIL

// -----------------------------------------------------------------------------
// UpdateImageBufferMapOverrideMode
// -----------------------------------------------------------------------------
// Same as UpdateImageBufferMapNotifyRemap except that you can override the 
// default image processing mode (normally set in the Preferences by the user)
// if desired

Boolean
UpdateImageBufferMapOverrideMode(
	IsadoraParameters*	ip,
	ImageBufferMap*		inBufferMap,
	ImageProcessingMode	inImageProcessingMode,				// kImageProcessUseDefault for default settings, or
															// kImageProcessLargerSlower, kImageProcessSmallerFaster, kImageProcessFixedSize, kImageProcessMatchFormatOnly
	UInt32				inBaseOutputBufferOnInputIndex0,	// only needed when inImageProcessingMode == kImageProcessMatchFormatOnly
	OSType				inForceFormat,						// if non zero -- force format to specified value
	Boolean*			outRemapped);						// CAN BE NIL

// -----------------------------------------------------------------------------
// GetImageBufferPtr
// -----------------------------------------------------------------------------
// Returns a pointer to the current image data in the input buffer indicated by
// the index value. If necessary, the image buffer is copied to the an inter-
// mediary buffer that matches the scale of the entire image buffer.

ImageBufferPtr
GetImageBufferPtr(
	IsadoraParameters*	ip,
	ImageBufferMap*		inBufferMap,
	UInt32				inIndex0);
	
// -----------------------------------------------------------------------------
// GetImageBufferPtr
// -----------------------------------------------------------------------------
// Returns a pointer to the current image data in the input buffer indicated by
// the index value. If necessary, the image buffer is copied to the an inter-
// mediary buffer that matches the scale of the entire image buffer.

inline ImageBufferPtr
GetOutputImageBufferPtr(
	ImageBufferMap*		inBufferMap,
	UInt32				inIndex0)
{
	if (inBufferMap->mOutputBuffersValid != 0
	&& inBufferMap->mOutputBufferPtrs != nil
	&& inIndex0 < inBufferMap->mOutputBufferCount) {
		return inBufferMap->mOutputBufferPtrs[inIndex0];
	}
	
	return nil;
}
	
// -----------------------------------------------------------------------------
// GetImageBufferPtrDontScale
// -----------------------------------------------------------------------------
// Returns a pointer to the current image data in the input buffer indicated by
// the index value. The actual image buffer ptr is returned -- no scaling to the
// intermediary buffer is done.

ImageBufferPtr
GetImageBufferPtrDontScale(
	IsadoraParameters*	ip,
	ImageBufferMap*		inBufferMap,
	UInt32				inIndex0);
	
// -----------------------------------------------------------------------------
// PrepareToDrawIntoImageBuffer
// -----------------------------------------------------------------------------
// Before calling quickdraw routines to draw into an image buffer, the client
// should call this routine to prepare the buffer.

bool
PrepareToDrawIntoImageBuffer(
	IsadoraParameters*	ip,
	ImageBufferPtr		inBuffer,
	void**				outSaveGWorld,
	void**				outSaveGDHandle);
	
// -----------------------------------------------------------------------------
// DoneDrawingIntoImageBuffer
// -----------------------------------------------------------------------------
// Should be called when the client has completed all quickdraw calls used to
// draw in a buffer.

void
DoneDrawingIntoImageBuffer(
	IsadoraParameters*	ip,
	void*				inSaveGWorld,
	void*				inSaveGDHandle);
	
		
#endif
