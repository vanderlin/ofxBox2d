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
    
    
    ofxBox2d                                box2d;            //    world
    ofxBox2dCircle                          ground;           //    ground
    ofVboMesh                               groundMesh;        //    ground mesh
    vector		<ofPtr<ofxBox2dCircle> >    circles;		  //	default box2d circles
	vector		<ofPtr<ofxBox2dRect> >		boxes;			  //	defalut box2d rects
	   
	
};
