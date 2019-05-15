#include "ofxBox2d.h"

#pragma mark - static helpers
// ------------------------------------------------------
float ofxBox2d::scale = 30.0;
void ofxBox2d::setScale(float s) {
	ofxBox2d::scale = s;
}

float ofxBox2d::getScale() {
	return ofxBox2d::scale;
}

ofVec2f ofxBox2d::toOf(const b2Vec2 v) {
	return ofVec2f(v.x * ofxBox2d::scale, v.y * ofxBox2d::scale);
}

ofVec2f ofxBox2d::toOf(float x, float y) {
	return ofVec2f(x * ofxBox2d::scale, y * ofxBox2d::scale);
}

float ofxBox2d::toOf(float f) {
	return f * ofxBox2d::scale;
}

b2Vec2 ofxBox2d::toB2d(ofPoint pt) {
	return b2Vec2(pt.x / ofxBox2d::scale, pt.y / ofxBox2d::scale);
}

b2Vec2 ofxBox2d::toB2d(float x, float y) {
	return b2Vec2(x / ofxBox2d::scale, y / ofxBox2d::scale);
}
float ofxBox2d::toB2d(float f) {
	return f / ofxBox2d::scale;
}

// ------------------------------------------------------ 
ofxBox2d::ofxBox2d() {
    enableContactEvents = false;
	world = NULL;
	m_bomb = NULL;
	
#ifdef TARGET_OPENGLES
    // touch grabbing
	for(int i=0; i<OF_MAX_TOUCH_JOINTS; i++) {
		touchJoints[i] = NULL;
	}
	for(int i=0; i<OF_MAX_TOUCH_JOINTS; i++) {
		touchBodies[i] = NULL;
	}
#else
	
    // mouse grabbing
	mouseJoint = NULL;
	mouseBody = NULL;
#endif
	ground = NULL;
	mainBody = NULL;
}

// ------------------------------------------------------
void ofxBox2d::clear() {
	
#ifdef TARGET_OPENGLES
	// destroy touch grabbing bodies
	for(int i=0; i<OF_MAX_TOUCH_JOINTS; i++) {
		if(touchBodies[i]) {
			if(world) world->DestroyBody(touchBodies[i]);
		}
	}
#else
	// destroy mouse grabbing body
	if(mouseBody) {
		if(world) world->DestroyBody(mouseBody);
	}
#endif
	
	if(world) {
	
		// Fix from: https://github.com/vanderlin/ofxBox2d/issues/62
		b2Body* f = world->GetBodyList();
		while (f) {
			b2Body* next = f->GetNext();
			world->DestroyBody(f);
			f = next;
		}
		b2Joint* j = world->GetJointList();
		while (j) {
			b2Joint* next = j->GetNext();
			world->DestroyJoint(j);
			j = next;
		}
		// This is not safe... we are deleting the world on init so..
		/* delete world;
		world = NULL;*/
	}
}

// ------------------------------------------------------
ofxBox2d::~ofxBox2d() {
	clear();
}

// calculateParticleIterations
// ------------------------------------------------------
int ofxBox2d::calculateParticleIterations(float32 gravity, float32 radius, float32 timeStep) {
	// In some situations you may want more particle iterations than this,
	// but to avoid excessive cycle cost, don't recommend more than this.
	const int32 B2_MAX_RECOMMENDED_PARTICLE_ITERATIONS = 8;
	const float32 B2_RADIUS_THRESHOLD = 0.01f;
	int32 iterations =
	(int32) ceilf(b2Sqrt(gravity / (B2_RADIUS_THRESHOLD * radius)) * timeStep);
	return b2Clamp(iterations, 1, B2_MAX_RECOMMENDED_PARTICLE_ITERATIONS);
}


// init
// ------------------------------------------------------
void ofxBox2d::init(float _hz, float _gx, float _gy) {
	
	// settings
	bHasContactListener = false;
	bCheckBounds		= false;
	bEnableGrabbing		= true;
	ofxBox2d::scale		= 30.0f;
	doSleep				= true;
	
	// gravity
	gravity.set(_gx, _gy);
	
#ifdef TARGET_OPENGLES    
    // touch grabbing
    for( int i=0; i<OF_MAX_TOUCH_JOINTS; i++ )
		touchJoints[ i ] = nullptr;
    for( int i=0; i<OF_MAX_TOUCH_JOINTS; i++ )
		touchBodies[ i ] = nullptr;
#else
    // mouse grabbing
	mouseJoint = nullptr;
	mouseBody  = nullptr;
#endif
	
	// ground/bounds
	// debug drawer
	debugRender.setScale(scale);
	debugRender.SetFlags(1);
    
    if(ground) {
        world->DestroyBody(ground);
        ground = nullptr;
    }

	//worldAABB.lowerBound.Set(-100.0f, -100.0f);
	//worldAABB.upperBound.Set(100.0f, 100.0f);
	delete world;
    world = nullptr;
	world = new b2World(b2Vec2(gravity.x, gravity.y));
    world->SetAllowSleeping(doSleep);
	//world->SetDebugDraw(&debugRender);
	
	// set the hz and interaction cycles
	hz = _hz;
	velocityIterations = 8;
	positionIterations = 3;
	particleIterations = world->CalculateReasonableParticleIterations(hz);
	
	ofLog(OF_LOG_NOTICE, "ofxBox2d:: - world created -");
}

