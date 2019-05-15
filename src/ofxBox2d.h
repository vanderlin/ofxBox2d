#pragma once

#include "ofxBox2dUtils.h"
#include "ofxBox2dPolygonUtils.h"
#include "ofxBox2dBaseShape.h"

#include "ofxBox2dCircle.h"
#include "ofxBox2dPolygon.h"
#include "ofxBox2dEdge.h"
#include "ofxBox2dRect.h"

#include "ofxBox2dJoint.h"
#include "ofxBox2dRender.h"
#include "ofxBox2dContactListener.h"

#ifdef TARGET_OPENGLES
#define OF_MAX_TOUCH_JOINTS		5			// max number of touch points on iPhone + iPad (this may change in the future though).
#endif

class ofxBox2dContactArgs : public ofEventArgs {
public:
	
	b2Fixture * a;
	b2Fixture * b;
};

class ofxBox2d : public b2ContactListener {
	
private:
	
	bool                enableContactEvents;
	float				hz;
	int					velocityIterations;
	int					positionIterations;
	int					particleIterations;
	ofPoint				gravity;
	static float		scale;
	
	// Called when two fixtures begin to touch.
	void BeginContact(b2Contact* contact) { 
		static ofxBox2dContactArgs args;
		args.a = contact->GetFixtureA();
		args.b = contact->GetFixtureB();
		ofNotifyEvent( contactStartEvents, args, this);
	}
	
	// Called when two fixtures cease to touch.
	void EndContact(b2Contact* contact) { 
		static ofxBox2dContactArgs args;
		args.a = contact->GetFixtureA();
		args.b = contact->GetFixtureB();
		ofNotifyEvent( contactEndEvents, args, this);
	}
	
	
public:
	
	// b2AABB				worldAABB;
	b2World *			world;
	ofxBox2dRender		debugRender;

	bool				doSleep;
	bool				bEnableGrabbing;
	bool				bCheckBounds;
	bool				bHasContactListener;
	
	b2BodyDef			bd;
	b2Body*				m_bomb;
	
#ifdef TARGET_OPENGLES    
    b2MouseJoint*		touchJoints[ OF_MAX_TOUCH_JOINTS ];
    b2Body*		        touchBodies[ OF_MAX_TOUCH_JOINTS ];
#else
	b2MouseJoint*		mouseJoint;    
	b2Body*				mouseBody;
#endif    
	b2Body*				ground;
	b2Body*				mainBody;

	// ------------------------------------------------------
    void enableEvents();
    void disableEvents();
	ofEvent <ofxBox2dContactArgs> contactStartEvents;
	ofEvent <ofxBox2dContactArgs> contactEndEvents;
	
	// ------------------------------------------------------ 
	ofxBox2d();
	~ofxBox2d();
	
	// init box2d with hz (fps) gravity x/y
	void init(float _hz=60.0f, float _gx=0.0f, float _gy=10.0f);
	
	// clear all bodies, joints
	void clear();
	
	// set the hertz (fps) this will update the
	// iteration cycles for particle systems.
	void setFPS(float _hz);
	
	// world scale
	//------------------------------------------------
	static void setScale(float s);
	static float getScale();
	
	// helpers of -> box2d
	//------------------------------------------------
	static ofVec2f toOf(const b2Vec2 v);
	static ofVec2f toOf(float x, float y);
	static float toOf(float f);
	
	static b2Vec2 toB2d(ofPoint pt);
	static b2Vec2 toB2d(float x, float y);
	static float toB2d(float f);
	
	// helper to get particle iterations based on gravity radius and hz
	// this is from liquidfun docs.
	int calculateParticleIterations(float32 gravity, float32 radius, float32 timeStep);
	
	
	// touching and mouse events
#ifdef TARGET_OPENGLES
	void		touchDown(ofTouchEventArgs &touch);
	void		touchMoved(ofTouchEventArgs &touch);
	void		touchUp(ofTouchEventArgs &touch);
#else
	void		mousePressed(ofMouseEventArgs &e);
	void		mouseDragged(ofMouseEventArgs &e);
	void		mouseReleased(ofMouseEventArgs &e);
#endif
	
	// grabbing shapes
	void registerGrabbing();
	void grabShapeDown(float x, float y, int id = -1 );		// -1 is reserved for mouse.
	void grabShapeUp(float x, float y, int id = -1 );		// -1 is reserved for mouse.
	void grabShapeDragged(float x, float y, int id = -1 );	// -1 is reserved for mouse.
	
	// get the main box2d world
	b2World * getWorld() { return world; }
	
	// number of bodies in the world
	int getBodyCount();
	
	// number of joints in the world
	int getJointCount();
	
    // get timestep
    float getTimeStep();
    
	// grabbing of shapes with mouse
	void enableGrabbing()  { bEnableGrabbing = true;  };
	void disableGrabbing() { bEnableGrabbing = false; };
	
	// contact listening
	void setContactListener(ofxBox2dContactListener * listener);
	
	// wake up all shapes
    void wakeupShapes();
	
	// set iterations for vel, and pos
	void setIterations(int velocityTimes, int positionTimes);
	
	// gravity
	void setGravityX(float x);
	void setGravityY(float y);
	void setGravity(float x, float y);
	void setGravity(ofPoint pt);
    ofPoint getGravity();
	
	// create bounds for the world
	void setBounds(ofPoint lowBounds, ofPoint upBounds);
	void createBounds(float x=0, float y=0, float w=ofGetWidth(), float h=ofGetHeight());
	void createBounds(ofRectangle rec);
	
	// make just a floor (bottom)
	void createGround(const ofPoint & p1, const ofPoint & p2);
	void createGround(float x1=0, float y1=ofGetHeight(), float x2=ofGetWidth(), float y2=ofGetHeight());
	void checkBounds(bool b);
	
	// main box2d cycle
	void update();
	void draw();
	void drawGround();
        
};
