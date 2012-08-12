
#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"

/* ****************************************
 
this example is still rough working out
lots of bugs but have fun!

****************************************** */



// ------------------------------------------------- App
class testApp : public ofBaseApp {
	
public:
	
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
	vector <ofPolyline>			lines;
	ofxBox2d					box2d;
	vector <ofxBox2dCircle>		circles;
	vector <ofxBox2dPolygon>	polyLines;
};
