//
//  ofxBox2dPolygonUtils.cpp
//  app
//
//  Created by Todd Vanderlin on 10/5/17.
//

#include "ofxBox2dPolygonUtils.h"
#include "triangle.h"

void triangulatePoints(char * flags, triangulateio * in, triangulateio * mid, triangulateio * out) {
	// this funciton, which calls triangulage is because we have a function called triangulate, so the compiler get's a bit confused.
	triangulate(flags, in,  mid, out);
}


//-------------------------------------------------------------------
vector <TriangleShape> ofxBox2dPolygonUtils::triangulate(ofPolyline contour, float angleConstraint, float sizeConstraint) {
	
	vector <TriangleShape> triangles;
	vector <ofPoint> outputPts;
	
	int bSize = contour.size();
	
	struct triangulateio in, out;
	in.numberofpoints = bSize;
	in.numberofpointattributes = 0;
	in.pointmarkerlist = NULL;
	in.pointlist = (REAL *) malloc(bSize * 2 * sizeof(REAL));
	in.numberofregions = 0;
	in.regionlist =  NULL;
	
	for(int i = 0; i < bSize; i++) {
		in.pointlist[i*2+0] = contour[i].x;
		in.pointlist[i*2+1] = contour[i].y;
	}
	
	out.pointlist = (REAL *) NULL;
	out.pointattributelist = (REAL *) NULL;
	out.pointmarkerlist = (int *) NULL;
	out.trianglelist = (int *) NULL;
	out.triangleattributelist = (REAL *) NULL;
	out.neighborlist = (int *) NULL;
	out.segmentlist = (int *) NULL;
	out.segmentmarkerlist = (int *) NULL;
	out.edgelist = (int *) NULL;
	out.edgemarkerlist = (int *) NULL;
	
	
	bool bConstrainAngle = false;
	bool bConstrainSize = false;
	
	if (angleConstraint > 0) bConstrainAngle = true;
	if (sizeConstraint > 0) bConstrainSize = true;
	
	
	
	string triangulateParams;
	
	triangulateParams += "z";   // start from zero
	triangulateParams += "Y";   // Prohibits the insertion of Steiner points (extra points) on the mesh boundary.
	triangulateParams += "Q";   // quiet!   change to V is you want alot of info
	
	if (bConstrainAngle == true){
		triangulateParams += "q" + ofToString( angleConstraint );
	}
	
	if (bConstrainSize == true){
		triangulateParams += "a" + ofToString( (int)sizeConstraint );
	}
	
	
	triangulatePoints((char *) triangulateParams.c_str(), &in, &out, NULL);
	
	
	/*
	 printf("Initial triangulation:\n\n");
	 //report(&mid, 1, 1, 1, 1, 1, 0);
	 for (int i = 0; i < out.numberofpoints; i++) {
	 printf("Point %4d:", i);
	 for (int j = 0; j < 2; j++) {
	 printf("  %.6g", out.pointlist[i * 2 + j]);
	 }
	 printf("\n");
	 
	 }
	 */
	
	
	int nTriangles = 0;
	triangles.clear();
	
	
	std::map < int , ofPoint  > goodPts;
	
	for (int i = 0; i < out.numberoftriangles; i++) {
		TriangleShape triangle;
		
		int whichPt;
		
		for (int j = 0; j < 3; j++){
			whichPt = out.trianglelist[i * 3 + j];
			triangle[j] = glm::vec2(  out.pointlist[ whichPt * 2 + 0],  out.pointlist[ whichPt * 2 + 1]);
			triangle.index[j] = whichPt;
		}
		
		ofPoint tr[3];
		tr[0] = ofPoint(triangle[0].x, triangle[0].y);
		tr[1] = ofPoint(triangle[1].x, triangle[1].y);
		tr[2] = ofPoint(triangle[2].x, triangle[2].y);
		
		// here we check if a triangle is "inside" a contour to drop non inner triangles
        auto center = getTriangleCenter(tr[0], tr[1], tr[2]);
        
		if(contour.inside(center.x, center.y)) {
			triangle.color = ofColor(ofRandom(0,255));
			triangles.push_back(triangle);
			
			
			// store the good points in a map
			for (int j = 0; j < 3; j++){
				goodPts[triangle.index[j]] = triangle[j];
			}
			nTriangles++;
		}
	}
	
	// put all good points in a vector and handle the remapping of indices.
	// the indices stored above were for all points, but since we drop triangles, we
	// can drop non used points, and then remap all the indces.
	// that happens here:
	
	outputPts.clear();
	std::map < int, int > indexChanges;
	std::map< int , ofPoint >::iterator iter;
	for (iter = goodPts.begin(); iter != goodPts.end(); ++iter) {
		//cout << iter->first << " " << iter->second << endl;
		indexChanges[iter->first] = outputPts.size();
		outputPts.push_back(iter->second);
	}
	
	// now, with the new, potentially smaller group of points, update all the indices of the triangles so their indices point right:
	
	for (int i = 0; i < triangles.size(); i++){
		for (int j = 0; j < 3; j++){
			triangles[i].index[j] = indexChanges[triangles[i].index[j]];
		}
		for (int j = 0; j < 3; j++){
            triangles[i][j] = glm::vec2(outputPts[triangles[i].index[j]].x, outputPts[triangles[i].index[j]].y);
        }
	}
	
	ofLogNotice() << outputPts.size();
	// now make a mesh, using indices:
	/*
	triangulatedMesh.clear();
	triangulatedMesh.setMode(OF_PRIMITIVE_TRIANGLES);
	for (int i = 0; i < outputPts.size(); i++){
		triangulatedMesh.addVertex(outputPts[i]);
	}
	
	for (int i = 0; i < triangles.size(); i++){
		triangulatedMesh.addIndex(triangles[i].index[0]);;
		triangulatedMesh.addIndex(triangles[i].index[1]);;
		triangulatedMesh.addIndex(triangles[i].index[2]);;
	}*/
	
	// depending on flags, we may need to adjust some of the memory clearing
	// (see tricall.c for full listings)
	// TODO: this should be agressively tested.
	
	free(in.pointlist);
	free(out.pointlist);
	if (out.pointattributelist != NULL) free(out.pointattributelist);
	if (out.pointmarkerlist != NULL) free(out.pointmarkerlist);
	free(out.trianglelist);
	if (out.triangleattributelist != NULL) free(out.triangleattributelist);
	
	return triangles;
}

