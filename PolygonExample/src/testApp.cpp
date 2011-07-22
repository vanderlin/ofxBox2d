#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	
	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetVerticalSync(true);
	
	// Box2d
	box2d.init();
	box2d.setGravity(0, 20);
	box2d.createGround();
	box2d.setFPS(30.0);
	
	
	// load the lines we saved...
	ifstream f;
	f.open(ofToDataPath("lines.txt").c_str());
	vector <string> strLines;
	while (!f.eof()) {
		string ptStr;
		getline(f, ptStr);
		strLines.push_back(ptStr);
	}
	f.close();
	
	for (int i=0; i<strLines.size(); i++) {
		vector <string> pts = ofSplitString(strLines[i], ",");
		if(pts.size() > 0) {
			ofxBox2dPolygon poly;
			for (int j=0; j<pts.size(); j+=2) {
				if(pts[j].size() > 0) {
					float x = ofToFloat(pts[j]);
					float y = ofToFloat(pts[j+1]);
					poly.addVertex(x, y);
				}				
			}
			poly.create(box2d.getWorld());
			polyLines.push_back(poly);
		}
	}
	
}

//--------------------------------------------------------------
void testApp::update() {
	
	// add some circles every so often
	if((int)ofRandom(0, 10) == 0) {
		ofxBox2dCircle c;
		c.setPhysics(1, 0.5, 0.1);
		c.setup(box2d.getWorld(), ofRandom(20, 50), -20, ofRandom(3, 10));
		circles.push_back(c);		
	}
	
	box2d.update();	
}

//--------------------------------------------------------------
void testApp::draw() {
	
	
	// some circles :)
	for (int i=0; i<circles.size(); i++) {
		ofFill();
		ofSetHexColor(0xc0dd3b);
		circles[i].draw();
	}
	
	ofSetHexColor(0x444342);
	ofNoFill();
	for (int i=0; i<lines.size(); i++) {
		lines[i].draw();
	}	
	for (int i=0; i<polyLines.size(); i++) {
		polyLines[i].draw();
	}	
	
	string info = "Draw a shape with the mouse\n";
	info += "Press 1 to add some circles\n";
	info += "Press c to clear everything\n";
	
	ofSetHexColor(0x444342);
	ofDrawBitmapString(info, 10, 15);
}


//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	
	if(key == '1') {
		ofxBox2dCircle c;
		c.setPhysics(1, 0.5, 0.5);
		c.setup(box2d.getWorld(), mouseX, mouseY, 10);
		circles.push_back(c);
	}
	
	if(key == 'c') {
		lines.clear();
		for (int i=0; i<polyLines.size(); i++) {
			polyLines[i].destroy();
		}
	}
	
	/*
	// want to save out some line...
	if(key == ' ') {
		ofstream f;
		f.clear();
		f.open(ofToDataPath("lines.txt").c_str());
		for (int i=0; i<lines.size(); i++) {
			for (int j=0; j<lines[i].size(); j++) {
				float x = lines[i][j].x;
				float y = lines[i][j].y;
				f << x << "," << y << ",";
			}
			f << "\n";
		}
		f.close();lines.clear();
	}*/
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
	lines.back().addVertex(x, y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	lines.push_back(ofPolyline());
	lines.back().addVertex(x, y);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
	
	ofxBox2dPolygon poly;
	lines.back().simplify();
	
	for (int i=0; i<lines.back().size(); i++) {
		poly.addVertex(lines.back()[i]);
	}
	
	//poly.setPhysics(1, .2, 1);  // uncomment this to see it fall!
	poly.create(box2d.getWorld());
	polyLines.push_back(poly);
	
	//lines.clear();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h) {
}

