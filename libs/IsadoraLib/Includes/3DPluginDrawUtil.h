// ===========================================================================
//	3DPluginDrawUtil			  (C)2007 Mark F. Coniglio. All rights reserved.
// ===========================================================================

#ifndef _H_3DPluginDrawUtil
#define	_H_3DPluginDrawUtil

#include "IsadoraTypes.h"
#include "ImageBufferCommon.h"
#include "IsadoraCallbacks.h"

#ifdef __APPLE_CC__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#elif defined(WIN32)
//#  include <gl/glaux.h>
#  include <gl/gl.h>
#  include <gl/glu.h>
#else
#  include <glaux.h>
#  include <gl.h>
#  include <glu.h>
#endif

#if TARGET_OS_MAC
#  ifndef __APPLE_CC__
#    include <glext.h>
#  endif
#elif TARGET_OS_WIN32
#  include <wingdi.h>
#  include "wglext.h" // Possibly the one from Isadora/Source/GL Extensions.
#  include "glext.h"  // Possibly the one from Isadora/Source/GL Extensions.
#endif

enum ContextType {
	kWindowRenderContext = 0,
	kBitmapRenderContext = 1
};
	

class CPluginTextureMap
{
public:
			CPluginTextureMap();
	
	void	InitPluginTextureMap(
				IsadoraParameters*	inIzzyParams,
				ActorInfo*			inActorInfo);

	void	SetRenderContextType(
				ContextType		inContextType);
					
	void	SetRenderChannel(
				SInt32				inRenderChannel);
				
	void	CreateTextureMap(
				ImageBufferPtr		inImageBufferPtr,
				bool				inStoreFirstTextureMapImage = false,
				bool				inRepeat = false);

	void	DisposeTextureMap();
		
	void	HandleTextureMapInput(
				Value*				inValue);

	bool	UpdateTextureMap(
				bool	inRepeat = false);

	bool	PrepareForRender(
				float*	outHScale,
				float*	outVScale);

	bool	TexMapCoordsArePixels() const
				{ return (mTextureMapInfo.mTexMapCoordsArePixels != 0); }
	
	inline ImageBufferPtr	GetTextureMap()
								{ return mImageBufferPtr; }
				
	inline int				GetTextureMapName() const
								{ return mTextureMapInfo.mTextureName; }
	
	inline UInt32			GetTextureMappingMode() const
								{ return mTextureMapInfo.mTextureMappingMode; }

private:
	IsadoraParameters*		ip;
	ActorInfo*				mActorInfo;
	
	ContextType				mContextType;
	SInt32					mChannel;
	ImageBufferPtr			mImageBufferPtr;

	TextureMapInfo			mTextureMapInfo;
	IBFormatInfo			mLastFormat;
	
	UInt32					mImgBufDataChangeCount;

};

class CPlugin3DRender
{
public:
			CPlugin3DRender();
	
	void	InitRender3DManager(
				IsadoraParameters*	inIzzyParams,
				ActorInfo*			inActorInfo);

	void	SetRenderInfo(
				Render3DCallback	inCallback,
				ThingPtr			inThing,
				void*				inRefCon1,
				void*				inRefCon2);

	void	Attach();
				
	void	Detach();
	
	void	Update();
	
	void	MarkNeedsDraw();
	
	ContextType	GetPendingRenderContextType() const
					{ return mPendingContextType; }
					
	void	SetPendingRenderContextType(
				ContextType		inPendingContext)
					{ mPendingContextType = inPendingContext; }
					
	void	SetPendingChannel(
				SInt32			inPendingChannel)
					{ mPendingChannel = inPendingChannel; }
					
	void	SetPendingLayer(
				bool			inPendingLayer)
					{ mPendingLayer = inPendingLayer; }
					
	void	SetPendingVisible(
				bool			inPendingVisible)
					{ mPendingVisible = inPendingVisible; }
					
private:
	IsadoraParameters*		ip;
	ActorInfo*				mActorInfo;
	
	Render3DInfo			mRenderInfo;

	ContextType				mCurrentContextType;
	ContextType				mPendingContextType;
	
	SInt32					mCurrentChannel;
	SInt32					mPendingChannel;
	
	bool					mCurrentVisible;
	bool					mPendingVisible;

	SInt32					mCurrentLayer;
	SInt32					mPendingLayer;
};

void
HardwareIDAlert(
	const char*	inErrorString,
	const char*	inInfoString);
	
#endif
