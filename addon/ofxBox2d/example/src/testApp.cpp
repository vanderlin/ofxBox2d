#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	
	
	ofSetVerticalSync(true);
	ofBackground(20, 20, 20);
	ofSetLogLevel(OF_LOG_NOTICE);
	
	font.loadFont("Times.ttf", 42, false, true, true);
	
	box2d.init();
	box2d.setGravity(0, 10);
	box2d.createFloor();
	box2d.setFPS(30.0);
	box2d.registerGrabbing();
	
	
}

//--------------------------------------------------------------
void testApp::update() {

	box2d.update();

}


//--------------------------------------------------------------
void testApp::draw() {
	
	
	for(int i=0; i<letters.size(); i++) {
		letters[i].draw();
	}
	
	
	string info = "";
	info += "FPS: "+ofToString(ofGetFrameRate())+"\n";
	ofSetColor(255, 255, 255);
	ofDrawBitmapString(info, 30, 30);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
	letters.back().addVertex(x, y);
}

void testApp::mousePressed(int x, int y, int button) {
	letters.push_back(LetterShape());
}

void testApp::mouseReleased(int x, int y, int button) {
	
	letters.back().createShape(box2d.getWorld(), ofGetWidth()/2, ofGetHeight()/2);
}


//--------------------------------------------------------------
void testApp::keyPressed(int key) {

	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
	
	ofTTFCharacter keyCharacter;
	keyCharacter = font.getCharacterAsPoints('T');
	
	// the new letter that we just pressed
	LetterShape letter;
	
	for (int i=0; i<keyCharacter.contours.size(); i++) {
		if(i!=0)ofNextContour(true);
		for (int j=0; j<keyCharacter.contours[i].pts.size(); j++) {
			ofPoint p = keyCharacter.contours[i].pts[j];
			letter.addVertex(p);
		}
	}

}


