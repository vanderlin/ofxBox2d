#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	
	ofSetVerticalSync(true);
	ofBackground(20, 20, 20);
	
	box2d.init();
	box2d.setGravity(0, 0);
	box2d.createBounds();
	box2d.setFPS(30.0);
	box2d.registerGrabbing();
	
	index	= 0;
	num		= 14;
	
	for (int i = 0; i < num; i++) {
		circles[i].setPhysics(0.2, 0.85, .7);
		circles[i].setup(box2d.getWorld(), ofRandom(200, 500), ofRandom(200, 500), 17);
	}
	for (int i = 0; i < num; i++) {
		if (i == 0) {
			joints[0].setup(box2d.getWorld(), circles[num-1].body, circles[0].body);
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
	for (int i = 0; i < num; i++)  {
		ofSetColor(30, 100, 190);
		circles[i].draw();
	}
	
	for (int i = 0; i < num; i++) {
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
	if (index <= num) {
		if (index < num) {
			circles[index].setPosition(ofVec2f(x, y));
		}
		
		if (index == num) {
			// could set the length manually //
			b2Vec2 a1 = circles[num-1].body->GetWorldCenter();
			b2Vec2 a2 = circles[0].body->GetWorldCenter();
			b2Vec2 d = a2 - a1;
			
			joints[0].setLength(d.Length());
			index = 0;
		} else if( index > 0 ) {
			// or destroy the joint and call the setup function //
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

