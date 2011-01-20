#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofSetVerticalSync(true);
	ofBackground(20, 20, 20);
	
	box2d.init();
	box2d.setGravity(0, 10);
	box2d.createGround();
	box2d.setFPS(30.0);
	box2d.registerGrabbing();
	
	index = 0;
	
	for (int i = 0; i < 10; i++) {
		circles[i].setPhysics(0.4, 0.5, .7);
		circles[i].setup(box2d.getWorld(), ofRandom(200, 500), ofRandom(200, 500), 14);
	}
	for (int i = 0; i < 10; i++) {
		if (i == 0) {
			joints[0].setup(box2d.getWorld(), circles[9].body, circles[0].body);
		} else {
			joints[i].setup(box2d.getWorld(), circles[i].body, circles[i-1].body);
		}
	}
}

//--------------------------------------------------------------
void testApp::update() {
	box2d.update();
}

//--------------------------------------------------------------
void testApp::draw() {
	for (int i = 0; i < 10; i++)  {
		ofSetColor(30, 100, 190);
		circles[i].draw();
	}
	
	for (int i = 0; i < 10; i++) {
		ofSetColor(200, 180, 20);
		joints[i].draw();
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	if (index <= 10) {
		if (index < 10) {
			circles[index].setPosition(ofVec2f(x, y));
		}
		if (index == 10) {
			joints[0].destroy();
			joints[0].setup(box2d.getWorld(), circles[9].body, circles[0].body);
			index = 0;
		} else if( index > 0 ) {
			joints[index].destroy();
			joints[index].setup(box2d.getWorld(), circles[index].body, circles[index-1].body);
			index++;
		}
	}
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h) {

}

