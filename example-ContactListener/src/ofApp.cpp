#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	
	ofSetVerticalSync(true);
	ofBackgroundHex(0xfdefc2);

	box2d.init();
    box2d.enableEvents();   // <-- turn on the event listener
	box2d.setGravity(0, 10);
	box2d.createGround();
	box2d.setFPS(60.0);
	box2d.registerGrabbing();
	
	// register the listener so that we get the events
	ofAddListener(box2d.contactStartEvents, this, &testApp::contactStart);
	ofAddListener(box2d.contactEndEvents, this, &testApp::contactEnd);

	// load the 8 sfx soundfile
	for (int i=0; i<N_SOUNDS; i++) {
		sound[i].loadSound("sfx/"+ofToString(i)+".mp3");
		sound[i].setMultiPlay(true);
		sound[i].setLoop(false);
	}
}


//--------------------------------------------------------------
void testApp::contactStart(ofxBox2dContactArgs &e) {
	if(e.a != NULL && e.b != NULL) { 
		
		// if we collide with the ground we do not
		// want to play a sound. this is how you do that
		if(e.a->GetType() == b2Shape::e_circle && e.b->GetType() == b2Shape::e_circle) {
			
			SoundData * aData = (SoundData*)e.a->GetBody()->GetUserData();
			SoundData * bData = (SoundData*)e.b->GetBody()->GetUserData();
			
			if(aData) {
				aData->bHit = true;
				sound[aData->soundID].play();
			}
			
			if(bData) {
				bData->bHit = true;
				sound[bData->soundID].play();
			}
		}
	}
}

//--------------------------------------------------------------
void testApp::contactEnd(ofxBox2dContactArgs &e) {
	if(e.a != NULL && e.b != NULL) { 
		
		SoundData * aData = (SoundData*)e.a->GetBody()->GetUserData();
		SoundData * bData = (SoundData*)e.b->GetBody()->GetUserData();
		
		if(aData) {
			aData->bHit = false;
		}
		
		if(bData) {
			bData->bHit = false;
		}
	}
}



//--------------------------------------------------------------
void testApp::update() {
	
	box2d.update();
	
	// add some circles every so often
	if((int)ofRandom(0, 50) == 0) {
		shared_ptr <ofxBox2dCircle> c = shared_ptr <ofxBox2dCircle>(new ofxBox2dCircle);
		c.get()->setPhysics(1, 0.5, 0.9);
		c.get()->setup(box2d.getWorld(), (ofGetWidth()/2)+ofRandom(-30, 30), -20, ofRandom(20, 50));
		
		c.get()->setData(new SoundData());
		SoundData * sd = (SoundData*)c.get()->getData();
		sd->soundID = ofRandom(0, N_SOUNDS);
		sd->bHit	= false;
		
		circles.push_back(c);	
	}

}


//--------------------------------------------------------------
void testApp::draw() {
	
	
	for(size_t i=0; i<circles.size(); i++) {
		ofFill();
		SoundData * data = (SoundData*)circles[i].get()->getData();
		
		if(data && data->bHit) ofSetHexColor(0xff0000);
		else ofSetHexColor(0x4ccae9);
		

		circles[i].get()->draw();
	}
	
	
	string info = "";
	info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n";
	ofSetHexColor(0x444342);
	ofDrawBitmapString(info, 30, 30);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	if(key == 't') ofToggleFullscreen();
    if(key == '1') box2d.enableEvents();
    if(key == '2') box2d.disableEvents();
    
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
    shared_ptr <ofxBox2dCircle> c = shared_ptr <ofxBox2dCircle>(new ofxBox2dCircle);
    c.get()->setPhysics(1, 0.5, 0.9);
    c.get()->setup(box2d.getWorld(), x, y, ofRandom(20, 50));
    
    c.get()->setData(new SoundData());
    SoundData * sd = (SoundData*)c.get()->getData();
    sd->soundID = ofRandom(0, N_SOUNDS);
    sd->bHit	= false;
    
    circles.push_back(c);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::resized(int w, int h){
}

