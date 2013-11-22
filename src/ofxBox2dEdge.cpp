//
//  ofxBox2dEdge.cpp
//  example-Bodies
//
//  Created by Todd Vanderlin on 11/18/13.
//
//

#include "ofxBox2dEdge.h"

//----------------------------------------
void ofxBox2dEdge::clear() {
    ofPolyline::clear();
    ofxBox2dBaseShape::destroy();
    mesh.clear();
}

//----------------------------------------
void ofxBox2dEdge::destroy() {
    ofPolyline::clear();
    ofxBox2dBaseShape::destroy();
    mesh.clear();
}

//----------------------------------------
void ofxBox2dEdge::create(b2World * b2dworld) {
   
    bFlagShapeUpdate = false;
    
    if(size() < 2) {
		printf("need at least 3 points\n");
		return;
	}
	
	if (body != NULL) {
		b2dworld->DestroyBody(body);
		body = NULL;
	}
	
	// create the body from the world (1)
	b2BodyDef		bd;
	bd.type			= density <= 0.0 ? b2_staticBody : b2_dynamicBody;
	body			= b2dworld->CreateBody(&bd);
    
    vector<ofPoint>&pts = ofPolyline::getVertices();
	for(int i=1; i<(int)size(); i++) {
        b2EdgeShape edge;
        edge.Set(screenPtToWorldPt(pts[i-1]), screenPtToWorldPt(pts[i]));
        body->CreateFixture(&edge, density);
    }
    mesh.clear();
    mesh.setUsage(body->GetType()==b2_staticBody?GL_STATIC_DRAW:GL_DYNAMIC_DRAW);
    mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
    for(int i=0; i<(int)size(); i++) {
        mesh.addVertex(ofVec3f(pts[i].x, pts[i].y));
    }
    
    flagHasChanged();
    alive = true;
}

/*
 These were in ofPolyline and now are gone?
 */
//----------------------------------------
void ofxBox2dEdge::addVertexes(vector <ofVec2f> &pts) {
	for (int i=0; i<pts.size(); i++) {
        ofPolyline::addVertex(pts[i].x, pts[i].y);
	}
    flagHasChanged();
}

//----------------------------------------
void ofxBox2dEdge::addVertexes(ofPolyline &polyline) {
	for (int i=0; i<polyline.size(); i++) {
		ofPolyline::addVertex(polyline[i].x, polyline[i].y);
	}
    flagHasChanged();
}

//----------------------------------------
void ofxBox2dEdge::updateShape() {
    if(body==NULL) return;
    const b2Transform& xf = body->GetTransform();
    ofPolyline::clear();
    mesh.clear();
    mesh.setUsage(body->GetType()==b2_staticBody?GL_STATIC_DRAW:GL_DYNAMIC_DRAW);
    mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
   
    for (b2Fixture * f = body->GetFixtureList(); f; f = f->GetNext()) {
        b2EdgeShape * edge = (b2EdgeShape*)f->GetShape();
        
        if(edge) {
            ofPolyline::addVertex(worldPtToscreenPt(edge->m_vertex2));
            ofPolyline::addVertex(worldPtToscreenPt(edge->m_vertex1));
            
            mesh.addVertex(ofVec3f(worldPtToscreenPt(edge->m_vertex2)));
            mesh.addVertex(ofVec3f(worldPtToscreenPt(edge->m_vertex1)));
        }
    }
    
    bFlagShapeUpdate = true;
    flagHasChanged();
}

//----------------------------------------
void ofxBox2dEdge::draw() {
    if(body==NULL) return;
   
	if(!bFlagShapeUpdate && body->GetType() != b2_staticBody) {
        printf("Need to update shape first\n");
    }
    mesh.draw(OF_MESH_WIREFRAME);
    //ofPolyline::draw();
    bFlagShapeUpdate = false;
}









