#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetVerticalSync(true);
	
	// Box2d
	box2d.init();
	box2d.setGravity(0, 20);
	box2d.createGround();
	box2d.setFPS(30.0);
	
}

//--------------------------------------------------------------
void testApp::update() {
	box2d.update();	
}

//--------------------------------------------------------------
void testApp::draw() {
	
	for(int i=0; i<particles.size(); i++) {
		particles[i].draw();
	}
	
	string info = "FPS: "+ofToString(ofGetFrameRate(), 1);
	info += "\nClick to add particles\nThe letters and color are from the custom data.";
	ofSetHexColor(0x444342);
	ofDrawBitmapString(info, 20, 20);
}


//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	CustomParticle p;
	p.setPhysics(1.0, 0.5, 0.3);
	p.setup(box2d.getWorld(), x, y, ofRandom(20, 60));
	p.setupTheCustomData();
	particles.push_back(p);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h) {
}

