// ===========================================================================
// UGraphicsCommon			(C) 2006 Mark F. Coniglio.  all rights reserved.
// ===========================================================================
// Defines that specify how UGraphics will do its drawing

#ifndef _H_UGraphicsCommon
#define _H_UGraphicsCommon


#if TARGET_OS_MAC
	#define	QD_DRAW							TRUE
	#define	CG_CONTEXT_ENABLE				FALSE
	#define	GDI_DRAW						FALSE
#elif TARGET_OS_WIN32
	#define	QD_DRAW							FALSE
	#define	CG_CONTEXT_ENABLE				FALSE
	#define	GDI_DRAW						TRUE
#endif

#define USE_CG_CONTEXT						(CG_CONTEXT_ENABLE && TARGET_OS_MAC)

#define USE_WIN_BACKBUFFER					0

const SInt32 UGRAPHICS_DONTDRAW				= 0x80000000;

#endif	// #ifndef _H_UGraphicsCommon
