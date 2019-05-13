//
//  ofxBox2dRender.cpp
//  particle-system
//
//  Created by Todd Vanderlin on 10/23/17.
//

#include "ofxBox2dRender.h"
#include "ofxBox2d.h"


void ofxBox2dRender::setScale(float f) {
	scaleFactor = f;
}

void ofxBox2dRender::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
	ofSetColor(color.r*255.0, color.g*255.0, color.b*255.0);
	ofBeginShape();
	for (int i = 0; i < vertexCount; ++i) {
		ofVec2f p = ofxBox2d::toOf(vertices[i]);
		ofVertex(p.x, p.y);
	}
	ofEndShape();
}
void ofxBox2dRender::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
	
	ofSetHexColor(0xffffff);
	ofBeginShape();
	for(int i=0; i<vertexCount; ++i) {
		ofVec2f p = ofxBox2d::toOf(vertices[i]);
		ofVertex(p.x, p.y);
	}
	ofEndShape();
	
	
}
void ofxBox2dRender::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {
	const float32 k_segments = 16.0f;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;
	ofSetColor(color.r*255.0, color.g*255.0, color.b*255.0);
	ofBeginShape();
	for (int i = 0; i < k_segments; i++) {
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		ofVertex(v.x, v.y);
		theta += k_increment;
	}
	ofEndShape();
}
void ofxBox2dRender::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {
	// 		const float32 k_segments    = 16.0f;
	// 		const float32 k_increment   = 2.0f * b2_pi / k_segments;
	// 		float32 theta			    = 0.0f;
	float rad = (radius*scaleFactor);
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ofSetColor(255, 255, 255, 200);
	ofFill();
	ofDrawCircle(center.x*scaleFactor, center.y*scaleFactor, rad);
}
void ofxBox2dRender::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
	ofSetColor(255, 255, 255, 200);
	ofDrawLine(ofxBox2d::toOf(p1), ofxBox2d::toOf(p2));
}
void ofxBox2dRender::DrawTransform(const b2Transform& xf) {
}

void ofxBox2dRender::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) {
}
void ofxBox2dRender::DrawString(int x, int y, const char* string, ...) {
}
void ofxBox2dRender::DrawAABB(b2AABB* aabb, const b2Color& color) {
	ofSetColor(color.r*255.0, color.g*255.0, color.b*255.0);
	ofBeginShape();
	ofVertex(aabb->lowerBound.x, aabb->lowerBound.y);
	ofVertex(aabb->upperBound.x, aabb->lowerBound.y);
	ofVertex(aabb->upperBound.x, aabb->upperBound.y);
	ofVertex(aabb->lowerBound.x, aabb->upperBound.y);
	ofEndShape();
}

void ofxBox2dRender::DrawParticles(const b2Vec2 *centers, float32 radius, const b2ParticleColor *colors, int32 count) {
	
}

