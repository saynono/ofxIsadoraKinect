// ===========================================================================
//	IzzyOpenGLConstants			 (C)2006 Mark F. Coniglio. All rights reserved.
// ===========================================================================
// Constants used by the OpenGL portions of the program, primarily having to
// do with how texture maps are blended.
//
// This code was created to handle a bug in the MacBook OpenGL drivers which
// did not handle the GL_ADD tex env mode properly. At this point, the first
// case (which includes GL_ADD) has been deprecated. All code should use
// GL_MODULATE as the tex env mode.
//

#ifndef _H_IZZYOPENGLCONSTANTS
#define _H_IZZYOPENGLCONSTANTS

#if 0
	#define	IZZY_TEX_ENV_MODE		GL_ADD			// DEPRECATED
	#define IZZY_TEX_COLOR			0.0f			// DEPRECATED
	#define	IZZY_GL_ALWAYS_BLEND	FALSE			// DEPRECATED
#else
	#define	IZZY_TEX_ENV_MODE		GL_MODULATE
	#define IZZY_TEX_COLOR			1.0f
	#define	IZZY_GL_ALWAYS_BLEND	TRUE
#endif

#endif	// #ifndef _H_IZZYOPENGLCONSTANTS
