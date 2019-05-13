#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"
#include "TextureShape.h"

// ------------------------------------------------- App
class ofApp : public ofBaseApp {
	
public:
	
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

    ofxBox2d box2d;

    // a vector of all the textures
    vector <shared_ptr<ofTexture>> textures;

    // a vector of all the texture shapes
    vector <shared_ptr<TextureShape> > shapes;
};
