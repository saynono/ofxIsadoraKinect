// ===========================================================================
//	MacCompatiblity				 (C)2006 Mark F. Coniglio. All rights reserved.
// ===========================================================================
// #defines types normally present under the Windows API so that code
// using these types can successfully compile under Mac OS X

#ifndef _H_MacCompatiblity
#define _H_MacCompatiblity

	#if TARGET_OS_MAC

		typedef	SInt8		BYTE;
		typedef	SInt16		INT;
		typedef	UInt32		UINT;
		typedef SInt32		LONG;
		typedef UInt32		ULONG;
		typedef UInt32		DWORD;

		typedef char*		LPSTR;
		 
		typedef void*		HINSTANCE;
		typedef void*		HWND;
		typedef void*		HDC;
		typedef	void*		HGLRC;

		typedef struct {
			LONG	left;
			LONG	top;
			LONG	right;
			LONG	bottom;
		} RECT;
		typedef RECT RECT;

		void
		MessageBox(
			HWND		/* inWindow */,
			const char*	/* inMessage */,
			const char*	/* inTitle */,
			UInt32		/* inFlags */);
			
		const UInt32	MB_OK		= 1;

	#endif

#endif
