#include "ofApp.h"

static int pts[] = {257,219,257,258,259,274,263,325,266,345,266,352,269,369,276,387,286,415,291,425,302,451,308,462,316,472,321,480,328,488,333,495,339,501,345,505,350,507,365,515,370,519,377,522,382,525,388,527,405,534,426,538,439,539,452,539,468,540,485,540,496,541,607,541,618,539,625,537,641,530,666,513,682,500,710,476,723,463,727,457,729,453,732,450,734,447,738,440,746,423,756,404,772,363,779,343,781,339,784,327,789,301,792,278,794,267,794,257,795,250,795,232,796,222,796,197,797,195,797,188,796,188};
static int nPts  = 61*2;

//--------------------------------------------------------------
void ofApp::setup() {
    
	ofSetVerticalSync(true);
	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);
	
	bMouseForce = false;
	
	box2d.init();
	box2d.setGravity(0, 10);
	box2d.createGround();
	box2d.setFPS(60.0);
	box2d.registerGrabbing();
	
	// lets add a contour to start
	for(int i=0; i<nPts; i+=2) {
		float x = pts[i];
		float y = pts[i+1];
		edgeLine.addVertex(x, y);
	}
	
	// make the shape
	edgeLine.setPhysics(0.0, 0.5, 0.5);
	edgeLine.create(box2d.getWorld());
}

//--------------------------------------------------------------
void ofApp::update() {
	
	box2d.update();
	
	if(bMouseForce) {
		float strength = 8.0f;
		float damping  = 0.7f;
		float minDis   = 100;
		for(int i=0; i<circles.size(); i++) {
			circles[i].get()->addAttractionPoint(mouseX, mouseY, strength);
			circles[i].get()->setLinearDamping(damping);
		}
		for(int i=0; i<customParticles.size(); i++) {
			customParticles[i].get()->addAttractionPoint(mouseX, mouseY, strength);
			customParticles[i].get()->setLinearDamping(damping);
		}
	}
	
	// remove shapes offscreen
	ofRemove(boxes, ofxBox2dBaseShape::shouldRemoveOffScreen);
	ofRemove(circles, ofxBox2dBaseShape::shouldRemoveOffScreen);
	ofRemove(customParticles, ofxBox2dBaseShape::shouldRemoveOffScreen);
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
	
    for(auto &particle : customParticles) {
		particle->draw();
	}
	
	ofNoFill();
	ofSetHexColor(0x444342);
	if(drawing.size()==0) {
		edgeLine.draw();
	}
	else drawing.draw();
	
	string info = "";
	info += "Press [s] to draw a line strip ["+ofToString(bDrawLines)+"]\n"; 
	info += "Press [f] to toggle Mouse Force ["+ofToString(bMouseForce)+"]\n"; 
	info += "Press [c] for circles\n";
	info += "Press [b] for blocks\n";
	info += "Press [z] for custom particle\n";
	info += "Total Bodies: "+ofToString(box2d.getBodyCount())+"\n";
	info += "Total Joints: "+ofToString(box2d.getJointCount())+"\n\n";
	info += "FPS: "+ofToString(ofGetFrameRate())+"\n";
	ofSetHexColor(0x444342);
	ofDrawBitmapString(info, 30, 30);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	if(key == 'c') {
        auto circle = make_shared<ofxBox2dCircle>();
    	circle->setPhysics(3.0, 0.53, 0.1);
		circle->setup(box2d.getWorld(), mouseX, mouseY, ofRandom(4, 20));
        circles.push_back(circle);
	}
	
	if(key == 'b') {
		float w = ofRandom(4, 20);	
		float h = ofRandom(4, 20);
        auto box = make_shared<ofxBox2dRect>();
		box->setPhysics(3.0, 0.53, 0.1);
		box->setup(box2d.getWorld(), mouseX, mouseY, w, h);
        boxes.push_back(box);
	}
	
	if(key == 'z') {
        auto particle = make_shared<CustomParticle>(box2d.getWorld(), mouseX, mouseY);
        customParticles.push_back(particle);
	}	
		
	if(key == 'f') bMouseForce = !bMouseForce;
	if(key == 't') ofToggleFullscreen();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	drawing.addVertex(x, y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	
    // if we have a body clear the line and destroy b2d object
	if(edgeLine.isBody()) {
		drawing.clear();
		edgeLine.destroy();
	}
	
	drawing.addVertex(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	
	drawing.setClosed(false);
	drawing.simplify();
	
	edgeLine.addVertexes(drawing);
	//polyLine.simplifyToMaxVerts(); // this is based on the max box2d verts
	edgeLine.setPhysics(0.0, 0.5, 0.5);
	edgeLine.create(box2d.getWorld());
		
	drawing.clear();
}

//--------------------------------------------------------------
void ofApp::resized(int w, int h){
	
}
