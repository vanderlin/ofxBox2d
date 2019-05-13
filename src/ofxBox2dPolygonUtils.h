/*
 *  ofxBox2dPolygonUtils.h
 *  ofxBox2d
 *
 *  Created by Todd Vanderlin on 1/18/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"

class TriangleShape {
public:
	TriangleShape() {
		color.set(ofRandom(255), ofRandom(255), ofRandom(255));
	}
	int index[3];
	ofColor color;
	float area;

    glm::vec2 a,b,c;
    void draw() {
        ofDrawTriangle(a,b,c);
    }
    glm::vec2& operator[] (int index) {
        if (index == 0) return a;
        else if (index == 1) return b;
        else if (index == 2) return c;
        else {
            return a;
        }
    }
};

class ofxBox2dPolygonUtils {

public:
	
	
	// from zach ofxTriangleMesh
	// way better imp https://github.com/ofZach/ofxTriangleMesh
	static vector <TriangleShape> triangulate(ofPolyline contour, float angleConstraint = 28, float sizeConstraint = -1);

	static bool isPointInsidePolygon(const ofPoint & p, const vector<ofDefaultVertexType> & polygon);
	static ofPolyline getConvexHull(vector<ofDefaultVertexType>&linePts);
	static ofPolyline getConvexHull(ofPolyline &line);
	static ofPoint getTriangleCenter(ofPoint &a, ofPoint &b, ofPoint &c);
	static ofPoint getTriangleCenter(ofPoint *tr);
};
