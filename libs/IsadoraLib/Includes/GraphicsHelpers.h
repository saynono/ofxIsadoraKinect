// ===========================================================================
// GraphicsHelpers				(C) 2010 Mark F. Coniglio.  all rights reserved.
// ===========================================================================

#ifndef _H_GraphicsHelpers
#define _H_GraphicsHelpers
#pragma once

#ifdef __cplusplus

#define _min2(a,b) ((a) < (b)) ? (a) : (b)
#define _max2(a,b) ((a) > (b)) ? (a) : (b)
// #define _sect_lines_(a1, a2, b1, b2, o1, o2)	{ (o1) = (o2) = 0; if ((a1) < (a2) && (b1) < (b2)) { if ((a1) < (b1) && (a2) > (b1)) { (o1) = max_2(b1); (o2) = (a2); } else if ((b1) < (a1) && (b2) > (a1)) { (o1) = (a1); (o2) = (b2); } } }

inline void
_sect_lines_(
	SInt16	a1,
	SInt16	a2,
	SInt16	b1,
	SInt16	b2,
	SInt16&	o1,
	SInt16&	o2)
{
	o1 = o2 = 0;
	// if the two line lengths are > 0
	if ((a1) < (a2) && (b1) < (b2)) {
		// if startpoint of segment a is to the left of b, and endpoint of a > startpoint of b
		if ((a1) <= (b1) && (a2) > (b1)) {
			o1 = _max2(a1, b1);
			o2 = _min2(a2, b2);
		// if startpoint of segment b is to the left of a, and endpoint of a > startpoint of b
		} else if ((b1) <= (a1) && (b2) > (a1)) {
			o1 = _max2(a1, b1);
			o2 = _min2(a2, b2);
		}
	}
}

// POINT FUNCTIONS

inline bool
IzzyPtInRect(
	Point				inPoint,
	const Rect*			inRect)
{
	return (inPoint.h >= inRect->left && inPoint.h < inRect->right) && (inPoint.v >= inRect->top && inPoint.v < inRect->bottom);
}

inline void
IzzyAddPoints(
	Point	inA,
	Point	inB,
	Point*	outPt)
{
	outPt->h = inA.h + inB.h;
	outPt->v = inA.v + inB.v;
}

// subtracts point b from point a
inline void
IzzySubPoints(
	Point	inFromPt,
	Point	inSubPt,
	Point*	outPt)
{
	outPt->h = inFromPt.h - inSubPt.h;
	outPt->v = inFromPt.v - inSubPt.v;
}

// RECTANGLE MANIPULATION

inline void
IzzySetRect(
	Rect*				ioRect,
	SInt16				inLeft,
	SInt16				inTop,
	SInt16				inRight,
	SInt16				inBottom)
{
	ioRect->left	= inLeft;
	ioRect->right	= inRight;
	ioRect->top		= inTop;
	ioRect->bottom	= inBottom;
}

inline void
IzzyOffsetRect(
	Rect*		ioRect,
	SInt16		dx,
	SInt16		dy)
{
	ioRect->left	+= dx;
	ioRect->right	+= dx;
	ioRect->top		+= dy;
	ioRect->bottom	+= dy;
}

inline void
IzzyInsetRect(
	Rect*		ioRect,
	SInt16		dx,
	SInt16		dy)
{
	ioRect->left	+= dx;
	ioRect->right	-= dx;
	ioRect->top		+= dy;
	ioRect->bottom	-= dy;
}

inline bool
IzzyEmptyRect(
	const Rect*			inRect)
{
	return (inRect->left >= inRect->right || inRect->top >= inRect->bottom);
}

inline bool
IzzyEqualRect(
	const Rect*			r1,
	const Rect*			r2)
{
	return ((r1)->left == (r2)->left && (r1)->top == (r2)->top && (r1)->right == (r2)->right && (r1)->bottom == (r2)->bottom);
}

inline void
IzzyUnionRect(
  const Rect *  s1,
  const Rect *  s2,
  Rect *        d)
{
	bool empty1 = IzzyEmptyRect(s1);
	bool empty2 = IzzyEmptyRect(s2);
	
	if (empty1 == false && empty2 == false) {
		(d)->left = _min2((s1)->left, (s2)->left); (d)->top = _min2((s1)->top, (s2)->top); (d)->right = _max2((s1)->right, (s2)->right); (d)->bottom = _max2((s1)->bottom, (s2)->bottom);
	} else if (empty1 == true && empty2 == false) {
		*d = *s2;
	} else if (empty1 == false && empty2 == true) {
		*d = *s1;
	} else {
		(d)->left = (d)->top = (d)->right = (d)->bottom = 0;
	}
}

inline bool
IzzySectRect(
  const Rect *  s1,
  const Rect *  s2,
  Rect *        d)
{
	_sect_lines_((s1)->left, (s1)->right, (s2)->left, (s2)->right, (d)->left, (d)->right);
	_sect_lines_((s1)->top, (s1)->bottom, (s2)->top, (s2)->bottom, (d)->top, (d)->bottom);
	if ((d)->left >= (d)->right || (d)->top >= (d)->bottom) { (d)->left = (d)->right = (d)->top = (d)->bottom = 0; }
	return IzzyEmptyRect(d) == false;
}

