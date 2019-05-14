//
//  ParticleSystem.h
//  box2d-stress-test
//
//  Created by Todd Vanderlin on 9/28/17.
//
//

#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"
#include <Box2D/Box2D.h>

namespace ofxBox2dParticleSystem {
	
	class ParticleSystem {
		
		ofVboMesh mesh;
		b2World * world;
		b2ParticleSystem * particleSystem;
		
		// rendering offset for glPointSize 0-1
		float pointSizeOffset;
		
		// for now I have a single particle flag
		// I want a better system to combine flags
		uint32 particleFlag;
		
	public:
		
		ParticleSystem();
		
		// helpers
		ofVec2f toVec2f(const b2Vec2& p) {
			return ofVec2f(p.x, p.y);
		}
		
		ofVec2f toOf(const b2Vec2& p) {
			float scale = ofxBox2d::getScale();
			return ofVec2f(p.x * scale, p.y * scale);
		}
		
		float toOf(const float v) {
			float scale = ofxBox2d::getScale();
			return v * scale;
		}
		
		b2Vec2 toB2d(const ofVec2f& p) {
			float scale = ofxBox2d::getScale();
			return b2Vec2(p.x / scale, p.y / scale);
		}
		
		// init the system creates the world and system
		void init(b2World * _world, int _maxParticles=1000);
		
		// tick
		void tick();
		
		// update the mesh with new data
		void updateMesh();
		
		// render the mesh
		void draw();
		
		// render all shapes
		void drawShapes(float scaleFactor=1);
		
		// render out connection
		void drawConnections(ofColor color = ofColor::white, bool withWeights=false);
		
		// render any shapes in the b2d world
		void drawShape(b2Fixture* fixture, const b2Transform& xf, const b2Color& color, float scaleFactor=1);

		// add a single particle
		void addParticle(float x, float y, uint32 flags=b2_waterParticle);
		
		// add a grouping of particles
		void addParticleGroup(float x, float y, uint32 flags=b2_waterParticle, float rad=50);
		
		// remove all particles
		void clearParticles();
		
		// remove any particles outside bounds
		// return total removed
		int removeOutsideBounds(const ofRectangle &bounds);
		
		// remove the particle at index
		// this will update the mesh
		void removeParticle(int index);
		
		// set the radius of the particle system
		void setRadius(float radius);
		
		// set the particle type
		uint32 setParticleType(int type);
		
		// set max particles
		void setMaxParticles(int count);
		
		// set the offset of the glPointsize
		void setPointSizeOffsetPercent(float pct);
		
		// get all postion (scaled) to world
		vector <ofVec2f> getPositions();

		// get radius (scaled)
		float getRadius();
		
		// radius plus buffer
		float getRenderRadius();
		
		// get total particles
		int getTotalParticles();
		
		// helper to create a body
		b2Body * createBody(const b2BodyDef* def);
		
		// box2d world
		b2World * getWorld();
		
		// liquid fun particle system
		b2ParticleSystem * getParticleSystem();
		
		// these are the possible particle types
		static vector <string> particleTypesNames;
        
	};
}

