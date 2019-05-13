#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofBackgroundHex(0xfdefc2);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetVerticalSync(true);

	// Box2d
	box2d.init();
	box2d.setGravity(0, 10);
	box2d.createGround();
	box2d.setFPS(60.0);

	breakupIntoTriangles = true;
    bFill = false;
    
	// load the shapes we saved...
    polyShapes.push_back(loadSavedShape("shape_0.xml"));
    polyShapes.push_back(loadSavedShape("shape_1.xml"));
}

//--------------------------------------------------------------
shared_ptr<ofxBox2dPolygon> ofApp::loadSavedShape(const std::string& file) {
    
    ofXml xml;
    auto poly = std::make_shared<ofxBox2dPolygon>();
    
    if(xml.load(file)) {
        vector <glm::vec3> pnts;
        auto children = xml.getChildren("point");
        
        for(auto &child : children) {
            float x = child.getAttribute("x").getFloatValue();
            float y = child.getAttribute("y").getFloatValue();
            pnts.push_back(glm::vec3(x, y, 0));
        }
        
        poly->addVertices(pnts);
        poly->setPhysics(1.0, 0.3, 0.3);
        poly->triangulate();
        poly->create(box2d.getWorld());
        
    }
    return poly;
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
    ofRemove(polyShapes, [](shared_ptr<ofxBox2dPolygon> shape) -> bool {
        return !ofRectangle(0, -400, ofGetWidth(), ofGetHeight()+400).inside(shape->getPosition());
    });
    ofRemove(circles, [](shared_ptr<ofxBox2dCircle> shape) -> bool {
        return !ofRectangle(0, -400, ofGetWidth(), ofGetHeight()+400).inside(shape->getPosition());
    });
    
	box2d.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

    for(auto &circle : circles) {
		ofFill();
		ofSetHexColor(0xc0dd3b);
		circle->draw();
	}

	ofSetHexColor(0x444342);
	ofFill();
	shape.draw();

	ofSetHexColor(0x444342);
    bFill ? ofFill() : ofNoFill();
    for(auto & poly : polyShapes) {
        poly->isTriangulated() ? poly->drawTriangles() : poly->draw();
    }

	string info = "Draw a shape with the mouse\n";
	info += "Press 1 to add some circles\n";
	info += "Press c to clear everything\n";
    info += "Press b to toggle fill\n";
	info += "Press t to break object up into triangles/convex poly: "+string(breakupIntoTriangles?"true":"false")+"\n";
	info += "Total Bodies: "+ofToString(box2d.getBodyCount())+"\n";
	info += "Total Joints: "+ofToString(box2d.getJointCount())+"\n\n";
	info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n";
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
    if(key == 'b') bFill = !bFill;
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
//
//    int i = 0;
//    for(auto s : shapes) {
//        ofXml xml;
//        for(auto &pnt: s) {
//            auto child = xml.appendChild("point");
//            child.setAttribute("x", pnt.x);
//            child.setAttribute("y", pnt.y);
//        }
//        xml.save("shape_"+ofToString(i)+".xml");
//        i ++;
//    }
//    return;
	if(breakupIntoTriangles) {

		// This is the manual way to triangulate the shape
		// you can then add many little triangles

		// first simplify the shape
		shape.simplify();

		// resample shape
		auto resampled = shape.getResampledBySpacing(25);

		// triangulate the shape, return am vector of traingles
        auto tris = ofxBox2dPolygonUtils::triangulate(resampled);

		// now loop through all the triangles and make a box2d triangle
		for(int i=0; i<tris.size(); i++) {

			auto triangle = std::make_shared<ofxBox2dPolygon>();
			
            triangle->addTriangle(ofDefaultVertexType(tris[i].a.x, tris[i].a.y, 0),
                                  ofDefaultVertexType(tris[i].b.x, tris[i].b.y, 0),
                                  ofDefaultVertexType(tris[i].c.x, tris[i].c.y, 0));
            
			triangle->setPhysics(1.0, 0.3, 0.3);
			
            triangle->create(box2d.getWorld());

			polyShapes.push_back(triangle);
		}

	}
	else {
		auto poly = std::make_shared<ofxBox2dPolygon>();
		poly->addVertices(shape.getVertices());
		poly->setPhysics(1.0, 0.3, 0.3);
		poly->triangulate();
		poly->create(box2d.getWorld());
		polyShapes.push_back(poly);
	}

	// done with shape clear it now
	shape.clear();
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}
