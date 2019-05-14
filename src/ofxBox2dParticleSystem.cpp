//
//  ParticleSystem.cpp
//  box2d-stress-test
//
//  Created by Todd Vanderlin on 9/28/17.
//
//

// drawing connections...
/*
const b2Vec2 * vels = m_particleSystem->GetVelocityBuffer();
const b2ParticleContact * pairs = m_particleSystem->GetContacts();
ofMesh connectionsMesh;
connectionsMesh.setMode(OF_PRIMITIVE_LINES);
int nPairs = m_particleSystem->GetContactCount();
for(int i=0; i<nPairs; i++) {
	b2Vec2 a = m_particleSystem->GetPositionBuffer()[pairs[i].GetIndexA()];
	b2Vec2 b = m_particleSystem->GetPositionBuffer()[pairs[i].GetIndexB()];
	
	connectionsMesh.addVertex(ofVec3f(a.x, a.y));
	connectionsMesh.addVertex(ofVec3f(b.x, b.y));
}

ofSetColor(255);
connectionsMesh.draw();
*/
#include "ofxBox2dParticleSystem.h"

using namespace ofxBox2dParticleSystem;

vector <string> ParticleSystem::particleTypesNames = {
	"waterParticle",
	"zombieParticle",
	"wallParticle",
	"springParticle",
	"elasticParticle",
	"viscousParticle",
	"powderParticle",
	"tensileParticle",
	"colorMixingParticle",
	"destructionListenerParticle",
	"barrierParticle",
	"staticPressureParticle",
	"reactiveParticle",
	"repulsiveParticle"
};

ParticleSystem::ParticleSystem() {
	world = NULL;
	particleSystem = NULL;
}

//--------------------------------------------------------------
void ParticleSystem::init(b2World * _world, int _maxParticles) {
	
	pointSizeOffset = 1;
	
	// set the world from
	world = _world;
	
	/*if (particleSystem != NULL) {
		delete particleSystem;
		particleSystem = NULL;
	}*/

	// create the main particle system
	const b2ParticleSystemDef particleSystemDef;
	particleSystem = world->CreateParticleSystem(&particleSystemDef);
	
	// gravity scale
	particleSystem->SetGravityScale(1);//0.4f);
	
	// density
	particleSystem->SetDensity(1.2f);
	
	// default particle flag
	particleFlag = b2_waterParticle;
	
	// set max particles this will allocate the mesh
	// and setup the b2dparticles systems
	setMaxParticles(_maxParticles);
}

//--------------------------------------------------------------
void ParticleSystem::tick() {
	updateMesh();
}

#pragma mark - box2d helpers (maybe move to a b2dHelpers)
//--------------------------------------------------------------
b2Body * ParticleSystem::createBody(const b2BodyDef* def) {
	return world->CreateBody(def);
}

#pragma mark - add particles
//--------------------------------------------------------------
void ParticleSystem::addParticle(float x, float y, uint32 flags) {
    
	b2ParticleDef pd;
	pd.position = ofxBox2d::toB2d(x, y);
	pd.flags = flags;
	
	particleSystem->CreateParticle(pd);
}

//--------------------------------------------------------------
void ParticleSystem::addParticleGroup(float x, float y, uint32 flags, float rad) {
	
	b2CircleShape shape;
	shape.m_p = ofxBox2d::toB2d(x, y);
	shape.m_radius = ofxBox2d::toB2d(rad);
	
	b2ParticleGroupDef pg;
	pg.shape = &shape;
	pg.flags = flags;
	pg.groupFlags = b2_rigidParticleGroup;
	
	particleSystem->CreateParticleGroup(pg);
}

#pragma mark - clean up
//--------------------------------------------------------------
void ParticleSystem::clearParticles() {
	for (int i=0; i<particleSystem->GetParticleCount(); i++) {
		particleSystem->DestroyParticle(i);
		mesh.setVertex(i, ofVec3f(-1000, -1000, 0));
	}
}

//--------------------------------------------------------------
int ParticleSystem::removeOutsideBounds(const ofRectangle &bounds) {
	int count = 0;
	int n = particleSystem->GetParticleCount();
	b2Vec2 * positions = particleSystem->GetPositionBuffer();
	for(int i=0; i<n; i++) {
		ofVec2f pos = toOf(positions[i]);
		if(!bounds.inside(pos)) {
			mesh.setVertex(i, ofVec3f(-1000, -1000, 0));
			particleSystem->DestroyParticle(i);
			count ++;
		}
	}
	return count;
}

