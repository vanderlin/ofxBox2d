/*
 *  ofxBox2dPolygon.cpp
 *  ofxBox2d
 *
 *  Created by Todd Vanderlin on 1/14/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#include "ofxBox2dPolygon.h"
#include "ofxBox2dPolygonUtils.h"

//----------------------------------------
ofxBox2dPolygon::ofxBox2dPolygon() { 
	bIsTriangulated = false;
	bIsSimplified   = false;
}

//----------------------------------------
ofxBox2dPolygon::~ofxBox2dPolygon() { 
}

//----------------------------------------
void ofxBox2dPolygon::destroy() {
	ofxBox2dBaseShape::destroy();
	poly.clear();
}

//----------------------------------------
void ofxBox2dPolygon::setup(b2World * b2dworld) {
	
	
	
}

//----------------------------------------
void ofxBox2dPolygon::calculateArea() {
	int i, j, n = poly.size();
	float polyArea = 0;
	for (i = 0; i < n; i++) {
		j = (i + 1) % n;
		polyArea += poly[i].x * poly[j].y;
		polyArea -= poly[j].x * poly[i].y;
	}
	polyArea /= 2.0;
	area = polyArea;
}

//----------------------------------------
void ofxBox2dPolygon::calculateCentroid() {
	float cx = 0, cy = 0;
	int i, j, n = poly.size();
	
	float factor = 0;
	for (i = 0; i < n; i++) {
		j = (i + 1) % n;
		factor = (poly[i].x * poly[j].y
				  - poly[j].x * poly[i].y);
		cx += (poly[i].x + poly[j].x) * factor;
		cy += (poly[i].y + poly[j].y) * factor;
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
	bounds = poly.getBoundingBox();
}

//----------------------------------------
void ofxBox2dPolygon::addVertex(const ofVec2f & p) {
	poly.addVertex(p);
}

//----------------------------------------
void ofxBox2dPolygon::addVertex(float x, float y) {
	poly.addVertex(x, y);
}

//----------------------------------------
void ofxBox2dPolygon::addVertexes(const vector <ofVec2f> & polyPoints) {
	if(polyPoints.size() <= 0) return;
	for(int i=0; i<polyPoints.size(); i++) {
		poly.addVertex(polyPoints[i]);	
	}
}

//----------------------------------------
void ofxBox2dPolygon::addVertexes(const ofPolyline &polyline) {
	if(polyline.size() <= 0) return;
	for(int i=0; i<polyline.size(); i++) {
		poly.addVertex(polyline[i]);	
	}
}

//----------------------------------------
void ofxBox2dPolygon::addTriangle(const ofVec2f &a, const ofVec2f &b, const ofVec2f &c) {
	addVertex(a); addVertex(b);	addVertex(c);		
}

//----------------------------------------
void ofxBox2dPolygon::simplify(float tolerance) {
	poly.simplify(tolerance);	
	bIsSimplified = true;
}

//----------------------------------------
void ofxBox2dPolygon::triangulate(float resampleAmt, int nPointsInside) {
	
	triangles.clear();
	
	if(poly.size() > 0) {
		
		// copy over the points into a polyline
		ofPolyline polyOutline;
		
		// make sure to close the polyline and then
		// simplify and resample by spacing...
		poly.setClosed(true);
		if(!bIsSimplified)	poly.simplify();
		poly = poly.getResampledBySpacing(resampleAmt);
		
		// save the outline...
		polyOutline = poly;
		
		// add some random points inside then
		// triangulate the polyline...
		if(nPointsInside!=-1) addRandomPointsInside(poly, nPointsInside);
		triangles = triangulatePolygonWithOutline(poly, polyOutline);
		
	}
	
	bIsTriangulated = true;
}


//----------------------------------------
void ofxBox2dPolygon::create(b2World * b2dworld) {

	if(poly.size() < 3) {
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
	if(bIsTriangulated) {
		
		b2PolygonShape	shape;
		b2FixtureDef	fixture;
		b2Vec2			verts[3];
		
		ofVec2f a, b, c;
		for (int i=0; i<triangles.size(); i++) {
			
			a = triangles[i].a;
			b = triangles[i].b;
			c = triangles[i].c;

			verts[0].Set(a.x/OFX_BOX2D_SCALE, a.y/OFX_BOX2D_SCALE);
			verts[1].Set(b.x/OFX_BOX2D_SCALE, b.y/OFX_BOX2D_SCALE);
			verts[2].Set(c.x/OFX_BOX2D_SCALE, c.y/OFX_BOX2D_SCALE);
			
			shape.Set(verts, 3);
			
			fixture.density		= density;
			fixture.restitution = bounce;
			fixture.friction	= friction;
			fixture.shape		= &shape;
			
			body->CreateFixture(&fixture);
			
		}
	}
	else {
		
		int nPts = poly.size();
		b2Vec2			verts[nPts];
		
		for (int i=0; i<poly.size(); i++) {
			verts[i].Set(poly[i].x/OFX_BOX2D_SCALE, poly[i].y/OFX_BOX2D_SCALE);
		}
	
		for (int i=1; i<nPts; i++) {
			
			b2PolygonShape	shape;
			b2FixtureDef	fixture;
		
			b2Vec2 v1 = verts[i-1];
			b2Vec2 v2 = verts[i];
			shape.SetAsEdge(v1, v2);
		
			fixture.shape		= &shape;
			fixture.density		= density;
			fixture.restitution = bounce;
			fixture.friction	= friction;
			
			body->CreateFixture(&fixture);
			
		
		}
		
		/*
		Need to figure out mass data....
		*/
		
	
	}
	
	// update the area and centroid
	updateShape();
}

//----------------------------------------
void ofxBox2dPolygon::draw() {
	
	//ofNoFill();
	//ofRect(bounds);
	//ofVec2f centroid = getCenter();
	//ofCircle(centroid.x, centroid.y, 2);
	
	
	if(body == NULL) {
		return;	
	}
	
	const b2Transform& xf = body->GetTransform();
	
	for (b2Fixture * f = body->GetFixtureList(); f; f = f->GetNext()) {
		b2PolygonShape * poly = (b2PolygonShape*)f->GetShape();
		
		if(poly) {
			
			ofPolyline p;
			for(int i=0; i<poly->GetVertexCount(); i++) {
				b2Vec2 pt = b2Mul(xf, poly->GetVertex(i));
				p.addVertex(pt.x*OFX_BOX2D_SCALE, pt.y*OFX_BOX2D_SCALE);
			}
			p.draw();
			
			for(int i=0; i<poly->GetVertexCount(); i++) {
				b2Vec2 pt = b2Mul(xf, poly->GetVertex(i));
				ofCircle(pt.x*OFX_BOX2D_SCALE, pt.y*OFX_BOX2D_SCALE, 1);
			}				
		}
	}
	
	
}












