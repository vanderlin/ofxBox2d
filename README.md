ofxBox2d
=====================================

![ofxBox2d](http://farm7.staticflickr.com/6010/5964216482_a11debc021_b.jpg)

Introduction
------------
This is a simple wrapper for Box2d using Openframeworks. The examples below are still in progress, but should be stable for the most part. Please open up a issue if you have suggestions or find bugs. The wrapper is using Box2d version 2.3.

Thanks,
Todd

Instructions
------------

When making a vector of objects you need to be careful. You either need to make a vector of pointers of use the `shared_ptr` object.     
    
Everytime you push into the vector `circles` the object is destroyed and the created.
This causing issues for the `b2dBody body` object owned by Box2d.       
  
***Incorrect way to store objects.***   

```
vector <ofxBox2dCircle> circles;
ofxBox2dCircle circle;
circles.push_back(circle);
```

***Here is the how to create a vector of Box2d objects.***   

```
// in your header files
vector <shared_ptr<ofxBox2dCircle> > circles;

// now add a circle to the vector
shared_ptr<ofxBox2dCircle> circle = shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle);

// to grab the pointer you use the get() function of shared_ptr (std::shared_ptr)
circle.get()->setPhysics(3.0, 0.53, 0.1);
circle.get()->setup(box2d.getWorld(), 100, 100, 10);
circles.push_back(circle);
```

Installation
------------

Place ofxBox2d within a folder in the apps folder of the OF dir tree:

    openframeworks/addons/ofxBox2d

Compatibility
------------

The `master` branch is compatible with [the current release of openFrameworks](http://openframeworks.cc/download), and the `develop` branch tries to stay up to date with openFrameworks on GitHub.