//--------------------------------------------------------------
void ParticleSystem::removeParticle(int index) {
	particleSystem->DestroyParticle(index);
}

#pragma mark - setters
//--------------------------------------------------------------
void ParticleSystem::setRadius(float radius) {
	particleSystem->SetRadius(ofxBox2d::toB2d(radius));
	#pragma warning maybe recaluelate the particle inter ?
}

//--------------------------------------------------------------
void ParticleSystem::setPointSizeOffsetPercent(float pct) {
	pointSizeOffset = pct;
}

//--------------------------------------------------------------
void ParticleSystem::setMaxParticles(int count) {
	
	// set the max particles
	particleSystem->SetMaxParticleCount(count);
	
	mesh.clear();
	
	for(int i=0; i<count; i++) {
		mesh.addVertex(ofVec3f(-1000, -1000, 0));
	}
	
	// set the mesh to draw points
	mesh.setMode(OF_PRIMITIVE_POINTS);
}

//--------------------------------------------------------------
uint32 ParticleSystem::setParticleType(int type) {
	
	switch(type) {
		case 0:
			particleFlag = b2_waterParticle;
			break;
		case 1:
             particleFlag = b2_zombieParticle;
			break;
		case 2:
			particleFlag = b2_wallParticle;
			break;
		case 3:
			particleFlag = b2_springParticle;
			break;
		case 4:
			particleFlag = b2_elasticParticle;
			break;
		case 5:
			particleFlag = b2_viscousParticle;
			break;
		case 6:
			particleFlag = b2_powderParticle;
			break;
		case 7:
			particleFlag = b2_tensileParticle;
			break;
		case 8:
			particleFlag = b2_colorMixingParticle;
			break;
		case 9:
			particleFlag = b2_destructionListenerParticle;
			break;
		case 10:
			particleFlag = b2_barrierParticle;
			break;
		case 11:
			particleFlag = b2_staticPressureParticle;
			break;
		case 12:
			particleFlag = b2_reactiveParticle;
			break;
		case 13:
			particleFlag = b2_repulsiveParticle;
			break;
	}
	cout << "particle flag " << particleFlag << endl;
	
	// update all the particles with the new type
	if(particleSystem != NULL) {
		int n = particleSystem->GetParticleCount();
		for (int i=0; i<n; i++) {
			particleSystem->SetParticleFlags(i, particleFlag);
		}
	}
	
	return particleFlag;
}


#pragma mark - getters
//--------------------------------------------------------------
b2World * ParticleSystem::getWorld() {
	return world;
}

//--------------------------------------------------------------
b2ParticleSystem * ParticleSystem::getParticleSystem() {
	return particleSystem;
}

//--------------------------------------------------------------
int ParticleSystem::getTotalParticles() {
	return particleSystem->GetParticleCount();
}

//--------------------------------------------------------------
vector <ofVec2f> ParticleSystem::getPositions() {
	float scale = ofxBox2d::getScale();
	int particleCount = particleSystem->GetParticleCount();
	b2Vec2 * pos = particleSystem->GetPositionBuffer();
	vector <ofVec2f> positions;
	for(int i=0; i<particleCount; i++) {
		positions.push_back(ofVec2f(pos[i].x * scale, pos[i].y * scale));
	}
	return positions;
}

//--------------------------------------------------------------
float ParticleSystem::getRadius() {
	float scale = ofxBox2d::getScale();
	return ((particleSystem->GetRadius()) * scale) * pointSizeOffset;
}

float ParticleSystem::getRenderRadius() {
	float scale = ofxBox2d::getScale();
	return ((particleSystem->GetRadius() * 1.5) * scale) * pointSizeOffset;
}

#pragma mark - rendering
//--------------------------------------------------------------
void ParticleSystem::updateMesh() {
	
	// update the particle system mesh
	int particleCount = particleSystem->GetParticleCount();
	const b2Vec2 * pos = particleSystem->GetPositionBuffer();
	
	// this is great! (how hard are the contacting eachother 0 - 1
	// const float32 * weights = particleSystem->GetWeightBuffer();
	mesh.clear();
	for (int i=0; i<particleCount; i++) {
		mesh.addVertex(ofVec3f(pos[i].x, pos[i].y, 0));
		// mesh.setColor(i, ofFloatColor(1, weights[i], 1));
	}
}

