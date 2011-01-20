#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {	
	
	ofSetVerticalSync(true);
	ofBackground(20, 20, 20);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetFrameRate(30);
	ofxiPhoneSetOrientation(UIDeviceOrientationLandscapeLeft);
	
	scroller.setup();
	scroller.width = 2000;
	
	box2d.init();
	box2d.setGravity(0, 10);
	box2d.createBounds(0, 0, scroller.width, ofGetHeight());
	box2d.checkBounds(true);	// fix this...
	box2d.setFPS(30.0);
	box2d.registerGrabbing();
	box2d.setIterations(1, 1);
	
	
	
	/*
	polyBtn.name = "Draw Polygons";
	polyBtn.bToggle = true;
	
	clearBtn.name = "Clear";
	circleBtn.name = "Add Circles";
	
	
	buttons.push_back(&polyBtn);
	buttons.push_back(&clearBtn);
	buttons.push_back(&circleBtn);
	*/
}

//--------------------------------------------------------------
void testApp::update(){
	
	box2d.setGravity(ofxAccelerometer.getForce());
	box2d.update();
	scroller.update();
}

//--------------------------------------------------------------
void testApp::draw(){	

	
	ofPushMatrix();
	ofTranslate(scroller.pos.x, scroller.pos.y);
	
	scroller.draw();
	box2d.drawGround();

	for(int i=0; i<circles.size(); i++) {
		ofFill();
		ofSetHexColor(0xBFff45);
		circles[i].draw();
	}
	
	ofPopMatrix();
	
	/*
	for (int i=0; i<buttons.size(); i++) {
		buttons[i]->draw(20, 40+(i*50));
	}
	
	// draw the poly shape that we are about to make
	ofNoFill();
	ofBeginShape();
	for(int i=0; i<polyShape.size(); i++) {
		ofVertex(polyShape[i].x, polyShape[i].y);
	}
	ofEndShape();
	
	ofNoFill();
	for(int i=0; i<polyShape.size(); i++) {
		ofCircle(polyShape[i].x, polyShape[i].y, 1);
	}
	
	

	
	
	for(int i=0; i<polygons.size(); i++) {
		ofSetHexColor(0xFF2545);
		ofNoFill();
		polygons[i].draw();
	}
	
	
	for(int i=0; i<boxes.size(); i++) {
		ofFill();
		ofSetHexColor(0xBF2545);
		boxes[i].draw();
	}
	
	
	
	
	string info = "";
	//info += "Press [c] for circles\n";
	//info += "Press [b] for blocks\n";
	//info += "Press [z] for custom particle\n";
	info += "Total Bodies: "+ofToString(box2d.getBodyCount())+"\n";
	 info += "Total Joints: "+ofToString(box2d.getJointCount())+"\n\n";
	info += "FPS: "+ofToString(ofGetFrameRate())+"\n";
	ofSetColor(255, 255, 255);
	ofDrawBitmapString(info, 30, 30);
	 */
	
//	ofLine(scroller.toucheDown[0], ofVec2f(scroller.x, scroller.y));
//	ofLine(scroller.toucheDown[0], ofVec2f(scroller.x, scroller.toucheDown[0].y));
}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs &touch){
	
	printf("n:%i\n", touch.id);
	/*
	if (polyBtn.bToggle) polyShape.push_back(ofVec2f(touch.x, touch.y));
	
	SimpleButton * newBtn = NULL;
	for (int i=0; i<buttons.size(); i++) {
		if(buttons[i]->inside(touch.x, touch.y)) {
			buttons[i]->bToggle = !buttons[i]->bToggle;
			if(buttons[i]->bToggle) newBtn = buttons[i];
		}
	}
	
	if(clearBtn.bToggle) {
		polyShape.clear();
		for(int i=0; i<polygons.size(); i++) {
			polygons[i].destroy();
		}
		polygons.clear();	
		clearBtn.bToggle = false;	
	}
	
	if(newBtn != NULL) { 
		for (int i=0; i<buttons.size(); i++) {
			if(buttons[i]->name != newBtn->name) {
				buttons[i]->bToggle = false;
			}
		}	
	}
	
	
	if(circleBtn.bToggle) {
		ofxBox2dCircle circle;
		circle.setPhysics(1.0, 0.4, 0.3);;
		circle.setup(box2d.getWorld(), touch.x, touch.y, ofRandom(20, 50));
		circles.push_back(circle);
	}
	 */

	ofVec2f pt(touch.x, touch.y);

	if(touch.id == 0) {
		scroller.toucheDown[0].set(touch.x, touch.y);
		scroller.nTouches = 1;
		
		scroller.startX = scroller.pos.x;
		scroller.touchOffset = ABS(scroller.pos.x-touch.x);
		printf("x: %f\n", touch.x);
		printf("scx: %f\n", scroller.x);
		printf("touchOffset: %f\n", scroller.touchOffset);

	}
	if(touch.id == 1) {
		scroller.toucheDown[1].set(touch.x, touch.y);
		scroller.nTouches = 2;
	}
	
	
	if(touch.id == 2) {
		
		ofxBox2dCircle circle;
		circle.setPhysics(1.0, 0.4, 0.3);;
		circle.setup(box2d.getWorld(), touch.x+scroller.touchOffset, touch.y, ofRandom(20, 50));
		circles.push_back(circle);
		
	}
	
	
}

//--------------------------------------------------------------
void testApp::touchCancelled(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs &touch){
	
	ofVec2f pt(touch.x, touch.y);
	
	if(touch.id == 0) {
		scroller.toucheMove[0].set(touch.x, touch.y);
		scroller.nTouches = 1;
		
		
		float dx = touch.x - scroller.toucheDown[0].x;
		float mx = scroller.startX + dx;//scroller.touchOffset;
		
		printf("mx: %f\n", mx);
		printf("dx: %f\n", dx);
		scroller.des.x = mx;
		
		
		float maxVel = 60.0;
		float vx = MIN(maxVel, ABS(touch.x - scroller.prevx)) / maxVel;
		scroller.vx = vx;
		printf("speed x: %f\n", vx);
		scroller.prevx = touch.x;
	
	}
	if(touch.id == 1) {
		scroller.toucheMove[1].set(touch.x, touch.y);
		scroller.nTouches = 2;
	
	}
	
	
	
	/*if (polyBtn.bToggle) polyShape.push_back(ofVec2f(touch.x, touch.y));
	
	if(circleBtn.bToggle) {
		ofxBox2dCircle circle;
		circle.setPhysics(1.0, 0.4, 0.3);;
		circle.setup(box2d.getWorld(), touch.x, touch.y, ofRandom(20, 50));
		circles.push_back(circle);
	}
	 */
}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs &touch) {

	if(touch.id == 0) {

		// this is crap
		/*
		float inc = MIN(1.0, (4.0 * scroller.vx));
		scroller.des.x *= inc; 
		printf("inc %f\n", inc);*/
		
	}
	/*
	if (polyBtn.bToggle) {
		
		ofxBox2dPolygon poly;
		poly.setup(box2d.world);
		poly.addVertexes(polyShape);
		poly.simplify(2.0);
		poly.triangulate();
		poly.create(box2d.world);
		polygons.push_back(poly);
		
		polyShape.clear();
	}
	 */
}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs &touch){

}
