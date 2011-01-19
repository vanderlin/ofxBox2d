

#pragma once
#include "ofMain.h"
#include "ofxBox2dPolygon.h"

class ofxBox2dLine : public ofxBox2dPolygon {
	
public:
	
	//------------------------------------------------	
	ofxBox2dLine() {
	}
	
	//------------------------------------------------
	void setup(b2World * b2dworld, const ofPoint &p1, const ofPoint &p2) {
		
		if(b2dworld == NULL) {
			ofLog(OF_LOG_NOTICE, "ofxBox2dLine :: setup : - must have a valid world -");
			return;
		}
		
		ofxBox2dPolygon::setup(b2dworld);
		addVertex(p1);
		addVertex(p2);
		
		//create();
	}
	
};