//-------------------------------------------------------------------
ofPoint ofxBox2dPolygonUtils::getTriangleCenter(ofPoint &a, ofPoint &b, ofPoint &c) {
	ofPoint tr[3];
	tr[0] = a; tr[1] = b; tr[2] = c;
	return getTriangleCenter(tr);
}

//-------------------------------------------------------------------
ofPoint ofxBox2dPolygonUtils::getTriangleCenter(ofPoint *tr) {
	float c_x = (tr[0].x + tr[1].x + tr[2].x) / 3;
	float c_y = (tr[0].y + tr[1].y + tr[2].y) / 3;
	return ofPoint(c_x, c_y);
}

//-------------------------------------------------------------------
bool ofxBox2dPolygonUtils::isPointInsidePolygon(const ofPoint & p, const vector<ofDefaultVertexType> &polygon) {
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
//-------------------------------------------------------------------
static CoordType cross(const hPoint &O, const hPoint &A, const hPoint &B) {
	return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

// Returns a list of points on the convex hull in counter-clockwise order.
// Note: the last point in the returned list is the same as the first one.
//-------------------------------------------------------------------
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

//-------------------------------------------------------------------
ofPolyline ofxBox2dPolygonUtils::getConvexHull(vector<ofDefaultVertexType>&linePts){
	
	vector < hPoint > ptsIn;
	
	for (size_t i = 0; i < linePts.size(); i++){
		hPoint pt;
		pt.x = linePts[i].x;
		pt.y = linePts[i].y;
		
		ptsIn.push_back(pt);
	}
	vector < hPoint > ptsOut;
	
	ptsOut =  calcConvexHull(ptsIn);
	
	ofPolyline outLine;
	
	for (size_t i = 0; i < ptsOut.size(); i++){
		outLine.addVertex(ofPoint(ptsOut[i].x, ptsOut[i].y));
	}
	
	return outLine;
}

//-------------------------------------------------------------------
ofPolyline ofxBox2dPolygonUtils::getConvexHull(ofPolyline &line){
	return getConvexHull(line.getVertices());
}
