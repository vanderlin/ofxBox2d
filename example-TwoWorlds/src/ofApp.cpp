#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetFrameRate(60);
	
	// the world bounds
	boundsA.set(0, 0, 500, 500);
	boundsB.set(500, 0, 500, 500);
	
	// setup world A
	box2dA.init();
	box2dA.setFPS(60);
	box2dA.setGravity(0, -10);
	box2dA.createBounds(boundsA);
	box2dA.registerGrabbing();
	
	// setup world B
	box2dB.init();
	box2dB.setFPS(60);
	box2dB.setGravity(0, 10);
	box2dB.createBounds(boundsB);
	box2dB.registerGrabbing();
	
	// add some cirlces to world A
	for(int i=0; i<10; i++) {
		auto c = std::make_shared<ofxBox2dCircle>();
		c.get()->setPhysics(1, 0.5, 1);
		c.get()->setup(box2dA.getWorld(), 250+ofRandom(-50, 50), 10, ofRandom(10,30));
		circlesA.push_back(c);
	}
	
	// add some cirlces to world B
	for(int i=0; i<10; i++) {
		auto c = std::make_shared<ofxBox2dCircle>();
		c.get()->setPhysics(1, 0.5, 1);
		c.get()->setup(box2dB.getWorld(), 750+ofRandom(-50, 50), 10, ofRandom(10,30));
		circlesB.push_back(c);
	}
	
	// we can also have a vector of any shape that is
	// shared between both worlds
	for(int i=0; i<20; i++) {
		
		auto r = std::make_shared<ofxBox2dRect>();
		r.get()->setPhysics(1, 0.7, 0.9);
		
		// add to world A
		if(i <= 9) {
			r.get()->setup(box2dA.getWorld(), 250+ofRandom(-50, 50), 10, ofRandom(10,30), ofRandom(10,30));
		}
		
		// add to world B
		else {
			r.get()->setup(box2dB.getWorld(), 750+ofRandom(-50, 50), 10, ofRandom(10,30), ofRandom(10,30));
		}
		
		// add to one vector
		sharedRects.push_back(r);
	}
}


//--------------------------------------------------------------
void ofApp::update(){
	box2dA.update();
	box2dB.update();
}

//--------------------------------------------------------------
void ofApp::drawGravity(ofPoint p, ofPoint gravity) {
	
	float angle = (atan2(gravity.y, gravity.x) * 180 / PI) - 90;
	float len   = MIN(200, gravity.length()*10); // scale it up a bit
	
	ofPushMatrix();
	ofTranslate(p.x, p.y);
	ofRotate(angle);
	ofDrawLine(0, 0, 0, len);
	ofDrawTriangle(0, len,
	              -5, len-10,
	               5, len-10);
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	// world A 
	boundsA.inside(ofGetMouseX(), ofGetMouseY()) ? ofSetColor(80) : ofSetColor(100);
	ofFill();
	ofDrawRectangle(boundsA);

	// world A 
	boundsB.inside(ofGetMouseX(), ofGetMouseY()) ? ofSetColor(180) : ofSetColor(200);
	ofFill();
	ofDrawRectangle(boundsB);

	ofFill();
	
	// A World Circles
	for (int i=0; i<circlesA.size(); i++) {
		ofSetHexColor(0xBFE364);
		circlesA[i].get()->draw();
	}
	
	// B World Circles
	for (int i=0; i<circlesB.size(); i++) {
		ofSetHexColor(0xE83AAB);
		circlesB[i].get()->draw();
	}
	
	// Shared Rects
	for (int i=0; i<sharedRects.size(); i++) {
		ofSetHexColor(0x2F9BA1);
		ofFill();
		sharedRects[i].get()->draw();
	}
   
	ofSetColor(255); ofDrawBitmapString("World A\nGravity "+ofToString(box2dA.getGravity().x, 1)+","+ofToString(box2dA.getGravity().y, 1), 250, ofGetHeight()/2);
	ofSetColor(90);  ofDrawBitmapString("World B\nGravity "+ofToString(box2dB.getGravity().x, 1)+","+ofToString(box2dB.getGravity().y, 1), 750, ofGetHeight()/2);
	
	ofPoint centerA(250, 250);
	ofPoint centerB(750, 250);
	ofSetColor(255);
	drawGravity(centerA, box2dA.getGravity());
	drawGravity(centerB, box2dB.getGravity());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

	float maxGravity = 100;
	
	if(boundsA.inside(x, y)) {
		float gx = ofMap(x, 0, 500, -maxGravity, maxGravity);
		float gy = ofMap(y, 0, ofGetHeight(), -maxGravity, maxGravity);
		box2dA.setGravity(gx, gy);
	}
	else if(boundsB.inside(x, y)) {
		float gx = ofMap(x, 500, ofGetWidth(), -maxGravity, maxGravity);
		float gy = ofMap(y, 0, ofGetHeight(), -maxGravity, maxGravity);
		box2dB.setGravity(gx, gy);
	}
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
