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
#include "ofxBox2d.h"

//----------------------------------------
ofxBox2dPolygon::ofxBox2dPolygon() { 
	bIsTriangulated = false;
	bIsSimplified   = false;
    ofPolyline::setClosed(true);

}

//----------------------------------------
ofxBox2dPolygon::~ofxBox2dPolygon() { 
}

//----------------------------------------
void ofxBox2dPolygon::clear() {
	ofxBox2dBaseShape::destroy();
    ofPolyline::clear();
    mesh.clear();
}

//----------------------------------------
void ofxBox2dPolygon::destroy() {
	ofxBox2dBaseShape::destroy();
	clear();
}

//----------------------------------------
void ofxBox2dPolygon::addTriangle(const glm::vec2 &a, const glm::vec2 &b, const glm::vec2 &c) {
	addVertex(glm::vec3(a, 0)); addVertex(glm::vec3(b, 0));	addVertex(glm::vec3(c, 0));
	addVertex(glm::vec3(a, 0));
	close();
}

//----------------------------------------
void ofxBox2dPolygon::simplify(float tolerance) {
	ofPolyline::simplify(tolerance);	
	bIsSimplified = true;
}

//----------------------------------------
void ofxBox2dPolygon::simplifyToMaxVerts() {
    ofPolyline p = getResampledByCount(b2_maxPolygonVertices);
    if(p.size()) {
        ofPolyline::clear();
        ofPolyline::addVertices(p.getVertices());
    }
}

//----------------------------------------
void ofxBox2dPolygon::triangulate(float angleConstraint, float sizeConstraint) {
    
	triangles.clear();
	
	if(size() > 0) {
        triangles = ofxBox2dPolygonUtils::triangulate(*this, angleConstraint, sizeConstraint);
	}

	bIsTriangulated = true;
}

//----------------------------------------
void ofxBox2dPolygon::makeConvexPoly() {
	ofPolyline convex = ofxBox2dPolygonUtils::getConvexHull(ofPolyline::getVertices());
    ofPolyline::clear();
    ofPolyline::addVertices(convex.getVertices());
    bIsTriangulated = false;
}

//----------------------------------------
void ofxBox2dPolygon::create(b2World * b2dworld) {

	if(size() < 3) {
		ofLog(OF_LOG_NOTICE, "need at least 3 points: %i\n", (int)size());
		return;	
	}
	
	if (body != NULL) {
		b2dworld->DestroyBody(body);
		body = NULL;
	}

	float scale = ofxBox2d::getScale();
    auto center = getCentroid2D();
    
	b2BodyDef		bd;
	bd.type			= density <= 0.0 ? b2_staticBody : b2_dynamicBody;
	body			= b2dworld->CreateBody(&bd);

	if(bIsTriangulated) {
	
		b2PolygonShape	shape;
		b2FixtureDef	fixture;
		b2Vec2			verts[3];
        
        // move all the triangles center offset
        for (auto &tri : triangles) {
            for(int i=0; i<3; i++) {
                tri[i].x -= center.x;
                tri[i].y -= center.y;
            }
        }
        
        
        for (auto &tri : triangles) {
           
            verts[0] = toB2d(tri.a);
            verts[1] = toB2d(tri.b);
            verts[2] = toB2d(tri.c);
			
			shape.Set(verts, 3);
			
			fixture.density		= density;
			fixture.restitution = bounce;
			fixture.friction	= friction;
			fixture.shape		= &shape;
        
			body->CreateFixture(&fixture);
		}
        
        // move the body to the center
        body->SetTransform(toB2d(center), 0);

        // build the mesh
        auto & pts = ofPolyline::getVertices();
        mesh.clear();
        ofPath path;
        path.setMode(ofPath::POLYLINES);
        for (auto &pnt : pts) {
            pnt -= center;
            path.lineTo(pnt.x, pnt.y);
        }
        mesh = path.getTessellation();
        mesh.setUsage(GL_STATIC_DRAW);
        
	}
	else {
		
        makeConvexPoly();
		
		auto & pts = ofPolyline::getVertices();
        vector<b2Vec2>verts;
	  
        // move all the points to 0, 0
        for(auto &pnt : pts) {
            pnt -= center;
        }
        
		// double down safety
		for (int i=0; i<MIN((int)pts.size(), b2_maxPolygonVertices); i++) {
            verts.push_back(toB2d(pts[i]));
        }
		
		b2PolygonShape shape;
        shape.Set(&verts[0], verts.size()-1);
        
        fixture.shape		= &shape;
        fixture.density		= density;
        fixture.restitution = bounce;
        fixture.friction	= friction;
        
        body->CreateFixture(&fixture);
        
        // move the body to the center
        body->SetTransform(toB2d(center), 0);
        
        // build the mesh
        mesh.clear();
        ofPath path;
        path.setMode(ofPath::POLYLINES);
        for (auto &pnt : pts) {
            path.lineTo(pnt.x, pnt.y);
        }
        mesh = path.getTessellation();
        mesh.setUsage(GL_STATIC_DRAW);
    }
    
    flagHasChanged();
    alive = true;
}

