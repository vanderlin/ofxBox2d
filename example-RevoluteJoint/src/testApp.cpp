#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	
	ofSetVerticalSync(true);
	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);
    ofDisableAntiAliasing();
    
    
	box2d.init();
	box2d.setGravity(0, 10.0);
	box2d.setFPS(30.0);
	box2d.registerGrabbing();
	
	anchor.setup(box2d.getWorld(), ofGetWidth()/2, ofGetHeight()/2, 5);
	
	// first we add just a few rectangles
	for (int i=0; i<1; i++) {
		ofPtr<ofxBox2dRect> rect = ofPtr<ofxBox2dRect>(new ofxBox2dRect);
		rect->setPhysics(3.0, 0.53, 0.1);
		rect->setup(box2d.getWorld(), (ofGetWidth()/2)+i*(w-h)+(w-h)/2, ofGetHeight()/2, w, h);
		rects.push_back(rect);
	}
	
	// now connect each rectangle with a joint
	for (int i=0; i<rects.size(); i++) {
		
		ofPtr<ofxBox2dRevoluteJoint> joint = ofPtr<ofxBox2dRevoluteJoint>(new ofxBox2dRevoluteJoint);
		
		// if this is the first point connect to the top anchor.
		if(i == 0) {
			joint->setup(box2d.getWorld(), anchor.body, rects[i]->body, b2Vec2(0,0), b2Vec2(-(w-h)/2,0));
            joint->disableLimit();
		}
		else {
			joint->setup(box2d.getWorld(), rects[i-1]->body, rects[i]->body, b2Vec2((w-h)/2,0), b2Vec2(-(w-h)/2,0));
		}
		joints.push_back(joint);
	}
}

//--------------------------------------------------------------
void testApp::update() {
	box2d.update();	
}


//--------------------------------------------------------------
void testApp::draw() {
	
	for(int i=0; i<rects.size(); i++) {
		ofFill();
		ofSetHexColor(0x01b1f2);
		rects[i]->draw();
	}
	
	for(int i=0; i<joints.size(); i++) {
		ofSetHexColor(0x666564);
		joints[i]->draw();
	}
    
	ofSetHexColor(0xf2ab01);
    anchor.draw();
	
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

		// add a new rectangle
		ofPtr<ofxBox2dRect> rect = ofPtr<ofxBox2dRect>(new ofxBox2dRect);
		rect->setPhysics(3.0, 0.53, 0.1);
        ofVec2f lastAnchor = joints.back()->getAnchor();
        float angle = 0;
        for (int i=0; i<joints.size(); i++) {
            angle += joints[i]->getJointAngle();
        }
        ofVec2f nextAnchor = lastAnchor + ofVec2f(1.5*(w-h), 0).rotate(angle);
		rect->setup(box2d.getWorld(), nextAnchor.x, nextAnchor.y-30, w, h);
        rect->setRotation(angle);
		rects.push_back(rect);
        
		// get this cirlce and the prev cirlce
		int a = (int)rects.size()-2;
		int b = (int)rects.size()-1; 
        
		// now connect the new circle with a joint
		ofPtr<ofxBox2dRevoluteJoint> joint = ofPtr<ofxBox2dRevoluteJoint>(new ofxBox2dRevoluteJoint);
		joint->setup(box2d.getWorld(), rects[a]->body, rects[b]->body, b2Vec2((w-h)/2,0), b2Vec2(-(w-h)/2,0));
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

