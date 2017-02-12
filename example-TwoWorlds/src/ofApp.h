#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	void drawGravity(ofPoint p, ofPoint gravity);

	ofxBox2d box2dA;
	ofxBox2d box2dB;

	vector<shared_ptr<ofxBox2dCircle>> circlesA;
	vector<shared_ptr<ofxBox2dCircle>> circlesB;
	vector<shared_ptr<ofxBox2dRect>> sharedRects;

	ofRectangle boundsA;
	ofRectangle boundsB;
};
