// ===========================================================================
// UGraphics				(C) 2005 Mark F. Coniglio.  all rights reserved.
// ===========================================================================
// Isadora abstracts all drawing through the UGraphics interface. You can use
// this class to draw so that all drawing is cross platform comaptible.

#ifndef _H_UGraphics
#define _H_UGraphics

#include "UGraphicsCommon.h"
#include "GraphicsHelpers.h"
#include <vector>
#include <stack>
#include <math.h>

#include "ColorRefs.h"
#include "3DDrawFlags.h"
#include "UTextTraits.h"

class	LView;
class	UClipRgn;
class	UPolygon;
class	URegion;
class	CImageBuffer;

struct	ImageBuffer;
struct 	TextTraitsRecord;

// ---------------------------------------------------------------------------
//	DbgGuiLeak
// ---------------------------------------------------------------------------
#define ENABLE_GUI_DEBUG_LEAK_TEST		(FALSE && GDI_DRAW && DEBUG)
class DbgGuiLeak
{
public:
	explicit DbgGuiLeak ()
	{
		#if ENABLE_GUI_DEBUG_LEAK_TEST
		_guiResCount = ::GetGuiResources(::GetCurrentProcess(), GR_GDIOBJECTS);
		#endif
	}
	
	~DbgGuiLeak ()
	{
		#if ENABLE_GUI_DEBUG_LEAK_TEST
		int leaks = ::GetGuiResources (::GetCurrentProcess (), GR_GDIOBJECTS) - _guiResCount;
		if (leaks != 0) {
			log_info( "Gui Resources Leaked: %d\n", leaks);
		}
		#endif
	}

private:
	#if ENABLE_GUI_DEBUG_LEAK_TEST
	unsigned _guiResCount;
	#endif
};

// ---------------------------------------------------------------------------

class FloatARGB
{
public:
	FloatARGB()
	{
	}
	
	FloatARGB(
		const RGBColor& inColor)
	{
		alpha = 1.0;
		red = (float) inColor.red / 65535.0;
		green = (float) inColor.green / 65535.0;
		blue = (float) inColor.blue / 65535.0;
	}
	
	FloatARGB& operator = (const RGBColor& inColor)
	{
		alpha = 1.0;
		red = (float) inColor.red / 65535.0;
		green = (float) inColor.green / 65535.0;
		blue = (float) inColor.blue / 65535.0;
		return (*this);
	}
	
	float	red;
	float	green;
	float	blue;
	float	alpha;
};

// ---------------------------------------------------------------------------

class UGraphics
{
public:

			UGraphics();
			
			~UGraphics();
	
		// FOCUS
	inline GrafPtr	GetMacPort() const
						{ return mMacPort; }
						
	inline bool		IsFocused() const
					{
						#if QD_DRAW
							return mMacPort != nil;
						#elif GDI_DRAW
							return mFocusedView != nil;
						#endif
					}
				
	bool	Focus(
				LView*	inView);
				
	void	Unfocus();
	
	void	Flush();

private:
	void	UnfocusIfFocused();

public:
	void	GetResolution(
				float	*outH,
				float	*outV);
	
		// ORIGIN
	void	SetOrigin(
				SInt16	inH,
				SInt16	inV);
	
	void	SetOriginMaintainClip(
				SInt16	inH,
				SInt16	inV);
	
	void	GetOrigin(
				SInt16*	outH,
				SInt16*	outV) const;
	
		// CLIP
#if GDI_DRAW
	void	AdoptCurrentGDIClip();
#endif

		// COLORS
	inline static
	UInt16	CalcColorPercent(
				UInt16			inOrigColor,
				float			inPercent)
			{
				SInt32 outResult = (SInt32)(inOrigColor * inPercent / 100.0);
				if (outResult < 0)
					outResult = 0;
				if (outResult > 0x0000FFFF)
					outResult = 0xFFFF;

				return (UInt16) outResult;
			}