//------------------------------------------------
void ofxBox2dPolygon::addAttractionPoint (ofVec2f pt, float amt) {
    // we apply forces at each vertex. 
    if(body != NULL) {
        const b2Transform& xf = body->GetTransform();
		
        for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
            b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();
            
            if(poly) {
                b2Vec2 P = toB2d(pt);
                
                for(int i=0; i<poly->GetVertexCount(); i++) {
                    b2Vec2 qt = b2Mul(xf, poly->GetVertex(i));
                    b2Vec2 D = P - qt; 
                    b2Vec2 F = amt * D;
                    body->ApplyForce(F, P, true);
                }                    
            }
        }
    }
}


//----------------------------------------
void ofxBox2dPolygon::addAttractionPoint (float x, float y, float amt) {
    addAttractionPoint(ofVec2f(x, y), amt);
}

//----------------------------------------
void ofxBox2dPolygon::addRepulsionForce(float x, float y, float amt) {
	addRepulsionForce(ofVec2f(x, y), amt);
}

//----------------------------------------
void ofxBox2dPolygon::addRepulsionForce(ofVec2f pt, float amt) {
	// we apply forces at each vertex. 
    if(body != NULL) {
        const b2Transform& xf = body->GetTransform();
		
        for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
            b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();
            
            if(poly) {
                b2Vec2 P = toB2d(pt);
                
                for(int i=0; i<poly->GetVertexCount(); i++) {
                    b2Vec2 qt = b2Mul(xf, poly->GetVertex(i));
                    b2Vec2 D = P - qt; 
                    b2Vec2 F = amt * D;
                    body->ApplyForce(-F, P, true);
                }                    
            }
        }
    }
}

//----------------------------------------
vector <ofDefaultVertexType>& ofxBox2dPolygon::getPoints() {
    if(body != NULL) {
	
		const b2Transform& xf = body->GetTransform();
	
		for (b2Fixture * f = body->GetFixtureList(); f; f = f->GetNext()) {
			b2PolygonShape * poly = (b2PolygonShape*)f->GetShape();
		
			if(poly) {
				ofPolyline::clear();
				for(int i=0; i<poly->GetVertexCount(); i++) {
					b2Vec2 pt = b2Mul(xf, poly->GetVertex(i));
                    ofPolyline::addVertex(glm::vec3(pt.x, pt.y, 0));
				}
				if(isClosed()) ofPolyline::close();
			}
		}
	}

    return ofPolyline::getVertices();
}


//------------------------------------------------
void ofxBox2dPolygon::draw() {
	if(body == NULL) {
        ofLog(OF_LOG_ERROR, "ofxBox2dPolygon::draw body null\n");
        return;
	}
    ofPushMatrix();
    ofTranslate(getPosition());
    ofRotateDeg(getRotation(), 0, 0, 1);
    if (ofGetCurrentRenderer()->getFillMode() == OF_FILLED) {
		mesh.draw();
	} else {
		mesh.drawWireframe();
	}
    ofPopMatrix();
}

//------------------------------------------------
void ofxBox2dPolygon::drawTriangles() {
	if(body == NULL) {
		ofLog(OF_LOG_ERROR, "ofxBox2dPolygon::draw body null\n");
		return;
	}
	ofPushMatrix();
	ofTranslate(getPosition());
	ofRotateDeg(getRotation(), 0, 0, 1);
	for (int i=0; i<triangles.size(); i++) {
		triangles[i].draw();
	}
	ofPopMatrix();
}













