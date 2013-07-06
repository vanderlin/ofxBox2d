#include "testApp.h"

static int pts[] = {257,219,257,258,259,274,263,325,266,345,266,352,269,369,276,387,286,415,291,425,302,451,308,462,316,472,321,480,328,488,333,495,339,501,345,505,350,507,365,515,370,519,377,522,382,525,388,527,405,534,426,538,439,539,452,539,468,540,485,540,496,541,607,541,618,539,625,537,641,530,666,513,682,500,710,476,723,463,727,457,729,453,732,450,734,447,738,440,746,423,756,404,772,363,779,343,781,339,784,327,789,301,792,278,794,267,794,257,795,250,795,232,796,222,796,197,797,195,797,188,796,188};
static int nPts  = 61*2;

//--------------------------------------------------------------
void testApp::setup() {
	ofSetVerticalSync(true);
	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);
	
	bMouseForce = false;
	
	box2d.init();
	box2d.setGravity(0, 10);
	box2d.createGround();
	box2d.setFPS(30.0);
	box2d.registerGrabbing();
	
	// lets add a contour to start
	for (int i=0; i<nPts; i+=2) {
		float x = pts[i];
		float y = pts[i+1];
		polyLine.addVertex(x, y);
	}
	
	// make the shape
	polyLine.setPhysics(0.0, 0.5, 0.5);
	polyLine.create(box2d.getWorld());		
	
}

//--------------------------------------------------------------
void testApp::update() {
	
	
	
	box2d.update();
	
	
	if(bMouseForce) {
		float strength = 8.0f;
		float damping  = 0.7f;
		float minDis   = 100;
		for(int i=0; i<circles.size(); i++) {
			circles[i].addAttractionPoint(mouseX, mouseY, strength);
			circles[i].setDamping(damping, damping);
		}
		for(int i=0; i<customParticles.size(); i++) {
			customParticles[i].addAttractionPoint(mouseX, mouseY, strength);
			customParticles[i].setDamping(damping, damping);
		}
		
	}
	
}


//--------------------------------------------------------------
void testApp::draw() {
	
	
	for(int i=0; i<circles.size(); i++) {
		ofFill();
		ofSetHexColor(0x90d4e3);
		circles[i].draw();
	}
	
	for(int i=0; i<boxes.size(); i++) {
		ofFill();
		ofSetHexColor(0xe63b8b);
		boxes[i].draw();
	}
	
	for(int i=0; i<customParticles.size(); i++) {
		customParticles[i].draw();
	}
	
	ofNoFill();
	ofSetHexColor(0x444342);
	if(drawing.size()==0) polyLine.draw();
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
void testApp::keyPressed(int key) {
	
	if(key == 'c') {
		float r = ofRandom(4, 20);		// a random radius 4px - 20px
		ofxBox2dCircle circle;
		circle.setPhysics(3.0, 0.53, 0.1);
		circle.setup(box2d.getWorld(), mouseX, mouseY, r);
		circles.push_back(circle);
	}
	
	if(key == 'b') {
		float w = ofRandom(4, 20);	
		float h = ofRandom(4, 20);	
		ofxBox2dRect rect;
		rect.setPhysics(3.0, 0.53, 0.1);
		rect.setup(box2d.getWorld(), mouseX, mouseY, w, h);
		boxes.push_back(rect);
	}
	
	if(key == 'z') {
		float r = ofRandom(3, 10);		// a random radius 4px - 20px
		CustomParticle p;
		p.setPhysics(0.4, 0.53, 0.31);
		p.setup(box2d.getWorld(), mouseX, mouseY, r);
		p.color.r = ofRandom(20, 100);
		p.color.g = 0;
		p.color.b = ofRandom(150, 255);
		customParticles.push_back(p);
	}	
		
	if(key == 'f') bMouseForce = !bMouseForce;
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
	drawing.addVertex(x, y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	
	if(polyLine.isBody()) {
		drawing.clear();
		polyLine.destroy();	
	}
	
	drawing.addVertex(x, y);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
	
	drawing.setClosed(false);
	drawing.simplify();
	
	polyLine.addVertexes(drawing);
	polyLine.simplify();
	polyLine.setPhysics(0.0, 0.5, 0.5);
	polyLine.create(box2d.getWorld());
		
	drawing.clear();

}

//--------------------------------------------------------------
void testApp::resized(int w, int h){
	
}

