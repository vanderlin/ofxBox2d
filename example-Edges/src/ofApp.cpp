
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofDisableAntiAliasing();
	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetVerticalSync(true);

	// Box2d
	box2d.init();
	box2d.setGravity(0, 30);
	box2d.createGround();
	box2d.setFPS(60.0);

	// load the lines we saved...

	ofBuffer buffer = ofBufferFromFile("lines.txt");

	for (auto line: buffer.getLines()) {
		vector <string> pts = ofSplitString(line, ",");
		if(pts.size() > 0) {
			auto edge = std::make_shared<ofxBox2dEdge>();
			for (auto j=0; j<pts.size(); j+=2) {
				if(pts[j].size() > 0) {
					float x = ofToFloat(pts[j]);
					float y = ofToFloat(pts[j+1]);
					edge.get()->addVertex(x, y);
				}
			}
			edge.get()->create(box2d.getWorld());
			edges.push_back(edge);
		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	// add some circles every so often
	if((int)ofRandom(0, 10) == 0) {
		auto c = std::make_shared<ofxBox2dCircle>();
		c.get()->setPhysics(0.2, 0.2, 0.002);
		c.get()->setup(box2d.getWorld(), ofRandom(20, 50), -20, ofRandom(3, 10));
		c.get()->setVelocity(0, 15); // shoot them down!
		circles.push_back(c);
	}

	box2d.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	// some circles :)
	for (int i=0; i<circles.size(); i++) {
		ofFill();
		ofSetHexColor(0xc0dd3b);
		circles[i].get()->draw();
	}

	ofSetHexColor(0x444342);
	ofNoFill();
	for (int i=0; i<lines.size(); i++) {
		lines[i].draw();
	}
	for (int i=0; i<edges.size(); i++) {
		edges[i].get()->draw();
	}

	string info = "Draw a shape with the mouse\n";
	info += "Press 1 to add some circles\n";
	info += "Press c to clear everything\n";

	ofSetHexColor(0x444342);
	ofDrawBitmapString(info, 10, 15);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	if(key == '1') {
		auto c = std::make_shared<ofxBox2dCircle>();
		c.get()->setPhysics(1, 0.5, 0.5);
		c.get()->setup(box2d.getWorld(), mouseX, mouseY, 10);
		circles.push_back(c);
	}

	if(key == 'c') {
		lines.clear();
		edges.clear();
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
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	lines.back().addVertex(x, y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	lines.push_back(ofPolyline());
	lines.back().addVertex(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

	auto edge = std::make_shared<ofxBox2dEdge>();
	lines.back().simplify();

	for (auto i=0; i<lines.back().size(); i++) {
		edge.get()->addVertex(lines.back()[i]);
	}

	//poly.setPhysics(1, .2, 1);  // uncomment this to see it fall!
	edge.get()->create(box2d.getWorld());
	edges.push_back(edge);

	//lines.clear();
}
