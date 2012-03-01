
#pragma once
#include "ofMain.h"
#include "Box2D.h"
#include "ofxBox2dUtils.h"

class ofxBox2dBaseShape {
	
public:
	b2FixtureDef	fixture;
	b2BodyDef		bodyDef;
	b2Body*			body;
	
	bool			dead;
	bool			alive;
	bool			setMassFromShape;
	
	float			density;
	float			bounce;
	float			friction;
	ofxBox2dBaseShape();	
	
	//----------------------------------------
	~ofxBox2dBaseShape();
	
	//----------------------------------------
	bool isBody();
	
	//----------------------------------------
    static bool shouldRemove(ofxBox2dBaseShape &b);
	bool isFixed();
	bool isSleeping();
    
	//----------------------------------------
	b2World* getWorld();
	
	//----------------------------------------
	virtual void create();
	
	//------------------------------------------------ 
	virtual void setBounce(float val);
	
	//------------------------------------------------ 
	virtual void setDensity(float val);
	
	//----------------------------------------
	virtual void setFriction(float val);
	
	//------------------------------------------------ 
	virtual void setPhysics(float density, float bounce, float friction);
	

	//------------------------------------------------ 
	void* setData(void*data);
	
	//------------------------------------------------ 
	void* getData();
	
	//------------------------------------------------ 
	virtual void setFilterData(b2Filter filter);
	
	//------------------------------------------------ 
	virtual void enableGravity(bool b);
	
	//------------------------------------------------ 
	virtual void setFixedRotation(bool b);
	virtual void setRotationFriction(float f);
	float getRotation();
	
	//------------------------------------------------ 
	virtual void setPosition(float x, float y);
	virtual void setPosition(ofVec2f p);
	
	//------------------------------------------------ 
	ofVec2f getPosition();
	ofVec2f getB2DPosition();
	
	
	//------------------------------------------------ 
	virtual void setVelocity(float x, float y);
	virtual void setVelocity(ofVec2f p);
	ofVec2f getVelocity();
	
	//------------------------------------------------ 
	virtual void setDamping(float f);
	virtual void setDamping(float fx, float fy);
	
	
	
	//------------------------------------------------
	virtual void addForce(ofVec2f frc, float scale);
	
	//------------------------------------------------
	virtual void addImpulseForce(ofVec2f pt, ofVec2f amt);
	
	//------------------------------------------------
	virtual void addRepulsionForce(ofVec2f pt, float radius, float amt);
	
	//------------------------------------------------
	virtual void destroy();
	
	//------------------------------------------------
	virtual void update();
	virtual void draw();
	
};






