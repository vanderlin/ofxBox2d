
#pragma once
#include "ofMain.h"
#include "ofxBox2dBaseShape.h"
#include "ofxBox2dPolygonUtils.h"

class ofxBox2dPolygon : public ofxBox2dBaseShape, public ofPolyline {

private:
	
	bool	bIsSimplified;
	bool    bIsTriangulated;
	bool	bSetAsEdge;
	
	float   area;
	ofVec2f center;
	void calculateCentroid();
	float calculateArea();
	
public:
	
	ofPolyline				drawShape;
	ofRectangle				bounds;
	vector <TriangleShape>	triangles;
	
	//----------------------------------------
	ofxBox2dPolygon();
	~ofxBox2dPolygon();
	void destroy();
	
	void setup(b2World * b2dworld);

	//----------------------------------------
	void addTriangle(const ofVec2f &a, const ofVec2f &b, const ofVec2f &c);
	void addVertexes(vector <ofVec2f> &pts);
	void addVertexes(ofPolyline &polyline);

	//----------------------------------------
	// Polygon helper functions
	//----------------------------------------
	void simplify(float tolerance=0.3);
	void triangulate(float resampleAmt=20, int nPointsInside=-1);
	
	//----------------------------------------
	ofVec2f getCenter();
	float   getArea()     { return area; };
	bool	isGoodShape() { return calculateArea() > 15; }
	void	setAsEdge(bool set) { bSetAsEdge = set; }
	//------------------------------------------------
	void addAttractionPoint(ofVec2f pt, float amt=1);
	void addAttractionPoint(float x, float y, float amt=1);
	void addRepulsionForce(float x, float y, float amt);
	void addRepulsionForce(ofVec2f pt, float amt);

	//----------------------------------------
	void updateShape();
	void create(b2World * b2dworld);
	void draw();
	
};