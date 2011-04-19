
#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"



// A simple little Data class. This is were
// you can store anything you want.
class Data {
public:
	Data()  { 
		id = (int)ofRandom(0, 100);
	}
	
	int		r, g, b;
	string  name;
	int		id;
};


// A Custom Particle extedning the box2d circle
class CustomParticle : public ofxBox2dCircle {
	
public:
	
	Data * myData;
	
	void setupTheCustomData() {
		
		// we are using a Data pointer because 
		// box2d needs to have a pointer not 
		// a referance
		myData = new Data();
		myData->id = ofRandom(0, 100);
		myData->r  = ofRandom(0, 255);
		myData->g  = ofRandom(0, 255);
		myData->b  = ofRandom(0, 255);
		setData(myData);
	}
	
	void draw() {
		Data* theData = (Data*)getData();
		if(theData) {
			
			// Evan though we know the data object lets just 
			// see how we can get the data out from box2d
			// you would use this when using a contact listener
			// or tapping into box2d's solver.
			
			float radius = getRadius();
			glPushMatrix();
			glTranslatef(getPosition().x, getPosition().y, 0);
			ofSetColor(theData->r, theData->g, theData->b);
			ofFill();
			ofCircle(0, 0, radius);	
			ofSetColor(255);
			ofDrawBitmapString(ofToString(theData->id), -10, -10);
		}
		
		glPopMatrix();
	}
};

// ------------------------------------------------- App
class testApp : public ofBaseApp {
	
public:
	
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
	ofxBox2d					box2d;
	vector <CustomParticle>		particles;
	
};
