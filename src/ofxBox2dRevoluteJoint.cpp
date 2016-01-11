/*
 *  ofxBox2dRevoluteJoint.cpp
 *  jointExample
 *
 *  Created by Neil Mendoza on 1/10/16.
 *  Copyright 2016 Neil Mendoza. All rights reserved.
 *
 */

#include "ofxBox2dRevoluteJoint.h"

//----------------------------------------
ofxBox2dRevoluteJoint::ofxBox2dRevoluteJoint() {
	world = NULL;
	joint = NULL;
	alive = false;
}

//----------------------------------------
ofxBox2dRevoluteJoint::ofxBox2dRevoluteJoint(b2World* b2world, b2Body* body1, b2Body* body2, bool enableLimit, float lowerAngle, float upperAngle, bool bCollideConnected) {
	ofxBox2dRevoluteJoint();
	setup(b2world, body1, body2, enableLimit, lowerAngle, upperAngle, bCollideConnected);
}

//----------------------------------------
ofxBox2dRevoluteJoint::ofxBox2dRevoluteJoint(b2World* b2world, b2Body* body1, b2Body* body2, b2Vec2 anchor, bool enableLimit, float lowerAngle, float upperAngle, bool bCollideConnected) {
	ofxBox2dRevoluteJoint();
	setup(b2world, body1, body2, anchor, enableLimit, lowerAngle, upperAngle, bCollideConnected);
}

//----------------------------------------
ofxBox2dRevoluteJoint::ofxBox2dRevoluteJoint(b2World* b2world, b2RevoluteJointDef jointDef) {
    ofxBox2dRevoluteJoint();
    setup(b2world, jointDef);
}

//----------------------------------------
void ofxBox2dRevoluteJoint::setup(b2World* b2world, b2Body* body1, b2Body* body2, bool enableLimit, float lowerAngle, float upperAngle, bool bCollideConnected) {
	
	if(body1 == NULL || body2 == NULL) {
		ofLog(OF_LOG_NOTICE, "ofxBox2dRevoluteJoint :: setup : - box2d body is NULL -");
		return;
	}
	
    b2Vec2 a;
	a = body1->GetWorldCenter();
	
	setup(b2world, body1, body2, a, enableLimit, lowerAngle, upperAngle, bCollideConnected);
    
    alive = true;
}

//----------------------------------------
void ofxBox2dRevoluteJoint::setup(b2World* b2world, b2Body* body1, b2Body* body2, b2Vec2 anchor, bool enableLimit, float lowerAngle, float upperAngle, bool bCollideConnected) {

	if(body1 == NULL || body2 == NULL) {
		ofLog(OF_LOG_NOTICE, "ofxBox2dRevoluteJoint :: setup : - box2d body is NULL -");
		return;
	}

	b2RevoluteJointDef jointDef;
	jointDef.Initialize(body1, body2, anchor);
	jointDef.collideConnected	= bCollideConnected;
	jointDef.lowerAngle	= lowerAngle;
	jointDef.upperAngle = upperAngle;
    jointDef.enableLimit = enableLimit;
	
    setup(b2world, jointDef);
}

//----------------------------------------
void ofxBox2dRevoluteJoint::setup(b2World* b2world, b2RevoluteJointDef jointDef) {

    setWorld(b2world);
    
    joint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
	
	alive = true;
}

//----------------------------------------
void ofxBox2dRevoluteJoint::setWorld(b2World* w) {
	if(w == NULL) {
		ofLog(OF_LOG_NOTICE, "ofxBox2dRevoluteJoint :: setWorld : - box2d world needed -");	
		return;
	}
	world = w;
}

//----------------------------------------
bool ofxBox2dRevoluteJoint::isSetup() {
	if (world == NULL) {
		ofLog(OF_LOG_NOTICE, "ofxBox2dRevoluteJoint :: world must be set!");
		return false;
	}
	if (joint == NULL) {
		ofLog(OF_LOG_NOTICE, "ofxBox2dRevoluteJoint :: setup function must be called!");
		return false;
	}
	return true;
}


//----------------------------------------
void ofxBox2dRevoluteJoint::draw() {
	if(!alive) return;
	
	b2Vec2 p1 = joint->GetAnchorA();
	b2Vec2 p2 = joint->GetAnchorB();
	
	p1 *= OFX_BOX2D_SCALE;
	p2 *= OFX_BOX2D_SCALE;
	ofDrawLine(p1.x, p1.y, p2.x, p2.y);
}

//----------------------------------------
void ofxBox2dRevoluteJoint::destroy() {
	if (!isSetup()) return;
	if(joint) {
		world->DestroyJoint(joint);
	}
	joint = NULL;
	alive = false;
}

void ofxBox2dRevoluteJoint::setLowerAngle(float lowerAngle)
{
    if (joint) joint->SetLimits(lowerAngle, joint->GetUpperLimit());
    else ofLogError() << "joint is null";
}

float ofxBox2dRevoluteJoint::getLowerAngle() const
{
    if (joint) return joint->GetLowerLimit();
    else
    {
        ofLogError() << "joint is null";
        return 0.f;
    }
}

void ofxBox2dRevoluteJoint::setUpperAngle(float upperAngle)
{
    if (joint) joint->SetLimits(joint->GetLowerLimit(), upperAngle);
    else ofLogError() << "joint is null";
}

float ofxBox2dRevoluteJoint::getUpperAngle() const
{
    if (joint) return joint->GetUpperLimit();
    else
    {
        ofLogError() << "joint is null";
        return 0.f;
    }
}

void ofxBox2dRevoluteJoint::setEnableLimit(bool enableLimit)
{
    if (joint) joint->EnableLimit(enableLimit);
    else ofLogError() << "joint is null";
}

bool ofxBox2dRevoluteJoint::getEnableLimit() const
{
    if (joint) return joint->IsLimitEnabled();
    else
    {
        ofLogError() << "joint is null";
        return false;
    }
}

//----------------------------------------
ofVec2f ofxBox2dRevoluteJoint::getReactionForce(float inv_dt) const {
	b2Vec2 vec = getReactionForceB2D(inv_dt);
	return ofVec2f(vec.x, vec.y);
}
b2Vec2 ofxBox2dRevoluteJoint::getReactionForceB2D(float inv_dt) const {
	if(joint) {
		return joint->GetReactionForce(inv_dt);
	}
	return b2Vec2(0, 0);
}
float ofxBox2dRevoluteJoint::getReactionTorque(float inv_dt) const {
	if(joint) {
		return (float)joint->GetReactionTorque(inv_dt);
	}
	return 0;
}




