// ===========================================================================
//	StGLFlag					 (C)2004 Mark F. Coniglio	  All rights reserved.
// ===========================================================================
//
// A handy C++ class for setting and restoring OpenGL flags.
//
// Call the constructor to set an OpenGL flag. At the moment this object is
// instantiated, the current state of the constructor is stored, and the new
// state is set. When this object's destructor is called, this original
// state is automatically restored.
// 
// Example Usage: StGLFlag lights(GL_LIGHTING, TRUE); -- Turns on lighting,
// restoring GL_LIGHTING to its previous state upon destruction

#ifndef _H_StGLFlag
#define _H_StGLFlag

#include "3DPluginDrawUtil.h" // for gl includes, for now

#define kStGLReportError	1

#if kStGLReportError && defined(__ISADORA_APP__) && DEBUG
	#include "COpenGLUtil.h"
	#define	StGLErrReport()		COpenGLUtil::GLReportError()
#else
	#define	StGLErrReport()		{ }
#endif

// --------------------------------------------------------------------------------

class StGLFlag
{
public:

	inline StGLFlag(
		GLenum		inGLFlag,
		GLboolean	inEnable)
	{
		GLenum err;
		do {
			err = glGetError();
		} while (err != 0);

		// remember GL flag being modified
		mGLFlag = inGLFlag;
		// get current flag setting
		mSavedEnableFlag = glIsEnabled(mGLFlag);
		err = glGetError();
		
		mNeedsRestore = GL_FALSE;
		
		// if desired setting doesn't match current setting
		if (inEnable != mSavedEnableFlag || err != 0) {
			
			// mark us as needing to be restored
			mNeedsRestore = GL_TRUE;
			
			if (inEnable == GL_TRUE) {
				glEnable(mGLFlag);
			} else {
				glDisable(mGLFlag);
			}
			
			StGLErrReport();
		}
	}
	
	inline ~StGLFlag()
	{
		if (mNeedsRestore == GL_TRUE) {
			if (mSavedEnableFlag == GL_TRUE)
				glEnable(mGLFlag);
			else
				glDisable(mGLFlag);
			StGLErrReport();
		}
	}

private:
	GLenum		mGLFlag;
	GLboolean	mNeedsRestore;
	GLboolean	mSavedEnableFlag;
};

// --------------------------------------------------------------------------------

class StDepthWriteMask
{
public:

	inline StDepthWriteMask(
		GLboolean	inEnable)
	{
		// get current depth write mask setting
		glGetBooleanv(GL_DEPTH_WRITEMASK, &mSavedDepthWriteMask);
		StGLErrReport();
		
		mNeedsRestore = GL_FALSE;
		
		// if desired setting doesn't match current setting
		if (inEnable != mSavedDepthWriteMask) {
			
			// indicate we'll need to be restored later
			mNeedsRestore = GL_TRUE;
			
			// change the depth mask setting
			if (inEnable == GL_TRUE) {
				glDepthMask(GL_TRUE);
			} else {
				glDepthMask(GL_FALSE);
			}
			StGLErrReport();
		}
	}
	
	inline ~StDepthWriteMask()
	{
		// restore if necessary
		if (mNeedsRestore == GL_TRUE) {
			glDepthMask(mSavedDepthWriteMask);
			StGLErrReport();
		}
	}

private:
	GLboolean	mNeedsRestore;
	GLboolean	mSavedDepthWriteMask;
};

// --------------------------------------------------------------------------------

class StTexEnvMode
{
public:

	inline StTexEnvMode(
		GLint	inTexEnvMode)
	{
		// get current tex env mode
		glGetTexEnviv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, &mSavedTexEnvMode);
		StGLErrReport();
		
		mNeedsRestore = GL_FALSE;
		
		// if desired setting doesn't match current setting
		if (inTexEnvMode != mSavedTexEnvMode) {
			// indicate we'll need to be restored later
			mNeedsRestore = GL_TRUE;
			// set tex env mode
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, inTexEnvMode);
			StGLErrReport();
		}
	}
	
	inline ~StTexEnvMode()
	{
		// restore if necessary
		if (mNeedsRestore == GL_TRUE) {
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mSavedTexEnvMode);
			StGLErrReport();
		}
	}

private:
	GLboolean	mNeedsRestore;
	GLint		mSavedTexEnvMode;
};
#endif
