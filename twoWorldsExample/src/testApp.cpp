#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    ofSetFrameRate(60);
    
    
    // the world bounds
    boundsA.set(0, 0, 500, 500);
    boundsB.set(500, 0, 500, 500);
    
    
    // setup world A
    box2dA.init();
    box2dA.setFPS(60);
    box2dA.setGravity(0, -10);
    box2dA.createBounds(boundsA);
    box2dA.registerGrabbing();
    
    // setup world B
    box2dB.init();
    box2dB.setFPS(60);
    box2dB.setGravity(0, 10);
    box2dB.createBounds(boundsB);
    box2dB.registerGrabbing();
    
    
    // add some cirlces to world A
    for(int i=0; i<10; i++) {
        ofxBox2dCircle c;
        c.setPhysics(1, 0.5, 1);
        c.setup(box2dA.getWorld(), 250+ofRandom(-50, 50), 10, ofRandom(10,30));
        circlesA.push_back(c);
    }
    
    
    
    // add some cirlces to world B
    for(int i=0; i<10; i++) {
        ofxBox2dCircle c;
        c.setPhysics(1, 0.5, 1);
        c.setup(box2dB.getWorld(), 750+ofRandom(-50, 50), 10, ofRandom(10,30));
        circlesB.push_back(c);
    }
    
    
    // we can also have a vector of any shape that is
    // shared between both worlds
    for(int i=0; i<20; i++) {
        
        ofxBox2dRect r;
        r.setPhysics(1, 0.7, 0.9);
        
        // add to world A
        if(i <= 9) {
            r.setup(box2dA.getWorld(), 250+ofRandom(-50, 50), 10, ofRandom(10,30), ofRandom(10,30));            
        }
        
        // add to world B
        else {
            r.setup(box2dB.getWorld(), 750+ofRandom(-50, 50), 10, ofRandom(10,30), ofRandom(10,30));
        }
        
        // add to one vector
        sharedRects.push_back(r);
    }
}


//--------------------------------------------------------------
void testApp::update(){
    
    
    box2dA.update();
    box2dB.update();
    
   
}

//--------------------------------------------------------------
void testApp::drawGravity(ofPoint p, ofPoint gravity) {
    
    float angle = (atan2(gravity.y, gravity.x) * 180 / PI) - 90;
    float len   = MIN(200, gravity.length()*10); // scale it up a bit
    
    ofPushMatrix();
    ofTranslate(p.x, p.y);
    ofRotate(angle);
    ofLine(0, 0, 0, len);
    ofTriangle(0, len, 
               -5, len-10, 
               5, len-10);
    ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::draw(){
    
    // world A 
    boundsA.inside(ofGetMouseX(), ofGetMouseY()) ? ofSetColor(80) : ofSetColor(100);
    ofFill();
    ofRect(boundsA);

    // world A 
    boundsB.inside(ofGetMouseX(), ofGetMouseY()) ? ofSetColor(180) : ofSetColor(200);
    ofFill();
    ofRect(boundsB);

    ofFill();
    
    // A World Circles
    for (int i=0; i<circlesA.size(); i++) {
        ofSetHexColor(0xBFE364);
        circlesA[i].draw();
    }
    
    // B World Circles
    for (int i=0; i<circlesB.size(); i++) {
        ofSetHexColor(0xE83AAB);
        circlesB[i].draw();
    }
    
    // Shared Rects
    for (int i=0; i<sharedRects.size(); i++) {
        ofSetHexColor(0x2F9BA1);
        ofFill();
        sharedRects[i].draw();
    }
    
    
   
    ofSetColor(255); ofDrawBitmapString("World A\nGravity "+ofToString(box2dA.getGravity().x, 1)+","+ofToString(box2dA.getGravity().y, 1), 250, ofGetHeight()/2);
    ofSetColor(90);  ofDrawBitmapString("World B\nGravity "+ofToString(box2dB.getGravity().x, 1)+","+ofToString(box2dB.getGravity().y, 1), 750, ofGetHeight()/2);    
    
    
    ofPoint centerA(250, 250);
    ofPoint centerB(750, 250);
    ofSetColor(255);
    drawGravity(centerA, box2dA.getGravity());
    drawGravity(centerB, box2dB.getGravity());
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {
    
   
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

    float maxGravity = 100;
    
    if(boundsA.inside(x, y)) {
        float gx = ofMap(x, 0, 500, -maxGravity, maxGravity);
        float gy = ofMap(y, 0, ofGetHeight(), -maxGravity, maxGravity);
        box2dA.setGravity(gx, gy);
    }
    else if(boundsB.inside(x, y)) {
        float gx = ofMap(x, 500, ofGetWidth(), -maxGravity, maxGravity);
        float gy = ofMap(y, 0, ofGetHeight(), -maxGravity, maxGravity);
        box2dB.setGravity(gx, gy);
    }
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}