/*
 *  ofxBox2dRect.cpp
 *  ofxBox2dExample
 *
 *  Created by Todd Vanderlin on 3/7/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#include "ofxBox2dRect.h"
#include "ofxBox2d.h"

//------------------------------------------------

ofxBox2dRect::ofxBox2dRect() {
}

//------------------------------------------------
void ofxBox2dRect::setup(b2World * b2dworld, ofRectangle rec, float angle) {

        if(b2dworld == NULL) {
            ofLog(OF_LOG_NOTICE, "- must have a valid world -");
            return;
        }
        
        width = rec.width/2;
        height = rec.height/2;
    
    
    
        auto p = rec.getCenter();
        b2BodyDef bodyDef;
        if(density == 0.f) bodyDef.type    = b2_staticBody;
        else               bodyDef.type    = b2_dynamicBody;
        auto x = toB2d(p.x);
        auto y = toB2d(p.y);

        bodyDef.position.Set(x, y);
        bodyDef.angle = ofDegToRad(angle);
        
        body = b2dworld->CreateBody(&bodyDef);
        
        setRectangle(rec);
        
////        b2PolygonShape shape;
//        shape = make_unique<b2PolygonShape>();
//        shape->SetAsBox(toB2d(width), toB2d(height));
//        fixture.shape        = shape.get();
//        fixture.density        = density;
//        fixture.friction    = friction;
//        fixture.restitution = bounce;
//
//        fixturePtr = body->CreateFixture(&fixture);
//        setPosition(p.x, p.y);
//
//        updateMesh();
        alive = true;
    
}

//------------------------------------------------
void ofxBox2dRect::setup(b2World * b2dworld, float x, float y, float w, float h, float angle) {
    ofRectangle r;
    r.setFromCenter(x, y, w, h);
    setup(b2dworld, r, angle);
}
void ofxBox2dRect::setRectangle(const ofRectangle& rect){
    if(body){
        if(body->GetFixtureList()){
            body->DestroyFixture(body->GetFixtureList());
        }
        width = rect.width/2;
        height = rect.height/2;
        
        
        if(shape == nullptr){
            shape = make_unique<b2PolygonShape>();
        }
        
        
        shape->SetAsBox(toB2d(width), toB2d(height));
        fixture.shape        = shape.get();
        fixture.density      = density;
        fixture.friction    = friction;
        fixture.restitution = bounce;
        
        fixturePtr = body->CreateFixture(&fixture);

        auto p = rect.getCenter();
        setPosition(p.x, p.y);
        
        updateMesh();
        
    }
}


// Temporary fix until OF 0.8.0
static void rectangle(ofPath & path, const ofRectangle & r){
	path.moveTo(r.getTopLeft());
	path.lineTo(r.getTopRight());
	path.lineTo(r.getBottomRight());
	path.lineTo(r.getBottomLeft());
	path.close();
}

//------------------------------------------------
void ofxBox2dRect::updateMesh() {
    float w = getWidth();
    float h = getHeight();
    ofPath path;
    rectangle(path, ofRectangle(-w/2, -h/2, w, h));
    mesh.clear();
    mesh = path.getTessellation();
    mesh.setUsage(GL_STATIC_DRAW);
}
/*
//------------------------------------------------
ofPolyline& ofxBox2dRect::getRectangleShape() {

    if(isBody()) {

        shape.clear();
        const b2Transform& xf = body->GetTransform();

        for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
            b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();
            if(poly) {
                for(int i=0; i<poly->m_count; i++) {
                    b2Vec2 pt = b2Mul(xf, poly->m_vertices[i]);
                    shape.addVertex(worldPtToscreenPt(pt));
                }
            }
        }
    }
    // we are a rectangle so close it
    shape.setClosed(true);
    return shape;

}*/

//------------------------------------------------
void ofxBox2dRect::addRepulsionForce(float fx, float fy, float amt) {
	addRepulsionForce(ofVec2f(fx,fy), amt);
}

//------------------------------------------------
void ofxBox2dRect::addRepulsionForce(ofVec2f pt, float amt) {
	if(body != NULL) {
		const b2Transform& xf = body->GetTransform();
		for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
			b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();

			if(poly) {
				b2Vec2 P = toB2d(pt);
				for (int i=0; i<poly->GetVertexCount(); i++) {
					b2Vec2 qt = b2Mul(xf, poly->GetVertex(i));
					b2Vec2 D = P - qt;
					b2Vec2 F = amt * D;
					body->ApplyForce(-F, P, true);
				}
			}
		}
	}
}


//------------------------------------------------
// In ofxBox2dRect.h:
// We compute the force for all four (transformed) corners of the rect.
// This keeps the rect's orientation correct!
void ofxBox2dRect::addAttractionPoint (float fx, float fy, float amt) {
	addAttractionPoint(ofVec2f(fx, fy), amt);
}

//------------------------------------------------
void ofxBox2dRect::addAttractionPoint (ofVec2f pt, float amt) {

	if(body != NULL) {
		const b2Transform& xf = body->GetTransform();
		for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
			b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();

			if(poly) {
				b2Vec2 P = toB2d(pt);
				for (int i=0; i<poly->GetVertexCount(); i++) {
					b2Vec2 qt = b2Mul(xf, poly->GetVertex(i));
					b2Vec2 D = P - qt;
					b2Vec2 F = amt * D;
					body->ApplyForce(F, P, true);
				}
			}
		}
	}
}

//------------------------------------------------
void ofxBox2dRect::draw() {

	if(body == NULL) {
		return;
	}
	
    ofPushMatrix();
    ofTranslate(ofxBox2dBaseShape::getPosition());
    ofRotateDeg(getRotation());
    mesh.draw();
    ofPopMatrix();

    /*
    const b2Transform& xf = body->GetTransform();
    ofPolyline shape;
    for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
        b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();
        if(poly) {
            for(int i=0; i<poly->m_count; i++) {
                b2Vec2 pt = b2Mul(xf, poly->m_vertices[i]);
                shape.addVertex(worldPtToscreenPt(pt));
            }
        }
    }
    shape.close();
    shape.draw();*/


    // update the polyline
    // getRectangleShape();
    /*
    ofPath path;
    for (int i=0; i<shape.size(); i++) {
        if(i==0)path.moveTo(shape[i]);
        else path.lineTo(shape[i]);
    }

    // draw the path
    path.setColor(ofGetStyle().color);
    path.setFilled(ofGetStyle().bFill);
    path.draw();

    // are we sleeping
    if(isSleeping()) {
        ofPushStyle();
        ofEnableAlphaBlending();
        path.setColor(ofColor(255, 100));
        path.setFilled(true);
        path.draw();
        ofPopStyle();
    }
	*/
}

//------------------------------------------------
ofRectangle ofxBox2dRect::getRect(){
    ofRectangle r;
    auto p = getPosition();
    r.setFromCenter(p.x, p.y, getWidth(), getHeight());
    return r;
}
















