#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	
	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetVerticalSync(true);
	ofDisableAntiAliasing();
    
    // find all the texture files and load them
    ofDirectory dir;
    ofDisableArbTex();
    int n = dir.listDir("textures");
    for (int i=0; i<n; i++) {
        auto tex = make_shared<ofTexture>();
        ofLoadImage(*tex, dir.getPath(i));
        
        tex->setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
        tex->setTextureWrap(GL_REPEAT, GL_REPEAT);
        
        textures.push_back(tex);
    }
    printf("%i Textures Loaded\n", (int)textures.size());
    
	// Box2d
	box2d.init();
	box2d.setGravity(0, 10);
	box2d.createGround();
	box2d.setFPS(60.0);
    box2d.registerGrabbing();
    
}

//--------------------------------------------------------------
void ofApp::update() {
	
	// add some circles every so often
    if((int)ofRandom(0, 10) == 0) {
        
        auto tex = textures[(int)ofRandom(textures.size())];
        float x = (ofGetWidth()/2)+ofRandom(-20, 20);
        float y = -20;
        float r = ofRandom(10, 50);
        
        shapes.push_back(make_shared<TextureShape>(box2d.getWorld(), tex, x, y, r));
        
    }
	
	box2d.update();	
}

//--------------------------------------------------------------
void ofApp::draw() {
	
    for (auto &shape : shapes) {
        shape->draw();
    }
    
	// some debug information
	string info = "Textures from subtlepatterns.com\n";
	info += "Press c to clear everything\n";
	
	ofSetHexColor(0x444342);
	ofDrawBitmapString(info, 10, 15);
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	
	if(key == 'c') {
		shapes.clear();
	}
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
void ofApp::windowResized(int w, int h) {
}

