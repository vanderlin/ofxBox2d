/*
 *  ofxBox2dBaseShape.cpp
 *  ofxBox2d
 *
 *  Created by Nick Hardeman on 1/14/11.
 *  Copyright 2011 Arnold Worldwide. All rights reserved.
 *
 */

#include "ofxBox2dBaseShape.h"
#include "ofxBox2d.h"

//----------------------------------------
ofxBox2dBaseShape::ofxBox2dBaseShape() {
	
	setMassFromShape = true;
	alive = false;
	body  = NULL;
	
	density     = 0.0;
	bounce		= 0.0;
	friction	= 0.0;
	bodyDef.allowSleep = true;
}

//----------------------------------------
ofxBox2dBaseShape::~ofxBox2dBaseShape() {
	ofLog(OF_LOG_VERBOSE, "~ofxBox2dBaseShape(%p)\n", body);
	if (isBody()) {
		destroy();
	}
}

//------------------------------------------------
void ofxBox2dBaseShape::destroy() {
	
	if(getWorld() == NULL) {
		ofLog(OF_LOG_NOTICE, "ofxBox2dBaseShape:: - must have a valid world -");
		return;
	}
	else if(body == NULL) {
		ofLog(OF_LOG_NOTICE, "ofxBox2dBaseShape:: - null body -");
		return;
	}
    
	getWorld()->DestroyBody(body);
	body  = NULL;
	alive = false;
}

//----------------------------------------
bool ofxBox2dBaseShape::shouldRemove(shared_ptr<ofxBox2dBaseShape> shape) {
    return !shape.get()->alive;
}
bool ofxBox2dBaseShape::shouldRemoveOffScreen(shared_ptr<ofxBox2dBaseShape> shape) {
    return !ofRectangle(0, 0, ofGetWidth(), ofGetHeight()).inside(shape.get()->getPosition());
}

//----------------------------------------
bool ofxBox2dBaseShape::isBody() {
	if (body == NULL) {
		return false;
	}
	return true;
}

bool ofxBox2dBaseShape::isFixed() {
	return density == 0.f ? true : false;
}

bool ofxBox2dBaseShape::isSleeping() {
    if(isBody()) {
        return !body->IsAwake();
    }
    else { 
        ofLog(OF_LOG_ERROR, "ofxBox2dBaseShape:: - body is not defined -");
        return false;
    }
}
//
b2World* ofxBox2dBaseShape::getWorld() {
	if (isBody()) {
		return body->GetWorld();
	}
	return NULL;
}

//----------------------------------------
void ofxBox2dBaseShape::create() {}

//------------------------------------------------ 
void ofxBox2dBaseShape::setBounce(float val) {
	bounce = val;
    if (body) {
        for (auto * f = body->GetFixtureList(); f; f = f->GetNext()) {
            f->SetRestitution(bounce);
        }
    }
}

//------------------------------------------------ 
void ofxBox2dBaseShape::setDensity(float val) {
    density = val;
    if (body) {
        for (auto * f = body->GetFixtureList(); f; f = f->GetNext()) {
            f->SetDensity(density);
        }
    }
}

//----------------------------------------
void ofxBox2dBaseShape::setFriction(float val) {
	friction = val;
    if (body) {
        for (auto * f = body->GetFixtureList(); f; f = f->GetNext()) {
            f->SetFriction(friction);
        }
    }
}

//------------------------------------------------ 
void ofxBox2dBaseShape::setPhysics(float density, float bounce, float friction) {
    setFriction(friction);
    setDensity(density);
    setBounce(bounce);
}


//------------------------------------------------ 
void* ofxBox2dBaseShape::setData(void*data) {
	
	if(data == NULL) {
		ofLog(OF_LOG_NOTICE, "ofxBox2dBaseShape:: - data is NULL -");
		return NULL;
	}
	
	if(isBody()) {
		//ofLog(OF_LOG_NOTICE, "ofxBox2dBaseShape:: - custom data set %p", data);
		body->SetUserData(data);
		return data;
	}
	else {
		ofLog(OF_LOG_NOTICE, "ofxBox2dBaseShape:: - must have a valid body -");
	}
    return NULL;
}

//------------------------------------------------ 
void* ofxBox2dBaseShape::getData() {
	if(body) {
		return body->GetUserData();
	}
	else {
		ofLog(OF_LOG_NOTICE, "ofxBox2dBaseShape:: - must have a valid body -");
		return NULL;
	}
}

//------------------------------------------------
void ofxBox2dBaseShape::setFilterData(b2Filter filter) {
    for( b2Fixture * f = body->GetFixtureList(); f; f = f->GetNext() ){
        f->SetFilterData(filter);
    }
}

