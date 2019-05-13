/*
 *  ofxBox2dJoint.cpp
 *  jointExample
 *
 *  Created by Nick Hardeman on 1/19/11.
 *  Copyright 2011 Nick Hardeman. All rights reserved.
 *
 */

#include "ofxBox2dJoint.h"
#include "ofxBox2d.h"

//----------------------------------------
ofxBox2dJoint::ofxBox2dJoint() {
	world = NULL;
	joint = NULL;
	alive = false;
}

//----------------------------------------
ofxBox2dJoint::ofxBox2dJoint(b2World* b2world, b2Body* body1, b2Body* body2, float frequencyHz, float damping, bool bCollideConnected) {
	ofxBox2dJoint();
	setup(b2world, body1, body2, frequencyHz, damping, bCollideConnected);
}

//----------------------------------------
ofxBox2dJoint::ofxBox2dJoint(b2World* b2world, b2Body* body1, b2Body* body2, b2Vec2 anchor1, b2Vec2 anchor2, float frequencyHz, float damping, bool bCollideConnected) {
	ofxBox2dJoint();
	setup(b2world, body1, body2, anchor1, anchor2, frequencyHz, damping, bCollideConnected);
}

//----------------------------------------
ofxBox2dJoint::ofxBox2dJoint(b2World* b2world, b2DistanceJointDef jointDef) {
    ofxBox2dJoint();
    setup(b2world, jointDef);
}

//----------------------------------------
void ofxBox2dJoint::setup(b2World* b2world, b2Body* body1, b2Body* body2, float frequencyHz, float damping, bool bCollideConnected) {
	
	if(body1 == NULL || body2 == NULL) {
		ofLog(OF_LOG_NOTICE, "ofxBox2dJoint :: setup : - box2d body is NULL -");
		return;
	}
	
	b2Vec2 a1, a2;
	a1 = body1->GetWorldCenter();
	a2 = body2->GetWorldCenter();
	
	setup(b2world, body1, body2, a1, a2, frequencyHz, damping, bCollideConnected);
    
    alive = true;
}

//----------------------------------------
void ofxBox2dJoint::setup(b2World* b2world, b2Body* body1, b2Body* body2, b2Vec2 anchor1, b2Vec2 anchor2, float frequencyHz, float damping, bool bCollideConnected) {

	if(body1 == NULL || body2 == NULL) {
		ofLog(OF_LOG_NOTICE, "ofxBox2dJoint :: setup : - box2d body is NULL -");
		return;
	}

	b2DistanceJointDef jointDef;
	jointDef.Initialize(body1, body2, anchor1, anchor2);
	jointDef.collideConnected	= bCollideConnected;
	jointDef.frequencyHz		= frequencyHz;
	jointDef.dampingRatio		= damping;	
	
    setup(b2world, jointDef);
}

//----------------------------------------
void ofxBox2dJoint::setup(b2World* b2world, b2DistanceJointDef jointDef) {

    setWorld(b2world);
    
    joint = (b2DistanceJoint*)world->CreateJoint(&jointDef);
	
	alive = true;
}

//----------------------------------------
void ofxBox2dJoint::setupMouseJoint(b2World* b2world, b2Body* bodyMouse, b2Body* bodyObj, float frequencyHz, float damping) {
    jointType = e_mouseJoint;
    b2MouseJointDef jointDef;
    
    jointDef.bodyA = bodyMouse;
    jointDef.bodyB = bodyObj;
    jointDef.target = bodyObj->GetWorldCenter();
    jointDef.maxForce = bodyObj->GetMass()*1000;
    jointDef.frequencyHz = frequencyHz;
    jointDef.dampingRatio = damping;
    
    
    setWorld(b2world);
    
    joint = world->CreateJoint(&jointDef);
    
    alive = true;
}

void ofxBox2dJoint::updateTarget() {
    if (joint && jointType == e_mouseJoint) {
        ((b2MouseJoint *)joint)->SetTarget(joint->GetBodyA()->GetWorldCenter());
    }
}


//----------------------------------------
void ofxBox2dJoint::setWorld(b2World* w) {
	if(w == NULL) {
		ofLog(OF_LOG_NOTICE, "ofxBox2dJoint :: setWorld : - box2d world needed -");	
		return;
	}
	world = w;
}

//----------------------------------------
bool ofxBox2dJoint::isSetup() {
	if (world == NULL) {
		ofLog(OF_LOG_NOTICE, "ofxBox2dJoint :: world must be set!");
		return false;
	}
	if (joint == NULL) {
		ofLog(OF_LOG_NOTICE, "ofxBox2dJoint :: setup function must be called!");
		return false;
	}
	return true;
}


//----------------------------------------
void ofxBox2dJoint::draw() {
	if(!alive) return;
	ofVec2f p1 = ofxBox2d::toOf(joint->GetAnchorA());
	ofVec2f p2 = ofxBox2d::toOf(joint->GetAnchorB());
	ofDrawLine(p1, p2);
}

//----------------------------------------
void ofxBox2dJoint::destroy() {
	if (!isSetup()) return;
	if(joint) {
		world->DestroyJoint(joint);
	}
	joint = NULL;
	alive = false;
}



//----------------------------------------
void ofxBox2dJoint::setLength(float len) {
    if(joint && jointType == e_distanceJoint) {
        ((b2DistanceJoint *)joint)->SetLength((float32)ofxBox2d::toB2d(len));
    }
}
float ofxBox2dJoint::getLength() {
    if(joint && jointType == e_distanceJoint) {
        return (float)((b2DistanceJoint *)joint)->GetLength();
    }
    return 0;
}

//----------------------------------------
void ofxBox2dJoint::setFrequency(float freq) {
    if(joint && jointType == e_distanceJoint) {
        ((b2DistanceJoint *)joint)->SetFrequency((float32)freq);
    }
}
float ofxBox2dJoint::getFrequency() {
    if(joint && jointType == e_distanceJoint) {
        return (float)((b2DistanceJoint *)joint)->GetFrequency();
    }
    return 0;
}

//----------------------------------------
void ofxBox2dJoint::setDamping(float ratio) {
    if(joint) {
        ((b2DistanceJoint *)joint)->SetDampingRatio((float32)ratio);
    }
}
float ofxBox2dJoint::getDamping() {
    if(joint && jointType == e_distanceJoint) {
        return (float)((b2DistanceJoint *)joint)->GetDampingRatio();
    }
    return 0;
}


//----------------------------------------
ofVec2f ofxBox2dJoint::getReactionForce(float inv_dt) const {
	b2Vec2 vec = getReactionForceB2D(inv_dt);
	return ofVec2f(vec.x, vec.y);
}
b2Vec2 ofxBox2dJoint::getReactionForceB2D(float inv_dt) const {
	if(joint) {
		return joint->GetReactionForce(inv_dt);
	}
	return b2Vec2(0, 0);
}
float ofxBox2dJoint::getReactionTorque(float inv_dt) const {
	if(joint) {
		return (float)joint->GetReactionTorque(inv_dt);
	}
	return 0;
}




