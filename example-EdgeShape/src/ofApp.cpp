#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	
	ofDisableAntiAliasing();
	ofBackgroundHex(0x1F2C30);
	ofSetLogLevel(OF_LOG_NOTICE);
	
	// scrolling ground
	groundSpeed = 0.02;
	groundTimer = 0;
	groundRes   = 50;
	
	// build the ground first
	for(int i=0; i<groundRes; i++) {
		float x = ofMap(i, 0, groundRes-1, 0, ofGetWidth());
		float y = ofGetHeight()/2 + ofRandom(-30, 100);
		groundLine.addVertex(x, y);
	}

	box2d.init();
	box2d.setGravity(0, 20);
	box2d.setFPS(60.0);
	box2d.registerGrabbing();
}

//--------------------------------------------------------------
void ofApp::update() {
	
	ofRemove(circles, ofxBox2dBaseShape::shouldRemoveOffScreen);
	ofRemove(boxes, ofxBox2dBaseShape::shouldRemoveOffScreen);
	
	box2d.update();
	
	
	ground.clear();
	ground.addVertexes(groundLine);
	ground.create(box2d.getWorld());
}

//--------------------------------------------------------------
void ofApp::draw() {
	
	
	for(int i=0; i<circles.size(); i++) {
		ofFill();
		ofSetHexColor(0x90d4e3);
		circles[i].get()->draw();
	}
	
	for(int i=0; i<boxes.size(); i++) {
		ofFill();
		ofSetHexColor(0xe63b8b);
		boxes[i].get()->draw();
	}
	

	float   t = ofGetElapsedTimef() * 1.2;
	float   y = ofSignedNoise(t) * 200 + (sin(t)* 100);
	
	if(ofGetElapsedTimef() - groundTimer > groundSpeed) {
	
		float newHeight = ofGetHeight()/2 + y;
		int n = (int)groundLine.size();
	
		groundTimer = ofGetElapsedTimef();

		for(int i=n-1; i>=1; i--) {
			groundLine[i].y = groundLine[i-1].y;
		}
		groundLine[0].y = newHeight;
		
	}
	
	ofSetColor(255);
	ground.updateShape();
	ground.draw();
	
	string info = "";
	info += "Press [c] for circles\n";
	info += "Press [b] for blocks\n";
	info += "Total Bodies: "+ofToString(box2d.getBodyCount())+"\n";
	info += "Total Joints: "+ofToString(box2d.getJointCount())+"\n\n";
	info += "FPS: "+ofToString(ofGetFrameRate())+"\n";
	ofSetColor(255);
	ofDrawBitmapString(info, 30, 30);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	if(key == 'c') {
		float r = ofRandom(4, 20);		// a random radius 4px - 20px
		circles.push_back(shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle));
		ofxBox2dCircle * circle = circles.back().get();
		circle->setPhysics(3.0, 0.53, 0.1);
		circle->setup(box2d.getWorld(), mouseX, mouseY, r);
	}
	
	if(key == 'b') {
		float w = ofRandom(4, 20);	
		float h = ofRandom(4, 20);
		boxes.push_back(shared_ptr<ofxBox2dRect>(new ofxBox2dRect));
		ofxBox2dRect * rect = boxes.back().get();
		rect->setPhysics(3.0, 0.53, 0.1);
		rect->setup(box2d.getWorld(), mouseX, mouseY, w, h);
	}
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::resized(int w, int h){
	
}
