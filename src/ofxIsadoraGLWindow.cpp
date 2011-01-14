/*
 *  ofxIsadoraGLWindow.cpp
 *  IsadoraTest2
 *
 *  Created by say nono on 24.10.10.
 *  Copyright 2010 www.say-nono.com. All rights reserved.
 *
 */

#include "ofxIsadoraGLWindow.h"

#include "ofMain.h"


ofxIsadoraGLWindow::ofxIsadoraGLWindow() 
{	
	windowPos.set(0, 0);
	windowSize.set(0, 0);
	screenSize.set(0, 0);
	
	nFrameCount				= 0;
	windowMode				= 0;
	timeNow, timeThen, fps	= 0.0f;
	
	frameRate				= 0;
}


void ofxIsadoraGLWindow::setupOpenGL(int w, int h, int screenMode)
{	
	//ofSetupGraphicDefaults();
	windowMode = screenMode;
	windowSize.set(w, h);
}

void ofxIsadoraGLWindow::initializeWindow()
{	
}

void  ofxIsadoraGLWindow::runAppViaInfiniteLoop(ofBaseApp * appPtr)
{
//	appPtr->setup();
//	appPtr->update();
}



void ofxIsadoraGLWindow::update()
{	
	ofGetAppPtr()->update();
}

void ofxIsadoraGLWindow::draw()
{
	
	ofGetAppPtr()->draw();
	
	
	// FPS calculation stolen from Memos code. Thanks Memo :)
	
	timeNow = ofGetElapsedTimef();
	if( (timeNow - timeThen) > 0)
	{
		fps = 1.0 / (timeNow-timeThen);
		frameRate *= 0.9f;
		frameRate += 0.1f*fps;
	}
	timeThen = timeNow;
	
	// increase the overall frame count
	nFrameCount++;			
}






void ofxIsadoraGLWindow::setWindowPosition(int x, int y)
{
	windowPos.set(x, y);
}

ofPoint	ofxIsadoraGLWindow::getWindowPosition() 
{
	return windowPos;
}

int	ofxIsadoraGLWindow::getWindowMode()
{
	return windowMode;
}

float ofxIsadoraGLWindow::getFrameRate()
{
	return frameRate;
}

int	ofxIsadoraGLWindow::getFrameNum()
{
	return nFrameCount;
}

void ofxIsadoraGLWindow::setFrameRate(float targetRate)
{	
}

void ofxIsadoraGLWindow::setWindowShape(int w, int h)
{
	windowSize.set(w, h);
}

ofPoint	ofxIsadoraGLWindow::getWindowSize()
{
	return windowSize;
}

ofPoint	ofxIsadoraGLWindow::getScreenSize()
{
	return screenSize;
}