	static inline
	void	GetGrayPercent(
				float		inValue,
				RGBColor*	outColor)
			{
				outColor->red = outColor->green = outColor->blue = (UInt16) (0xFFFF * (inValue / 100.0));
			}

	inline static
	void	RGBToColor(
				UInt8			inRed,
				UInt8			inGreen,
				UInt8			inBlue,
				RGBColor*		outColor)
			{
				outColor->red	= (((UInt16) inRed << 8) | inRed);
				outColor->green	= (((UInt16) inGreen << 8) | inGreen);
				outColor->blue	= (((UInt16) inBlue << 8) | inBlue);
			}

	static
	void	ChangeBrightnessPercent(
				RGBColor*	inColor,
				float		inPercent);

	void	SetForegroundGrayPercent(
				float			inPercent);
				
	void	SetBackgroundGrayPercent(
				float			inPercent);
				
	void	SetForegroundColorRef(
				ColorRef		inColorRef);
	
	void	SetBackgroundColorRef(
				ColorRef		inColorRef);

	void	SetForegroundRGBColor(
				UInt8 inRed,
				UInt8 inGreen,
				UInt8 inBlue);
	
	void	GetHighlightColor(
				RGBColor*		outColor);
				
	void	GetForeColor(
				RGBColor*		inColor);
				
	void	GetBackColor(
				RGBColor*		inColor);
				
	void	RGBForeColor(
				const RGBColor*	inColor);
	
	void	RGBBackColor(
				const RGBColor*	inColor);
	
	OSStatus DrawThemeListBoxFrame(
				const Rect *			inRect,
				ThemeDrawState			inState);

	void	SetThemeBackground(
				ThemeBrush   inBrush,
				SInt16       inDepth,
				bool	      inIsColorDevice);
	
	void	BackPat(
				const Pattern*	inPattern);
				
		// PEN AND LINES
	void	PenSize(
				SInt16			inPenSize);
	
	void	PenNormal();
	
	void	GetPenState(
				PenState*		inPenState);
				
	void	SetPenState(
				const PenState*	inPenState);
				
	void	MoveTo(
				SInt16				inX,
				SInt16				inY);
	
	void	LineTo(
				SInt16				inX,
				SInt16				inY);
	
	void	DrawLine(
				SInt32				inX1,
				SInt32				inY1,
				SInt32				inX2,
				SInt32				inY2);
				
	void	DrawLines(
				SInt32				inLineCount,						// number of lines to draw
				const Point*		inPoints);							// will be inLineCount points in array
	
	void	DrawLines(
				SInt32				inLineCount,						// number of lines to draw
				const Point*		inPoints,							// will be inLineCount points in array
				SInt32				inHOffset,							// horizontal offset
				SInt32				inVOffset,							// vertical offset
				SInt32				inStartPointH,
				SInt32				inStartPointV,
				SInt32				inDestPointH = UGRAPHICS_DONTDRAW,	// extra destination point if not UGRAPHICS_DONTDRAW
				SInt32				inDestPointV = UGRAPHICS_DONTDRAW,	// extra destination point if not UGRAPHICS_DONTDRAW
				UInt32				inSkipBytesExtra = 0);				// extra bytes to skip on each iteration
	
	void	Move(
				SInt16				inX,
				SInt16				inY);
	
	void	Line(
				SInt16				inX,
				SInt16				inY);
	
	void	DrawLine(
				SInt16				x1,
				SInt16				y1,
				SInt16				x2,
				SInt16				y2,
				SInt16				inLineWidth)
			{
				PenSize(inLineWidth);
				MoveTo(x1,y1);
				LineTo(x2,y2);
			}
	
		// SHAPES
	void	EraseRect(
				const Rect*			inRect);
	
	void	PaintRect(
				const Rect*			inRect);
	
	void	InvertRect(
				const Rect*			inRect);
	
	void	PaintOval(
				const Rect*			inRect);
	
	void	PaintRoundRect(
				const Rect*			inRect,
				SInt16				inHCurve,
				SInt16				inVCurve);
	
