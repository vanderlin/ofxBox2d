#pragma once

#include "ofxOpenCv.h"
#include "ofxVectorMath.h"

typedef struct {
	
	ofxCvBlob		myBlob;
	vector <float>	curvePoints;
	bool			bFingerPointRuns;
	
} ContourBlob;