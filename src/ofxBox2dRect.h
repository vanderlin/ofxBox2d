

#pragma once
#include "ofMain.h"
#include "ofxBox2dBaseShape.h"

// It would be nice to use ofRectangle but it is top/left and
// box2d needs a center rectangle.

class ofxBox2dRect : public ofxBox2dBaseShape {

private:
    float width, height;
	b2PolygonShape shape;
	b2Fixture * fixtureRect;
    
public:
	ofVboMesh mesh;
	//------------------------------------------------
	ofxBox2dRect();	
	
	//------------------------------------------------
	void setup(b2World * b2dworld, ofRectangle rec,uint16 categoryBits, uint16 maskBits);
	void setup(b2World * b2dworld, float x, float y, float w, float h,uint16 categoryBits, uint16 maskBits);
	
	//------------------------------------------------
	void setType(b2BodyType type);
	b2BodyType getType();

	//------------------------------------------------
	void setShape(float w,float h);

	//------------------------------------------------
    float getWidth()  { return width * 2;  }
    float getHeight() { return height * 2; }
    
    void updateMesh();
    void draw();

	//------------------------------------------------
	void addAttractionPoint(float fx, float fy, float amt);
	void addAttractionPoint(ofVec2f pt, float amt);

	void addRepulsionForce(float fx, float fy, float amt);
	void addRepulsionForce(ofVec2f pt, float amt);

};














