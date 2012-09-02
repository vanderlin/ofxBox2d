/*
 *  ofxBox2dCircle.cpp
 *  ofxBox2dExample
 *
 *  Created by Todd Vanderlin on 3/7/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#include "ofxBox2dCircle.h"

//------------------------------------------------
ofxBox2dCircle::ofxBox2dCircle() {
}

//------------------------------------------------
void ofxBox2dCircle::setup(b2World * b2dworld, float x, float y, float radius) {
	
	
	if(b2dworld == NULL) {
		ofLog(OF_LOG_NOTICE, "ofxBox2dCircle :: setup : - must have a valid world -");
		return;
	}
	
	// these are used to create the shape
	b2CircleShape shape;
	
	shape.m_p.SetZero();
	shape.m_radius		= radius / OFX_BOX2D_SCALE;
	this->radius		= radius;
	
	fixture.shape		= &shape;
	fixture.density		= density;
	fixture.friction	= friction;
	fixture.restitution	= bounce;
	
	if(density == 0.f)	bodyDef.type	= b2_staticBody;
	else				bodyDef.type	= b2_dynamicBody;
	
	bodyDef.position.Set(x/OFX_BOX2D_SCALE, y/OFX_BOX2D_SCALE);
	
	body  = b2dworld->CreateBody(&bodyDef);
	body->CreateFixture(&fixture);
}
void ofxBox2dCircle::setup(b2World * b2dworld, ofVec2f &pts, float radius) {
    setup(b2dworld, pts.x, pts.y, radius);
}

//------------------------------------------------
void ofxBox2dCircle::addRepulsionForce(float x, float y, float amt) {
	addRepulsionForce(ofVec2f(x, y), amt);
}

//------------------------------------------------
void ofxBox2dCircle::addRepulsionForce(ofVec2f pt, float amt) {
	const b2Transform& xf = body->GetTransform();
	b2Vec2 P(pt.x/OFX_BOX2D_SCALE, pt.y/OFX_BOX2D_SCALE);
	
	float cx  =  body->GetPosition().x / OFX_BOX2D_SCALE;
	float cy  =  body->GetPosition().y / OFX_BOX2D_SCALE;
	float r   =  getRadius() / OFX_BOX2D_SCALE;
	float ori =  DEG_TO_RAD * getRotation();
	
	b2Vec2 A(cx,cy); 
	b2Vec2 B(cx+r*cos(ori), cy+r*sin(ori));
	
	b2Vec2 qtA = b2Mul(xf, A);
	b2Vec2 qtB = b2Mul(xf, B);
	b2Vec2 DA = P - qtA; 
	b2Vec2 DB = P - qtB;
	b2Vec2 FA = amt * DA;
	b2Vec2 FB = amt * DB;
	
	body->ApplyForce(-FA, P);
	body->ApplyForce(-FB, P);
}

//------------------------------------------------
void ofxBox2dCircle::addAttractionPoint(float x, float y, float amt) {
	addAttractionPoint(ofVec2f(x, y), amt);
}

//------------------------------------------------
void ofxBox2dCircle::addAttractionPoint(ofVec2f pt, float amt) {
	const b2Transform& xf = body->GetTransform();
	b2Vec2 P(pt.x/OFX_BOX2D_SCALE, pt.y/OFX_BOX2D_SCALE);
	
	float cx  =  body->GetPosition().x / OFX_BOX2D_SCALE;
	float cy  =  body->GetPosition().y / OFX_BOX2D_SCALE;
	float r   =  getRadius() / OFX_BOX2D_SCALE;
	float ori =  DEG_TO_RAD * getRotation();
	
	b2Vec2 A(cx,cy); 
	b2Vec2 B(cx+r*cos(ori), cy+r*sin(ori));
	
	b2Vec2 qtA = b2Mul(xf, A);
	b2Vec2 qtB = b2Mul(xf, B);
	b2Vec2 DA = P - qtA; 
	b2Vec2 DB = P - qtB;
	b2Vec2 FA = amt * DA;
	b2Vec2 FB = amt * DB;
	
	body->ApplyForce(FA, P);
	body->ApplyForce(FB, P);
}

//------------------------------------------------
float ofxBox2dCircle::getRadius() {
	return radius;
}

//------------------------------------------------ 
/*
 TODO: Should we even do this....?
 Im not sure about this it seems like a bad idea.
 I cant figure out another way to change the radius of
 a shape that we have
 
 -- any help here :) --
 
 here is a solution for changing the radius on the fly without
 destroying the shape - chrispie
 
 */
void ofxBox2dCircle::setRadius(float r) {
	this->radius = r;
	
	for (b2Fixture* f= body->GetFixtureList(); f; f = f->GetNext())
	{
		f->GetShape()->m_radius=r/OFX_BOX2D_SCALE;
	}
}

//------------------------------------------------
void ofxBox2dCircle::draw() {
	
	if(!isBody()) return;
	
	ofPushMatrix();
	ofTranslate(getPosition().x, getPosition().y, 0);
	ofRotate(getRotation(), 0, 0, 1);
	ofCircle(0, 0, radius);
	
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(0);
	ofLine(0, 0, radius, 0);
    if(isSleeping()) {
        ofSetColor(255, 100);
        ofCircle(0, 0, radius);
    }
    ofPopStyle();
    
	ofPopMatrix();
	
}