// ------------------------------------------------------ enable events
void ofxBox2d::enableEvents() {
    if(world != NULL) {
        world->SetContactListener(this);
    }
}

// ------------------------------------------------------ disable events
void ofxBox2d::disableEvents() {
    if(world != NULL) {
        world->SetContactListener(NULL);
    }
}

// ------------------------------------------------------ grab shapes
void ofxBox2d::setContactListener(ofxBox2dContactListener * listener) {
	
	if(world != NULL) {
		bHasContactListener = true;
		world->SetContactListener(listener);
	}
	else {
		ofLog(OF_LOG_WARNING, "ofxBox2d:: - world not inited -");
	}
}

// ------------------------------------------------------ grab shapes Events
void ofxBox2d::registerGrabbing() {
#ifdef TARGET_OPENGLES
	ofAddListener(ofEvents().touchDown, this, &ofxBox2d::touchDown);
	ofAddListener(ofEvents().touchMoved, this, &ofxBox2d::touchMoved);
	ofAddListener(ofEvents().touchUp, this, &ofxBox2d::touchUp);
#else
	ofAddListener(ofEvents().mousePressed, this, &ofxBox2d::mousePressed);
	ofAddListener(ofEvents().mouseDragged, this, &ofxBox2d::mouseDragged);
	ofAddListener(ofEvents().mouseReleased, this, &ofxBox2d::mouseReleased);
#endif
}

#ifdef TARGET_OPENGLES
void ofxBox2d::touchDown(ofTouchEventArgs &touch) {
	grabShapeDown(touch.x, touch.y, touch.id);
}
void ofxBox2d::touchMoved(ofTouchEventArgs &touch) {
	grabShapeDragged(touch.x, touch.y, touch.id);
}
void ofxBox2d::touchUp(ofTouchEventArgs &touch) {
	grabShapeUp(touch.x, touch.y, touch.id);
}
#else
void ofxBox2d::mousePressed(ofMouseEventArgs &e) {
	grabShapeDown(e.x, e.y);
}
void ofxBox2d::mouseDragged(ofMouseEventArgs &e) {
	grabShapeDragged(e.x, e.y);
}
void ofxBox2d::mouseReleased(ofMouseEventArgs &e) {
	grabShapeUp(e.x, e.y);
}
#endif

// ------------------------------------------------------ 
void ofxBox2d::grabShapeDown(float x, float y, int id) {
	
	if(world == NULL) {
		ofLog(OF_LOG_WARNING, "ofxBox2d:: - Need a world, call init first! -");
		return;
	}
	
	if(bEnableGrabbing) {
		b2Vec2 p(x/ofxBox2d::scale, y/ofxBox2d::scale);
        
#ifdef TARGET_OPENGLES  
        if(id >= 0 && id < OF_MAX_TOUCH_JOINTS)
        {
            if(touchJoints[id] != NULL)
               return;            
            
            if( touchBodies[id] == NULL) {
                b2BodyDef bd;
                touchBodies[id] = world->CreateBody(&bd);
            }
        }
        else        
            return;		// invalid mouse / touch id.
#else
      if (mouseJoint != NULL) {
          return;
      }
            
      if(mouseBody == NULL) {
         b2BodyDef bd;
         mouseBody = world->CreateBody(&bd);
      }
        
#endif

		// Make a small box.
		b2AABB aabb;
		b2Vec2 d;
		d.Set(0.001f, 0.001f);
		aabb.lowerBound = p - d;
		aabb.upperBound = p + d;
		
		// Query the world for overlapping shapes.
		QueryCallback callback(p);
		world->QueryAABB(&callback, aabb);
		
		if (callback.m_fixture) {
			b2Body* body = callback.m_fixture->GetBody();
			b2MouseJointDef md;                
			md.bodyB    = body;
			md.target   = p;
			md.maxForce = 1000.0f * body->GetMass();
#ifdef TARGET_OPENGLES
            md.bodyA    = touchBodies[id];
            touchJoints[id] = (b2MouseJoint*)world->CreateJoint(&md);
#else
            md.bodyA    = mouseBody;
            mouseJoint  = (b2MouseJoint*)world->CreateJoint(&md);
#endif           
                
			body->SetAwake(true);
		}
		
		
	}
	
}

