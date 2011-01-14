// =================================================================================
//	IsadoraPluginPrefix				  ©2010 Mark F. Coniglio -- All Rights Reserved.
// =================================================================================

#ifdef ISADORA_PLUGIN

	#ifdef WIN32
		#include <windows.h>
		#include <movies.h>
	#endif

	#if TARGET_OS_MAC
		#include <Carbon/Carbon.h>
	#endif

#endif