#pragma once

#include "ofMain.h"
#include "ofxVectorMath.h"

typedef struct{

	ofxPoint3f P0;
	ofxPoint3f P1;

} Segment;


// dot product (3D) which allows vector operations in arguments
#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y + (u).z * (v).z)
#define norm2(v)   dot(v,v)        // norm2 = squared length of vector
#define norm(v)    sqrt(norm2(v))  // norm = length of vector
#define d2(u,v)    norm2(u-v)      // distance squared = norm2 of difference
#define d(u,v)     norm(u-v)       // distance = norm of difference


// simplifyDP():
//  This is the Douglas-Peucker recursive simplification routine
//  It just marks vertices that are part of the simplified polyline
//  for approximating the polyline subchain v[j] to v[k].
//    Input:  tol = approximation tolerance
//            v[] = polyline array of vertex points 
//            j,k = indices for the subchain v[j] to v[k]
//    Output: mk[] = array of markers matching vertex array v[]
static void simplifyDP( float tol, ofxPoint3f* v, int j, int k, int* mk ){
    if (k <= j+1) // there is nothing to simplify
        return;

    // check for adequate approximation by segment S from v[j] to v[k]
    int     maxi = j;          // index of vertex farthest from S
    float   maxd2 = 0;         // distance squared of farthest vertex
    float   tol2 = tol * tol;  // tolerance squared
    Segment S = {v[j], v[k]};  // segment from v[j] to v[k]
    ofxVec3f  u;
	u = S.P1 - S.P0;   // segment direction vector
    double  cu = dot(u,u);     // segment length squared

    // test each vertex v[i] for max distance from S
    // compute using the Feb 2001 Algorithm's dist_ofxPoint3f_to_Segment()
    // Note: this works in any dimension (2D, 3D, ...)
    ofxVec3f  w;
    ofxPoint3f   Pb;                // base of perpendicular from v[i] to S
    double  b, cw, dv2;        // dv2 = distance v[i] to S squared

    for (int i=j+1; i<k; i++)
    {
        // compute distance squared
        w = v[i] - S.P0;
        cw = dot(w,u);
        if ( cw <= 0 )
            dv2 = d2(v[i], S.P0);
        else if ( cu <= cw )
            dv2 = d2(v[i], S.P1);
        else {
            b = cw / cu;
            Pb = S.P0 + b * u;
            dv2 = d2(v[i], Pb);
        }
        // test with current max distance squared
        if (dv2 <= maxd2) 
            continue;
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
//===================================================================
 

// Copyright 2002, softSurfer (www.softsurfer.com)
// This code may be freely used and modified for any purpose
// providing that this copyright notice is included with it.
// SoftSurfer makes no warranty for this code, and cannot be held
// liable for any real or imagined damage resulting from its use.
// Users of this code must verify correctness for their application.

// Assume that classes are already given for the objects:
//    ofxPoint3f and Vector with
//        coordinates {float x, y, z;}    // as many as are needed
//        operators for:
//            == to test equality
//            != to test inequality
//            (Vector)0 = (0,0,0)         (null vector)
//            ofxPoint3f  = ofxPoint3f ± Vector
//            Vector = ofxPoint3f - ofxPoint3f
//            Vector = Vector ± Vector
//            Vector = Scalar * Vector    (scalar product)
//            Vector = Vector * Vector    (cross product)
//    Segment with defining endpoints {ofxPoint3f P0, P1;}
//===================================================================


// poly_simplify():
//    Input:  tol = approximation tolerance
//            V[] = polyline array of vertex points 
//            n   = the number of points in V[]
//    Output: sV[]= simplified polyline vertices (max is n)
//    Return: m   = the number of points in sV[]
static int poly_simplify( float tol, ofxPoint3f* V, int n, ofxPoint3f* sV ){
    int    i, k, m, pv;            // misc counters
    float  tol2 = tol * tol;       // tolerance squared
    ofxPoint3f* vt = new ofxPoint3f[n];      // vertex buffer
    int*   mk = new int[n];  // marker buffer
	memset(mk, 0, sizeof(int) * n );

    // STAGE 1.  Vertex Reduction within tolerance of prior vertex cluster
    vt[0] = V[0];              // start at the beginning
    for (i=k=1, pv=0; i<n; i++) {
        if (d2(V[i], V[pv]) < tol2)
            continue;
        vt[k++] = V[i];
        pv = i;
    }
    if (pv < n-1)
        vt[k++] = V[n-1];      // finish at the end

    // STAGE 2.  Douglas-Peucker polyline simplification
    mk[0] = mk[k-1] = 1;       // mark the first and last vertices
    simplifyDP( tol, vt, 0, k-1, mk );

    // copy marked vertices to the output simplified polyline
    for (i=m=0; i<k; i++) {
        if (mk[i])
            sV[m++] = vt[i];
    }
	
	vt = NULL;
	delete vt;
    
	mk = NULL;
	delete mk;
	
    return m;         // m vertices in simplified polyline
}
