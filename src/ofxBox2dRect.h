

#pragma once
#include "ofMain.h"
#include "ofxBox2dBaseShape.h"

// It would be nice to use ofRectangle but it is top/left and
// box2d needs a center rectangle.

class ofxBox2dRect : public ofxBox2dBaseShape {

private:
    float width, height;
    
    b2Fixture* fixturePtr = nullptr;
    
    unique_ptr<b2PolygonShape> shape = nullptr;
//    b2PolygonShape shape;
public:
	ofVboMesh mesh;
	//------------------------------------------------
	ofxBox2dRect();	
	
	//------------------------------------------------
	void setup(b2World * b2dworld, ofRectangle rec, float angle=0);
	void setup(b2World * b2dworld, float x, float y, float w, float h, float angle=0);
	   
	//------------------------------------------------
    float getWidth()  { return width * 2;  }
    float getHeight() { return height * 2; }
    
    
    void setRectangle(const ofRectangle& rect);
    ofRectangle getRect() ;
    
    void updateMesh();
    void draw();

	//------------------------------------------------
	void addAttractionPoint(float fx, float fy, float amt);
	void addAttractionPoint(ofVec2f pt, float amt);

	void addRepulsionForce(float fx, float fy, float amt);
	void addRepulsionForce(ofVec2f pt, float amt);

};














