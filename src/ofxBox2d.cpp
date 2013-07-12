#include "ofxBox2d.h"

// ------------------------------------------------------ 
ofxBox2d::ofxBox2d() {
	world = NULL;
	m_bomb = NULL;
#ifdef TARGET_OPENGLES
    // touch grabbing
    for( int i=0; i<OF_MAX_TOUCH_JOINTS; i++ )
		touchJoints[ i ] = NULL;
    for( int i=0; i<OF_MAX_TOUCH_JOINTS; i++ )
		touchBodies[ i ] = NULL;
#else
    // mouse grabbing
	mouseJoint = NULL;
	mouseBody  = NULL;
#endif
	ground = NULL;
	mainBody = NULL;
}

// ------------------------------------------------------
ofxBox2d::~ofxBox2d() {
	
#ifdef TARGET_OPENGLES
    // destroy touch grabbing bodies
    for(int i=0; i<OF_MAX_TOUCH_JOINTS; i++) {
        if(touchBodies[i]) {
        	if(world)
            	world->DestroyBody(touchBodies[i]);
        }
    }
#else
    // destroy mouse grabbing body
    if(mouseBody) {
		if(world)
            world->DestroyBody(mouseBody);
	}
#endif
    if(world) {
        delete world;
        world = NULL;
    }
}

// ------------------------------------------------------ init
void ofxBox2d::init() {
	
	// settings
	bHasContactListener = false;
	bCheckBounds		= false;
	bEnableGrabbing		= true;
	scale				= OFX_BOX2D_SCALE;
	doSleep				= true;
	
	// gravity
	gravity.set(0, 5.0f);
	setFPS(60.0);
	velocityIterations = 40;
	positionIterations = 20;
		
#ifdef TARGET_OPENGLES    
    // touch grabbing
    for( int i=0; i<OF_MAX_TOUCH_JOINTS; i++ )
		touchJoints[ i ] = NULL;
    for( int i=0; i<OF_MAX_TOUCH_JOINTS; i++ )
		touchBodies[ i ] = NULL;
#else
    // mouse grabbing
	mouseJoint = NULL;
	mouseBody  = NULL;
#endif
	// ground/bounds
	ground	   = NULL;
	
	// debug drawer
	debugRender.setScale(scale);
	debugRender.SetFlags(1);
	
	//worldAABB.lowerBound.Set(-100.0f, -100.0f);
	//worldAABB.upperBound.Set(100.0f, 100.0f);
	
	world = new b2World(b2Vec2(gravity.x, gravity.y), doSleep);
	world->SetDebugDraw(&debugRender);
	
	ofLog(OF_LOG_NOTICE, "ofxBox2d:: - world created -");
	
	world->SetContactListener(this);
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
		b2Vec2 p(x/OFX_BOX2D_SCALE, y/OFX_BOX2D_SCALE);		
        
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
	b2Vec2 p(x/OFX_BOX2D_SCALE, y/OFX_BOX2D_SCALE);
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
	if(world)
		return world->GetJointCount();
	return 0;
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

// ------------------------------------------------------ set gravity
void ofxBox2d::setGravity(ofPoint pt) {
    setGravity(pt.x, pt.y);
}
void ofxBox2d::setGravity(float x, float y) {
	if(world == NULL) {
		ofLog(OF_LOG_WARNING, "ofxBox2d:: - Need a world, call init first! -");
		return;
	}
	world->SetGravity(b2Vec2(x, y));
    wakeupShapes();
}
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
	
	b2BodyDef bd;
	ground = world->CreateBody(&bd);
	
	b2PolygonShape shape;
	shape.SetAsEdge(b2Vec2(x1/OFX_BOX2D_SCALE, y1/OFX_BOX2D_SCALE), b2Vec2(x2/OFX_BOX2D_SCALE, y2/OFX_BOX2D_SCALE));
	ground->CreateFixture(&shape, 0.0f);

}
// ------------------------------------------------------ create Ground
void ofxBox2d::createGround(const ofPoint & p1, const ofPoint & p2) {
	createGround(p1.x, p1.y, p2.x, p2.y);
}

// ------------------------------------------------------ create bounds
void ofxBox2d::createBounds(ofRectangle &rec) {
	createBounds(rec.x, rec.y, rec.width, rec.height);
}

