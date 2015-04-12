#pragma once
#include "ofMain.h"
#include "Box2D.h"
#include "ofxBox2dUtils.h"

#define BOX2D_DEFAULT_FREQ      4.0
#define BOX2D_DEFAULT_DAMPING   0.5

class ofxBox2dJoint {
	
public:
	
	b2World			*	world;
	b2DistanceJoint *	joint;
	int					jointType;
	bool				alive;
	
	//----------------------------------------
	ofxBox2dJoint();
	ofxBox2dJoint(b2World* b2world, b2Body* body1, b2Body* body2, float frequencyHz=4.f, float damping=.5f, bool bCollideConnected=true);
	ofxBox2dJoint(b2World* b2world, b2Body* body1, b2Body* body2, b2Vec2 anchor1, b2Vec2 anchor2, float frequencyHz=4.f, float damping=.5f, bool bCollideConnected=true);
	
	//----------------------------------------
	void setWorld(b2World * w);
	void setup(b2World* b2world, b2Body* body1, b2Body* body2, float frequencyHz=4.f, float damping=.5f, bool bCollideConnected=true);
	void setup(b2World* b2world, b2Body* body1, b2Body* body2, b2Vec2 anchor1, b2Vec2 anchor2, float frequencyHz=4.f, float damping=.5f, bool bCollideConnected=true);
	
	//----------------------------------------
	bool isSetup();
	void draw();
	void destroy();
	
	//----------------------------------------
	// Manipulating the length can lead to non-physical behavior when the frequency is zero.
	void  setLength(float len);
	float getLength();
	
	void  setFrequency(float freq);
	float getFrequency();
	
	void  setDamping(float ratio);
	float getDamping();
	
	//----------------------------------------
	ofVec2f getReactionForce(float inv_dt) const;
	b2Vec2  getReactionForceB2D(float inv_dt) const;
	float   getReactionTorque(float inv_dt) const;
};












