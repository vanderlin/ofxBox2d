#pragma once

#include "ofMain.h"
#include "Box2D.h"
#include "ofxBox2dUtils.h"

#define BOX2D_DEFAULT_FREQ      4.0
#define BOX2D_DEFAULT_DAMPING   0.5

class ofxBox2dRevoluteJoint {

public:

	b2World         * world;
	b2RevoluteJoint * joint;
	bool              alive;

	//----------------------------------------
	ofxBox2dRevoluteJoint();
	ofxBox2dRevoluteJoint(b2World* b2world, b2Body* body1, b2Body* body2, bool enableLimit = false, float lowerLimit = -0.01f, float upperLimit = 0.01f, bool bCollideConnected=true);
	ofxBox2dRevoluteJoint(b2World* b2world, b2Body* body1, b2Body* body2, b2Vec2 anchor0, bool enableLimit = false, float lowerLimit = -0.01f, float upperLimit = 0.01f, bool bCollideConnected=true);
	ofxBox2dRevoluteJoint(b2World* b2world, b2Body* body1, b2Body* body2, b2Vec2 anchor1, b2Vec2 anchor2, bool enableLimit = false, float lowerLimit = -0.01f, float upperLimit = 0.01f, bool bCollideConnected=true);
	ofxBox2dRevoluteJoint(b2World* b2world, b2RevoluteJointDef jointDef);

	//----------------------------------------
	void setWorld(b2World * w);
	void setup(b2World* b2world, b2Body* body1, b2Body* body2, bool enableLimit = false, float lowerLimit = -0.01f, float upperLimit = 0.01f, bool bCollideConnected=true);
	void setup(b2World* b2world, b2Body* body1, b2Body* body2, b2Vec2 anchor0, bool enableLimit = false, float lowerLimit = -0.01f, float upperLimit = 0.01f, bool bCollideConnected=true);
	void setup(b2World* b2world, b2Body* body1, b2Body* body2, b2Vec2 anchor1, b2Vec2 anchor2, bool enableLimit = false, float lowerLimit = -0.01f, float upperLimit = 0.01f, bool bCollideConnected=true);
	void setup(b2World* b2world, b2RevoluteJointDef jointDef);

	//----------------------------------------
	bool isSetup();
	void draw();
	void destroy();

	//----------------------------------------
	//read-only
	float getJointAngle(); //in radians

	//----------------------------------------
	//read-write
	void setLimitEnabled(bool toggle);
	bool getLimitEnabled();
	void setLowerLimit(float radangle);
	float getLowerLimit();
	void setUpperLimit(float radangle);
	float getUpperLimit();
	void setLimits(float lowerAngle, float upperAngle);

	void setMotorEnabled(bool toggle);
	bool getMotorEnabled();
	void setMotorSpeed(float speed);
	float getMotorSpeed();
	void setMaxMotorTorque(float torque);
	float getMaxMotorTorque();

};
