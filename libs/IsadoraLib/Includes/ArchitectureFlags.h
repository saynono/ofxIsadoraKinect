// ===========================================================================
//	ArchitectureFlags.h			 (C)2007 Mark F. Coniglio	  All rights reserved.
// ===========================================================================
//
// Defines SSE_EXTENSIONS or ALTIVEC_EXTENSIONS based on the current platform
//

#ifndef _H_ArchitectureFlags
#define _H_ArchitectureFlags

#ifndef __APPLE_CC__
#include <TargetConditionals.h>
#endif

#ifndef TARGET_CPU_PPC
	#error "TARGET_CPU_PPC must be defined here"
#endif

#ifndef TARGET_CPU_X86
	#error "TARGET_CPU_X86 must be defined here"
#endif

#ifdef __MWERKS__
	#define	ASM	asm
#else
	#define ASM _asm
#endif

#define	SSE_EXTENSIONS			TARGET_CPU_X86
#define ALTIVEC_EXTENSIONS		TARGET_CPU_PPC

#endif

