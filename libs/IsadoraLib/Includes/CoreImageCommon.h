//===========================================================================
//	CoreImageCommon				   (C)2007 Mark F. Coniglio. All rights reserved.
// ===========================================================================
// When compiling the CoreVideo enabled version of Isadora, the constant
// ISADORA_CORE should be defined in the project. This will enable the
// COMPILE_CORE_VIDEO flag which controls the compilation of CoreVideo
// features, which are specific to MacOS 10.4

#ifndef _H_CoreImageCommon
#define	_H_CoreImageCommon


#ifdef ISADORA_CORE

	#define	COMPILE_CORE_VIDEO		1

#else

	#define	COMPILE_CORE_VIDEO		0

#endif

#endif	// #ifndef _H_CoreImageCommon
