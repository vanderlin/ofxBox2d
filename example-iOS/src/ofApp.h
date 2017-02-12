#pragma once

#include "ofMain.h"
#include "ofxiOS.h"
#include "ofxiOSCoreMotion.h"
#include "ofxBox2d.h"

class ofApp : public ofxiOSApp {

public:
	void setup();
	void update();
	void draw();

	void touchDoubleTap(ofTouchEventArgs &touch);

	ofxiOSCoreMotion coreMotion;

	ofxBox2d box2d;
	vector <shared_ptr<ofxBox2dCircle>> circles;
};
