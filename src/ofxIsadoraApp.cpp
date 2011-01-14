/*
 *  myIsadoraApp.cpp
 *  IsadoraTest2
 *
 *  Created by say nono on 24.10.10.
 *  Copyright 2010 www.say-nono.com. All rights reserved.
 *
 */

#include "ofxIsadoraApp.h"


#define RECONNECT_TIME 400

ofxIsadoraApp::ofxIsadoraApp(){
}

ofxIsadoraApp::~ofxIsadoraApp(){
}

//--------------------------------------------------------------
void ofxIsadoraApp::setup(){
	connectTime = 0;
	pingTime = 0;
	deltaTime = 0;
	lastFrameTime = 0;
	_inited = false;

}

//--------------------------------------------------------------
void ofxIsadoraApp::exit(){
}

//--------------------------------------------------------------
void ofxIsadoraApp::initStuff(){
	kinect.init();
	kinect.setVerbose(true);
	kinect.open();
	grayImage.allocate(kinect.getWidth(), kinect.getHeight());	
	_inited = true;
	printf("ofxIsadoraApp::initStuff()\n");
}

//--------------------------------------------------------------
void ofxIsadoraApp::update(){
	
//	printf("ofxIsadoraApp::update()");
	
	if(!_inited) initStuff();
	else{
		kinect.update();
		if(grayImage.getWidth() != kinect.width) grayImage.allocate(kinect.getWidth(), kinect.getHeight());	
		grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
	}
	
}

//--------------------------------------------------------------
void ofxIsadoraApp::draw(){
	printf("myIsadoraApp::draw\n");
	
}

//--------------------------------------------------------------
void ofxIsadoraApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void ofxIsadoraApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void ofxIsadoraApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void ofxIsadoraApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofxIsadoraApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofxIsadoraApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofxIsadoraApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------


bool ofxIsadoraApp::getVideoFrame(UInt32* outData, int w, int h){
	
	if(!_inited) return false;
	
	SInt16 red = 250;
	SInt16 green = 78;
	SInt16 blue = 24;
	int outStride = 16;
	
	int cnt = 0;
	unsigned char* pixels = kinect.getPixels();
	
	int l = w*h;
	for(int i=0;i<l;i++){
		red		= (SInt16)(pixels[cnt++]);
		green	= (SInt16)(pixels[cnt++]);
		blue	= (SInt16)(pixels[cnt++]);
		////			cnt++;
		*(outData) = ARGB_(blue, green, red,255);
		outData++;
		if(cnt%w*3==0) outData = (UInt32*)((char*) outData + outStride);
	}
	return true;
}

bool ofxIsadoraApp::getDepthFrame(UInt32* outData, int w, int h){
	
	if(!_inited) return false;
	
	SInt16 grey = 250;
	int outStride = 16;
	
	int cnt = 0;
	unsigned char* pixels = kinect.getDepthPixels();
	
	int l = w*h;
	for(int i=0;i<l;i++){
		grey		= (SInt16)(pixels[cnt*2]);
		cnt++;
		*(outData) = ARGB_(grey, grey, grey,255);
		outData++;
		if(cnt%w==0){
			outData = (UInt32*)((char*) outData + outStride);
			cnt += w;
		}
	}	
	return true;
}


void ofxIsadoraApp::ping(){
	printf("new video frame ping\n");
}