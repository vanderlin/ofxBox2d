#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    
	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetVerticalSync(true);
    ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL);
    
	// Box2d
	box2d.init();
	box2d.setGravity(0, 0);
	box2d.createBounds();
	box2d.setFPS(60.0);
	
}

//--------------------------------------------------------------
void testApp::update() {
	box2d.update();	
}

//--------------------------------------------------------------
void testApp::draw() {
	
	for(int i=0; i<particles.size(); i++) {
		particles[i].get()->draw();
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
	shared_ptr<CustomParticle> p = shared_ptr<CustomParticle>(new CustomParticle);
	p.get()->setPhysics(1.0, 0.5, 0.3);
	p.get()->setup(box2d.getWorld(), x, y, ofRandom(20, 60));
    p.get()->setVelocity(ofRandom(-30, 30), ofRandom(-30, 30));
	p.get()->setupTheCustomData();
	particles.push_back(p);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h) {
}

