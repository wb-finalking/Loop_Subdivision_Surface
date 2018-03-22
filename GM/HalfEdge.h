#ifndef _HALFEDGE_
#define _HALFEDGE_
#include "Vector4.h"

struct Vertex {
	Vector4* vector;

	int halfEdge;
};

struct Face {
	//int verts[3];
	vector<int> verts;
};

struct HalfEdge {
	//another halfedge inedx of  this halfedge index
	int pair;
};

#endif