
#pragma once
#include "ofMain.h"
#include "ofxVectorMath.h"
#include "ofxBox2d.h"

// ------------------------------------------------- a simple extended box2d circle
class CustomParticle : public ofxBox2dCircle {
	
public:
	CustomParticle() {
	}
	ofColor color;
	void draw() {
		float radius = getRadius();
		
		glPushMatrix();
		glTranslatef(getPosition().x, getPosition().y, 0);
		
		ofSetColor(color.r, color.g, color.b);
		ofFill();
		ofCircle(0, 0, radius);	
		
		glPopMatrix();
		
	}
};

// -------------------------------------------------

class testApp : public ofBaseApp {
	
public:
	
	testApp();
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void resized(int w, int h);
	
	float px, py;
	bool							bDrawLines;
	bool							bMouseForce;
	
	ofxBox2d						box2d;			  //	the box2d world
	vector		<ofxBox2dCircle>	circles;		  //	default box2d circles
	vector		<ofxBox2dPolygon>	polygons;		  //	defalut box2d polgons
	vector		<ofxBox2dRect>		boxes;			  //	defalut box2d rects
	vector      <ofxBox2dLine>		lines;			  //	default box2d lines (hacked)
	vector		<CustomParticle>	customParticles;  //	this is a custom particle the extends a cirlce
	
	ofxBox2dCircle					ballJoints[5];	  //	ball joints
	ofxBox2dJoint					joints[5];		  //	box2d joints
	
	ofxBox2dLine					lineStrip;		  //	a linestrip for drawing
	
};

