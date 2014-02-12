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
	float				fps;
	int					velocityIterations;
	int					positionIterations;
	
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
	
    ofPoint				gravity;

public:
	
	// b2AABB				worldAABB;
	b2World *			world;
	ofxBox2dRender		debugRender;
	
	float				scale;
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
	
	void		init();
	void		setFPS(float theFps) { fps = theFps; }
	
#ifdef TARGET_OPENGLES
	void		touchDown(ofTouchEventArgs &touch);
	void		touchMoved(ofTouchEventArgs &touch);
	void		touchUp(ofTouchEventArgs &touch);
#else
	void		mousePressed(ofMouseEventArgs &e);
	void		mouseDragged(ofMouseEventArgs &e);
	void		mouseReleased(ofMouseEventArgs &e);
#endif
	
	void		registerGrabbing();
	void		grabShapeDown(float x, float y, int id = -1 );		// -1 is reserved for mouse.
	void		grabShapeUp(float x, float y, int id = -1 );		// -1 is reserved for mouse.
	void		grabShapeDragged(float x, float y, int id = -1 );	// -1 is reserved for mouse.
	
	b2World*	getWorld()		  { return world;				   }
	int			getBodyCount();
	int			getJointCount();
	
	void		enableGrabbing()  { bEnableGrabbing = true;  };
	void		disableGrabbing() { bEnableGrabbing = false; };
	
	void		setContactListener(ofxBox2dContactListener * listener);
	
    void        wakeupShapes();
	void		setIterations(int velocityTimes, int positionTimes);
	void		setGravity(float x, float y);
	void		setGravity(ofPoint pt);
    ofPoint     getGravity();
    
	void		setBounds(ofPoint lowBounds, ofPoint upBounds);
	void		createBounds(float x=0, float y=0, float w=ofGetWidth(), float h=ofGetHeight());
	void		createBounds(ofRectangle rec);
	
	void		createGround(const ofPoint & p1, const ofPoint & p2);
	void		createGround(float x1=0, float y1=ofGetHeight(), float x2=ofGetWidth(), float y2=ofGetHeight());
	void		checkBounds(bool b);
	
	void		update(); 
	void		draw();
	void		drawGround();
        
};
