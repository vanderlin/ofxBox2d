/*
 *  ofxBox2dPolygonUtils.h
 *  ofxBox2d
 *
 *  Created by Todd Vanderlin on 1/18/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"
#include "del_interface.hpp"
#include <algorithm>

using namespace tpp;


//This is for polygon/contour simplification - we use it to reduce the number of points needed in
//representing the letters as openGL shapes - will soon be moved to ofGraphics.cpp

// From: http://softsurfer.com/Archive/algorithm_0205/algorithm_0205.htm
// Copyright 2002, softSurfer (www.softsurfer.com)
// This code may be freely used and modified for any purpose
// providing that this copyright notice is included with it.
// SoftSurfer makes no warranty for this code, and cannot be held
// liable for any real or imagined damage resulting from its use.
// Users of this code must verify correctness for their application.

typedef struct{
	ofVec2f P0;
	ofVec2f P1;
}Segment;

typedef struct {
	ofVec2f a,b,c;	
} TriangleShape;

// dot product (3D) which allows vector operations in arguments
//#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y + (u).z * (v).z)
#define norm2(v)   dot(v,v)        // norm2 = squared length of vector
#define norm(v)    sqrt(norm2(v))  // norm = length of vector
#define d2(u,v)    norm2(u-v)      // distance squared = norm2 of difference
#define d(u,v)     norm(u-v)       // distance = norm of difference

static void simplifyDP(float tol, ofVec2f* v, int j, int k, int* mk ){
    if (k <= j+1) // there is nothing to simplify
        return;
	
    // check for adequate approximation by segment S from v[j] to v[k]
    int     maxi	= j;          // index of vertex farthest from S
    float   maxd2	= 0;         // distance squared of farthest vertex
    float   tol2	= tol * tol;  // tolerance squared
    Segment S		= {v[j], v[k]};  // segment from v[j] to v[k]
    ofVec2f u;
	u				= S.P1 - S.P0;   // segment direction vector
    double  cu		= u.dot(u);// dot(u,u);     // segment length squared
	
    // test each vertex v[i] for max distance from S
    // compute using the Feb 2001 Algorithm's dist_ofPoint_to_Segment()
    // Note: this works in any dimension (2D, 3D, ...)
    ofVec2f  w;
    ofVec2f   Pb;                // base of perpendicular from v[i] to S
    float  b, cw, dv2;        // dv2 = distance v[i] to S squared
	
    for (int i=j+1; i<k; i++){
        // compute distance squared
        w = v[i] - S.P0;
        cw = w.dot(u);
        if ( cw <= 0 ) dv2 = v[i].distanceSquared( S.P0 );
        else if ( cu <= cw ) dv2 = v[i].distanceSquared( S.P1 );
        else {
            b = (float)(cw / cu);
            Pb = S.P0 + u*b;
            dv2 = v[i].distanceSquared( Pb );
        }
        // test with current max distance squared
        if (dv2 <= maxd2) continue;
		
        // v[i] is a new max vertex
        maxi = i;
        maxd2 = dv2;
    }
    if (maxd2 > tol2)        // error is worse than the tolerance
    {
        // split the polyline at the farthest vertex from S
        mk[maxi] = 1;      // mark v[maxi] for the simplified polyline
        // recursively simplify the two subpolylines at v[maxi]
        simplifyDP( tol, v, j, maxi, mk );  // polyline v[j] to v[maxi]
        simplifyDP( tol, v, maxi, k, mk );  // polyline v[maxi] to v[k]
    }
    // else the approximation is OK, so ignore intermediate vertices
    return;
}


//-------------------------------------------------------------------
// needs simplifyDP which is above
static vector <ofVec2f> simplifyContour(vector <ofVec2f> &V, float tol) {
	
	int n = V.size();
	vector <ofVec2f> sV;
	if(n <= 2) return sV;
	sV.assign(n, ofVec2f());
	
    int    i, k, m, pv;            // misc counters
    float  tol2 = tol * tol;       // tolerance squared
    ofVec2f * vt = new ofVec2f[n];
	int * mk = new int[n];
	
	memset(mk, 0, sizeof(int) * n );
	
    // STAGE 1.  Vertex Reduction within tolerance of prior vertex cluster
    vt[0] = V[0];              // start at the beginning
    for (i=k=1, pv=0; i<n; i++) {
        if (V[i].distanceSquared( V[pv] ) < tol2) continue;
		
        vt[k++] = V[i];
        pv = i;
    }
    if (pv < n-1) vt[k++] = V[n-1];      // finish at the end
	
    // STAGE 2.  Douglas-Peucker polyline simplification
    mk[0] = mk[k-1] = 1;       // mark the first and last vertices
    simplifyDP( tol, vt, 0, k-1, mk );
	
    // copy marked vertices to the output simplified polyline
    for (i=m=0; i<k; i++) {
        if (mk[i]) sV[m++] = vt[i];
    }
	
	//get rid of the unused points
	if( m < (int)sV.size() ) sV.erase( sV.begin()+m, sV.end() );
	
	delete [] vt;
	delete [] mk;
	
	return sV;
}

//-------------------------------------------------------------------
static bool insidePolygon(const ofVec2f & p, const vector<ofVec2f> & polygon){
	if(polygon.size() < 1) return false;
	int counter = 0;
	int i;
	double xinters;
	ofVec2f p1,p2;
	float x = p.x; 
	float y = p.y;
	int N = polygon.size();
	
	p1 = polygon[0];
	for (i=1;i<=N;i++) {
		p2 = polygon[i % N];
		if (y > MIN(p1.y,p2.y)) {
			if (y <= MAX(p1.y,p2.y)) {
				if (x <= MAX(p1.x,p2.x)) {
					if (p1.y != p2.y) {
						xinters = (y-p1.y)*(p2.x-p1.x)/(p2.y-p1.y)+p1.x;
						if (p1.x == p2.x || x <= xinters)
							counter++;
					}
				}
			}
		}
		p1 = p2;
	}
	
	if (counter % 2 == 0) return false;
	else return true;
}

//-------------------------------------------------------------------
static bool insidePolygon(const ofVec2f & p, const ofPolyline &polygon){
	if(polygon.size() < 1) return false;
	int counter = 0;
	int i;
	double xinters;
	ofVec2f p1,p2;
	float x = p.x; 
	float y = p.y;
	int N = polygon.size();
	
	p1 = polygon[0];
	for (i=1;i<=N;i++) {
		p2 = polygon[i % N];
		if (y > MIN(p1.y,p2.y)) {
			if (y <= MAX(p1.y,p2.y)) {
				if (x <= MAX(p1.x,p2.x)) {
					if (p1.y != p2.y) {
						xinters = (y-p1.y)*(p2.x-p1.x)/(p2.y-p1.y)+p1.x;
						if (p1.x == p2.x || x <= xinters)
							counter++;
					}
				}
			}
		}
		p1 = p2;
	}
	
	if (counter % 2 == 0) return false;
	else return true;
}

//-------------------------------------------------------------------
static float getTriangleRadius(ofVec2f v1, ofVec2f v2, ofVec2f v3) {
    
    float a = v1.distance(v2);
    float b = v2.distance(v3);
    float c = v3.distance(v1);
    
    float k = 0.5 * (a+b+c);
    
    float r = sqrt( k * ((k-a)*(k-b)*(k-c)) ) / k;
    return r;
}

//-------------------------------------------------------------------
static ofVec2f getTriangleCenter(ofVec2f v1, ofVec2f v2, ofVec2f v3) {
    
    float a = v2.distance(v3);
    float b = v1.distance(v3);
    float c = v1.distance(v2);
    float d = a+b+c;
    
    float ix = ((a * v1.x) + (b * v2.x) + (c * v3.x)) / d;
    float iy = ((a * v1.y) + (b * v2.y) + (c * v3.y)) / d;
    
    return ofVec2f(ix, iy);
}


//-------------------------------------------------------------------
static float getArea(const vector <ofVec2f> pts) {
	int i, j, n = pts.size();
	float polyArea = 0;
	for (i = 0; i < n; i++) {
		j = (i + 1) % n;
		polyArea += pts[i].x * pts[j].y;
		polyArea -= pts[j].x * pts[i].y;
	}
	polyArea /= 2.0;
	return polyArea;
}

//-------------------------------------------------------------------
static float getTriangleArea(ofVec2f &a, ofVec2f &b, ofVec2f &c) {
	vector <ofVec2f> pts;
	pts.push_back(a);
	pts.push_back(b);
	pts.push_back(c);
	int i, j, n = pts.size();
	float polyArea = 0;
	for (i = 0; i < n; i++) {
		j = (i + 1) % n;
		polyArea += pts[i].x * pts[j].y;
		polyArea -= pts[j].x * pts[i].y;
	}
	polyArea /= 2.0;
	return polyArea;
}

//-------------------------------------------------------------------
static ofRectangle getPolygonBounds(const vector <ofVec2f> & vertices) {
	ofRectangle bounds;
	bounds.x        = 100000;
	bounds.y	    = 100000;
	bounds.width    = 0;
	bounds.height   = 0;
	
	float farRight  = -100000;
	float bottom    = -100000;
	
	for (int i=0; i<vertices.size(); i++) {
		if( vertices[i].x < bounds.x ) bounds.x = vertices[i].x;	
		if( vertices[i].y < bounds.y ) bounds.y = vertices[i].y;
		if( vertices[i].x > farRight ) farRight = vertices[i].x; 
		if( vertices[i].y > bottom   ) bottom   = vertices[i].y; 
	}
	
	bounds.width  = ABS(farRight - bounds.x);
	bounds.height = ABS(bottom - bounds.y); 
	return bounds;
}

//-------------------------------------------------------------------
static void addRandomPointsInside(ofPolyline &poly, int amt=100) {
	
	ofRectangle bounds = poly.getBoundingBox();
	
	for (int i=0; i<amt; i++) {
		float x = ofRandom(bounds.x, bounds.x+bounds.width);
		float y = ofRandom(bounds.y, bounds.y+bounds.height);
		if( insidePolygon(ofVec2f(x, y), poly) ) poly.addVertex(x, y);
	}
}

//-------------------------------------------------------------------
static void addRandomPointsInside(vector <ofVec2f> & vertices, int amt=100) {
	
	ofRectangle bounds = getPolygonBounds(vertices);
	
	for (int i=0; i<amt; i++) {
		float x = ofRandom(bounds.x, bounds.x+bounds.width);
		float y = ofRandom(bounds.y, bounds.y+bounds.height);
		if(insidePolygon(ofVec2f(x, y), vertices)) vertices.push_back(ofVec2f(x, y));
	}
}


//-------------------------------------------------------------------
static vector <TriangleShape> triangulatePolygonWithOutline(const ofPolyline &pts, 
															const ofPolyline &polyOutline) {
	
	vector <TriangleShape> triangles;
	
	if(pts.size() < 3 || polyOutline.size() < 3) return triangles;
	
	// now triangluate from the polyline
	vector <Delaunay::Point>	delaunayPts;
	Delaunay::Point				tempP;
	for(int i=0; i<pts.size(); i++) {
		tempP[0] = pts[i].x;
		tempP[1] = pts[i].y;
		delaunayPts.push_back(tempP);
	}
	
	Delaunay delobject(delaunayPts);
	delobject.Triangulate();
	
	// get all the triangles
	for(Delaunay::fIterator fit=delobject.fbegin(); fit!=delobject.fend(); ++fit) {
		
		float triArea = delobject.area(fit);
		
		int pta   = delobject.Org(fit);
		int ptb   = delobject.Dest(fit);
		int ptc   = delobject.Apex(fit);
		
		if(pta == -1 || ptb == -1 || ptc == -1) {
			printf("don't have a triangle man!\n");
			continue;
		}
		
		ofVec2f a = pts[pta];
		ofVec2f b = pts[ptb];
		ofVec2f c = pts[ptc];
		
		ofVec2f center = getTriangleCenter(a, b, c);
		
		float disA  = 5;//ofDist(center.x, center.y, a.x, a.y);
		float disB  = 5;//ofDist(center.x, center.y, b.x, b.y);
		float disC  = 5;//ofDist(center.x, center.y, c.x, c.y);
		
        /*
		if(triArea < 1.0 || 
		   disA < 2.0 ||
		   disB < 2.0 ||
		   disC < 2.0) {
			continue;
		}
		if(!insidePolygon(center, polyOutline)) {
			continue;	
		}*/
		
		TriangleShape shape;
		shape.a = a;
		shape.b = b;
		shape.c = c;
		triangles.push_back(shape);
		
	}	
	
	return triangles;
}


