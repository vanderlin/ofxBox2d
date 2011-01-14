
#pragma once
#include "ofMain.h"
#include "ofxVectorMath.h"
#include "ofxBox2d.h"

// ------------------------------------------------- a simple extended box2d circle
class LetterShape : public ofxBox2dPolygon {
	
public:
	ofColor color;
	
};

// -------------------------------------------------

class testApp : public ofBaseApp {
	
public:
	
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	
	ofTrueTypeFont					font;
	ofxBox2d						box2d;			 
	vector		<LetterShape>		letters;		 

};