	void	FrameRect(
				const Rect*			inRect);
	
	void	FrameOval(
				const Rect*			inRect);
	
	void	FrameRoundRect(
				const Rect*			inRect,
				SInt16				inHCurve,
				SInt16				inVCurve);
	
	void	PaintRgn(
				RgnHandle			inRgn);
	
	void	FillCRgn(
				RgnHandle			inRgn,
				PixPatHandle		inPixPat);

	void	DrawShadedBoxPercent(
				const Rect*	inBox,
				ColorRef	bkgColor,
				float		inTopLeftPercent,
				float		inBotRightPercent);

	void	DrawShadedOutlinePercent(
				const Rect*	inBox,
				ColorRef	bkgColor,
				float		topLeftPercent,
				float		botRightPercent);
				
	void	Draw3DRect(
				const Rect&	inRect,
				ColorRef	inFillColor,
				double		inLightenPercent,
				double		inDarkenPercent,
				SInt16		inBorderWidth,
				BorderFlags	inBorderFlags,
				bool		inInset,
				bool		inDrawBordersOnly = false);
	
		// PATTERNS
	void	PenPat(
				const Pattern*	inPattern);

		// POLYGONS
	UPolygon*	OpenPolygon();
	
	void		ClosePolygon();
	
	void		KillPolygon(
					UPolygon*	inPoly);

	void		FillAndFramePolygon(
					const UPolygon*	inPoly,
					const RGBColor&	inFillColor,
					const RGBColor&	inFrameColor,
					SInt32			inFrameSize);

	void		FramePolygon(
					const UPolygon*	inPoly,
					const RGBColor&	inFrameColor,
					SInt32			inFrameSize);

	void		PaintPolygon(
					const UPolygon*	inPoly,
					const RGBColor&	inFillColor);
					
	RgnHandle	PolygonToQDRegion(
					const UPolygon*	inPoly);
	
		// Regions
	URegion*	OpenRegion();
	
	void		CloseRegion();
	
	void		KillRegion(
					URegion*	inRegion);
	
	void		AccumulateRegion(
					const Rect&	inRect);

	void		AccumulateRoundRectRegion(
					const Rect&	inRect,
					SInt16		inRadiusH,
					SInt16		inRadiusV);

	void		AccumulateOvalRegion(
					const Rect&	inRect);

	static void	OffsetRegion(
					URegion*	inRegion,
					SInt16		inOffsetH,
					SInt16		inOffsetV);

	static bool	IsRegionEmpty(
					const URegion*	inRegion);
			
	void		InvalidateRegion(
					const URegion*	inRegion);

	void		FrameRegion(
					const URegion*	inRegion);

	void		PaintRegion(
					const URegion*	inRegion);

		// FONTS	
	void	ForceFontChanged()
				{
					mFontNeedsUpdate = true;
				}
	
	void	SetupFont(
				const char*			inFontName,
				SInt16				inFontSize,
				SInt16				inFontStyle,
				SInt16				inFontMode);
				
	void	TextFont(
				SInt16				inFont);
				
	void	TextSize(
				SInt16				inSize);
				
	void	TextFace(
				Style				inStyle);
				
	void	TextMode(
				SInt16				inStyle);
				
	void	PenMode(
				SInt16				inStyle);
				
	void	SetBlendModeAlpha(
				float				inAlpha);	// 0.0 -> 1.0
				
	SInt32	PStringWidth(
				ConstStr255Param	inString)
			{
				return PrivateTextWidth(inString+1, inString[0], false);
			}

	SInt32	CStringWidth(
				const char*			inString)
			{
				return PrivateTextWidth(inString, std::strlen(inString), false);
			}

	// MFC 11/15/07: added this function to allow measurement of UT8 encoded strings
	SInt32	UTF8CStringWidth(
				const char*			inString)
			{
				return PrivateTextWidth(inString, std::strlen(inString), true);
			}

