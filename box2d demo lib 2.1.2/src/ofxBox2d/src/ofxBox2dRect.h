

#pragma once
#include "ofMain.h"
#include "ofxBox2dBaseShape.h"

class ofxBox2dRect : public ofxBox2dBaseShape {
private:
	float _width, _height;
public:
	

	//------------------------------------------------
	
	ofxBox2dRect() {
	}
	
	//------------------------------------------------
	void setup(b2World * b2dworld, ofRectangle rec) {
		setup(b2dworld, rec.x, rec.y, rec.width, rec.height);
	}
	
	//------------------------------------------------
	void setup(b2World * b2dworld, float x, float y, float w, float h) {
		
		if(b2dworld == NULL) {
			ofLog(OF_LOG_NOTICE, "- must have a valid world -");
			return;
		}
		
		if (OF_RECTMODE_CORNER) {
			w/=2; h/=2;
			x += w; y += h;
		}
		
		_width	= w;
		_height	= h;
		
		b2PolygonShape shape;
		shape.SetAsBox(w/OFX_BOX2D_SCALE, h/OFX_BOX2D_SCALE);
				
		fixture.shape		= &shape;
		fixture.density		= density;
		fixture.friction	= friction;
		fixture.restitution = bounce;
		
		b2BodyDef bodyDef;
		if(density == 0.f)
			bodyDef.type	= b2_staticBody;
		else
			bodyDef.type	= b2_dynamicBody;
		bodyDef.position.Set(x/OFX_BOX2D_SCALE, y/OFX_BOX2D_SCALE);	
		
		
		body = b2dworld->CreateBody(&bodyDef);
		body->CreateFixture(&fixture);
		
	}
	
	float getWidth() {
		return _width;
	}
	
	float getHeight() {
		return _height;
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














