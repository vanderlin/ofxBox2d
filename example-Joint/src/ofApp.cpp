#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	
	ofSetVerticalSync(true);
	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);
    ofDisableAntiAliasing();
    
    
	box2d.init();
	box2d.setGravity(10, 0);
	box2d.setFPS(60.0);
	box2d.registerGrabbing();
	
	anchor.setup(box2d.getWorld(), 20, ofGetHeight()/2, 4);
	
	// first we add just a few circles
	for (int i=0; i<3; i++) {
		auto circle = make_shared<ofxBox2dCircle>();
		circle->setPhysics(3.0, 0.53, 0.1);
		circle->setup(box2d.getWorld(), ofGetWidth()/2, 100+(i*20), 8);
		circles.push_back(circle);
	}
	
	// now connect each circle with a joint
	for (int i=0; i<circles.size(); i++) {
		
		auto joint = make_shared<ofxBox2dJoint>(box2d.getWorld(), i == 0 ? anchor.body : circles[i-1]->body, circles[i]->body);
		
		joint->setLength(25);
		joints.push_back(joint);
	}
}

//--------------------------------------------------------------
void ofApp::update() {
	box2d.update();	
}


//--------------------------------------------------------------
void ofApp::draw() {
	
	ofSetHexColor(0xf2ab01);
	anchor.draw();
	
    for(auto &circle : circles) {
		ofFill();
		ofSetHexColor(0x01b1f2);
		circle->draw();
	}
	
    for(auto &joint : joints) {
		ofSetHexColor(0x444342);
		joint->draw();
	}
	
	string info = "";
	info += "Press [n] to add a new joint\n";
	info += "click and pull the chain around\n";
	info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n";
	ofSetHexColor(0x444342);
	ofDrawBitmapString(info, 30, 30);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	if(key == 'n') {
		
		// add a new circle
		auto circle = make_shared<ofxBox2dCircle>();
		circle->setPhysics(3.0, 0.53, 0.1);
		circle->setup(box2d.getWorld(), circles.back()->getPosition().x+ofRandom(-30, 30), circles.back()->getPosition().y-30, 8);
		circles.push_back(circle);
	
		// get this cirlce and the prev cirlce
		int a = (int)circles.size()-2;
		int b = (int)circles.size()-1; 

		// now connect the new circle with a joint
		auto joint = make_shared<ofxBox2dJoint>(box2d.getWorld(), circles[a]->body, circles[b]->body);
		joint.get()->setLength(25);
		joints.push_back(joint);
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

