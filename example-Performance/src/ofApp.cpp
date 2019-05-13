#include "ofApp.h"

static bool removeShapeOffScreen(shared_ptr<ofxBox2dBaseShape> shape) {
    if (!ofRectangle(0, -400, ofGetWidth(), ofGetHeight()+400).inside(shape.get()->getPosition())) {
        return true;
    }
    return false;
}

//--------------------------------------------------------------
void ofApp::setup() {
    
 
	ofDisableAntiAliasing();
	ofBackgroundHex(0x1F2C30);
	ofSetLogLevel(OF_LOG_NOTICE);
    ofSetVerticalSync(true);
    
	box2d.init();
	box2d.setGravity(0, 10);
	box2d.setFPS(60.0);
	box2d.registerGrabbing();
    box2d.createGround();
    
    ground.setPhysics(0, 0, 0);
    ground.setup(box2d.getWorld(), ofGetWidth()/2, ofGetHeight()+200, 400);
    
    groundMesh.setUsage(GL_STATIC_DRAW);
    groundMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    for(int i=0; i<100; i++) {
        float t = ofMap(i, 0, 99, 0.0, TWO_PI);
        ofVec3f v(ofGetWidth()/2, ofGetHeight()+200);
        groundMesh.addVertex(v);
        v.x += cos(t)*400;
        v.y += sin(t)*400;
        groundMesh.addVertex(v);
    }
}

//--------------------------------------------------------------
void ofApp::update() {
   
    ofRemove(circles, removeShapeOffScreen);
    ofRemove(boxes, removeShapeOffScreen);
    
	box2d.update();
    
    float r = ofRandom(4, 20);
    auto circle = make_shared<ofxBox2dCircle>();
    circle->setPhysics(3.0, 0.53, 0.1);
    circle->setup(box2d.getWorld(), ofGetWidth()/2+ofRandom(-100, 100), -200+ofRandom(30, 100), r);
    circles.push_back(circle);

    float w = ofRandom(4, 20);
    float h = ofRandom(4, 20);
    auto box = make_shared<ofxBox2dRect>();
    box->setPhysics(3.0, 0.53, 0.1);
    box->setup(box2d.getWorld(), ofGetWidth()/2+ofRandom(-100, 100), -200+ofRandom(30, 100), w, h);
    boxes.push_back(box);
}

//--------------------------------------------------------------
void ofApp::draw() {
	
	
    for(auto &circle : circles) {
		ofFill();
		ofSetHexColor(0x90d4e3);
		circle->draw();
	}
	
    for(auto &box : boxes) {
		ofFill();
		ofSetHexColor(0xe63b8b);
		box->draw();
	}

    ofSetColor(255, 100);
    groundMesh.draw();
    
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
        for(int i=0; i<10; i++) {
            float r = ofRandom(2, 5);
            auto circle = make_shared<ofxBox2dCircle>();
            circle->setPhysics(3.0, 0.53, 0.1);
            circle->setup(box2d.getWorld(), ofGetMouseX()+ofRandom(-10, 10), ofGetMouseY()+ofRandom(-10, 10), r);
            circles.push_back(circle);
        }
    } else if(key == 'b') {
        float w = ofRandom(2, 20);
        float h = ofRandom(2, 20);
        auto box = make_shared<ofxBox2dRect>();
        box->setPhysics(3.0, 0.53, 0.1);
        box->setup(box2d.getWorld(), ofGetMouseX(), ofGetMouseY(), w, h);
        boxes.push_back(box);
    }
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

