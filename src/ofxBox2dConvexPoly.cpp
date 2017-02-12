/*
 *  ofxBox2dConvexPoly.cpp
 *  ofxBox2dExample
 *
 *  Created by Todd Vanderlin on 3/7/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#include "ofxBox2dConvexPoly.h"
#include "ofxBox2dPolygon.h"





ofPolyline convexHull(ofPolyline & line){
    
    vector < hPoint > ptsIn;
    for (int i = 0; i < line.getVertices().size(); i++){
        hPoint pt;
        pt.x = line.getVertices()[i].x;
        pt.y = line.getVertices()[i].y;
        
        ptsIn.push_back(pt);
    }
    vector < hPoint > ptsOut;
    
    ptsOut =  calcConvexHull(ptsIn);
    
    ofPolyline out;
    
    for (int i = 0; i < ptsOut.size(); i++){
        out.addVertex(ofPoint(ptsOut[i].x, ptsOut[i].y));
    }
    
    return out;
    
}


////----------------------------------------
//ofxBox2dConvexPoly::~ofxBox2dPolygon() { 
//}
//
////----------------------------------------
//void ofxBox2dConvexPoly::destroy() {
//	ofxBox2dBaseShape::destroy();
//	//clear();
//}



//------------------------------------------------
ofxBox2dConvexPoly::ofxBox2dConvexPoly() {
}

//------------------------------------------------
void ofxBox2dConvexPoly::setup(b2World * b2dworld, ofPolyline & _line){

    ofPolyline line = convexHull(_line);
    line.getVertices().erase(line.getVertices().end()-1);
    
    
    b2Vec2 * vertices;
    int32  vertexCount = line.getVertices().size();
    vertices = new b2Vec2[vertexCount];
    ofPoint pos;
    ghettoRadius = 0;
    for (int i = 0; i < vertexCount; i++){
        vertices[i].x = line.getVertices()[i].x;
        vertices[i].y = line.getVertices()[i].y;
        pos.x += line.getVertices()[i].x;
        pos.y += line.getVertices()[i].y;
        
    }
    pos /= (float)vertexCount;
    
    for (int i = 0; i < vertexCount; i++){
        float dist = (pos - line.getVertices()[i]).length();
        if (dist > ghettoRadius){
            ghettoRadius = dist;
        }
    }

    
    for (int i = 0; i < vertexCount; i++){
        vertices[i].x /= OFX_BOX2D_SCALE;
        vertices[i].y /= OFX_BOX2D_SCALE;
    }
    
    ofPoint posCent = ofPoint(200,200) - pos;
    
    pos /= OFX_BOX2D_SCALE;
    posCent /= OFX_BOX2D_SCALE;
    ghettoRadius    /= OFX_BOX2D_SCALE;

	ofPath path;
	for (int i = 0; i < vertexCount; i++){
		vertices[i].x -= pos.x;
		vertices[i].y -= pos.y;
		ofDefaultVertexType cur(vertices[i].x, vertices[i].y, 0);
		polyPts.addVertex(cur);
		path.lineTo(cur);
	}
	gpuCachedTesselation = path.getTessellation();
	
    
    float x = pos.x ;
    float y = pos.y ;
	
	if(b2dworld == NULL) {
		ofLog(OF_LOG_NOTICE, "ofxBox2dConvexPoly :: setup : - must have a valid world -");
		return;
	}
	
	// these are used to create the shape
	
	shape.Set(vertices, vertexCount);
    
    delete vertices;
    
	fixture.shape		= &shape;
	fixture.density		= density;
	fixture.friction	= friction;
	fixture.restitution	= bounce;
	
	if(density == 0.f)	bodyDef.type	= b2_staticBody;
	else				bodyDef.type	= b2_dynamicBody;
	
	bodyDef.position.Set(x,y);
	
	body  = b2dworld->CreateBody(&bodyDef);
	body->CreateFixture(&fixture);
    
    scale = 1;
}

//------------------------------------------------
void ofxBox2dConvexPoly::setScale(float _scale){
    
    
    if(!isBody()) return;
    
    b2Fixture* fix = body->GetFixtureList();
    
    scale = _scale;
    
    b2PolygonShape* shape = (b2PolygonShape*) fix->GetShape();
    
    for (int i = 0; i < polyPts.size(); i++){
        shape->m_vertices[i].Set(polyPts[i].x*scale, polyPts[i].y*scale); 
    }
    
    
}



void ofxBox2dConvexPoly::addAttractionPoint (ofVec2f pt, float amt) {
    // we apply forces at each vertex. 
    if(body != NULL) {
        const b2Transform& xf = body->GetTransform();
		
        for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
            b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();
            
            if(poly) {
                b2Vec2 P(pt.x/OFX_BOX2D_SCALE, pt.y/OFX_BOX2D_SCALE);
                
                for(int i=0; i<poly->GetVertexCount(); i++) {
                    b2Vec2 qt = b2Mul(xf, poly->GetVertex(i));
                    b2Vec2 D = P - qt; 
                    b2Vec2 F = amt * D;
                    body->ApplyForce(F, P, true);
                }                    
            }
        }
    }
}


//----------------------------------------
void ofxBox2dConvexPoly::addAttractionPoint (float x, float y, float amt) {
    addAttractionPoint(ofVec2f(x, y), amt);
}

//----------------------------------------
void ofxBox2dConvexPoly::addRepulsionForce(float x, float y, float amt) {
	addRepulsionForce(ofVec2f(x, y), amt);
}
void ofxBox2dConvexPoly::addRepulsionForce(ofVec2f pt, float amt) {
	// we apply forces at each vertex. 
    if(body != NULL) {
        const b2Transform& xf = body->GetTransform();
		
        for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
            b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();
            
            if(poly) {
                b2Vec2 P(pt.x/OFX_BOX2D_SCALE, pt.y/OFX_BOX2D_SCALE);
                
                for(int i=0; i<poly->GetVertexCount(); i++) {
                    b2Vec2 qt = b2Mul(xf, poly->GetVertex(i));
                    b2Vec2 D = P - qt; 
                    b2Vec2 F = amt * D;
                    body->ApplyForce(-F, P, true);
                }                    
            }
        }
    }
}




//------------------------------------------------
void ofxBox2dConvexPoly::draw() {
	if(!isBody()) return;
	
	ofPushMatrix();
	ofTranslate(getPosition().x, getPosition().y, 0);
	ofRotateDeg(getRotation(), 0, 0, 1);
	ofScale(OFX_BOX2D_SCALE*scale, OFX_BOX2D_SCALE*scale);
	gpuCachedTesselation.draw();
	ofPopMatrix();
}

