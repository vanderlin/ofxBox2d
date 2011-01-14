/*
 *  ofxBox2dPolygon.cpp
 *  ofxBox2d
 *
 *  Created by Todd Vanderlin on 1/14/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#include "ofxBox2dPolygon.h"


//----------------------------------------
ofxBox2dPolygon::ofxBox2dPolygon() { 
}

//----------------------------------------
ofxBox2dPolygon::~ofxBox2dPolygon() { 
}

//----------------------------------------
void ofxBox2dPolygon::calculateArea() {
	int i, j, n = vertexes.size();
	float polyArea = 0;
	for (i = 0; i < n; i++) {
		j = (i + 1) % n;
		polyArea += vertexes[i].x * vertexes[j].y;
		polyArea -= vertexes[j].x * vertexes[i].y;
	}
	polyArea /= 2.0;
	area = polyArea;
}

//----------------------------------------
void ofxBox2dPolygon::calculateCentroid() {
	float cx = 0, cy = 0;
	int i, j, n = vertexes.size();
	
	float factor = 0;
	for (i = 0; i < n; i++) {
		j = (i + 1) % n;
		factor = (vertexes[i].x * vertexes[j].y
				  - vertexes[j].x * vertexes[i].y);
		cx += (vertexes[i].x + vertexes[j].x) * factor;
		cy += (vertexes[i].y + vertexes[j].y) * factor;
	}
	area *= 6.0f;
	factor = 1 / area;
	cx *= factor;
	cy *= factor;
	center.set(cx, cy);	
}

//----------------------------------------
ofVec2f ofxBox2dPolygon::getCenter() {	
	if(!body) return 0;
	const b2Transform& xf = body->GetTransform();
	b2Vec2 b2Center = b2Mul(xf, b2Vec2(center.x/OFX_BOX2D_SCALE, center.y/OFX_BOX2D_SCALE));
	return ofVec2f(b2Center.x*OFX_BOX2D_SCALE, b2Center.y*OFX_BOX2D_SCALE);
};

//----------------------------------------
void ofxBox2dPolygon::updateShape() {
	
	calculateArea();
	calculateCentroid();
}

//----------------------------------------
void ofxBox2dPolygon::addVertex(const ofVec2f & p) {
	vertexes.push_back(p);
}

//----------------------------------------
void ofxBox2dPolygon::addVertex(float x, float y) {
	addVertex(ofVec2f(x, y));
}

//----------------------------------------
void ofxBox2dPolygon::addVertexes(const vector <ofVec2f> & polyPoints) {
	for(int i=0; i<polyPoints.size(); i++) {
		addVertex(polyPoints[i]);	
	}
}

//----------------------------------------
void ofxBox2dPolygon::create() {

	if(vertexes.size() < 2) {
		printf("need at least 2 points\n");
		return;	
	}
	
	
	
	// create the body
	b2PolygonShape	shape;
	b2FixtureDef	fixture;
	b2BodyDef		bd;
	bd.type = b2_dynamicBody;

	body = world->CreateBody(&bd);
	
	// add the verts and create the shape
	int totalVerts = (int)vertexes.size();
	b2Vec2 v1, v2;	
	for(int i=1; i<totalVerts; i++) {

		v1.Set(vertexes[i-1].x/OFX_BOX2D_SCALE, vertexes[i-1].y/OFX_BOX2D_SCALE);
		v2.Set(vertexes[i].x/OFX_BOX2D_SCALE, vertexes[i].y/OFX_BOX2D_SCALE);
		shape.SetAsEdge(v1, v2);
		
		fixture.density = 1.0;
		fixture.restitution = 0.4;
		fixture.friction	= 0.3;
		fixture.shape = &shape;
		
		body->CreateFixture(&fixture);
		
	}	
	
	// update the area and centroid
	updateShape();
	
}

//----------------------------------------
void ofxBox2dPolygon::draw() {
	
	if(body == NULL) {
		return;	
	}
	
	const b2Transform& xf = body->GetTransform();
	
	ofVec2f centroid = getCenter();
	ofCircle(centroid.x, centroid.y, 2);
	
	for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
		b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();
		
		if(poly) {
			ofBeginShape();
			for(int i=0; i<poly->GetVertexCount(); i++) {
				b2Vec2 pt = b2Mul(xf, poly->GetVertex(i));
				ofVertex(pt.x*OFX_BOX2D_SCALE, pt.y*OFX_BOX2D_SCALE);
			}
			ofEndShape(true);
			
			
		}
	}
	
}












