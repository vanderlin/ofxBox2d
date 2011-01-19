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
}

//----------------------------------------
ofxBox2dPolygon::~ofxBox2dPolygon() { 
}

//----------------------------------------
void ofxBox2dPolygon::setup(b2World * b2dworld) {
	
	
	
}

//----------------------------------------
void ofxBox2dPolygon::calculateArea() {
	int i, j, n = vertices.size();
	float polyArea = 0;
	for (i = 0; i < n; i++) {
		j = (i + 1) % n;
		polyArea += vertices[i].x * vertices[j].y;
		polyArea -= vertices[j].x * vertices[i].y;
	}
	polyArea /= 2.0;
	area = polyArea;
}

//----------------------------------------
void ofxBox2dPolygon::calculateCentroid() {
	float cx = 0, cy = 0;
	int i, j, n = vertices.size();
	
	float factor = 0;
	for (i = 0; i < n; i++) {
		j = (i + 1) % n;
		factor = (vertices[i].x * vertices[j].y
				  - vertices[j].x * vertices[i].y);
		cx += (vertices[i].x + vertices[j].x) * factor;
		cy += (vertices[i].y + vertices[j].y) * factor;
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
	vertices.push_back(p);
}

//----------------------------------------
void ofxBox2dPolygon::addVertex(float x, float y) {
	addVertex(ofVec2f(x, y));
}

//----------------------------------------
void ofxBox2dPolygon::addVertexes(const vector <ofVec2f> & polyPoints) {
	if(polyPoints.size() <= 0) return;
	for(int i=0; i<polyPoints.size(); i++) {
		addVertex(polyPoints[i]);	
	}
}

//----------------------------------------
void ofxBox2dPolygon::simplify(float tolerance) {
	
	// simplify the countour DP
	vertices = simplifyContour(vertices, tolerance);
	
}

//----------------------------------------
void ofxBox2dPolygon::triangulate() {
	vector <ofVec2f> orgPts = vertices;
	vertices = triangulatePolygon(vertices);
	bIsTriangulated = true;
}


//----------------------------------------
void ofxBox2dPolygon::create(b2World * b2dworld) {

	if(vertices.size() < 3) {
		printf("need at least 3 points\n");
		return;	
	}
	
	
	// create the body from the world (1)
	b2BodyDef		bd;
	bd.type			= b2_dynamicBody;
	body			= b2dworld->CreateBody(&bd);
	
	if(bIsTriangulated) {
		
		b2PolygonShape	shape;
		b2FixtureDef	fixture;
		b2Vec2			verts[3];
		
		for (int i=0; i<vertices.size(); i+=3) {
			
			verts[0].Set(vertices[i].x/OFX_BOX2D_SCALE, vertices[i].y/OFX_BOX2D_SCALE);
			verts[1].Set(vertices[i+1].x/OFX_BOX2D_SCALE, vertices[i+1].y/OFX_BOX2D_SCALE);
			verts[2].Set(vertices[i+2].x/OFX_BOX2D_SCALE, vertices[i+2].y/OFX_BOX2D_SCALE);
			
			shape.Set(verts, 3);
			
			fixture.density		= 1.0;
			fixture.restitution = 0.4;
			fixture.friction	= 0.3;
			fixture.shape		= &shape;
			
			body->CreateFixture(&fixture);
			
		}
	}
	/*
	// now triangluate from the polyline (3)
	vector <Delaunay::Point>	delaunayPts;
	Delaunay::Point				tempP;
	for(int i=0; i<vertices.size(); i++) {
		tempP[0] = vertices[i].x;
		tempP[1] = vertices[i].y;
		delaunayPts.push_back(tempP);
	}
	
	Delaunay delobject(delaunayPts);
	delobject.Triangulate();
	
	
	//ofPoint verts[3];
	int count = 0;
	
	// get all the triangles (4)
	for(Delaunay::fIterator fit=delobject.fbegin(); fit!=delobject.fend(); ++fit) {
		
		float triArea = delobject.area(fit);
		
		int pta   = delobject.Org(fit);
		int ptb   = delobject.Dest(fit);
		int ptc   = delobject.Apex(fit);
		ofVec2f a = vertices[pta];
		ofVec2f b = vertices[ptb];
		ofVec2f c = vertices[ptc];
		
		ofVec2f center;
		center += vertices[pta];
		center += vertices[ptb];
		center += vertices[ptc];
		center /= 3;
		
		float disA  = ofDist(center.x, center.y, a.x, a.y);
		float disB  = ofDist(center.x, center.y, b.x, b.y);
		float disC  = ofDist(center.x, center.y, c.x, c.y);
		
		if(triArea < 2.0 || 
		   disA < 2.0 ||
		   disB < 2.0 ||
		   disC < 2.0) {
			continue;
		}
		if(!insidePolygon(center, vertices)) {
			printf("pt outside polygon\n");
			continue;	
		}
		if(pta == -1 || ptb == -1 || ptc == -1) {
			continue;
		}
		
		b2PolygonShape	shape;
		b2FixtureDef	fixture;
		b2Vec2			verts[3];
		
		
		
		verts[0].Set(a.x/OFX_BOX2D_SCALE, a.y/OFX_BOX2D_SCALE);
		verts[1].Set(b.x/OFX_BOX2D_SCALE, b.y/OFX_BOX2D_SCALE);
		verts[2].Set(c.x/OFX_BOX2D_SCALE, c.y/OFX_BOX2D_SCALE);
		//int totalVerts = MIN(b2_maxPolygonVertices, (int)vertices.size()) ;
		//b2Vec2 verts[totalVerts];
		//for(int i=0; i<totalVerts; i++) {
		//	verts[i].Set(vertices[i].x/OFX_BOX2D_SCALE, vertices[i].y/OFX_BOX2D_SCALE);	
	//	}
		
		shape.Set(verts, 3);
		
		fixture.density		= 1.0;
		fixture.restitution = 0.4;
		fixture.friction	= 0.3;
		fixture.shape		= &shape;
		
		body->CreateFixture(&fixture);
		
		
	}	
	
	*/
	
	// update the area and centroid
	updateShape();
	
	
	// create the body
	/*b2BodyDef		bd;
	bd.type			= b2_dynamicBody;
	body			= b2dworld->CreateBody(&bd);
	
	
	b2PolygonShape	shape;
	b2FixtureDef	fixture;*/
	
	/*
	// add the verts and create the shape
	int totalVerts = (int)vertices.size();
	b2Vec2 v1, v2;	
	shape.m_vertexCount = totalVerts;
	for(int i=0; i<shape.GetVertexCount(); i++) {
		shape.m_vertices[i].Set(vertices[i].x/OFX_BOX2D_SCALE, vertices[i].y/OFX_BOX2D_SCALE);
		
		v1.Set(vertices[i-1].x/OFX_BOX2D_SCALE, vertices[i-1].y/OFX_BOX2D_SCALE);
		v2.Set(vertices[i].x/OFX_BOX2D_SCALE, vertices[i].y/OFX_BOX2D_SCALE);
		shape.SetAsEdge(v1, v2);
		
		
	}	
	 */
/*
	int totalVerts = MIN(b2_maxPolygonVertices, (int)vertices.size()) ;
	b2Vec2 verts[totalVerts];
	for(int i=0; i<totalVerts; i++) {
		verts[i].Set(vertices[i].x/OFX_BOX2D_SCALE, vertices[i].y/OFX_BOX2D_SCALE);	
	}
	
	shape.Set(verts, totalVerts);

	fixture.density		= 1.0;
	fixture.restitution = 0.4;
	fixture.friction	= 0.3;
	fixture.shape		= &shape;
	
	body->CreateFixture(&fixture);*/
	
	
}

//----------------------------------------
void ofxBox2dPolygon::draw() {
	
	ofNoFill();
	ofRect(bounds);
	
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
			
			for(int i=0; i<poly->GetVertexCount(); i++) {
				b2Vec2 pt = b2Mul(xf, poly->GetVertex(i));
				ofCircle(pt.x*OFX_BOX2D_SCALE, pt.y*OFX_BOX2D_SCALE, 2);
			}				
		}
	}
	
	
}












