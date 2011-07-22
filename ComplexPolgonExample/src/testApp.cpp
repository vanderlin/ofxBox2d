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
	
	
	
	// load the shape we saved...
	vector <ofVec2f> pts = loadPoints("shape.dat");
	ofxBox2dPolygon poly;
	
	// loop and add vertex points
	for (int i=0; i<pts.size(); i++) {
		poly.addVertex(pts[i]);
	}
	poly.setAsEdge(false);
	poly.triangulate(15);
	poly.setPhysics(1.0, 0.3, 0.3);
	poly.create(box2d.getWorld());
	triangles.push_back(poly);
	
}

//--------------------------------------------------------------
vector <ofVec2f> testApp::loadPoints(string file) {
	ifstream f;
	vector <ofVec2f> pts;
	f.exceptions ( ifstream::failbit | ifstream::badbit );
	try {
		f.open(ofToDataPath(file).c_str());
		string strLines;
		while (!f.eof()) {
			string ptStr;
			getline(f, ptStr);
			strLines+=ptStr;
		}
		f.close();
		
		vector <string>  ptsStr = ofSplitString(strLines, ",");
		for (int i=0; i<ptsStr.size(); i+=2) {
			float x = ofToFloat(ptsStr[i]);
			float y = ofToFloat(ptsStr[i+1]);
			pts.push_back(ofVec2f(x, y));
		}
	}
	catch (ifstream::failure e) {
		printf("no file to load...\n");
	}
	return pts;
}


//--------------------------------------------------------------
void testApp::update() {
	
	// add some circles every so often
	if((int)ofRandom(0, 10) == 0) {
		ofxBox2dCircle c;
		c.setPhysics(0.3, 0.5, 0.1);
		c.setup(box2d.getWorld(), (ofGetWidth()/2)+ofRandom(-20, 20), -20, ofRandom(10, 20));
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
	ofFill();
	shape.draw();
	
	ofSetHexColor(0x444342);
	ofFill(); // <- OF not working here... 
	for (int i=0; i<triangles.size(); i++) {
		triangles[i].draw();
	}	
	
	
	// some debug information
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
		shape.clear();
		for (int i=0; i<triangles.size(); i++) {
			triangles[i].destroy();
		}
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
	shape.addVertex(x, y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	shape.clear();
	shape.addVertex(x, y);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
	
	// This is the manuel way to triangulate the shape
	// you can then add many little triangles
	
	// first simplify the shape
	shape.simplify();
	
	// save the outline of the shape
	ofPolyline outline = shape;
	
	// resample shape
	ofPolyline resampled = shape.getResampledBySpacing(25);
	
	// trangleate the shape, return am array of traingles
	vector <TriangleShape> tris = triangulatePolygonWithOutline(resampled, outline);
	
	// add some random points inside
	addRandomPointsInside(shape, 255);
	
	// now loop through all the trainles and make a box2d triangle
	for (int i=0; i<tris.size(); i++) {
		ofxBox2dPolygon p;
		p.addTriangle(tris[i].a, tris[i].b, tris[i].c);
		p.setPhysics(1.0, 0.3, 0.3);
		p.setAsEdge(false);
		if(p.isGoodShape()) {
			p.create(box2d.getWorld());
			triangles.push_back(p);
		}
	}
	
	// done with shape clear it now
	shape.clear();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h) {
}

