

#pragma once
#include "ofMain.h"
#include "ofxBox2dBaseShape.h"

class ofxBox2dCircle : public ofxBox2dBaseShape {
private:
	float _radius;
	
public:
	
	
	
	//------------------------------------------------
	ofxBox2dCircle() {
	}
	
	//------------------------------------------------
	void setup(b2World * b2dworld, float x, float y, float radius) {
		
		
		if(b2dworld == NULL) {
			ofLog(OF_LOG_NOTICE, "ofxBox2dCircle :: setup : - must have a valid world -");
			return;
		}
		b2CircleShape shape;
		
		shape.m_p.SetZero();
		shape.m_radius = radius / OFX_BOX2D_SCALE;
		
		_radius			 = radius;
		
		fixture.shape	 = &shape;
		fixture.density  = density;
		fixture.friction = friction;
		fixture.restitution	= bounce;
		
		if(density == 0.f)
			bodyDef.type	= b2_staticBody;
		else
			bodyDef.type	= b2_dynamicBody;
		
		bodyDef.position.Set(x/OFX_BOX2D_SCALE, y/OFX_BOX2D_SCALE);
		
		body  = b2dworld->CreateBody(&bodyDef);
		body->CreateFixture(&fixture);
		
		
		
		//m_radius = 0.5f;
//		b2CircleShape shape;
//		shape.m_radius = m_radius;
//		m_character = body->CreateFixture(&shape, 20.0f);
//		
//		body->SetLinearVelocity(b2Vec2(0.0f, -50.0f));
//		
//		m_state = e_unknown;
		
		
		/*
		b2CircleShape shape;
		shape.m_radius = 1.0f;
		
		for (int32 i = 0; i < e_count; ++i)
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(0.0, 4.0f + 3.0f * i);
			
			m_bodies[i] = m_world->CreateBody(&bd);
			
			m_bodies[i]->CreateFixture(&shape, 1.0f);
			
			//m_bodies[i]->SetLinearVelocity(b2Vec2(0.0f, -100.0f));
		}
		*/
		
		
		/*
		world				= b2dworld;
		circle.radius		= size/OFX_BOX2D_SCALE;
		bIsFixed			= isFixed;
		
		if(isFixed) {
			circle.density		= 0;
			circle.restitution  = 0;
			circle.friction		= 0;
		}
		else {
			circle.density		= mass;
			circle.restitution  = bounce;
			circle.friction		= friction;
		}
		bodyDef.position.Set(x/OFX_BOX2D_SCALE, y/OFX_BOX2D_SCALE);	
		
		body = world->CreateBody(&bodyDef);
		if(body) {
			body->SetLinearVelocity(b2Vec2(0.0, 0.0));
			body->CreateShape(&circle);
			body->SetMassFromShapes();
		}
		*/
		
		// anything that you need called
		//init();
	}
	
	//------------------------------------------------
	float getRadius() {
		return _radius;
		/*
		b2Fixture * circle = body->GetFixtureList();
		if(circle) {
			b2CircleShape * circleShape = (b2CircleShape*)circle->GetShape();
			if(circleShape) {
				return circleShape->m_radius * OFX_BOX2D_SCALE;	
			}
		}
		 */
	}
	
	//------------------------------------------------ 
	void disableCollistion() {
		if(body != NULL) {
		//	circle.filter.maskBits = 0x0;		
		}
	}
	
	//------------------------------------------------
	/*
	 Im not sure about this it seems like a bad idea.
	 I cant figure out another way to change the radius of
	 a shape that we have
	 
	 -- any help here :) --
	 
	 */
	void setRadius(float r) {
		/*
		if(body != NULL) {
			for(b2Shape* s=body->GetShapeList(); s; s=s->GetNext()) {
				body->DestroyShape(s);
			}
			
			circle.radius	    = r/OFX_BOX2D_SCALE;
			circle.density		= mass;
			circle.restitution  = bounce;
			circle.friction		= friction;
			
			//body = world->CreateBody(&bodyDef);
			body->SetLinearVelocity(b2Vec2(0.0, 0.0));
			body->CreateShape(&circle);
			body->SetMassFromShapes();
		}
		 */
	}
	
	//------------------------------------------------
	virtual void draw() {
		
		
		//switch (fixture->GetType())
		//{
			//case b2Shape::e_circle:
			//{
		if(body == NULL) {
			return;
		}
		
		glPushMatrix();
		glTranslatef(getPosition().x, getPosition().y, 0);
		glRotatef(getRotation(), 0, 0, 1);
		
		ofSetHexColor(0xffffff);
		
		ofLine(0, 0, _radius, 0);
		
		if(isFixed()) {
			ofSetColor(255, 0, 255);
			ofFill();
			ofCircle(0, 0, _radius);	
		}
		else {
			ofSetColor(0, 255, 255);
			ofNoFill();
			ofCircle(0, 0, _radius);
			
			ofSetColor(255, 0, 255);
			ofFill();
			ofCircle(0, 0, _radius/10.0);
			
			ofSetColor(255, 255, 255);
			ofNoFill();
			ofCircle(0, 0, _radius/5.0);
		}
		glPopMatrix();
		
		/*
		float angle			= getRotation();
		//const b2XForm& xf	= body->GetXForm();
		b2Vec2	center		= body->GetPosition();
		b2Vec2	axis		= xf.R.col1;
		b2Vec2	p			= center + radius/OFX_BOX2D_SCALE * axis;
		
		ofSetColor(0xff00ff);
		ofLine(getPosition().x, getPosition().y, p.x*OFX_BOX2D_SCALE, p.y*OFX_BOX2D_SCALE);
		*/
	}
	
};














