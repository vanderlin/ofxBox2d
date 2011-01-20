#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxVectorMath.h"

#include "ofxContourAnalysisConstants.h"
#include "polylineSimplify.h"
#include "convexHull.h"

class ofxContourAnalysis {
	
public:
	
	float width; float height;
	
	ofxContourAnalysis() {
		width = 320;
		height = 240;
	}
	
	void setSize(float w, float h) {
		width = w; height = h;
	}
	
	void draw(vector <ofPoint> &contour, float x, float y, float w, float h, bool drawDots=false) {
		
		glPushMatrix();
		glTranslatef(x, y, 0);
		
		/*ofFill();
		ofSetColor(200, 200, 200);
		ofRect(0, 0, w, h);
		
		ofNoFill();
		ofSetColor(20, 20, 20);
		ofRect(0, 0, w, h);
		*/
		
		ofBeginShape();
		for(int i=0; i<contour.size(); i++) {
			ofSetColor(0xff4400);
			ofNoFill();
			ofVertex(ofMap(contour[i].x, 0.0, width, 0.0, w),
					 ofMap(contour[i].y, 0.0, height, 0.0, h));
		}
		ofEndShape(true);
		
		if(drawDots) {
			for(int i=0; i<contour.size(); i++) {
				ofFill();
				ofSetColor(0x33ff00);
				ofCircle(ofMap(contour[i].x, 0.0, width, 0.0, w),
						 ofMap(contour[i].y, 0.0, height, 0.0, h),
						 3);
			}
		}
		glPopMatrix();
	}
	
	//------------------------------------------------------------------------ smooth the contour
	void smooth(vector <ofPoint> &contourIn, vector <ofPoint> &contourOut, float smoothPct=1.0) {
		
		int length = MIN(contourIn.size(), contourOut.size());
		float invPct = 1.0 - smoothPct;
		
		//first copy the data 
		for(int i = 0; i < length; i++){
			contourOut[i] = contourIn[i];
		}
		
		//then smooth the contour
		//we start at 1 as we take a small pct of the prev value
		for(int i = 1; i < length; i++){
			contourOut[i] = contourOut[i] * smoothPct   +   contourOut[i-1] * invPct;
		}
	}
	
	//------------------------------------------------------------------------ simplify the contour
	void simplify(vector <ofPoint> &contourIn, vector <ofPoint> &contourOut, float tolerance) {
		
		int length = contourIn.size();
		
		//the polyLine simplify class needs data as a vector of ofxPoint3fs 
		ofxPoint3f  polyLineIn[length];
		ofxPoint3f  polyLineOut[length];
		
		//first we copy over the data to a 3d point array
		for(int i = 0; i < length; i++){
			polyLineIn[i].x = contourIn[i].x;
			polyLineIn[i].y = contourIn[i].y;
		}
		
		int numPoints = poly_simplify(tolerance, polyLineIn, length, polyLineOut);
		contourOut.clear();
		contourOut.assign(numPoints, ofPoint());
		
		//copy the data to our contourOut vector
		for(int i = 0; i < numPoints; i++){
			contourOut[i].x = polyLineOut[i].x;
			contourOut[i].y = polyLineOut[i].y;
		}
		
	}
	
	//------------------------------------------------------------------------ convexhull 
	void convexHull(vector <ofPoint> &contourIn, vector <ofPoint> &contourOut){
		
		int numPtsIn = contourIn.size();
		
		vector <hPoint> hullPointsIn;
		hullPointsIn.clear();
		hullPointsIn.assign(numPtsIn, hPoint());
		
		for(int i = 0; i < numPtsIn; i++){
			hullPointsIn[i].x = contourIn[i].x;
			hullPointsIn[i].y = contourIn[i].y;
		}
		
		vector <hPoint> hullPointsOut = calcConvexHull(hullPointsIn);
		int numOut = hullPointsOut.size();
		
		contourOut.clear();
		contourOut.assign(numOut, ofPoint());
		
		for(int i = 0; i < numOut; i++){
			contourOut[i].x = hullPointsOut[i].x;
			contourOut[i].y = hullPointsOut[i].y;
		}
	}
	
};


