// ------------------------------------------------------ 
void ofxBox2d::grabShapeUp(float x, float y, int id) {
#ifdef TARGET_OPENGLES     
    if(id >= 0 && id < OF_MAX_TOUCH_JOINTS) {
        if(touchJoints[id] && bEnableGrabbing){
			if(world == NULL) {
				ofLog(OF_LOG_WARNING, "ofxBox2d:: - Need a world, call init first! -");
				return;
			}
            world->DestroyJoint(touchJoints[id]);
            touchJoints[id] = NULL;
        }
    }
#else    
        if(mouseJoint && bEnableGrabbing) {
			if(world == NULL) {
				ofLog(OF_LOG_WARNING, "ofxBox2d:: - Need a world, call init first! -");
				return;
			}
            world->DestroyJoint(mouseJoint);
            mouseJoint = NULL;
        }
#endif
}

// ------------------------------------------------------ 
void ofxBox2d::grabShapeDragged(float x, float y, int id) {
	b2Vec2 p = toB2d(x, y);
#ifdef TARGET_OPENGLES  
    if(id >= 0 && id < OF_MAX_TOUCH_JOINTS) {
        if (touchJoints[id] && bEnableGrabbing)
            touchJoints[id]->SetTarget(p);
    }
#else    
    	if (mouseJoint && bEnableGrabbing)
        	mouseJoint->SetTarget(p);
#endif
    
    
}

// ------------------------------------------------------ 
int	ofxBox2d::getBodyCount() {
	if(world)
		return world->GetBodyCount();
	return 0;
}

// ------------------------------------------------------
int	ofxBox2d::getJointCount() {
	return world ? world->GetJointCount() : 0;
}

// ------------------------------------------------------ wake up
void ofxBox2d::wakeupShapes() {
    if(world == NULL) {
		ofLog(OF_LOG_WARNING, "ofxBox2d:: - Need a world, call init first! -");
		return;
	}
	
    b2Body* bodies = world->GetBodyList();
    while(bodies) {
        b2Body* b = bodies;
        if(b) {
            if( !b->IsAwake() ) b->SetAwake(true);
        }
        bodies = bodies->GetNext();
    }
    
}

// ------------------------------------------------------
void ofxBox2d::setFPS(float _hz) {
	hz = _hz;
	particleIterations = world->CalculateReasonableParticleIterations(hz);
	ofLogNotice() << "particleIterations " << ofToString(particleIterations);
}

#pragma mark - gravity
// ------------------------------------------------------
void ofxBox2d::setGravity(ofPoint pt) {
    setGravity(pt.x, pt.y);
}

//--------------------------------------------------------------
void ofxBox2d::setGravityX(float x) {
	setGravity(x, getGravity().y);
}

//--------------------------------------------------------------
void ofxBox2d::setGravityY(float y) {
	setGravity(getGravity().x, y);
}

//--------------------------------------------------------------
void ofxBox2d::setGravity(float x, float y) {
	if(world == NULL) {
		ofLog(OF_LOG_WARNING, "ofxBox2d:: - Need a world, call init first! -");
		return;
	}
	world->SetGravity(b2Vec2(x, y));
	
	// update the particle iteration based on gravity
	particleIterations = world->CalculateReasonableParticleIterations(hz);
	ofLogNotice() << "particleIterations " << ofToString(particleIterations);
	
	// wake up all shapes!
	wakeupShapes();
}

//--------------------------------------------------------------
ofPoint ofxBox2d::getGravity() {
	if(world == NULL) {
		ofLog(OF_LOG_WARNING, "ofxBox2d:: - Need a world, call init first! -");
		return ofPoint();
	}
    return ofPoint(world->GetGravity().x, world->GetGravity().y);
}

// ------------------------------------------------------ set bounds
void ofxBox2d::setBounds(ofPoint lowBounds, ofPoint upBounds) {
	//TODO: still need to work on this...
}

// ------------------------------------------------------ create Ground
void ofxBox2d::createGround(float x1, float y1, float x2, float y2) {
	
	if(world == NULL) {
		ofLog(OF_LOG_WARNING, "ofxBox2d:: - Need a world, call init first! -");
		return;
	}
	
	// if we have ground we need to destroy it
	if(ground != NULL) {
		world->DestroyBody(ground);
		ground = NULL;
	}
	
	b2BodyDef bd;
	ground = world->CreateBody(&bd);
	
	b2EdgeShape shape;
	shape.Set(toB2d(x1, y1), toB2d(x2, y2));
	ground->CreateFixture(&shape, 0.0f);

}
// ------------------------------------------------------ create Ground
void ofxBox2d::createGround(const ofPoint & p1, const ofPoint & p2) {
	createGround(p1.x, p1.y, p2.x, p2.y);
}

