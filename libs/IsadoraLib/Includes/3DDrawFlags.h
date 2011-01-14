// =================================================================================
//	3DDrawFlags				  ©2002 Mark F. Coniglio -- All Rights Reserved.
// =================================================================================

#ifndef _H_3DDrawFlags
#define _H_3DDrawFlags

#include "VidProcessCommon.h"

// ----------------------------------------------------------------------
// CONSTANTS

// flags used with Draw3DRect

enum {
	kBorderLeft		= (1L << 7),
	kBorderTop		= (1L << 6),
	kBorderRight	= (1L << 5),
	kBorderBottom	= (1L << 4),
	kSpaceLeft		= (1L << 3),
	kSpaceTop		= (1L << 2),
	kSpaceRight		= (1L << 1),
	kSpaceBottom	= (1L << 0),
	
	kBoxBorder		= (kBorderLeft | kBorderRight | kBorderTop | kBorderBottom)
};

typedef	UInt8		BorderFlags;

#endif
