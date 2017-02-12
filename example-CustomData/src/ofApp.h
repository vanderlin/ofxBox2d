#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"

// A simple little Data class. This is were
// you can store anything you want.
class Data {
public:
	ofColor color;
	string  name;
	int     id;
};

// A Custom Particle extending the box2d circle
class CustomParticle : public ofxBox2dCircle {
	
public:
	
	void setupTheCustomData() {
		
		static int colors[] = {0xcae72b, 0xe63b8f, 0x2bb0e7};
		static string abc   = "abcdefghijklmnopqrstuvwxyz";
		
		// we are using a Data pointer because 
		// box2d needs to have a pointer not 
		// a referance
		setData(new Data());
		Data * theData = (Data*)getData();
		
		theData->id = ofRandom(0, 100);
		theData->name += abc[(int)ofRandom(0, abc.size())];
		theData->color.setHex(colors[(int)ofRandom(0, 3)]);

		printf("setting the custom data!\n");
	}
	
	void draw() {
		Data* theData = (Data*)getData();
		if(theData) {
			
			// Even though we know the data object, let's just
			// see how we can get the data out from box2d.
			// You would use this when using a contact listener
			// or tapping into box2d's solver.
			
			float radius = getRadius();
			ofPushMatrix();
			ofTranslate(getPosition());
			ofRotateZDeg(getRotation());
			ofSetColor(theData->color);
			ofFill();
			ofDrawCircle(0, 0, radius);
			
			float textSize = radius/10;
			ofPushMatrix();
			ofScale(textSize, textSize);
			ofSetColor(255);
			ofDrawBitmapString(theData->name, -textSize/2, textSize);
			ofPopMatrix();
			
			ofPopMatrix();
		}
	}
};

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
	void windowResized(int w, int h);
	
	ofxBox2d                             box2d;
	vector <shared_ptr<CustomParticle> > particles;
	
};
