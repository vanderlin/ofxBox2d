#pragma once
#include "ofMain.h"
#include "Box2D.h"


#define OFX_BOX2D_SCALE 30.0f


static float b2dNum(float f) {
	return (float)f/OFX_BOX2D_SCALE;	
}


static b2Vec2 screenPtToWorldPt(ofVec2f p) {
	return b2Vec2(p.x/OFX_BOX2D_SCALE, p.y/OFX_BOX2D_SCALE);
}
static ofPoint worldPtToscreenPt(b2Vec2 p) {
	return ofPoint(p.x*OFX_BOX2D_SCALE, p.y*OFX_BOX2D_SCALE);
}

class QueryCallback : public b2QueryCallback {
	
public:
	QueryCallback(const b2Vec2& point) {
		m_point = point;
		m_fixture = NULL;
	}
	
	bool ReportFixture(b2Fixture* fixture) {
		b2Body* body = fixture->GetBody();
		if (body->GetType() == b2_dynamicBody)
		{
			bool inside = fixture->TestPoint(m_point);
			if (inside)
			{
				m_fixture = fixture;
				
				// We are done, terminate the query.
				return false;
			}
		}
		
		// Continue the query.
		return true;
	}
	
	b2Vec2 m_point;
	b2Fixture* m_fixture;
};