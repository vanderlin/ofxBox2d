#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	
	ofSetVerticalSync(true);
	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);

	box2d.init();
	box2d.setGravity(10, 0);
	box2d.setFPS(30.0);
	box2d.registerGrabbing();
	
	anchor.setup(box2d.getWorld(), 20, ofGetHeight()/2, 4);
	
	// first we add just a few circles
	for (int i=0; i<3; i++) {
		ofxBox2dCircle circle;
		circle.setPhysics(3.0, 0.53, 0.1);
		circle.setup(box2d.getWorld(), ofGetWidth()/2, 100+(i*20), 8);
		circles.push_back(circle);
	}
	
	// now connect each circle with a joint
	for (int i=0; i<circles.size(); i++) {
		
		ofxBox2dJoint joint;
		
		// if this is the first point connect to the top anchor.
		if(i == 0) {
			joint.setup(box2d.getWorld(), anchor.body, circles[i].body);		
		}
		else {
			joint.setup(box2d.getWorld(), circles[i-1].body, circles[i].body);
		}
		
		joint.setLength(25);
		joints.push_back(joint);
	}
}

//--------------------------------------------------------------
void testApp::update() {
	box2d.update();	
}


//--------------------------------------------------------------
void testApp::draw() {
	
	ofSetHexColor(0xf2ab01);
	anchor.draw();
	
	for(int i=0; i<circles.size(); i++) {
		ofFill();
		ofSetHexColor(0x01b1f2);
		circles[i].draw();
	}
	
	for(int i=0; i<joints.size(); i++) {
		ofSetHexColor(0x444342);
		joints[i].draw();
	}
	
	string info = "";
	info += "Press [n] to add a new joint\n";
	info += "click and pull the chain around\n";
	info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n";
	ofSetHexColor(0x444342);
	ofDrawBitmapString(info, 30, 30);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	
	if(key == 'n') {
		
		// add a new circle
		ofxBox2dCircle circle;
		circle.setPhysics(3.0, 0.53, 0.1);
		circle.setup(box2d.getWorld(), circles.back().getPosition().x+ofRandom(-30, 30), circles.back().getPosition().y-30, 8);
		circles.push_back(circle);
	
		// get this cirlce and the prev cirlce
		int a = (int)circles.size()-2;
		int b = (int)circles.size()-1; 

		// now connect the new circle with a joint
		ofxBox2dJoint joint;
		joint.setup(box2d.getWorld(), circles[a].body, circles[b].body);
		joint.setLength(25);
		joints.push_back(joint);
	}
	
	if(key == 't') ofToggleFullscreen();
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
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::resized(int w, int h){
}

