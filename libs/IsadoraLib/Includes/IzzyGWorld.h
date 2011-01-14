// =================================================================================
//	IzzyGWorld						  (C)2002 Mark F. Coniglio -- All Rights Reserved.
// =================================================================================
//
//	Isadora code should never call NewGWorld. Instead it should call IzzyNewGWorld,
//	as there is special interpretation of the inFormat argument depending on
//	whether or not this is a big or little endian system.

#ifndef _H_IzzyGWorld
#define _H_IzzyGWorld

OSErr
IzzyNewGWorld(
	GWorldPtr *			outOffscreenGWorld,
	short				inPixelDepth,
	const Rect *		boundsRect,
	CTabHandle			inCTable,
	GDHandle			inGDevice,
	GWorldFlags			inFlags,
	OSType				inFormat = 0);

#endif
