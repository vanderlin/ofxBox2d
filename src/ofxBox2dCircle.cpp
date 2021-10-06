/*
 *  ofxBox2dCircle.cpp
 *  ofxBox2dExample
 *
 *  Created by Todd Vanderlin on 3/7/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#include "ofxBox2dCircle.h"
#include "ofxBox2d.h"

//------------------------------------------------
ofxBox2dCircle::ofxBox2dCircle() {
}

//------------------------------------------------
void ofxBox2dCircle::setup(b2World * b2dworld, float x, float y, float radius, bool is_sensor) {
	
	
	if(b2dworld == NULL) {
		ofLog(OF_LOG_NOTICE, "ofxBox2dCircle :: setup : - must have a valid world -");
		return;
	}
	
	float scale = ofxBox2d::getScale();
	
	// these are used to create the shape
	b2CircleShape shape;
	
	shape.m_p.SetZero();
	shape.m_radius		= radius / scale;
	this->radius		= radius;
	
	fixture.shape		= &shape;
	fixture.density		= density;
	fixture.friction	= friction;
	fixture.restitution	= bounce;
    fixture.isSensor = is_sensor;
	
	if(density == 0.f)	bodyDef.type	= b2_staticBody;
	else				bodyDef.type	= b2_dynamicBody;
	
	bodyDef.position.Set(x/scale, y/scale);
	
	body  = b2dworld->CreateBody(&bodyDef);
	body->CreateFixture(&fixture);
    
    alive = true;
}

//------------------------------------------------
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
	b2Vec2 P = toB2d(pt);
	
	float cx  =  toB2d(body->GetPosition().x);
	float cy  =  toB2d(body->GetPosition().y);
	float r   =  toB2d(getRadius());
	float ori =  DEG_TO_RAD * getRotation();
	
	b2Vec2 A(cx,cy); 
	b2Vec2 B(cx+r*cos(ori), cy+r*sin(ori));
	
	b2Vec2 qtA = b2Mul(xf, A);
	b2Vec2 qtB = b2Mul(xf, B);
	b2Vec2 DA = P - qtA; 
	b2Vec2 DB = P - qtB;
	b2Vec2 FA = amt * DA;
	b2Vec2 FB = amt * DB;
	
	body->ApplyForce(-FA, P, true);
	body->ApplyForce(-FB, P, true);
}

//------------------------------------------------
void ofxBox2dCircle::addAttractionPoint(float x, float y, float amt) {
	addAttractionPoint(ofVec2f(x, y), amt);
}

//------------------------------------------------
void ofxBox2dCircle::addAttractionPoint(ofVec2f pt, float amt) {
	const b2Transform& xf = body->GetTransform();
	b2Vec2 P = toB2d(pt);
	
	float cx  = toB2d(body->GetPosition().x);
	float cy  = toB2d(body->GetPosition().y);
	float r   = toB2d(getRadius());
	float ori = DEG_TO_RAD * getRotation();
	
	b2Vec2 A(cx,cy); 
	b2Vec2 B(cx+r*cos(ori), cy+r*sin(ori));
	
	b2Vec2 qtA = b2Mul(xf, A);
	b2Vec2 qtB = b2Mul(xf, B);
	b2Vec2 DA = P - qtA; 
	b2Vec2 DB = P - qtB;
	b2Vec2 FA = amt * DA;
	b2Vec2 FB = amt * DB;
	
	body->ApplyForce(FA, P, true);
	body->ApplyForce(FB, P, true);
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
	
	for (b2Fixture* f= body->GetFixtureList(); f; f = f->GetNext()) {
		f->GetShape()->m_radius = toB2d(r);
	}
}

//------------------------------------------------
void ofxBox2dCircle::draw() {
	
	if(!isBody()) return;
	
	ofPushMatrix();
	ofTranslate(getPosition().x, getPosition().y, 0);
	ofRotateDeg(getRotation(), 0, 0, 1);
	ofDrawCircle(0, 0, radius);
	
    ofPushStyle();
    ofEnableAlphaBlending();
    ofSetColor(0);
	ofDrawLine(0, 0, radius, 0);
    if(isSleeping()) {
        ofSetColor(255, 100);
        ofDrawCircle(0, 0, radius);
    }
    ofPopStyle();
    
	ofPopMatrix();
	
}



ofRectangle ofxBox2dCircle::getRect() {
    ofRectangle r;
    r.setFromCenter(getPosition(), radius, radius);
    return r;
}