//--------------------------------------------------------------
void ParticleSystem::drawShape(b2Fixture* fixture, const b2Transform& xf, const b2Color& color, float scaleFactor) {
	
	if (!fixture) return;
	
	switch (fixture->GetType())
	{
		case b2Shape::e_circle:
		{
			b2CircleShape* circle = (b2CircleShape*)fixture->GetShape();
			
			b2Vec2 center = b2Mul(xf, circle->m_p);
			float32 radius = circle->m_radius;
			b2Vec2 axis = b2Mul(xf.q, b2Vec2(1.0f, 0.0f));
			
			ofPushMatrix();
			ofTranslate(center.x, center.y);
			ofDrawCircle(0, 0, radius*scaleFactor);
			ofPopMatrix();
		}
			break;
			
		case b2Shape::e_edge:
		{
			b2EdgeShape* edge = (b2EdgeShape*)fixture->GetShape();
			b2Vec2 v1 = b2Mul(xf, edge->m_vertex1);
			b2Vec2 v2 = b2Mul(xf, edge->m_vertex2);
			ofDrawLine(v1.x, v1.y, v2.x, v2.y);
		}
			break;
			
		case b2Shape::e_chain:
		{
			b2ChainShape* chain = (b2ChainShape*)fixture->GetShape();
			int32 count = chain->m_count;
			const b2Vec2* vertices = chain->m_vertices;
			
			b2Vec2 v1 = b2Mul(xf, vertices[0]);
			for (int32 i = 1; i < count; ++i)
			{
				b2Vec2 v2 = b2Mul(xf, vertices[i]);
				ofDrawLine(v1.x, v1.y, v2.x, v2.y);
				ofDrawCircle(v1.x, v1.y, 0.05f);
				v1 = v2;
			}
		}
			break;
			
		case b2Shape::e_polygon:
		{
			b2PolygonShape* poly = (b2PolygonShape*)fixture->GetShape();
			int32 vertexCount = poly->m_count;
			b2Assert(vertexCount <= b2_maxPolygonVertices);
			b2Vec2 vertices[b2_maxPolygonVertices];
			
			ofBeginShape();
			for (int32 i = 0; i < vertexCount; ++i)
			{
				vertices[i] = b2Mul(xf, poly->m_vertices[i]);
				ofVertex(vertices[i].x, vertices[i].y);
			}
			ofEndShape(true);
		}
			break;
			
		default:
			break;
	}
}

//--------------------------------------------------------------
void ParticleSystem::drawShapes(float scaleFactor) {
	
	float scale = ofxBox2d::getScale();
	
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(0, 0);
	ofScale(scale, scale);
	
	// render anything in the box2d world
	b2Body * bodyList = world->GetBodyList();
	for (b2Body* b = bodyList; b; b = b->GetNext()) {
		const b2Transform& xf = b->GetTransform();
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext()) {
			drawShape(f, xf, b2Color(0.5f, 0.9f, 0.5f), scaleFactor);
		}
	}
	
	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
void ParticleSystem::draw() {
	float scale = ofxBox2d::getScale();
	
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(0, 0);
	ofScale(scale, scale);
	
	float particlePointSize = getRenderRadius();

	glPointSize(particlePointSize);
	mesh.draw();
	glPointSize(1);
	
	ofPopMatrix();
	ofPopStyle();
	
}

//--------------------------------------------------------------
void ParticleSystem::drawConnections(ofColor color, bool withWeights) {
	
	const b2Vec2 * positions = particleSystem->GetPositionBuffer();
	
	/*
	const b2ParticleTriad * triads = particleSystem->GetTriads();
	int count = particleSystem->GetTriadCount();
	
	
	for (int i=0; i<count; i++) {
		const b2ParticleTriad contact = triads[i];
		
		ofSetColor(color);
		
		ofDrawLine(positions[contact.indexA].x * scale, positions[contact.indexA].y * scale,
				   positions[contact.indexB].x * scale, positions[contact.indexB].y * scale);
		
	}
	*/
	
	const b2ParticleContact * contacts = particleSystem->GetContacts();

	int count = particleSystem->GetContactCount();
	
	for (int i=0; i<count; i++) {
		const b2ParticleContact contact = contacts[i];
		ofSetColor(color, withWeights ? (contact.GetWeight() * 255) : 255);
		ofDrawLine(ofxBox2d::toOf(positions[contact.GetIndexA()].x), ofxBox2d::toOf(positions[contact.GetIndexA()].y),
				   ofxBox2d::toOf(positions[contact.GetIndexB()].x), ofxBox2d::toOf(positions[contact.GetIndexB()].y));
	}
}
