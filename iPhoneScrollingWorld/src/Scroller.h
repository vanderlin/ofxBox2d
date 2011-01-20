/*
 *  Scroller.h
 *  iPhoneofxBox2dExample
 *
 *  Created by Todd Vanderlin on 1/20/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"



class Scroller {

private:
	
	vector <ofColor>	 tempC;
	vector <ofRectangle> temp;
public:
	
	float vx, prevx;
	
	Scroller();
	~Scroller();
	ofVec2f				 pos, des;
	
	float startX;
	float touchOffset;
	int	nTouches;
	int width, height;
	float x, y;
	ofVec2f	toucheDown[2];
	ofVec2f	toucheMove[2];
	
	void setup();
	void update();
	void draw();
	
	

};