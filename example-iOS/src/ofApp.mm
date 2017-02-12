#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	coreMotion.setupMagnetometer();
	coreMotion.setupGyroscope();
	coreMotion.setupAccelerometer();
	coreMotion.setupAttitude(CMAttitudeReferenceFrameXMagneticNorthZVertical);

	ofSetFrameRate(30);
	ofBackgroundHex(0xfdefc2);

	box2d.init();
	box2d.setGravity(0, 10);
	box2d.setFPS(60);
	box2d.registerGrabbing();
	box2d.createBounds();
	box2d.setIterations(1, 1); // minimum for IOS

	for(int i=0; i<10; i++) {
		auto c = std::make_shared<ofxBox2dCircle>();
		c->setPhysics(1, 0.4, 0.4);
		c->setup(box2d.getWorld(), ofRandomWidth(), ofRandomHeight(), ofRandom(13, 25));
		circles.push_back(c);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	coreMotion.update();

	ofVec2f gravity = coreMotion.getAccelerometerData();
	gravity.y *= -1;
	gravity *= 30;
	box2d.setGravity(gravity);

	box2d.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetHexColor(0xABDB44);
	for(auto c : circles) {
		c->draw();
	}

	ofSetColor(90);
	ofDrawBitmapString("double tap to add more", 20, 30);
	ofDrawBitmapString(ofToString(ofGetFrameRate(), 0)+" fps", 20, 50);
}


//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs &touch){
	auto c = std::make_shared<ofxBox2dCircle>();
	c->setPhysics(1, 0.4, 0.4);
	c->setup(box2d.getWorld(), touch.x, touch.y, ofRandom(13, 25));
	circles.push_back(c);
}

