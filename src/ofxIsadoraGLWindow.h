/*
 *  ofxIsadoraGLWindow.h
 *  IsadoraTest2
 *
 *  Created by say nono on 24.10.10.
 *  Copyright 2010 www.say-nono.com. All rights reserved.
 *
 */

#ifndef _OFXISADORAGLWINDOW_H
#define _OFXISADORAGLWINDOW_H

#include "ofAppBaseWindow.h"

// alot of code adapted from ofQC by vade, thanx!

class ofxIsadoraGLWindow : public ofAppBaseWindow{
public:
	ofxIsadoraGLWindow();
	//virtual ofFFGLWindow(){};
	
	void	setupOpenGL(int w, int h, int screenMode);
	void	initializeWindow();
	void	runAppViaInfiniteLoop(ofBaseApp * appPtr);
	
	virtual void	hideCursor() {};
	virtual void	showCursor() {};
	
	virtual void	setWindowPosition(int x, int y);
	virtual void	setWindowShape(int w, int h);
	
	virtual int		getFrameNum();
	virtual	float	getFrameRate();
	
	virtual ofPoint	getWindowPosition(); 
	virtual ofPoint	getWindowSize();
	virtual ofPoint	getScreenSize();
	
	virtual void	setFrameRate(float targetRate);
	virtual void	setWindowTitle(string title){}
	
	virtual int		getWindowMode();
	
	virtual void	setFullscreen(bool fullscreen){}
	virtual void	toggleFullscreen(){}
	
	virtual void	enableSetupScreen(){}
	virtual void	disableSetupScreen(){}
	
	// additional methods not defined in ofAppBaseWindow
	
	virtual void		update();
	virtual void		draw();
	
	int					windowMode;
	ofPoint				screenSize;
	ofPoint				windowSize;
	ofPoint				windowPos;
	
	float				timeNow, timeThen, fps;
	int					nFrameCount;
	bool				bEnableSetupScreen;
	float				frameRate;	
};


#endif