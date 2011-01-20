/*
 *  Scroller.cpp
 *  iPhoneofxBox2dExample
 *
 *  Created by Todd Vanderlin on 1/20/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#include "Scroller.h"



Scroller::Scroller() {
}
Scroller::~Scroller() {
	
}


void Scroller::setup() {
	nTouches = 0;
	int amt = 0;
	float rx = 0;
	for (int i=0; i<amt; i++) {
		ofRectangle r;
		r.width = ofRandom(50, 100);
		r.x = rx;
		r.height = ofRandom(10, 100);
		r.y = 0;
		
		rx += r.width;
		temp.push_back(r);
		tempC.push_back(ofColor(ofRandom(255), 255, ofRandom(255)));	
		width += r.width;
	}
	
	x = 0;
	y = 0;
	touchOffset = 0;
	startX = 0;
	vx = 1.0;
}


void Scroller::update() {
	
	//des.x *= vx;
	pos += (des-pos) * 0.3;
}


void Scroller::draw() {
	return;
	ofPushMatrix();
	ofTranslate(pos.x, pos.y);
	ofFill();
	for (int i=0; i<temp.size(); i++) {
		ofSetColor(tempC[i]);
		ofRect(temp[i]);
		
		ofSetColor(100, 100, 100);
		ofDrawBitmapString(ofToString(i), temp[i].x+20, temp[i].y+20);
	}
	ofPopMatrix();
	
	for (int i=0; i<nTouches; i++) {
		ofFill();
		ofSetColor(255);	    ofCircle(toucheDown[i], 30);
		ofSetColor(10, 255, 0); ofCircle(toucheMove[i], 30);
	}
	
	
	ofLine(toucheMove[0].x, toucheMove[0].y, toucheMove[0].x+vx, toucheMove[0].y);
}


