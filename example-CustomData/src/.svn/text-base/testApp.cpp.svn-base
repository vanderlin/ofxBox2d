#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup() {
	
	ofBackground(0, 0, 0);
	ofSetVerticalSync(true);
	ofSetFrameRate(30.0);
	
	
	
	playHeadPosition = 0.0;
	threshold		 = 80;
	fadeAmount		 = 0.1;
	
	
	input.loadMovie("crowd-motionTest-wide-960pixelby240.mov");
	input.play();
	vw = input.getWidth();
	vh = input.getHeight();
	
	
	color.allocate(vw, vh);
	gray.allocate(vw, vh);
	contourAnalysis.setSize(vw, vh);
	
	// Box2d
	box2d.init();
	box2d.setGravity(0, 20);
	box2d.createBounds(0, 0, vw, vh);
	box2d.checkBounds(true);
	box2d.setFPS(30.0);
}


//--------------------------------------------------------------
void testApp::update() {
	
	
	box2d.update();	
	input.update();
	
	
	if(input.isFrameNew()) {
		
		color.setFromPixels(input.getPixels(), vw, vh);
		gray = color;
		gray.threshold(threshold, true);
		
		int cleanYmin = 0;
		int cleanYmax = 123;
		if(cleanYmax > vh) cleanYmax = vh;
		
		unsigned char * px = gray.getPixels();
		for(int x=0; x<vw; x++) {
			for(int y=cleanYmin; y<cleanYmax; y++) {
				int index = y * vw + x;
				px[index] = 0;
			}
		}
		
		
		gray.setFromPixels(px, vw, vh);
		contour.findContours(gray, 30, vw*vh, 20, true);
		
		if(contour.nBlobs > 0) {
			contourAnalysis.simplify(contour.blobs[0].pts, simpleCountour, 1.0);
			
			ofPoint p;
			float y = 0;
			lineStrip.setWorld(box2d.getWorld());
			lineStrip.clear();
			
			for(int i=simpleCountour.size()-1; i>0; i--) {
				p.x = simpleCountour[i].x;
				p.y = simpleCountour[i].y;
				lineStrip.addPoint(p.x, p.y);
			}
			
			lineStrip.createShape();
		}
		
	}
	
	
	
	
}

//--------------------------------------------------------------
void testApp::draw() {
	
	ofSetColor(0xffffff);
	color.draw(0, 0);
	gray.draw(0, vh);
	
	ofNoFill();
	ofSetColor(255, 0, 0);
	contourAnalysis.draw(simpleCountour, 0, 0, vw, vh);
	
	
	// draw the box2d balls
	for(int i=0; i<customParticles.size(); i++) {
		customParticles[i].draw();
	}
	
	lineStrip.draw();
	
	string info = "FPS: "+ofToString(ofGetFrameRate());
	info += "\nThreshold: "+ofToString(threshold);
	info += "\nFade Amount: "+ofToString(fadeAmount);
	info += "\nPlay Head: "+ofToString(input.getPosition()*100.0);
	info += "\n\nPress [space] to add big balls.";
	info += "\nPress [b] to add small balls.";
	
	ofDrawBitmapString(info, 20, (vh*2)+55);
}


//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	
	
	
	if(key == OF_KEY_UP) {
		threshold ++;
		if(threshold > 255) threshold = 255;
	}
	if(key == OF_KEY_DOWN) {
		threshold --;
		if(threshold < 0) threshold = 0;
	}
	
	
	if(key == OF_KEY_LEFT) {
		fadeAmount += 0.02;
		if(fadeAmount > 1.0) fadeAmount = 1.0;
	}
	if(key == OF_KEY_RIGHT) {
		fadeAmount -= 0.02;
		if(fadeAmount < 0.0) fadeAmount = 0.0;
	}
	
	// big balls
	if(key == ' ') {	
		float r = ofRandom(20, 40);		
		CustomParticle p;
		
		// Mass Bounce Friction
		p.setPhysics(0.001, 0.9, 0.01);
		p.setup(box2d.getWorld(), mouseX, mouseY, r);
		p.c = (int)ofRandom(30, 100);
		customParticles.push_back(p);
		
	}
	
	// small balls
	if(key == 'b') {	
		float r = ofRandom(8, 15);		
		CustomParticle p;
		
		// Mass Bounce Friction
		p.setPhysics(0.001, 0.3, 0.9);
		p.setup(box2d.getWorld(), mouseX, mouseY, r);
		p.c = (int)ofRandom(3, 255);
		customParticles.push_back(p);
		
	}
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
	input.setPosition((float)mouseX/(float)ofGetWidth());	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h) {
}

