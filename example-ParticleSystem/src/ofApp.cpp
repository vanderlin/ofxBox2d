#include "ofApp.h"

static int colors[] = {0xFFBDBA, 0xFFF68D, 0xFF6DA9, 0x030692, 0x1F1F1F};

//--------------------------------------------------------------
void ofApp::setup(){

    ofBackgroundHex(colors[0]);
    ofSetVerticalSync(true);
    
    box2d.init();
    box2d.setFPS(60);
    box2d.createBounds();
    
    int maxParticles = 5000; // 5k particles
    
    particleSystem.init(box2d.getWorld());
    particleSystem.setMaxParticles(maxParticles);
    particleSystem.setRadius(5);
    
    glm::vec2 center(ofGetWidth()/2, ofGetHeight()/2);
    
    for(int i=0; i<maxParticles; i++) {
        particleSystem.addParticle(center.x + ofRandom(-50, 50),
                                   center.y + ofRandom(-50, 50));
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    box2d.update();
    
    if (ofGetFrameNum() == 20) {
        auto rect = make_shared<ofxBox2dRect>();
        rect->setPhysics(1.2, 0.2, 0.2);
        rect->setup(box2d.getWorld(), ofGetWidth()/2, 100, 200, 200);
        rects.push_back(rect);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

    particleSystem.updateMesh();
    
    ofSetHexColor(colors[1]);
    particleSystem.draw();
    
    // get the connections between particles
    /*
    auto * contacts = particleSystem.getParticleSystem()->GetContacts();
    auto * positions = particleSystem.getParticleSystem()->GetPositionBuffer();
    int count = particleSystem.getParticleSystem()->GetContactCount();
    for(int i=0; i<count; i++) {
        auto contact = contacts[i];
        auto a = ofxBox2d::toOf(positions[contact.GetIndexA()]);
        auto b = ofxBox2d::toOf(positions[contact.GetIndexB()]);
        ofSetColor(0);
        ofDrawLine(a, b);
    }*/
    
    for(auto &rect : rects) {
        ofSetHexColor(colors[3]);
        ofFill();
        rect->draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == ' ') {
        auto rect = make_shared<ofxBox2dRect>();
        rect->setPhysics(0.8, 0.2, 0.2);
        rect->setup(box2d.getWorld(), ofGetMouseX(), ofGetMouseY(), ofRandom(10, 50), ofRandom(10, 50));
        rects.push_back(rect);
    }
    
    if (key == '1') {
        for(int i=0; i<particleSystem.getTotalParticles(); i++) {
            
            glm::vec2 center(ofGetWidth()/2, ofGetHeight()/2);
            
            particleSystem.addParticle(center.x + ofRandom(-50, 50), center.y + ofRandom(-50, 50));
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    particleSystem.addParticle(x, y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    box2d.createBounds();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
