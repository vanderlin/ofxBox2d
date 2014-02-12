ofxBox2d
=====================================

![ofxBox2d](http://farm7.staticflickr.com/6010/5964216482_a11debc021_b.jpg)

Introduction
------------
This is a simple wrapper for box2d using Openframeworks. The examples below are still in progressive, but should be stable for the most part. Please open up a issue if you have suggestions or find bugs. The wrapper is using the version Box2D v2.3

Thanks,
Todd

Instructions
------------
When making a vector of objects you need to be careful. You either need to make a vector of pointers of use the `ofPtr` object.     
    
Everytime you push into the vector `circles` the object is destroyed and the created.
This causing issues for the `b2dBody body` object owned by box2d.       
  
        
***Incorrect way to store objects.***         
```
vector <ofxBox2dCircle> circles;
ofxBox2dCircle circle;
circles.push_back(circle);
```

***Here is the how to create a vector of box2d objects.***   
```
// in your header files
vector <ofPtr<ofxBox2dCircle> > circles;


// now add a circle to the vector
ofPtr<ofxBox2dCircle> circle = ofPtr<ofxBox2dCircle>(new ofxBox2dCircle);

// to grab the pointer you use the get() function of ofPtr (std::shared_ptr)
circle.get()->setPhysics(3.0, 0.53, 0.1);
circle.get()->setup(box2d.getWorld(), 100, 100, 10);
circles.push_back(circle);
```

Installation
------------
Place ofxBox2d within a folder in the apps folder of the OF dir tree:
<pre>
openframeworks/addons/ofxBox2d
</pre>

Compatibility
------------
ofxBox2d is developed against the current version of OpenFramewroks.

If you are using a stable version (007, 0071, ...) of OpenFrameworks then you want to use a git tag of ofxBox2d for that version. You can select the tag in the Github "Current Branch" menu or clone and check it out using git.

For example, the following commands will clone ofxBox2d and switch to the OF 008 tagged version:
<pre>
git clone git://github.com/vanderlin/ofxBox2d.git
cd ofxBox2d
git checkout 008
</pre>



