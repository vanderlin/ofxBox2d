#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"

// ------------------------------------------------- App
class ofApp : public ofBaseApp {
	
public:
	
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	
	vector <ofPolyline>                  lines;
	ofxBox2d                             box2d;
	vector <shared_ptr<ofxBox2dCircle> > circles;
	vector <shared_ptr<ofxBox2dEdge> >   edges;
};
