#pragma once
#include "ofMain.h"
#include "Box2D.h"
#include "ofxBox2dUtils.h"

class ofxBox2dRevoluteJoint {
	
public:
	b2World			*	world;
	b2RevoluteJoint *	joint;
	bool				alive;
	
	//----------------------------------------
	ofxBox2dRevoluteJoint();
	ofxBox2dRevoluteJoint(b2World* b2world, b2Body* body1, b2Body* body2, bool enableLimit = false, float lowerAngle = -.25f * PI, float upperAngle = .25f * PI, bool bCollideConnected = false);
    ofxBox2dRevoluteJoint(b2World* b2world, b2Body* body1, b2Body* body2, b2Vec2 anchor, bool enableLimit = false, float lowerAngle = -.25f * PI, float upperAngle = .25f * PI, bool bCollideConnected = false);
    ofxBox2dRevoluteJoint(b2World* b2world, b2RevoluteJointDef jointDef);
	
	//----------------------------------------
	void setWorld(b2World * w);
	void setup(b2World* b2world, b2Body* body1, b2Body* body2, bool enableLimit = false, float lowerAngle = -.25f * PI, float upperAngle = .25f * PI, bool bCollideConnected = false);
	void setup(b2World* b2world, b2Body* body1, b2Body* body2, b2Vec2 anchor, bool enableLimit = false, float lowerAngle = -.25f * PI, float upperAngle = .25f * PI, bool bCollideConnected = false);
    void setup(b2World* b2world, b2RevoluteJointDef jointDef);
	
	//----------------------------------------
	bool isSetup();
	void draw();
	void destroy();
    
    void setLowerAngle(float lowerAngle);
    float getLowerAngle() const;
    
    void setUpperAngle(float upperAngle);
    float getUpperAngle() const;
    
    void setEnableLimit(bool enableLimit);
    bool getEnableLimit() const;
    
	//----------------------------------------
	ofVec2f getReactionForce(float inv_dt) const;
	b2Vec2  getReactionForceB2D(float inv_dt) const;
	float   getReactionTorque(float inv_dt) const;
};












