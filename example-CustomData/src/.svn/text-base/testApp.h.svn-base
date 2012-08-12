
#pragma once
#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxVectorMath.h"
#include "ofxBox2d.h"
#include "ofxContourAnalysis.h"

// ------------------------------------------------- a simple extended box2d circle
class CustomParticle : public ofxBox2dCircle {
	
public:
	
	float c; // the color
	
	CustomParticle() {
	}
	
	void draw() {
		float radius = getRadius();
		
		glPushMatrix();
		glTranslatef(getPosition().x, getPosition().y, 0);
		
		if(radius > 15) {
			ofSetColor(c, 255-c, c);
			ofFill();
			ofCircle(0, 0, radius);	
			
			ofSetColor(c-255, c, c);
			ofFill();
			ofCircle(0, 0, radius/1.3);	
			
			ofSetColor(255, 255, 255);
			ofFill();
			ofCircle(0, 0, 2);	
		}
		else {
			ofSetColor(250, c, 10);
			ofFill();
			ofCircle(0, 0, radius);	
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
	
	// Box2d
	ofxBox2d					box2d;			  //	the box2d world
	ofxBox2dLine				lineStrip;		  //	a linestrip for contours
	vector <CustomParticle>		customParticles;  //	this is a custom particle the extends a cirlce
	
	
	// OpenCV
	float					playHeadPosition;
	float					fadeAmount;
	int						threshold;
	int						vw, vh;
	
	// CV images
	ofVideoPlayer			input;
	ofxCvColorImage			color;
	ofxCvGrayscaleImage		gray;
	
	// contour work
	ofxCvContourFinder		contour;
	ofxContourAnalysis		contourAnalysis;
	vector <ofPoint>		simpleCountour;
	
};