	// MFC 11/15/07: changed this function to call PrivateTextWidth with the UTF8 flag set to false
	SInt32	TextWidth(
				const void*			inString,
				SInt32				inStringLen)
			{
				return PrivateTextWidth(inString, inStringLen, false);
			}

	// MFC 11/15/07: added this function to allow measurement of UT8 encoded strings
	SInt32	UTF8TextWidth(
				const void*			inString,
				SInt32				inStringLen)
			{
				return PrivateTextWidth(inString, inStringLen, true);
			}

private:
	// MFC 11/15/07: added this function to allow measurement of UT8 encoded strings
	SInt32	PrivateTextWidth(
				const void*			inString,
				SInt32				inStringLen,
				bool				inUTF8TextFormat);

public:
	inline void	DrawPStringAt(
					SInt16				h,
					SInt16				v,
					ConstStr255Param	inString,
					const RGBColor*		inColor = nil,
					const Rect*			inClipRect = nil)
				{
					DrawTextAt(h, v, inString+1, inString[0], inColor, inClipRect);
				}
			
	// MFC 11/15/07: changed this function to call PrivateDrawTextAt with the
	// UTF8 flag set to false
	inline void	DrawCStringAt(
					SInt16				h,
					SInt16				v,
					const char*			inString,
					const RGBColor*		inColor = nil,
					const Rect*			inClipRect = nil)
				{
					PrivateDrawTextAt(h, v, inString, std::strlen(inString), inColor, inClipRect, false);
				}
				
	// MFC 11/15/07: added this function to call PrivateDrawTextAt with the
	// UTF8 flag set to true
	inline void	DrawUTF8CStringAt(
					SInt16				h,
					SInt16				v,
					const char*			inString,
					const RGBColor*		inColor = nil,
					const Rect*			inClipRect = nil)
				{
					PrivateDrawTextAt(h, v, inString, std::strlen(inString), inColor, inClipRect, true);
				}
				
	// MFC 11/15/07: added this function to call PrivateDrawTextAt with the
	// UTF8 flag set to false
	void		DrawTextAt(
					SInt16				h,
					SInt16				v,
					const void*			inString,
					SInt32				inStringLen,
					const RGBColor*		inColor = nil,
					const Rect*			inClipRect = nil)
				{
					PrivateDrawTextAt(h, v, inString, inStringLen, inColor, inClipRect, false);
				}

	// MFC 11/15/07: added this function to call PrivateDrawTextAt with the
	// UTF8 flag set to true
	void		DrawUTF8TextAt(
					SInt16				h,
					SInt16				v,
					const void*			inString,
					SInt32				inStringLen,
					const RGBColor*		inColor = nil,
					const Rect*			inClipRect = nil)
				{
					PrivateDrawTextAt(h, v, inString, inStringLen, inColor, inClipRect, true);
				}

private:
	void		PrivateDrawTextAt(
					SInt16				h,
					SInt16				v,
					const void*			inString,
					SInt32				inStringLen,
					const RGBColor*		inColor /* = nil */,
					const Rect*			inClipRect /* = nil */,
					bool				inUTF8TextFormat = false);

public:
	SInt16	GetPortTextFont() const;
	
	SInt16	GetPortTextSize() const;
	
	Style	GetPortTextFace() const;
	
	SInt16	GetPortTextMode() const;
	
	void	GetFontInfo(
				FontInfo*			fi);
				
	void	SetFontStyleOverride(
				const ControlFontStyleRec&	inStyle);
				
	SInt16		SetPortTextTraits(
					SInt16				inTextTraitsID);
			
	SInt16		SetPortTextTraits(
					const TextTraitsRecord*	inTextTraits);
				
	void		LoadTextTraits(
					TextTraitsPtr	ioTextTraits);
				
	TextTraitsH	LoadTextTraits(
					ResIDT	inTextTraitsID);
					
	SInt16	GetJustifiedTextHeight(
				const void*		inText,
				SInt32			inLength,
				const Rect&		inRect,
				bool			inOneLineOnly);
	
	static
	SInt16	GetActualJustification( SInt16 inJustification );

