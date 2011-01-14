

#pragma once
#include "ofMain.h"
#include "ofxBox2dBaseShape.h"

class ofxBox2dLine : public ofxBox2dBaseShape {
	
public:
	
	//b2EdgeChainDef		edgeDef;
	//b2PolygonDef		strip;
	vector <ofPoint>	points;
	bool				bShapeCreated;
	bool				bIsLoop;
	
	//------------------------------------------------
	
	ofxBox2dLine() {
		bShapeCreated = false;
		bIsLoop		  = false;
	}
	
	//------------------------------------------------
	void setup(b2World * b2dworld, const ofPoint &p1, const ofPoint &p2) {
		
		if(b2dworld == NULL) {
			ofLog(OF_LOG_NOTICE, "ofxBox2dLine :: setup : - must have a valid world -");
			return;
		}
		/*
		// the line shape
		b2PolygonShape		shape;
		
		//shape.m_p.SetZero();
		shape.m_radius = 0.0f;
		shape.SetAsEdge(b2Vec2(p1.x/OFX_BOX2D_SCALE, p1.y/OFX_BOX2D_SCALE), 
						b2Vec2(p2.x/OFX_BOX2D_SCALE, p2.y/OFX_BOX2D_SCALE));
		
		
		//bodyDef.position.Set(0.0f, 2.0f);   // the body's origin position.
		
		// set the properties 
		fixture.shape	 = &shape;
		fixture.density  = density;
		fixture.restitution = bounce;
		fixture.friction = friction;
		
		// set the type of body
		if(density == 0.f) {
			bodyDef.type = b2_staticBody;
		} else {
			bodyDef.type = b2_dynamicBody;
		}
		
		// set the world and create the body
		body = b2dworld->CreateBody(&bodyDef);
		body->CreateFixture(&shape, density);
		 */
		
	}
	
	//------------------------------------------------
	void clear() {
		/*
		if(bShapeCreated) {
			for(int i=0; i<points.size(); i++) {
				points.erase(points.begin() + i);
			}
			points.clear();
			destroy();
			dead = false;
		}
		 */
	}
	
