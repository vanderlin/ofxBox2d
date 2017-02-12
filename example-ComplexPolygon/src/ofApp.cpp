#include "ofApp.h"

static bool shouldRemove(shared_ptr<ofxBox2dBaseShape>shape) {
	return !ofRectangle(0, -400, ofGetWidth(), ofGetHeight()+400).inside(shape->getPosition());
}

//--------------------------------------------------------------
void ofApp::setup() {

	ofDisableAntiAliasing();
	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetVerticalSync(true);

	// Box2d
	box2d.init();
	box2d.setGravity(0, 10);
	box2d.createGround();
	box2d.setFPS(30.0);

	breakupIntoTriangles = true;

	// load the shape we saved...
	auto pts = loadPoints("shape.dat");
	auto poly = std::make_shared<ofxBox2dPolygon>();
	poly->addVertices(pts);
	poly->setPhysics(1.0, 0.3, 0.3);
	poly->triangulatePoly();
	poly->create(box2d.getWorld());
	polyShapes.push_back(poly);
}

//--------------------------------------------------------------
vector <ofDefaultVertexType> ofApp::loadPoints(const std::string& file) {
	vector <ofDefaultVertexType> pts;
	vector <string>  ptsStr = ofSplitString(ofBufferFromFile(file).getText(), ",");
	for(int i=0; i<ptsStr.size(); i+=2) {
		float x = ofToFloat(ptsStr[i]);
		float y = ofToFloat(ptsStr[i+1]);
		pts.push_back(ofDefaultVertexType(x, y, 0));
	}
	return pts;
}


//--------------------------------------------------------------
void ofApp::update() {

	// add some circles every so often
	if((int)ofRandom(0, 10) == 0) {
		auto circle = std::make_shared<ofxBox2dCircle>();
		circle->setPhysics(0.3, 0.5, 0.1);
		circle->setup(box2d.getWorld(), (ofGetWidth()/2)+ofRandom(-20, 20), -20, ofRandom(10, 20));
		circles.push_back(circle);
	}

	// remove shapes offscreen
	ofRemove(circles, shouldRemove);
	// ofRemove(polyShapes, shouldRemove);

	box2d.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	// some circles :)
	for(int i=0; i<circles.size(); i++) {
		ofFill();
		ofSetHexColor(0xc0dd3b);
		circles[i]->draw();
	}

	ofSetHexColor(0x444342);
	ofFill();
	shape.draw();

	ofSetHexColor(0x444342);
	ofNoFill();
	for(int i=0; i<polyShapes.size(); i++) {
		polyShapes[i]->draw();

		ofDrawCircle(polyShapes[i]->getPosition(), 3);
	}

	// some debug information
	string info = "Draw a shape with the mouse\n";
	info += "Press 1 to add some circles\n";
	info += "Press c to clear everything\n";
	info += "Press t to break object up into triangles/convex poly: "+string(breakupIntoTriangles?"true":"false")+"\n";
	info += "Total Bodies: "+ofToString(box2d.getBodyCount())+"\n";
	info += "Total Joints: "+ofToString(box2d.getJointCount())+"\n\n";
	info += "FPS: "+ofToString(ofGetFrameRate())+"\n";
	ofSetHexColor(0x444342);
	ofDrawBitmapString(info, 10, 15);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	if(key == '1') {
		auto circle = std::make_shared<ofxBox2dCircle>();
		circle->setPhysics(0.3, 0.5, 0.1);
		circle->setup(box2d.getWorld(), mouseX, mouseY, ofRandom(10, 20));
		circles.push_back(circle);
	}

	if(key == 't') breakupIntoTriangles = !breakupIntoTriangles;

	if(key == 'c') {
		shape.clear();
		polyShapes.clear();
		circles.clear();
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	shape.addVertex(x, y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	shape.clear();
	shape.addVertex(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

	if(breakupIntoTriangles) {

		// This is the manual way to triangulate the shape
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
		for(int i=0; i<tris.size(); i++) {

			auto triangle = std::make_shared<ofxBox2dPolygon>();
			triangle->addTriangle(ofDefaultVertexType(tris[i].a.x,
																								tris[i].a.y,
																								0),
														ofDefaultVertexType(tris[i].b.x,
																								tris[i].b.y,
																								0),
														ofDefaultVertexType(tris[i].c.x,
																								tris[i].c.y,
																								0));
			triangle->setPhysics(1.0, 0.3, 0.3);
			triangle->create(box2d.getWorld());

			polyShapes.push_back(triangle);
		}

	}
	else {
		auto poly = std::make_shared<ofxBox2dPolygon>();
		poly->addVertices(shape.getVertices());
		poly->setPhysics(1.0, 0.3, 0.3);
		poly->triangulatePoly();
		poly->create(box2d.getWorld());
		polyShapes.push_back(poly);
	}

	// done with shape clear it now
	shape.clear();
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}
