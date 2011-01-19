#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	
	
//	ofRegisterTouchEvents(this);
	ofxAccelerometer.setup();
	ofxiPhoneAlerts.addListener(this);
	ofBackground(127,127,127);

	ofSetVerticalSync(true);
	ofBackground(20, 20, 20);
	ofSetLogLevel(OF_LOG_NOTICE);
	
	box2d.init();
	box2d.setGravity(0, 10);
	box2d.createBounds();
	
	box2d.setFPS(30.0);
	box2d.registerGrabbing();

	// make some bounds
	
	// now add some circles
	for(int i=0; i<30; i++) {
		float r = ofRandom(4, 20);
		ofxBox2dCircle circle;
		circle.setPhysics(3.0, 0.53, 0.1);
		circle.setup(box2d.getWorld(), ofGetWidth()/2, ofGetHeight()/2, r);
		circles.push_back(circle);
	}
}

//--------------------------------------------------------------
void testApp::update() {
	ofPoint gravity = ofxAccelerometer.getForce();
	gravity *= 30.0;
	gravity.y *= -1;
	box2d.setGravity(gravity);
	box2d.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	for(int i=0; i<circles.size(); i++) {
		circles[i].draw();
	}
	
	box2d.draw(); // used to draw the floor
}

//--------------------------------------------------------------
void testApp::exit(){

}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::lostFocus(){

}

//--------------------------------------------------------------
void testApp::gotFocus(){

}

//--------------------------------------------------------------
void testApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void testApp::deviceOrientationChanged(int newOrientation){

}

