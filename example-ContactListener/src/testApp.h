#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"


#define N_SOUNDS 5

class SoundData {
public:
	int	 soundID;
	bool bHit;
};


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
	
	// this is the function for contacts
	void contactStart(ofxBox2dContactArgs &e);
	void contactEnd(ofxBox2dContactArgs &e);

	// when the ball hits we play this sound
	ofSoundPlayer  sound[N_SOUNDS];
	
	ofxBox2d						box2d;			  //	the box2d world
	vector		<ofxBox2dCircle>	circles;		  //	default box2d circles
	
};