	SInt16	TruncString(
				short       width,
				Str255      theString,
				TruncCode   truncWhere);
	
	void	DrawTruncatedWithJust(
				ConstStringPtr	inString,
		  		const Rect&		inRect,
		  		SInt16			inJustification,
				bool			inTruncateString = true,
		  		TruncCode		inTruncWhere = truncEnd);

	void	DrawWithJustification(
				const void*		inText,
				SInt32			inLength,
				const Rect&		inRect,
				SInt16			inHorizJustification,
				SInt16			inVertJustification,	// 1 = top, 2 = middle, 3 = bottom
				bool			inOneLineOnly);
		
	
		// PICTURES
	void		DrawPictureWithID(
					ResIDT			inPicResID,
					const Rect&		inRect,
					const Rect&		inClipRect);

	void		DrawPictureWithID(
					ResIDT			inPicResID,
					const Rect&		inRect,
					const RgnHandle	inClipRgn = NULL);

	void		DrawPicture(
					PicHandle		inPicHandle,
					const Rect&		inRect,
					const Rect&		inClipRect);

	void		DrawPicture(
					PicHandle		inPicHandle,
					const Rect&		inRect,
					const RgnHandle	inClipRgn = NULL);

	void		DrawPictureWithAlpha(
					PicHandle		inPicHandle,
					const Rect&		inRect);

	static CImageBuffer*	MakeImageBufferFromPicture(
								PicHandle		inPicHandle,
								Rect*			inDestRect = NULL);
	
	static CImageBuffer*	MakeImageBufferFromPictureWithID(
								ResIDT			inPicResID,
								Rect*			inDestRect = NULL);
	
	static GWorldPtr		MakeGWorldFromPicture(
								PicHandle		inPicHandle,
								SInt16			inPixelDepth,
								Rect*			inDestRect = NULL,
								OSType			inPixelFormat = kIzzyRGBFormat,
								bool*			outCheckForAlpha = NULL);
		
	static GWorldPtr		MakeGWorldFromPictureWithID(
								ResIDT			inPicResID,
								SInt16			inPixelDepth,
								Rect*			inDestRect = NULL,
								OSType			inPixelFormat = kIzzyRGBFormat,
								bool*			outCheckForAlpha = NULL);
		
	static bool				SplitImageBufferIntoThreeParts(
								CImageBuffer&		inSrc,
								bool				inHorizontal,
								UInt32				inEdgePixels,
								CImageBuffer*		outLeft,
								CImageBuffer*		outMiddle,
								CImageBuffer*		outRight);

	void					DrawThreePartImageIntoRect(
								bool				inHorizontal,
								const CImageBuffer*	inLeft,
								const CImageBuffer*	inMiddle,
								const CImageBuffer*	inRight,
								const Rect&			inRect,
								bool				inTileMiddle = false);

		// ICONS

	void		PlotCIcon(
					  const Rect *  theRect,
					  CIconHandle   theIcon);

	static void	PlotIconWithID(
					const Rect *  theRect,
					ResIDT        inIconID);
					
	static void	PlotIcon(
					const Rect *  theRect,
					Handle        theIcon);
					
	static OSErr PlotIconID(
					const Rect *        theRect,
					IconAlignmentType   align,
					IconTransformType   transform,
					SInt16              theResID);
					
		// GWORLDS
	void		DrawGWorld(
					GWorldPtr		inGWorld,
					const Rect&		inSrcRect,
					const Rect&		inDstRect,
					bool			inHasAlpha);

	void		DrawGWorld(
					GWorldPtr		inGWorld,
					const Rect&		inDstRect,
					bool			inHasAlpha);

	static void		DrawGWorldWithAlpha(
						GWorldPtr		inSrcWorld,
						GWorldPtr		inDstWorld,
						const Rect*		inSrcRect,		// CAN BE NULL
						const Rect*		inDestRect);
					
