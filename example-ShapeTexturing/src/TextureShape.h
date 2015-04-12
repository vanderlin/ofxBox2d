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

static int hexColors[4] = {0x31988A, 0xFDB978, 0xFF8340, 0xE8491B};

class TextureShape {
  
public:
    
    
    ofImage    *    texturePtr;
    ofMesh          mesh;
    ofColor         color;
    ofxBox2dPolygon polyShape;
    
    TextureShape() {
        texturePtr = NULL;
    }
    void setup(ofxBox2d &world, float cx, float cy, float r) {
        
        color.setHex(hexColors[(int)ofRandom(4)]);
        
        mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        int   nPts  = 8;
        float scale = r / (float)texturePtr->getWidth();
        for (int i=0; i<nPts; i++) {
            float n = ofMap(i, 0, nPts-1, 0.0, TWO_PI);
            float x = cos(n);
            float y = sin(n);
            float d = ofRandom(-r/2, r/2);
            polyShape.addVertex(ofPoint(cx + (x * r + d), cy + (y * r + d)));
            mesh.addTexCoord(ofPoint(0, 0));
            mesh.addTexCoord(ofPoint(x * scale, y * scale));
        }
        
        polyShape.setPhysics(0.3, 0.5, 0.1);
    	polyShape.create(world.getWorld());


    }
    void setTexture(ofImage * texture) {
        texturePtr = texture;
        texturePtr->getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
        texturePtr->getTextureReference().setTextureWrap(GL_REPEAT, GL_REPEAT);
    }
    void draw() {
        
        mesh.clearVertices();
        vector<ofPoint> &pts = polyShape.getPoints();
        ofPoint center       = polyShape.getCentroid2D();
        for (int i=0; i<pts.size(); i++) {
            mesh.addVertex(center);
            mesh.addVertex(pts[i]);
        }
        mesh.addVertex(center);
        mesh.addVertex(pts.front());
        
        ofSetColor(color);
        texturePtr->bind();
        mesh.draw();
        texturePtr->unbind();
   
    }
    
};