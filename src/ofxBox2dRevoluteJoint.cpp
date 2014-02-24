//
//  ofxBox2dRevoluteJoint.cpp
//  example-Joint
//
//  Created by Koki Nomura on 2014/01/27.
//

#include "ofxBox2dRevoluteJoint.h"

//----------------------------------------
ofxBox2dRevoluteJoint::ofxBox2dRevoluteJoint() {
    world = NULL;
    joint = NULL;
    alive = false;
}

//----------------------------------------
ofxBox2dRevoluteJoint::ofxBox2dRevoluteJoint(b2World* b2world, b2Body * body1, b2Body * body2, float stiffness) {
    ofxBox2dRevoluteJoint();
    setup(b2world, body1, body2, stiffness);
}

//----------------------------------------
ofxBox2dRevoluteJoint::ofxBox2dRevoluteJoint(b2World* b2world, b2Body * body1, b2Body * body2, b2Vec2 anchor1, b2Vec2 anchor2, float stiffness) {
    ofxBox2dRevoluteJoint();    
    setup(b2world, body1, body2, anchor1, anchor2, stiffness);    
}

//----------------------------------------
void ofxBox2dRevoluteJoint::setWorld(b2World * w) {
    if (w == NULL) {
        ofLog(OF_LOG_NOTICE, "ofxBox2dRevoluteJoint :: setWorld : - box2d world needed -");
        return;
    }
    world = w;
    alive = true;
}

//----------------------------------------
void ofxBox2dRevoluteJoint::setup(b2World* b2world, b2Body * body1, b2Body * body2, float stiffness) {
    setup(b2world, body1, body2, b2Vec2(0,0), b2Vec2(0,0), stiffness);
}

//----------------------------------------
void ofxBox2dRevoluteJoint::setup(b2World* b2world, b2Body * body1, b2Body * body2, b2Vec2 anchor1, b2Vec2 anchor2, float stiffness) {
    
    setWorld(b2world);
    
    if (body1 == NULL || body2 == NULL) {
		ofLog(OF_LOG_NOTICE, "ofxBox2dRevoluteJoint :: setup : - box2d body is NULL -");
		return;
	}
    
    jointDef.Initialize(body1, body2, anchor1);
    //need to override anchor set in Initialize() if you don't want the anchor to be in b2Body a's center
    //(due to reasons I can't figure out!)
    jointDef.localAnchorA.Set(anchor1.x / OFX_BOX2D_SCALE, anchor1.y / OFX_BOX2D_SCALE);
    jointDef.localAnchorB.Set(anchor2.x / OFX_BOX2D_SCALE, anchor2.y / OFX_BOX2D_SCALE);
    jointDef.enableLimit = true;
    jointDef.lowerAngle     = -HALF_PI;
    jointDef.upperAngle     = HALF_PI;
    jointDef.enableMotor    = true;
    jointDef.maxMotorTorque = stiffness;
    jointDef.motorSpeed     = 0.0;
    joint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
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
    if (!alive) return;
    
    ofVec2f p = getAnchor();
    ofCircle(p.x, p.y, 4);
}

//----------------------------------------
void ofxBox2dRevoluteJoint::destroy() {
    if (!isSetup()) return;
	if (joint) {
		world->DestroyJoint(joint);
	}
	joint = NULL;
	alive = false;
}

//----------------------------------------
ofVec2f ofxBox2dRevoluteJoint::getAnchor() {
    if (joint) {
        b2Vec2 anchor =joint->GetAnchorA();
        anchor *= OFX_BOX2D_SCALE;
        return ofVec2f(anchor.x, anchor.y);
    }
    return ofVec2f(0, 0);
}

float ofxBox2dRevoluteJoint::getReferenceAngle() {
    if (joint) {
        joint->GetReferenceAngle() * RAD_TO_DEG;
    }
    return 0;
}

//----------------------------------------
float ofxBox2dRevoluteJoint::getJointAngle() {
    if (joint) {
        return joint->GetJointAngle() * RAD_TO_DEG;
    }
    return 0;
}

//----------------------------------------
float ofxBox2dRevoluteJoint::getJointSpeed() {
    if (joint) {
        return joint->GetJointSpeed();
    }
    return 0;
}

//----------------------------------------
void ofxBox2dRevoluteJoint::enableLimit() {
    if (joint) {
        joint->EnableLimit(true);
    }
}
void ofxBox2dRevoluteJoint::disableLimit() {
    if (joint) {
        joint->EnableLimit(false);
    }
}
bool ofxBox2dRevoluteJoint::isLimitEnabled() {
    if (joint) {
        joint->IsLimitEnabled();
    }
}

//----------------------------------------
void ofxBox2dRevoluteJoint::setLimits(float lowerAngle, float upperAngle) {
    if (joint) {
        joint->SetLimits(lowerAngle * DEG_TO_RAD, upperAngle * DEG_TO_RAD);
    }
}
float ofxBox2dRevoluteJoint::getLowerLimit() {
    if (joint) {
        return joint->GetLowerLimit() * RAD_TO_DEG;
    }
    return 0;
}
float ofxBox2dRevoluteJoint::getUpperLimit() {
    if (joint) {
        return joint->GetUpperLimit() * RAD_TO_DEG;
    }
    return 0;
}

//----------------------------------------
void ofxBox2dRevoluteJoint::enableMotor() {
    if (joint) {
        joint->EnableMotor(true);
    }
}
void ofxBox2dRevoluteJoint::disableMotor() {
    if (joint) {
        joint->EnableMotor(false);
    }
}
bool ofxBox2dRevoluteJoint::isMotorEnabled() {
    if (joint) {
        joint->IsMotorEnabled();
    }
}

//----------------------------------------
void ofxBox2dRevoluteJoint::setMotorSpeed(float speed) {
    if (joint) {
        joint->SetMotorSpeed(speed);
    }
}
float ofxBox2dRevoluteJoint::getMotorSpeed() {
    if (joint) {
        return joint->GetMotorSpeed();
    }
    return 0;
}

//----------------------------------------
void ofxBox2dRevoluteJoint::setStiffness(float stiffness) {
    if (joint) {
        joint->SetMaxMotorTorque(stiffness);
    }
}
float ofxBox2dRevoluteJoint::getStiffness() {
    if (joint) {
        return joint->GetMaxMotorTorque();
    }
    return 0;
}

//----------------------------------------
ofVec2f ofxBox2dRevoluteJoint::getReactionForce(float inv_dt) const {
	b2Vec2 vec = getReactionForceB2D(inv_dt);
	return ofVec2f(vec.x, vec.y);
}

b2Vec2 ofxBox2dRevoluteJoint::getReactionForceB2D(float inv_dt) const {
	if (joint) {
		return joint->GetReactionForce(inv_dt);
	}
	return b2Vec2(0, 0);
}

float ofxBox2dRevoluteJoint::getReactionTorque(float inv_dt) const {
	if (joint) {
		return (float)joint->GetReactionTorque(inv_dt);
	}
	return 0;
}