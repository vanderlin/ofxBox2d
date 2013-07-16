#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	
	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetVerticalSync(true);
	
    // find all the texture files and load them
    ofDirectory dir;
    ofDisableArbTex();
    int n = dir.listDir("textures");
    for (int i=0; i<n; i++) {
        textures.push_back(ofImage(dir.getPath(i)));
    }
    printf("%i Textures Loaded\n", (int)textures.size());
    
	// Box2d
	box2d.init();
	box2d.setGravity(0, 10);
	box2d.createGround();
	box2d.setFPS(30.0);
    box2d.registerGrabbing();
    

}

//--------------------------------------------------------------
void testApp::update() {
	
	// add some circles every so often
	if((int)ofRandom(0, 10) == 0) {
		 
        shapes.push_back(TextureShape());
        shapes.back().setTexture(&textures[(int)ofRandom(textures.size())]);
        shapes.back().setup(box2d, (ofGetWidth()/2)+ofRandom(-20, 20), -20, ofRandom(10, 50));
   
    }
	
	box2d.update();	
}

//--------------------------------------------------------------
void testApp::draw() {
	
	for (int i=0; i<shapes.size(); i++) {
        shapes[i].draw();
    }
    
	
	
	// some debug information
	string info = "Textures from subtlepatterns.com\n";
	info += "Press c to clear everything\n";
	
	ofSetHexColor(0x444342);
	ofDrawBitmapString(info, 10, 15);
}


//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	
	
	if(key == 'c') {
		for (int i=0; i<shapes.size(); i++) {
			shapes[i].polyShape.destroy();
		}
        shapes.clear();
	}
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
void testApp::windowResized(int w, int h) {
}

