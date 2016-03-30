/*
 *  ofxBox2dRevoluteJoint.cpp
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
ofxBox2dRevoluteJoint::ofxBox2dRevoluteJoint(b2World* b2world, b2Body* body1, b2Body* body2, bool enableLimit, float lowerLimit, float upperLimit, bool bCollideConnected) {
	ofxBox2dRevoluteJoint();
	setup(b2world, body1, body2, enableLimit, lowerLimit, upperLimit, bCollideConnected);
}

//----------------------------------------
ofxBox2dRevoluteJoint::ofxBox2dRevoluteJoint(b2World* b2world, b2Body* body1, b2Body* body2, b2Vec2 anchor0, bool enableLimit, float lowerLimit, float upperLimit, bool bCollideConnected) {
	ofxBox2dRevoluteJoint();
	setup(b2world, body1, body2, anchor0, enableLimit, lowerLimit, upperLimit, bCollideConnected);
}

//----------------------------------------
ofxBox2dRevoluteJoint::ofxBox2dRevoluteJoint(b2World* b2world, b2Body* body1, b2Body* body2, b2Vec2 anchor1, b2Vec2 anchor2, bool enableLimit, float lowerLimit, float upperLimit, bool bCollideConnected) {
	ofxBox2dRevoluteJoint();
	setup(b2world, body1, body2, anchor1, anchor2, enableLimit, lowerLimit, upperLimit, bCollideConnected);
}

//----------------------------------------
ofxBox2dRevoluteJoint::ofxBox2dRevoluteJoint(b2World* b2world, b2RevoluteJointDef jointDef) {
	ofxBox2dRevoluteJoint();
	setup(b2world, jointDef);
}

//----------------------------------------
void ofxBox2dRevoluteJoint::setup(b2World* b2world, b2Body* body1, b2Body* body2, bool enableLimit, float lowerLimit, float upperLimit, bool bCollideConnected) {

	if(body1 == NULL || body2 == NULL) {
		ofLog(OF_LOG_NOTICE, "ofxBox2dRevoluteJoint :: setup : - box2d body is NULL -");
		return;
	}

	b2Vec2 a1, a2;
	a1 = body1->GetWorldCenter();
	a2 = body2->GetWorldCenter();

	setup(b2world, body1, body2, a1, a2, enableLimit, lowerLimit, upperLimit, bCollideConnected);

	alive = true;
}

//----------------------------------------
void ofxBox2dRevoluteJoint::setup(b2World* b2world, b2Body* body1, b2Body* body2, b2Vec2 anchor0, bool enableLimit, float lowerLimit, float upperLimit, bool bCollideConnected) {

	if(body1 == NULL || body2 == NULL) {
		ofLog(OF_LOG_NOTICE, "ofxBox2dRevoluteJoint :: setup : - box2d body is NULL -");
		return;
	}

	b2RevoluteJointDef jointDef;
	jointDef.Initialize(body1, body2, anchor0);
	jointDef.enableLimit = enableLimit;
	jointDef.lowerAngle = lowerLimit;
	jointDef.upperAngle = upperLimit;
	jointDef.collideConnected = bCollideConnected;

	setup(b2world, jointDef);
}


//----------------------------------------
void ofxBox2dRevoluteJoint::setup(b2World* b2world, b2Body* body1, b2Body* body2, b2Vec2 anchor1, b2Vec2 anchor2, bool enableLimit, float lowerLimit, float upperLimit, bool bCollideConnected) {

	if (body1 == NULL || body2 == NULL) {
		ofLog(OF_LOG_NOTICE, "ofxBox2dRevoluteJoint :: setup : - box2d body is NULL -");
		return;
	}

	b2Vec2 anchor0;
	anchor0 = body1->GetWorldCenter();

	b2RevoluteJointDef jointDef;
	jointDef.Initialize(body1, body2, anchor0);
	jointDef.localAnchorA = anchor1;
	jointDef.localAnchorB = anchor2;
	jointDef.enableLimit = enableLimit;
	jointDef.lowerAngle = lowerLimit;
	jointDef.upperAngle = upperLimit;
	jointDef.collideConnected = bCollideConnected;

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
	if (w == NULL) {
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
	ofDrawLine(p1.x + 0.0001f, p1.y, p2.x, p2.y - 0.0001f);
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
float ofxBox2dRevoluteJoint::getJointAngle() { //in radians
	if (joint) {
		return (float)joint->GetJointAngle();
	}
	return 0;
}

//----------------------------------------
void ofxBox2dRevoluteJoint::setLimitEnabled(bool toggle) {
	if (joint) {
		joint->EnableLimit(toggle);
	}
}
bool ofxBox2dRevoluteJoint::getLimitEnabled() {
	if (joint) {
		return (bool)joint->IsLimitEnabled();
	}
	return false;
}

//----------------------------------------
void ofxBox2dRevoluteJoint::setLowerLimit(float angle) {
	if (joint) {
		joint->SetLimits((float32)angle, joint->GetUpperLimit());
	}
}
float ofxBox2dRevoluteJoint::getLowerLimit() {
	if (joint) {
		return (float)joint->GetLowerLimit();
	}
	return 0;
}

//----------------------------------------
void ofxBox2dRevoluteJoint::setUpperLimit(float angle) {
	if (joint) {
		joint->SetLimits(joint->GetLowerLimit(), (float32)angle);
	}
}
float ofxBox2dRevoluteJoint::getUpperLimit() {
	if (joint) {
		return (float)joint->GetUpperLimit();
	}
	return 0;
}

//----------------------------------------
void ofxBox2dRevoluteJoint::setLimits(float lowerAngle, float upperAngle) {
	if (joint) {
		joint->SetLimits((float32)lowerAngle, (float32)upperAngle);
	}
}

//----------------------------------------
void ofxBox2dRevoluteJoint::setMotorEnabled(bool toggle) {
	if (joint) {
		joint->EnableMotor(toggle);
	}
}
bool ofxBox2dRevoluteJoint::getMotorEnabled() {
	if (joint) {
		return (bool)joint->IsMotorEnabled();
	}
	return false;
}

//----------------------------------------
void ofxBox2dRevoluteJoint::setMotorSpeed(float speed) {
	if (joint) {
		joint->SetMotorSpeed((float32)speed);
	}
}
float ofxBox2dRevoluteJoint::getMotorSpeed() {
	if (joint) {
		return (float)joint->GetMotorSpeed();
	}
	return 0;
}

//----------------------------------------
void ofxBox2dRevoluteJoint::setMaxMotorTorque(float torque) {
	if (joint) {
		joint->SetMaxMotorTorque((float32)torque);
	}
}
float ofxBox2dRevoluteJoint::getMaxMotorTorque() {
	if (joint) {
		return (float)joint->GetMaxMotorTorque();
	}
	return 0;
}
