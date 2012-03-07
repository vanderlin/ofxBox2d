#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	
    
	ofRegisterTouchEvents(this);
	ofxAccelerometer.setup();
	ofxiPhoneAlerts.addListener(this);
	ofSetFrameRate(30);
	ofBackgroundHex(0xfdefc2);
    
    
    box2d.init();
    box2d.setGravity(0, 10);
    box2d.setFPS(60);
    box2d.registerGrabbing();
    box2d.createBounds();
    box2d.setIterations(1, 1); // minimum for IOS
    
    for (int i=0; i<10; i++) {
        ofxBox2dCircle c;
        c.setPhysics(1, 0.4, 0.4);
        c.setup(box2d.getWorld(), ofRandomWidth(), ofRandomHeight(), ofRandom(13, 25));
        circles.push_back(c);
    }
    
}

//--------------------------------------------------------------
void testApp::update() {
    
    ofVec2f gravity = ofxAccelerometer.getForce();
    gravity.y *= -1;
    gravity *= 30;
    box2d.setGravity(gravity);
    
    box2d.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofSetHexColor(0xABDB44);
    for(vector<ofxBox2dCircle>::iterator it = circles.begin(); it != circles.end(); ++it) {
        it->draw();
    }
 
    ofSetColor(90);
    ofDrawBitmapString("double tap to add more", 20, 30);
    ofDrawBitmapString(ofToString(ofGetFrameRate(), 0)+" fps", 20, 50);
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
    ofxBox2dCircle c;
    c.setPhysics(1, 0.4, 0.4);
    c.setup(box2d.getWorld(), touch.x, touch.y, ofRandom(13, 25));
    circles.push_back(c);
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


//--------------------------------------------------------------
void testApp::touchCancelled(ofTouchEventArgs& args){
    
}

