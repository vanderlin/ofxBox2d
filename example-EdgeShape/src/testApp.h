#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"

// -------------------------------------------------
class testApp : public ofBaseApp {
	
public:
	
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
	
    int                                     groundRes;
    float                                   groundSpeed;
    float                                   groundTimer;

	
	ofxBox2d                                box2d;
	ofPolyline                              groundLine;
	ofxBox2dEdge                            ground;
    
    vector		<shared_ptr<ofxBox2dCircle> >	circles;		  //	default box2d circles
	vector		<shared_ptr<ofxBox2dRect> >      boxes;			  //	defalut box2d rects
	
	
};