//-------------------------------------------------------------------
static vector <TriangleShape> triangulatePolygon(const vector <ofVec2f> &ptsIn, bool addPointsInside=false, int amt=100) {
	
	vector <ofVec2f> pts;
	vector <TriangleShape> triangles;

	if(ptsIn.size() < 3) return triangles;

	
	pts = ptsIn;
	
	if(addPointsInside) {
		addRandomPointsInside(pts, amt);
	}
	
	// now triangluate from the polyline (3)
	vector <Delaunay::Point>	delaunayPts;
	Delaunay::Point				tempP;
	for(int i=0; i<pts.size(); i++) {
		tempP[0] = pts[i].x;
		tempP[1] = pts[i].y;
		delaunayPts.push_back(tempP);
	}
	
	Delaunay delobject(delaunayPts);
	delobject.Triangulate();
	
	// get all the triangles
	for(Delaunay::fIterator fit=delobject.fbegin(); fit!=delobject.fend(); ++fit) {
		
		float triArea = delobject.area(fit);
		
		int pta   = delobject.Org(fit);
		int ptb   = delobject.Dest(fit);
		int ptc   = delobject.Apex(fit);
		
		if(pta == -1 || ptb == -1 || ptc == -1) {
			printf("don't have a triangle man!\n");
			continue;
		}
		
		ofVec2f a = pts[pta];
		ofVec2f b = pts[ptb];
		ofVec2f c = pts[ptc];
		
		ofVec2f center = getTriangleCenter(a, b, c);
		
		float disA  = 5;//ofDist(center.x, center.y, a.x, a.y);
		float disB  = 5;//ofDist(center.x, center.y, b.x, b.y);
		float disC  = 5;//ofDist(center.x, center.y, c.x, c.y);
		
		if(triArea < 1.0 || 
		   disA < 2.0 ||
		   disB < 2.0 ||
		   disC < 2.0) {
			continue;
		}
		if(!insidePolygon(center, pts)) {
			continue;	
		}
		
		TriangleShape shape;
		shape.a = a;
		shape.b = b;
		shape.c = c;
		triangles.push_back(shape);
		
	}	
	
	return triangles;
}

