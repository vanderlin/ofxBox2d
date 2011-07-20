

#pragma once
#include "ofMain.h"
#include "ofxBox2dBaseShape.h"

class ofxBox2dCircle : public ofxBox2dBaseShape {
	
private:
	
	float radius;
	
public:
	
	//------------------------------------------------
	ofxBox2dCircle();
	
	//------------------------------------------------
	void setup(b2World * b2dworld, float x, float y, float radius);
	
	//------------------------------------------------
	float getRadius();
	void  setRadius(float r);
	
	//------------------------------------------------
	virtual void draw();
	
};














