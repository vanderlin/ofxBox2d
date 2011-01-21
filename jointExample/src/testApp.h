#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxBox2d.h"

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
	ofxBox2d		box2d;
	ofxBox2dJoint	joints[20];
	ofxBox2dCircle	circles[20];
	
	int				index;
	int				num;
};

#endif