//-------------------------------------------------------------------
static vector <TriangleShape> triangulatePolygon(const ofPolyline &poly, bool addPointsInside=false, int amt=100) {
	vector <ofVec2f> pts;
	for (int i=0; i<poly.size(); i++) {
		pts.push_back(poly[i]);
	}
	return triangulatePolygon(pts, addPointsInside, amt);
}


// Implementation of Monotone Chain Convex Hull algorithm.
//-------------------------------------------------------------------

typedef long long CoordType;

struct hPoint {
	CoordType x, y;
	
	bool operator <(const hPoint &p) const {
		return x < p.x || (x == p.x && y < p.y);
	}
};

// 2D cross product.
// Return a positive value, if OAB makes a counter-clockwise turn,
// negative for clockwise turn, and zero if the points are collinear.
static CoordType cross(const hPoint &O, const hPoint &A, const hPoint &B)
{
	return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

// Returns a list of points on the convex hull in counter-clockwise order.
// Note: the last point in the returned list is the same as the first one.
static vector<hPoint> calcConvexHull(vector<hPoint> P) {
	int n = P.size(), k = 0;
	vector<hPoint> H(2*n);
	
	// Sort points lexicographically
	sort(P.begin(), P.end());
	
	// Build lower hull
	for (int i = 0; i < n; i++) {
		while (k >= 2 && cross(H[k-2], H[k-1], P[i]) <= 0) k--;
		H[k++] = P[i];
	}
	
	// Build upper hull
	for (int i = n-2, t = k+1; i >= 0; i--) {
		while (k >= t && cross(H[k-2], H[k-1], P[i]) <= 0) k--;
		H[k++] = P[i];
	}
	
	H.resize(k);
	return H;
}