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
    ofPolyline::setClosed(true);

}

//----------------------------------------
ofxBox2dPolygon::~ofxBox2dPolygon() { 
}

//----------------------------------------
void ofxBox2dPolygon::clear() {
	ofxBox2dBaseShape::destroy();
    ofxBox2dPolygon::clear();
    mesh.clear();
}

//----------------------------------------
void ofxBox2dPolygon::destroy() {
	ofxBox2dBaseShape::destroy();
	clear();
}

//----------------------------------------
void ofxBox2dPolygon::addTriangle(const ofVec2f &a, const ofVec2f &b, const ofVec2f &c) {
	addVertex(a); addVertex(b);	addVertex(c);
	
	// dont forget to close it
	addVertex(a);
	close();
}

/*
 These were in ofPolyline and now are gone?
*/
//----------------------------------------
void ofxBox2dPolygon::addVertexes(vector <ofVec2f> &pts) {
	for (int i=0; i<pts.size(); i++) {
		addVertex(pts[i].x, pts[i].y);
	}
}

//----------------------------------------
void ofxBox2dPolygon::addVertexes(ofPolyline &polyline) {
	for (int i=0; i<polyline.size(); i++) {
		addVertex(polyline[i].x, polyline[i].y);
	}
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
void ofxBox2dPolygon::triangulatePoly(float resampleAmt, int nPointsInside) {
	
	triangles.clear();
	bool wasClosed = isClosed();
	if(size() > 0) {
		
		// copy over the points into a polyline
		ofPolyline polyOutline;
		ofPolyline newPoly;
		
		// make sure to close the polyline and then
		// simplify and resample by spacing...
		setClosed(true);
		if(!bIsSimplified) simplify();
		newPoly = getResampledBySpacing(resampleAmt);
		
		// save the outline...
		polyOutline = newPoly;
		
		// add some random points inside then
		// triangulate the polyline...
		if(nPointsInside!=-1) addRandomPointsInside(newPoly, nPointsInside);
		triangles = triangulatePolygonWithOutline(newPoly, polyOutline);
			
		clear();
		if(wasClosed) ofPolyline::setClosed(wasClosed);
        
		// now add back into polyshape
		for (int i=0; i<newPoly.size(); i++) {
			addVertex(newPoly[i]);
		}
	}
	
	bIsTriangulated = true;
}

//----------------------------------------
void ofxBox2dPolygon::makeConvexPoly() {
    ofPolyline convex = getConvexHull(ofPolyline::getVertices());
    ofPolyline::clear();
    ofPolyline::addVertices(convex.getVertices());
    bIsTriangulated = false;
}

//----------------------------------------
void ofxBox2dPolygon::create(b2World * b2dworld) {

	if(size() <= 3) {
		ofLog(OF_LOG_NOTICE, "need at least 3 points: %i\n", (int)size());
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
        makeConvexPoly();
		vector<ofPoint> pts = ofPolyline::getVertices();
        vector<b2Vec2>verts;
        for (int i=0; i<MIN((int)pts.size(), b2_maxPolygonVertices); i++) {
            verts.push_back(screenPtToWorldPt(pts[i]));
        }
        b2PolygonShape shape;
        shape.Set(&verts[0], verts.size()-1);
        
        fixture.shape		= &shape;
        fixture.density		= density;
        fixture.restitution = bounce;
        fixture.friction	= friction;
        
        body->CreateFixture(&fixture);
    
        
    }
    
    vector<ofPoint> pts = ofPolyline::getVertices();
    mesh.clear();
    ofPath path;
    ofPoint center = getCentroid2D();
    for (int i=0; i<pts.size(); i++) {
        ofPoint p(pts[i].x, pts[i].y);
        p -= center;
        path.lineTo(p);
    }
    mesh = path.getTessellation();
    mesh.setUsage(GL_STATIC_DRAW);

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
                b2Vec2 P(pt.x/OFX_BOX2D_SCALE, pt.y/OFX_BOX2D_SCALE);
                
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
void ofxBox2dPolygon::addRepulsionForce(ofVec2f pt, float amt) {
	// we apply forces at each vertex. 
    if(body != NULL) {
        const b2Transform& xf = body->GetTransform();
		
        for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
            b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();
            
            if(poly) {
                b2Vec2 P(pt.x/OFX_BOX2D_SCALE, pt.y/OFX_BOX2D_SCALE);
                
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
vector <ofPoint>& ofxBox2dPolygon::getPoints() {
	
    if(body == NULL) {
		return ofPolyline::getVertices();
	}
	bool wasClosed = isClosed();
	const b2Transform& xf = body->GetTransform();
    ofPolyline::clear();
    ofPolyline::setClosed(wasClosed);
	for (b2Fixture * f = body->GetFixtureList(); f; f = f->GetNext()) {
		b2PolygonShape * poly = (b2PolygonShape*)f->GetShape();
		if(poly) {
            for(int i=0; i<poly->GetVertexCount(); i++) {
                ofPolyline::addVertex( worldPtToscreenPt(b2Mul(xf, poly->GetVertex(i))) );
			}
			if(isClosed()) ofPolyline::close();
		}
	}
    return ofPolyline::getVertices();
}


//----------------------------------------
void ofxBox2dPolygon::draw() {
	if(body == NULL) {
        ofLog(OF_LOG_ERROR, "ofxBox2dPolygon::draw body null\n");
        return;
	}
    ofPushMatrix();
    ofTranslate(getPosition());
    ofRotate(getRotation(), 0, 0, 1);
    mesh.draw(ofGetFill()==OF_OUTLINE?OF_MESH_WIREFRAME:OF_MESH_FILL);
    ofPopMatrix();
	
    /*
	const b2Transform& xf = body->GetTransform();
    for (b2Fixture * f = body->GetFixtureList(); f; f = f->GetNext()) {
		b2PolygonShape * poly = (b2PolygonShape*)f->GetShape();
		if(poly) {
            drawShape.clear();
            for(int i=0; i<poly->GetVertexCount(); i++) {
                drawShape.addVertex( worldPtToscreenPt(b2Mul(xf, poly->GetVertex(i))) );
			}
			if(isClosed()) drawShape.close();
            drawShape.draw();
		}
	}
	*/
}