// ------------------------------------------------------ create bounds
void ofxBox2d::createBounds(float x, float y, float w, float h) {
	
	if(world == NULL) {
		ofLog(OF_LOG_WARNING, "ofxBox2d:: - Need a world, call init first! -");
		return;
	}
	
	b2BodyDef bd;
	bd.position.Set(0, 0);
	ground = world->CreateBody(&bd);	
	
	b2PolygonShape shape;
	
	ofRectangle rec(x/OFX_BOX2D_SCALE, y/OFX_BOX2D_SCALE, w/OFX_BOX2D_SCALE, h/OFX_BOX2D_SCALE);
	
	
	//right wall
	shape.SetAsEdge(b2Vec2(rec.x+rec.width, rec.y), b2Vec2(rec.x+rec.width, rec.y+rec.height));
	ground->CreateFixture(&shape, 0.0f);
	
	//left wall
	shape.SetAsEdge(b2Vec2(rec.x, rec.y), b2Vec2(rec.x, rec.y+rec.height));
	ground->CreateFixture(&shape, 0.0f);
	
	// top wall
	shape.SetAsEdge(b2Vec2(rec.x, rec.y), b2Vec2(rec.x+rec.width, rec.y));
	ground->CreateFixture(&shape, 0.0f);
	
	// bottom wall
	shape.SetAsEdge(b2Vec2(rec.x, rec.y+rec.height), b2Vec2(rec.x+rec.width, rec.y+rec.height));
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
	
	float	timeStep		   = (1.0f / fps);
	world->Step(timeStep, velocityIterations, positionIterations);
	//world->Validate();	
	
}

// ------------------------------------------------------ 
void ofxBox2d::drawGround() {
	
	if(ground == NULL) return;
	
	const b2Transform& xf = ground->GetTransform();
	for (b2Fixture* f = ground->GetFixtureList(); f; f = f->GetNext()) {
		b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();
		if(poly) {
			ofNoFill();
			ofSetColor(120, 0, 120);
			ofBeginShape();
			for(int i=0; i<poly->m_vertexCount; i++) {
				b2Vec2 pt = b2Mul(xf, poly->m_vertices[i]);
				ofVertex(pt.x*OFX_BOX2D_SCALE, pt.y*OFX_BOX2D_SCALE);
			}
			ofEndShape(true);
		}
	}
	
	
	//
	//	GetFixtureList
	//	b2PolygonShape* poly = (b2PolygonShape*)fixture->GetShape();
	//	int32 vertexCount = poly->m_vertexCount;
	//	b2Assert(vertexCount <= b2_maxPolygonVertices);
	//	b2Vec2 vertices[b2_maxPolygonVertices];
	//	
	//	for (int32 i = 0; i < vertexCount; ++i)
	//	{
	//		vertices[i] = b2Mul(xf, poly->m_vertices[i]);
	//	}
	//	
	//	m_debugDraw->DrawSolidPolygon(vertices, vertexCount, color);
	//	
	/*
	 //draw the ground
	 if(ground != NULL) {
	 for(b2Shape* s=ground->GetShapeList(); s; s=s->GetNext()) {
	 
	 const b2XForm& xf = ground->GetXForm();		
	 b2PolygonShape* poly = (b2PolygonShape*)s;
	 int count = poly->GetVertexCount();
	 const b2Vec2* verts = poly->GetVertices();
	 ofEnableAlphaBlending();
	 ofFill();
	 ofSetColor(90, 90, 90, 100);
	 ofBeginShape();
	 for(int j=0; j<count; j++) {
	 
	 b2Vec2 pt = b2Mul(xf, verts[j]);
	 
	 ofVertex(pt.x*OFX_BOX2D_SCALE, pt.y*OFX_BOX2D_SCALE);
	 }
	 ofEndShape();
	 ofDisableAlphaBlending();
	 }
	 }
	 */
	
}

// ------------------------------------------------------ 
void ofxBox2d::draw() {
	/*
	 if(mouseJoint) {
	 b2Body* mbody = mouseJoint->GetBody2();
	 b2Vec2 p1 = mbody->GetWorldPoint(mouseJoint->m_localAnchor);
	 b2Vec2 p2 = mouseJoint->m_target;
	 
	 p1 *= OFX_BOX2D_SCALE;
	 p2 *= OFX_BOX2D_SCALE;
	 
	 //draw a line from touched shape
	 ofEnableAlphaBlending();
	 ofSetLineWidth(2.0);
	 ofSetColor(200, 200, 200, 200);
	 ofLine(p1.x, p1.y, p2.x, p2.y);
	 ofNoFill();
	 ofSetLineWidth(1.0);
	 ofCircle(p1.x, p1.y, 2);
	 ofCircle(p2.x, p2.y, 5);
	 ofDisableAlphaBlending();
	 }
	 
	 
	 */
	
	drawGround();
}

