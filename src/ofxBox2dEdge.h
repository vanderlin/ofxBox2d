//
//  ofxBox2dEdge.h
//  example-Bodies
//
//  Created by Todd Vanderlin on 11/18/13.
//
//


#pragma once
#include "ofMain.h"
#include "ofxBox2dBaseShape.h"
#include "ofxBox2dPolygonUtils.h"

class ofxBox2dEdge : public ofxBox2dBaseShape, public ofPolyline {
    
private:
		
    public:

    void addVertexes(ofPolyline &polyline);
    void addVertexes(vector <ofVec2f> &pts);
    
    void create(b2World * b2dworld);
    void updateShape();
    void draw();
};