	static void		DrawGWorldWithoutAlpha(
						GWorldPtr		inSrcWorld,
						GWorldPtr		inDstWorld,
						const Rect*		inSrcRect,		// CAN BE NULL
						const Rect*		inDestRect);
					
	static void		GetGWorldBounds(
						GWorldPtr		inGWorld,
						Rect*			outRect);

	void		DrawCImageBuffer(
					const CImageBuffer*	inImageBuffer,
					const Rect&			inDstRect,
					bool				inUseAlpha);
					
	void		DrawCImageBuffer(
					const CImageBuffer*	inImageBuffer,
					const Rect&			inSrcRect,
					const Rect&			inDstRect,
					bool				inUseAlpha);
	
	void		DrawImageBuffer(
					const struct ImageBuffer*	inImageBuffer,
					const Rect&			inSrcRect,
					const Rect&			inDstRect,
					bool				inUseAlpha = true);
					
	void		DrawImageBuffer(
					const struct ImageBuffer*	inImageBuffer,
					const Rect&			inDstRect,
					bool				inUseAlpha = true);
					
	static void	DrawGWorldToGrafPort(
					GWorldPtr		inSrcGWorld,
					GrafPtr			inDstPort,
					const Rect&		inDstRect);

#if TARGET_OS_WIN32
	static void		DrawPremultipliedAlphaHBITMAP(
						HDC				inDstHDC,
						HBITMAP			inSrcBitmap,
						const Rect&		inSrcRect,
						const Rect&		inDestRect);

	static HBITMAP	CreatePremultipliedDIBSectionFromAlphaGWorld(
						GWorldPtr		inGWorld);

	static HBITMAP	CreatePremultipliedDIBSectionFromCIcon(
						ResIDT			inCIconResID);

	static void		DrawAlphaGWorldAsHBITMAP(
						HDC				inDstHDC,
						GWorldPtr		inGWorld,
						const Rect&		inSrcRect,
						const Rect&		inDstRect);

	static HBITMAP	CreateNonAlphaDIBSectionGWorld(
						GWorldPtr		inGWorld);

	static void		DrawStandardGWorldAsHBITMAP(
						HDC				inDstHDC,
						GWorldPtr		inGWorld,
						const Rect&		inSrcRect,
						const Rect&		inDstRect);

	static void		DrawHBITMAP(
						HDC				inDstHDC,
						HBITMAP			inDIBSection,
						const Rect&		inSrcRect,
						const Rect&		inDstRect,
						bool			inHasAlpha = false);
#endif

	// CLIPPING
#if GDI_DRAW
	RgnHandle	GetCurrentGDIClipAsQDClip();

	RgnHandle	ConvertGDIClipToQDClip(
					HRGN		inRegion);

	HRGN		ConvertQDClipToGDIClip(
					RgnHandle	inRegion);
#endif

	void		SetQDClipRgnAndSyncGDIClip(
					RgnHandle		inQDClipRgn);
						
	void		GetQDClipRgn(
					RgnHandle		inQDClipRgn);
	
	void		SubtractFromCurrentQDClipRgn(
					RgnHandle		inQDClipRgn);
					
	void		ClipChanged();
	
	void		PushClip();
	
	void		PushClipAndSectWithRect(
					const Rect&		inRect);
	
	void		PopClip();
	
	void		DisposeClipRgn(
					UClipRgn*	inClip);
	
	UClipRgn*	GetClip();
	
	void		SetClip(
					UClipRgn*	inClip);
	
	void		SetClip(
					const Rect&	inRect);
	
	void		SectCurrentClipWithRoundRect(
					const Rect&	inRect,
					SInt16		inRadiusH,
					SInt16		inRadiusV);
			
	void		SectCurrentClipWithRgn(
					UClipRgn*	inClip);
	
	void		SectCurrentClipWithRect(
					const Rect&	inRect);

	void		SubtractFromCurrentClip(
					const Rect&	inRect);

#define DEBUG_UPDATE_REGION_ACTIVE	(TRUE && GDI_DRAW && DEBUG)

#if DEBUG_UPDATE_REGION_ACTIVE
	void		DebugWindowsUpdateRegion();
#else
	#define		DebugWindowsUpdateRegion() { }
#endif

