#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	
	ofSetVerticalSync(true);
	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofDisableAntiAliasing();
	
	box2d.init();
	box2d.setGravity(0, 0);
	box2d.setFPS(60.0);
		
	for(int i=0; i<100; i++) {
		
		float r = ofRandom(10, 20);
		auto circle = std::make_shared<ofxBox2dCircle>();
		circle.get()->setPhysics(3.0, 0.53, 0.9);
		circle.get()->setup(box2d.getWorld(), ofGetWidth()/2, ofGetHeight()/2, r);
		circles.push_back(circle);
		
		float w = ofRandom(4, 20);
		float h = ofRandom(4, 20);
		auto rect = std::make_shared<ofxBox2dRect>();
		rect.get()->setPhysics(3.0, 0.53, 0.9);
		rect.get()->setup(box2d.getWorld(), ofGetWidth()/2, ofGetHeight()/2, w, h);
		boxes.push_back(rect);
	}
}

//--------------------------------------------------------------
void ofApp::update() {
	
	box2d.update();	
	ofVec2f mouse(ofGetMouseX(), ofGetMouseY());
	float minDis = ofGetMousePressed() ? 300 : 200;

	for(int i=0; i<circles.size(); i++) {
		float dis = mouse.distance(circles[i].get()->getPosition());
		if(dis < minDis) circles[i].get()->addRepulsionForce(mouse, 9);
		else circles[i].get()->addAttractionPoint(mouse, 4.0);
		
		
	}
	for(int i=0; i<boxes.size(); i++) {
		float dis = mouse.distance(boxes[i].get()->getPosition());
		if(dis < minDis) boxes[i].get()->addRepulsionForce(mouse, 9);
		else boxes[i].get()->addAttractionPoint(mouse, 4.0);
	}
}


//--------------------------------------------------------------
void ofApp::draw() {
	
	for(int i=0; i<circles.size(); i++) {
		ofFill();
		ofSetHexColor(0xf6c738);
		circles[i].get()->draw();
	}
	
	for(int i=0; i<boxes.size(); i++) {
		ofFill();
		ofSetHexColor(0xBF2545);
		boxes[i].get()->draw();
	}
	
	// draw the ground
	box2d.drawGround();
	
	string info = "";
	info += "Press [c] for circles\n";
	info += "Press [b] for blocks\n";
	info += "Total Bodies: "+ofToString(box2d.getBodyCount())+"\n";
	info += "Total Joints: "+ofToString(box2d.getJointCount())+"\n\n";
	info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n";
	ofSetHexColor(0x444342);
	ofDrawBitmapString(info, 30, 30);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	if(key == 'c') {
		float r = ofRandom(14, 20);		// a random radius 4px - 20px
		auto circle = std::make_shared<ofxBox2dCircle>();
		circle.get()->setPhysics(3.0, 0.53, 0.9);
		circle.get()->setup(box2d.getWorld(), mouseX, mouseY, r);
		circles.push_back(circle);
	}
	
	if(key == 'b') {
		float w = ofRandom(14, 20);	
		float h = ofRandom(14, 20);	
		auto rect = std::make_shared<ofxBox2dRect>();
		rect.get()->setPhysics(3.0, 0.53, 0.9);
		rect.get()->setup(box2d.getWorld(), mouseX, mouseY, w, h);
		boxes.push_back(rect);
	}
	
	if(key == 't') ofToggleFullscreen();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ) {
	
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
