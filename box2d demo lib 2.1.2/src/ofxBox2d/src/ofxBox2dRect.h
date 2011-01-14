

#pragma once
#include "ofMain.h"
#include "ofxBox2dBaseShape.h"

class ofxBox2dRect : public ofxBox2dBaseShape {
	
public:
	

	//------------------------------------------------
	
	ofxBox2dRect() {
	}
	
	//------------------------------------------------
	void setup(b2World * b2dworld, float x, float y, float w, float h, bool isFixed=false) {
		
		if(b2dworld == NULL) {
			ofLog(OF_LOG_NOTICE, "- must have a valid world -");
			return;
		}
		
		world				= b2dworld;
		bIsFixed			= isFixed;
		
		//Rect Shape
		w/=2; h/=2;
		b2PolygonShape shape;
		shape.SetAsBox(w/OFX_BOX2D_SCALE, h/OFX_BOX2D_SCALE);
				
		fixture.shape    = &shape;
		fixture.density  = 1;
		fixture.friction = friction;
		//fixture.restitution = bounce;
		
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		x += w; y += h;
		bodyDef.position.Set(x/OFX_BOX2D_SCALE, y/OFX_BOX2D_SCALE);	
		
		
		body = world->CreateBody(&bodyDef);
		body->CreateFixture(&fixture);
		
		
		// anything that you need called
		init();
	}
	
	
	/*
	 //------------------------------------------------
	 float getRadius() {
	 b2Shape* shape		= body->GetShapeList();
	 b2CircleShape *data = (b2CircleShape*)shape;
	 return data->GetRadius() * OFX_BOX2D_SCALE;
	 }
	 */        
	//------------------------------------------------
	float getRotation() {
		
		if(body == NULL) {
			return 0;	
		}
		return ofRadToDeg(body->GetAngle());
	}
	
	
	//------------------------------------------------
	void draw() {
		
		if(body == NULL) {
			return;	
		}
		
		const b2Transform& xf = body->GetTransform();
		for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
			b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();
				
			if(poly) {
				ofFill();
				ofSetHexColor(0xBF2545);
				ofBeginShape();
				for(int i=0; i<poly->m_vertexCount; i++) {
					b2Vec2 pt = b2Mul(xf, poly->m_vertices[i]);
					ofVertex(pt.x*OFX_BOX2D_SCALE, pt.y*OFX_BOX2D_SCALE);
				}
				ofEndShape(true);
			}
		}
		
		
	}
	
};