// ------------------------------------------------------ create bounds
void ofxBox2d::createBounds(ofRectangle rec) {
	createBounds(rec.x, rec.y, rec.width, rec.height);
}

// ------------------------------------------------------ create bounds
void ofxBox2d::createBounds(float x, float y, float w, float h) {
	
	if(world == NULL) {
		ofLog(OF_LOG_WARNING, "ofxBox2d:: - Need a world, call init first! -");
		return;
	}
	
	// if we have ground we need to destroy it
	if(ground != NULL) {
		world->DestroyBody(ground);
		ground = NULL;
	}
    
	b2BodyDef bd;
	bd.position.Set(0, 0);
	ground = world->CreateBody(&bd);	
	
	b2EdgeShape shape;
	
	ofRectangle rec(toB2d(x), toB2d(y), toB2d(w), toB2d(h));
	
	
	// right wall
	shape.Set(b2Vec2(rec.x+rec.width, rec.y), b2Vec2(rec.x+rec.width, rec.y+rec.height));
	ground->CreateFixture(&shape, 0.0f);
	
	// left wall
	shape.Set(b2Vec2(rec.x, rec.y), b2Vec2(rec.x, rec.y+rec.height));
	ground->CreateFixture(&shape, 0.0f);
	
	// top wall
	shape.Set(b2Vec2(rec.x, rec.y), b2Vec2(rec.x+rec.width, rec.y));
	ground->CreateFixture(&shape, 0.0f);
	
	// bottom wall
	shape.Set(b2Vec2(rec.x, rec.y+rec.height), b2Vec2(rec.x+rec.width, rec.y+rec.height));
	ground->CreateFixture(&shape, 0.0f);
	
}

// ------------------------------------------------------ check if shapes are out of bounds
void ofxBox2d::checkBounds(bool b) {
	bCheckBounds = b;
}

// ------------------------------------------------------ 
void ofxBox2d::setIterations(int velocityTimes, int positionTimes) {
	velocityIterations = velocityTimes;
	positionIterations = positionTimes;
}


// ------------------------------------------------------ 
void ofxBox2d::update() {
	if(world == NULL) {
		ofLog(OF_LOG_WARNING, "ofxBox2d:: - Need a world, call init first! -");
		return;
	}
	
	// destroy the object if we are out of the bounds
	if(bCheckBounds) {
		/*
		 float top = 0;
		 float bottom = ofGetHeight();
		 float right = ofGetWidth();
		 float left = 0;
		 
		 b2Body* node = world->GetBodyList();
		 while(node) {
		 b2Body* b = node;
		 node = node->GetNext();
		 b2Vec2 p = b->GetPosition();
		 ofxBox2dBaseShape* base = (ofxBox2dBaseShape*)b->GetUserData();
		 if(base) {
		 //printf("dead:%i\n", base->dead);
		 
		 if(p.y*OFX_BOX2D_SCALE > bottom) {
		 base->dead = true;
		 world->DestroyBody(b);
		 }
		 if(p.y*OFX_BOX2D_SCALE < top) {
		 base->dead = true;
		 world->DestroyBody(b);
		 }
		 if(p.x*OFX_BOX2D_SCALE > right) {
		 base->dead = true;
		 world->DestroyBody(b);
		 }
		 if(p.x*OFX_BOX2D_SCALE < left) {
		 base->dead = true;
		 world->DestroyBody(b);
		 }
		 */
		
		
	}
	
	world->Step(getTimeStep(), velocityIterations, positionIterations, particleIterations);
}

// ------------------------------------------------------
float ofxBox2d::getTimeStep() {
    return hz > 0.0f ? 1.0f / hz : 0.0f;
}

// ------------------------------------------------------ 
void ofxBox2d::drawGround() {
	
	if(ground == NULL) return;
	
// 	const b2Transform& xf = ground->GetTransform();
	for (b2Fixture* f = ground->GetFixtureList(); f; f = f->GetNext()) {
		b2EdgeShape * edge = (b2EdgeShape*)f->GetShape();
		if(edge) {
			ofNoFill();
			ofSetColor(120, 0, 120);
			ofDrawLine(toOf(edge->m_vertex0), toOf(edge->m_vertex1));
		}
	}
}

// ------------------------------------------------------ 
void ofxBox2d::draw() {
	drawGround();
}

