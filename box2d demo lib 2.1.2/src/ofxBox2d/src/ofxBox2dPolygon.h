
#pragma once
#include "ofMain.h"
#include "ofxBox2dBaseShape.h"



class ofxBox2dPolygon : public ofxBox2dBaseShape {

private:
	
	float   area;
	ofVec2f center;
	void calculateCentroid();
	void calculateArea();
	
public:
	
	vector <ofVec2f> vertexes;
	
	//----------------------------------------
	ofxBox2dPolygon();
	~ofxBox2dPolygon();
	
	void setup(b2World * b2dworld);

	//----------------------------------------
	void addVertex(const ofVec2f & p);
	void addVertex(float x, float y);
	void addVertexes(const vector <ofVec2f> & polyPoints);
	
	//----------------------------------------
	ofVec2f getCenter();
	float   getArea()   { return area;   };
	
	//----------------------------------------
	void updateShape();
	void create();
	void draw();
	
};