		// CG CONTEXT				
#if USE_CG_CONTEXT
	void		FocusCGContext();
	void		UnfocusCGContext();
	void		SyncCGContext();
	void		RefocusCGContext();
	void		PushCGContextState();
	void		PopCGContextState();
#endif

	void	UpdateFont();

#if GDI_DRAW
	inline HDC	GetHDC()
					{ return mHDC; }
private:
	void	DoFocus(
				LView*	inFocusView);
				
	void	DoUnfocus();
				
	void	UpdateBrush();
	
	void	UpdatePen();
	
	HBRUSH	CreateWindowsBrushWithColor(
				const RGBColor&		inColor);
				
	HPEN	CreateWindowsPenWithColor(
				const RGBColor&		inColor,
				UInt32				inPenSize);

#endif

#if USE_CG_CONTEXT
private:
	void	SetFillColor(
				SInt16				inPenMode,
				const FloatARGB&	inColor);
				
	void	SetStrokeColor(
				SInt16				inPenMode,
				const FloatARGB&	inColor,
				float				inPenSize);
				
	void	PrepareToStroke();
	void	PrepareToFill();
#endif
	
private:

	LView*					mFocusedView;
	GrafPtr					mMacPort;

	std::stack<UClipRgn*>	mClipStack;
	
	SInt32			mPenMode;
	RGBColor		mForeColor;
	RGBColor		mBackColor;
	float			mBlendModeAlpha;
	ThemeBrush		mThemeBrush;
	
	Point			mMacOrigin;

	UPolygon*		mPolygon;
	URegion*		mRegion;

#if USE_CG_CONTEXT
	CGContextRef	mCGContext;
	bool			mCGContextFocused;
	SInt32			mContextSaveCount;
#endif

#if USE_CG_CONTEXT
	float			mFloatPenSize;
	FloatARGB		mFloatForeColor;
	FloatARGB		mFloatBackColor;
#endif

	bool		mFontNeedsUpdate;
	SInt16		mCurFontNum;
	SInt16		mCurFontSize;
	SInt16		mCurFontStyle;
	SInt16		mCurFontMode;
	FontInfo	mCurFontInfo;

#if GDI_DRAW
	DbgGuiLeak*	mGUICheck;
	
	HWND		mHWND;
	
	HDC			mHDC;
	

	bool		mIsOffscreen;
	
	POINT		mOldOrigin;
	SInt32		mViewOffsetH;
	SInt32		mViewOffsetV;

	HRGN		mSaveGDIClipRgn;
	
	// font information
	HFONT		mCurrentFont;

	// pen information
	bool		mPenNeedsUpdate;
	SInt16		mPenSize;
	Point		mPenLoc;
	HPEN		mCurrentPen;

	// foreground color
	HBRUSH		mForeBrush;
	HBRUSH		mBackBrush;
	
	// text alignment
	UINT		mSavedTextAlign;
	// background mode
	int			mSavedBackgroundMode; 
	
#endif
};

// ---------------------------------------------------------------------------

class StUGraphics
{
public:
						StUGraphics();
						
						~StUGraphics();
	
	inline UGraphics*	GetGraphics()
							{ return mGraphics; }
private:
	bool			mWasFocused;
	UGraphics*		mGraphics;
};
		
// ---------------------------------------------------------------------------

class StGraphicsClipRgnState {

public:
				StGraphicsClipRgnState (
					UGraphics*	inGraphics);
					
				~StGraphicsClipRgnState();
	
	void		ClipToDifference(const Rect&	inRect);

#if !USE_CG_CONTEXT
private:
	void		Save();
	
	void		DisposeSavedRgn();
	
	void		Restore();
#endif

private:
	UGraphics*	mGraphics;
#if !USE_CG_CONTEXT
	UClipRgn*	mClipRgn;
#endif
};
#endif
