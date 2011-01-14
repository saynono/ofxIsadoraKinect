// ===========================================================================
//	TextCommon			 ©2007 Mark F. Coniglio. All rights reserved.
// ===========================================================================
//
// Text buffer is a data type that allows character strings to be passed
// using the Value struct. See ValueCommon.h for more details.

#ifndef _H_TextCommon
#define _H_TextCommon
#pragma once

#include "IsadoraTypes.h"
#include "ValueCommon.h"

#define kTextMagic		0x12345678

struct TextBuffer
{
	DataValueInfo	mInfo;					// our data value info struct - required
	
	const char*		mText;					// c-string of the text
};

typedef struct TextBuffer		TextBuffer, *TextBufferPtr;

#endif