inline void
IzzyCalcRectCenter(
	const Rect&		inRect,
	Point*			outPt)
{
	outPt->h = inRect.left + (inRect.right - inRect.left) / 2;
	outPt->v = inRect.top + (inRect.bottom - inRect.top) / 2;
}

// RECTANGLE CONVERSION

#if TARGET_OS_WIN32
inline void
MacToWinRect(
	const Rect&	inRect,
	RECT*		outRect)
{
	outRect->left = inRect.left; outRect->top = inRect.top; outRect->right = inRect.right; outRect->bottom = inRect.bottom;
}

inline void
WinToMacRect(
	const RECT&	inRect,
	Rect*		outRect)
{
	outRect->left = (SInt16) inRect.left; outRect->top = (SInt16) inRect.top; outRect->right = (SInt16) inRect.right; outRect->bottom = (SInt16) inRect.bottom;
}
#endif


#include <cmath>

inline void
QDRectToCGRect(
	const Rect& inRect,
	CGRect*		outRect)
{
	outRect->origin.x = (float) inRect.left;
	outRect->origin.y = (float) inRect.top;
	outRect->size.width = (float) (inRect.right - inRect.left);
	outRect->size.height = (float) (inRect.bottom - inRect.top);
}

inline void
CGRectToQDRect(
	const CGRect&	inRect,
	Rect*			outRect)
{
	outRect->left = (SInt16)(std::floor(inRect.origin.x));
	outRect->top = (SInt16)(std::floor(inRect.origin.y));
	outRect->right = (SInt16)(std::ceil(inRect.origin.x + inRect.size.width));
	outRect->bottom = (SInt16)(std::ceil(inRect.origin.y + inRect.size.height));
	
}

inline void
InsetCGRect(
	CGRect& 	ioRect,
	float		dx,
	float		dy)
{
	ioRect.origin.x += dx;
	ioRect.origin.y += dy;
	ioRect.size.width -= dx * 2;
	ioRect.size.height -= dy * 2;
}

#else

#define	IzzyPtInRect(inPoint, inRect)				( ((inPoint).h >= (inRect)->left && (inPoint).h < (inRect)->right) && ((inPoint).v >= (inRect)->top && (inPoint).v < (inRect)->bottom) )
#define IzzyAddPoints(inA, inB, outPt)				{ (outPt)->h = (inA).h + (inB).h; (outPt)->v = (inA).v + (inB).v; }
#define IzzySubPoints(inA, inB, outPt)				{ (outPt)->h = (inA).h - (inB).h; (outPt)->v = (inA).v - (inB).v; }

#define	IzzySetRect(rect, l, t, r, b)				{ (rect)->left = (l); (rect)->top = (t); (rect)->right = (r); (rect)->bottom = (b); }
#define	IzzyOffsetRect(rect, h, v)					{ (rect)->left += (h); (rect)->top += (v); (rect)->right += (h); (rect)->bottom += (v); }
#define	IzzyInsetRect(rect, h, v)					{ (rect)->left += (h); (rect)->top += (v); (rect)->right -= (h); (rect)->bottom -= (v); }
#define IzzyEmptyRect(rect)							( (rect)->left >= (rect)->right || (rect)->top >= (rect)->bottom )
#define IzzyEqualRect(r1, r2)						( ((r1)->left == (r2)->left && (r1)->top == (r2)->top && (r1)->right == (r2)->right && (r1)->bottom == (r2)->bottom) )
#define IzzyUnionRect(s1, s2, d)					{ (d)->left = _min2((s1)->left, (s2)->left); (d)->top = _min2((s1)->top, (s2)->top); (d)->right = _max2((s1)->right, (s2)->right); (d)->bottom = _max2((s1)->bottom, (s2)->bottom); }
		
#define	WinToMacRect(inRect, outRect)				{ (outRect)->left = (inRect).left; (outRect).top = (inRect)->top; (outRect)->right = (inRect).right; (outRect)->bottom = (inRect).bottom; } 
#define	MacToWinRect(inRect, outRect)				{ (outRect)->left = (inRect).left; (outRect).top = (inRect)->top; (outRect)->right = (inRect).right; (outRect)->bottom = (inRect).bottom; } 

#define QDRectToCGRect(inRect, outRect)				{ (outRect)->origin.x = (inRect).left; (outRect)->origin.y = (inRect).top; (outRect)->size.width = (inRect).right - (inRect).left; (outRect)->size.height = (inRect).bottom - (inRect).top; }
#define CGRectToQDRect(inRect, outRect)				{ (outRect)->left = floor((inRect).origin.x); (outRect)->top = floor((inRect).origin.y); (outRect)->right = ceil((inRect).origin.x + (inRect).size.width); (outRect)->bottom = ceil((inRect).origin.y + (inRect).size.height); }
#define InsetCGRect(ioRect, dx, dy)					{ (ioRect).origin.x += dx; (ioRect).origin.y += dy; (ioRect).size.width -= dx * 2; (ioRect).size.height -= dy * 2; }

#endif


#endif
