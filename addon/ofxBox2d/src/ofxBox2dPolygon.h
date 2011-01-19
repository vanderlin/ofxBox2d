
#pragma once
#include "ofMain.h"
#include "ofxBox2dBaseShape.h"



class ofxBox2dPolygon : public ofxBox2dBaseShape {

private:
	
	bool    bIsTriangulated;
	float   area;
	ofVec2f center;
	void calculateCentroid();
	void calculateArea();
	
public:
	
	ofRectangle		 bounds;
	vector <ofVec2f> vertices;
	
	//----------------------------------------
	ofxBox2dPolygon();
	~ofxBox2dPolygon();
	
	void setup(b2World * b2dworld);

	//----------------------------------------
	void addVertex(const ofVec2f & p);
	void addVertex(float x, float y);
	void addVertexes(const vector <ofVec2f> & polyPoints);
	
	//----------------------------------------
	// Polygon helper functions
	//----------------------------------------
	void simplify(float tolerance);
	void triangulate();
	
	//----------------------------------------
	ofVec2f getCenter();
	float   getArea()   { return area;   };
	
	//----------------------------------------
	void updateShape();
	void create(b2World * b2dworld);
	void draw();
	
};