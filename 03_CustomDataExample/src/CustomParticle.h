//
//  CustomParticle.h
//  CustomDataExample
//
//  Created by Jeffrey Crouse on 11/21/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"

// A simple little Data class. This is were
// you can store anything you want.
class Data {
public:
	ofColor color;
	string  name;
	int		id;
};


// A Custom Particle extending the box2d circle
class CustomParticle : public ofxBox2dCircle {
public:
	void setupTheCustomData();
	void draw();
};
