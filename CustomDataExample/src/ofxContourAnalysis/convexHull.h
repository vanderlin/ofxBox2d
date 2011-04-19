#pragma once

#include "ofxVectorMath.h"
#include "ofMain.h"

// Implementation of Monotone Chain Convex Hull algorithm.
#include <algorithm>
 
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
static vector<hPoint> calcConvexHull(vector<hPoint> P)
{
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