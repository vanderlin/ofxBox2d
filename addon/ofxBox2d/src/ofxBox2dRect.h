

#pragma once
#include "ofMain.h"
#include "ofxBox2dBaseShape.h"

class ofxBox2dRect : public ofxBox2dBaseShape {

private:
    
	float width, height;
    ofPolyline   shape;
public:
	
	//------------------------------------------------
	ofxBox2dRect();	
	
	//------------------------------------------------
	void setup(b2World * b2dworld, ofRectangle rec);
	void setup(b2World * b2dworld, float x, float y, float w, float h);
	
	float       getWidth();
	float       getHeight();
    ofPolyline& getRectangleShape();
	
	//------------------------------------------------
    void draw();

	//------------------------------------------------
	void addAttractionPoint(float x, float y, float amt);
	void addAttractionPoint(ofVec2f pt, float amt);

	void addRepulsionForce(float x, float y, float amt);
	void addRepulsionForce(ofVec2f pt, float amt);

};














