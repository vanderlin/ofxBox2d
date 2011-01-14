
#pragma once
#include "ofMain.h"
#include "ofxBox2dBaseShape.h"



class ofxBox2dPolygon {

public:
	
	//----------------------------------------
	ofxBox2dPolygon();
	~ofxBox2dPolygon();
	
	//----------------------------------------
	void addVertex(const ofVec2f & p);
	void addVertex(float x, float y);
	
};