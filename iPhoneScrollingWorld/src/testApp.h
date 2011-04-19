#pragma once

#include "ofMain.h"
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"
#include "ofxBox2d.h"
#include "Scroller.h"

class SimpleButton : public ofRectangle {
public:
	bool bOn;
	bool bToggle;
	string name;
	SimpleButton() {
		bOn = false;
		bToggle = false;
	}
	
	void draw(float x, float y) {
		this->x = x;
		this->y = y;
		width	= 100;
		height  = 30;
		
		
		ofFill();
		bToggle ? ofSetColor(200, 200, 200) : ofSetColor(30, 30, 30);
		ofRect((*this));
		
		ofSetColor(255, 0, 255);
		ofDrawBitmapString(name, x+5, y+10);
	}
};

class testApp : public ofxiPhoneApp {
	
public:
	
	void setup();
	void update();
	void draw();
	
	void touchDown(ofTouchEventArgs &touch);
	void touchMoved(ofTouchEventArgs &touch);
	void touchUp(ofTouchEventArgs &touch);
	void touchDoubleTap(ofTouchEventArgs &touch);
	void touchCancelled(ofTouchEventArgs &touch);
	Scroller scroller;
	
	ofxBox2d						box2d;			  //	the box2d world
	vector		<ofxBox2dCircle>	circles;		  //	default box2d circles
	
	/*
	vector		<ofVec2f>			polyShape;
	vector		<ofxBox2dPolygon>	polygons;		  //	defalut box2d polgons
	vector		<ofxBox2dRect>		boxes;			  //	defalut box2d rects
	
	SimpleButton					polyBtn;
	SimpleButton					clearBtn;
	SimpleButton					circleBtn;
	
	vector		<SimpleButton*>		buttons;
	*/
};