//------------------------------------------------ 
void ofxBox2dBaseShape::enableGravity(bool b) {
    if (body) {
        body->SetGravityScale(b ? 1 : 0);
    }
    else {
        bodyDef.gravityScale = b ? 1 : 0;
    }
}

//------------------------------------------------ 
void ofxBox2dBaseShape::setFixedRotation(bool b) {
	if(body) {
        body->SetFixedRotation(b);
    }
    else {
        bodyDef.fixedRotation = b;
    }
}

//------------------------------------------------
float ofxBox2dBaseShape::getRotation() {
	if(body != NULL) {
		return ofRadToDeg(body->GetAngle());
	}
    else return 0;
}

void ofxBox2dBaseShape::setRotation(float angle){
    body->SetTransform(body->GetWorldCenter(), DEG_TO_RAD * angle);
}


//------------------------------------------------ 
void ofxBox2dBaseShape::setPosition(float x, float y) {
	if(!body || body == NULL) {
		ofLog(OF_LOG_NOTICE, "ofxBox2dBaseShape:: - Body is NULL -");
		return;
	}
	body->SetTransform(b2Vec2(toB2d(x), toB2d(y)), body->GetAngle());
	body->SetAwake(true);
}

void ofxBox2dBaseShape::setPosition(ofVec2f p) {
	setPosition(p.x, p.y);
}

//------------------------------------------------
ofVec2f ofxBox2dBaseShape::toOf(const b2Vec2 v){
	float scale = ofxBox2d::getScale();
	return ofVec2f(v.x * scale, v.y * scale);
}
float ofxBox2dBaseShape::toOf(const float f) {
	float scale = ofxBox2d::getScale();
	return f * scale;
}

b2Vec2 ofxBox2dBaseShape::toB2d(const ofVec2f pt) {
	float scale = ofxBox2d::getScale();
	return b2Vec2(pt.x / scale, pt.y / scale);
}
float ofxBox2dBaseShape::toB2d(const float f) {
	float scale = ofxBox2d::getScale();
	return f / scale;
}


//------------------------------------------------ 
ofVec2f ofxBox2dBaseShape::getPosition() {
	ofVec2f p;
	if(body != NULL) {
        const b2Transform& xf = body->GetTransform();
        b2Vec2 pos      = body->GetLocalCenter();
        b2Vec2 b2Center = b2Mul(xf, pos);
		p = toOf(b2Center);
    }
	return p;
}

//------------------------------------------------
ofVec2f ofxBox2dBaseShape::getB2DPosition() {
	return getPosition() / ofxBox2d::getScale();
}

//------------------------------------------------ 
void ofxBox2dBaseShape::setVelocity(float x, float y) {
	if(body != NULL) {
		body->SetLinearVelocity(b2Vec2(x, y));
	}
    else {
        bodyDef.linearVelocity = b2Vec2(x, y);
    }
}
void ofxBox2dBaseShape::setVelocity(ofVec2f p) {
	setVelocity(p.x, p.y);
}
ofVec2f ofxBox2dBaseShape::getVelocity() {
	return ofVec2f(body->GetLinearVelocity().x, body->GetLinearVelocity().y);
}

//------------------------------------------------
void ofxBox2dBaseShape::setLinearDamping(float f) {
    if(body != NULL) {
        body->SetLinearDamping(f);
    }
    else {
        bodyDef.linearDamping = f;
    }
}

//------------------------------------------------
void ofxBox2dBaseShape::setAngularDamping(float f) {
    if(body != NULL) {
        body->SetAngularDamping(f);
    }
    else {
        bodyDef.angularDamping = f;
    }
}

//------------------------------------------------
void ofxBox2dBaseShape::addForce(ofVec2f frc, float scale) {
	if(body != NULL) {
		frc *= scale;
		body->ApplyForce(b2Vec2(frc.x, frc.y), body->GetPosition(), true);
	}
}

//------------------------------------------------
void ofxBox2dBaseShape::addImpulseForce(ofVec2f point, ofVec2f force) {
	if(body != NULL) {
		body->ApplyLinearImpulse(b2Vec2(force.x, force.y), toB2d(point), true);
	}
}

//------------------------------------------------
void ofxBox2dBaseShape::addRepulsionForce(ofVec2f pt, float radius, float amt) {
	if(body != NULL) {
		b2Vec2 P = toB2d(pt);
		b2Vec2 D = P - body->GetPosition(); 
		if(D.LengthSquared() < radius) {;
			P.Normalize();
			b2Vec2 F = amt * D;
			body->ApplyForce(-F, body->GetWorldCenter(), true);
		}
	}
}



//------------------------------------------------
void ofxBox2dBaseShape::update() { }
void ofxBox2dBaseShape::draw() { }
