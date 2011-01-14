/*
 *  myIsadoraApp.h
 *  IsadoraTest2
 *
 *  Created by say nono on 24.10.10.
 *  Copyright 2010 www.say-nono.com. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxIsadoraGLApp.h"
#include "VidColorMacros.h"

#include "ofxOpenCv.h"
#include "ofxKinect.h"
//#include "nonoColor.h"

class ofxIsadoraApp : public ofxIsadoraGLApp {

public:
	
	ofxIsadoraApp();
	virtual ~ofxIsadoraApp();
	
	
	void setup();
	void exit();
	void initStuff();
	void update();
	void draw();
	 
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
	bool getVideoFrame(UInt32* outData, int w, int h);
	bool getDepthFrame(UInt32* outData, int w, int h);	
	void ping();
	
private:
	
	int			pingTime;
	int			connectTime;
	int			deltaTime;
	int			lastFrameTime;
	
	bool					_inited;
	
	ofxKinect				kinect;
	ofxCvGrayscaleImage 	grayImage;
	ofxCvColorImage			colorImg;
	
};