	//------------------------------------------------
	void create() {
		/*
		if(!getWorld()) {
			ofLog(OF_LOG_NOTICE, "ofxBox2dLine :: create : - must have a valid world -");
			return;
		}
		
		int numPoints = points.size();
		if(numPoints <= 0) return;
		*/
		/*
		b2Vec2 pts[numPoints];
		for(int i=0; i<numPoints; i++) {
			pts[i].Set(points[i].x/OFX_BOX2D_SCALE, points[i].y/OFX_BOX2D_SCALE);
		}	
		
		
		bodyDef.position.Set(0, 0);//b2dNum(ofGetWidth()/2), b2dNum(ofGetHeight()/2));
		body = world->CreateBody(&bodyDef);
		*/
		//bodyDef.position.Set(x/OFX_BOX2D_SCALE, y/OFX_BOX2D_SCALE);	
		
		
		//body = world->CreateBody(&bodyDef);
		/*
		ofPoint a,b;
		a = points[0];
		for(int i = 1; i < points.size(); ++i) {
			b = points[i];
			b2PolygonShape shape;			
			shape.SetAsEdge(
				 b2Vec2(a.x/OFX_BOX2D_SCALE, a.y/OFX_BOX2D_SCALE)
				,b2Vec2(b.x/OFX_BOX2D_SCALE, b.y/OFX_BOX2D_SCALE)
			);
			a = b;
			body->CreateFixture(&shape,0.0f);
			//shape.m_vertices[i].Set(points[i].x/OFX_BOX2D_SCALE, points[i].y/OFX_BOX2D_SCALE);
			//b2PolygonShape shape;
			//shape.SetAsEdge(b2Vec2(x1/OFX_BOX2D_SCALE, y1/OFX_BOX2D_SCALE), b2Vec2(x2/OFX_BOX2D_SCALE, y2/OFX_BOX2D_SCALE));
		}
		 */
//		shape.setAs
		 //
		
		/*
		 b2CircleDef weight;
		 weight.filter.maskBits = 0x0000;
		 weight.density = 4.0f;
		 weight.radius = 0.5f;
		 weight.localPosition.Set(0, 0);
		 body->CreateShape(&weight);
		 */
		//type = e_unknownShape;
		//userData = NULL;
		//edgeDef.friction = 0.2f;
		//edgeDef.restitution = 0.96f;
		//edgeDef.density = 1.0f;
		//filter.categoryBits = 0x0001;
		//filter.maskBits = 0xFFFF;
		//filter.groupIndex = 0;
		//isSensor = false;
		
		//edgeDef.vertexCount = numPoints;
		//edgeDef.vertices = pts;
		//edgeDef.isALoop  = bIsLoop;
		//body->CreateShape(&edgeDef);
		
		//body->SetMassFromShapes();
		
		//bShapeCreated = true;
		
		// anything that you need called
		//init();
	}
	
	
	
	
	/*
	 //------------------------------------------------
	 float getRadius() {
	 b2Shape* shape		= body->GetShapeList();
	 b2CircleShape *data = (b2CircleShape*)shape;
	 return data->GetRadius() * OFX_BOX2D_SCALE;
	 }
	 
	 //------------------------------------------------
	 float getRotation() {
	 
	 const  b2XForm& xf	= body->GetXForm();
	 float  r			= getRadius()/OFX_BOX2D_SCALE;
	 b2Vec2 a			= xf.R.col1;
	 b2Vec2 p1			= body->GetPosition();
	 b2Vec2 p2			= p1 + r * a;
	 
	 float dx = p2.x+r/2 - p1.x+r/2;
	 float dy = p2.y - p1.y;
	 return ofRadToDeg(atan2(dy, dx));
	 
	 }
	 */
	
	
	//------------------------------------------------
	void draw() {
		/*
		if(body == NULL) return;
		
		const b2Transform& xf = body->GetTransform();
		for (b2Fixture* f = body->GetFixtureList(); f; f = f->GetNext()) {
			b2PolygonShape* poly = (b2PolygonShape*)f->GetShape();
			//
			//			
			//			b2PolygonShape* poly = (b2PolygonShape*)fixture->GetShape();
			//			int32 vertexCount = poly->m_vertexCount;
			//			b2Assert(vertexCount <= b2_maxPolygonVertices);
			//			b2Vec2 vertices[b2_maxPolygonVertices];
			//			
			//			for (int32 i = 0; i < vertexCount; ++i)
			//			{
			//				vertices[i] = b2Mul(xf, poly->m_vertices[i]);
			//			}
			//			
			//			m_debugDraw->DrawSolidPolygon(vertices, vertexCount, color);
			//			
			
			printf("%f\n", body->GetAngle());
			if(poly) {
				ofNoFill();
				ofSetColor(120, 120, 120);
				ofBeginShape();
				for(int i=0; i<poly->m_vertexCount; i++) {
					b2Vec2 pt = b2Mul(xf, poly->m_vertices[i]);
					ofVertex(pt.x*OFX_BOX2D_SCALE, pt.y*OFX_BOX2D_SCALE);
				}
				ofEndShape(true);
			}
		}
		*/
		// TODO: can all this code be removed??
		/*
		 if(dead) return;
		 
		 //wow this is a pain
		 b2Shape* s = body->GetShapeList();
		 const b2XForm& xf = body->GetXForm();
		 b2PolygonShape* poly = (b2PolygonShape*)s;
		 int count = poly->GetVertexCount();
		 const b2Vec2* localVertices = poly->GetVertices();
		 b2Assert(count <= b2_maxPolygonVertices);
		 b2Vec2 verts[b2_maxPolygonVertices];
		 for(int32 i=0; i<count; ++i) {
		 verts[i] = b2Mul(xf, localVertices[i]);
		 }
		 
		 
		 ofSetColor(0, 255, 255);
		 glBegin(GL_LINE_LOOP);
		 for (int32 i = 0; i <count; i++) {
		 printf("%i",  i);
		 glVertex2f(verts[i].x*OFX_BOX2D_SCALE, verts[i].y*OFX_BOX2D_SCALE);
		 }
		 glEnd();*/
		
		
		
		// TODO: check this in draw
		/*
		int32 count = shape.m_vertexCount;
		const b2Transform& xf = body->GetTransform();
		b2Assert(count <= b2_maxPolygonVertices);
		b2Vec2 verts[count];
		for (int32 i = 0; i < count; ++i) {
			verts[i] = b2Mul(xf, shape.m_vertices[i]);
		}
		*/
		//
		
		
		
		/*
		ofBeginShape();
		ofSetColor(255, 0, 255);
		ofNoFill();
		for(int i=0; i<points.size(); i++) {
			ofVertex(points[i].x, points[i].y);
		}
		ofEndShape();
		 */
	}
	
};














