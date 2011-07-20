
#pragma once
#include "ofMain.h"
#include "ofxBox2dBaseShape.h"
#include "ofxBox2dPolygonUtils.h"


class ofxBox2dPolygon : public ofxBox2dBaseShape {

private:
	
	bool	bIsSimplified;
	bool    bIsTriangulated;
	float   area;
	ofVec2f center;
	void calculateCentroid();
	void calculateArea();
	
public:
	
	ofRectangle				bounds;
	vector <TriangleShape>	triangles;
	ofPolyline				poly;
	
	//----------------------------------------
	ofxBox2dPolygon();
	~ofxBox2dPolygon();
	void destroy();
	
	void setup(b2World * b2dworld);

	//----------------------------------------
	void addVertex(const ofVec2f & p);
	void addVertex(float x, float y);
	void addVertexes(const vector <ofVec2f> & polyPoints);
	void addVertexes(const ofPolyline &polyline);
	void addTriangle(const ofVec2f &a, const ofVec2f &b, const ofVec2f &c);
	
	//----------------------------------------
	// Polygon helper functions
	//----------------------------------------
	void simplify(float tolerance=0.3);
	void triangulate(float resampleAmt=20, int nPointsInside=-1);
	
	//----------------------------------------
	ofVec2f getCenter();
	float   getArea()   { return area;   };
	
	//----------------------------------------
	void updateShape();
	void create(b2World * b2dworld);
	void draw();
	
};