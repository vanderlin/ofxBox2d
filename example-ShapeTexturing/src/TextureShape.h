//
//  TextureShape.h
//  example-ShapeTexturing
//
//  Created by Todd Vanderlin on 7/16/13.
//
//

#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"

class TextureShape : public ofxBox2dPolygon {
  
public:
    
    shared_ptr<ofTexture>      texture;
    ofMesh                     mesh;
    
    TextureShape(b2World * world, shared_ptr<ofTexture> tex, float cx, float cy, float r) {
        
        texture = tex;
       
        mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        
        int   nPts  = ofRandom(3, 10);
        float step = TWO_PI / float(nPts-1);
        float angle = 0;
        for (int i=0; i<nPts; i++) {
            
            float x = cos(angle);
            float y = sin(angle);
           
            float px = (x * r);
            float py = (y * r);
            
            auto tc = texture->getCoordFromPoint(px, py);
            
            addVertex(cx + px, cy + py);
        
            mesh.addVertex({0, 0, 0});   mesh.addTexCoord({0.5, 0.5});
            mesh.addVertex({px, py, 0}); mesh.addTexCoord({tc.x, tc.y});
            
            angle += step;
        }
        
        setPhysics(0.3, 0.5, 0.1);
        create(world);
        
    }
    void draw() {
        
        ofPushMatrix();
        ofTranslate(getPosition());
        ofRotateDeg(getRotation());
        
        ofSetColor(255);
        
        texture->bind();
        mesh.draw();
        texture->unbind();
        
        ofPopMatrix();
    }
    
};
