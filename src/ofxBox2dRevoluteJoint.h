#pragma once

#include "ofMain.h"
#include "Box2D.h"
#include "ofxBox2dUtils.h"

// based on Todd Vanderlin's ofxBox2dJoint class
// set enableLimits and enableMotor true by default for ease of use.
class ofxBox2dRevoluteJoint {
    
public:
    
	b2World         *   world;
	b2RevoluteJoint *   joint;
	b2RevoluteJointDef  jointDef;
	bool                alive;
	
    //----------------------------------------
    ofxBox2dRevoluteJoint();
    ofxBox2dRevoluteJoint(b2World* b2world, b2Body * body1, b2Body * body2, float stiffness=5.0f);
    ofxBox2dRevoluteJoint(b2World* b2world, b2Body * body1, b2Body * body2, b2Vec2 anchor1, b2Vec2 anchor2, float stiffness=5.0f);

    //----------------------------------------
	void setWorld(b2World * w);
    void setup(b2World* b2world, b2Body * a, b2Body * b, float stiffness=5.0f);
    void setup(b2World* b2world, b2Body * a, b2Body * b, b2Vec2 anchorA, b2Vec2 anchorB, float stiffness=5.0f);

    //----------------------------------------
    bool isSetup();
    void draw();
    void destroy();    
    
    //----------------------------------------
    ofVec2f getAnchor();
    float getReferenceAngle();
    float getJointAngle();
    float getJointSpeed();
    
    //----------------------------------------    
    void enableLimit();
    void disableLimit();
    bool isLimitEnabled();
    
    void setLimits(float lowerAngle, float upperAngle);
    float getLowerLimit();
    float getUpperLimit();
    
    void enableMotor();
    void disableMotor();
    bool isMotorEnabled();
    
    void setMotorSpeed(float speed);
    float getMotorSpeed();
    
    void setStiffness(float stiffness);
    float getStiffness();
    
	//----------------------------------------
	ofVec2f getReactionForce(float inv_dt) const;
	b2Vec2  getReactionForceB2D(float inv_dt) const;
	float   getReactionTorque(float inv_dt) const;
};