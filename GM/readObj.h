#ifndef _READOBJ_
#define _READOBJ_
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "HalfEdge.h"

class ReadObj
{
public:
	static bool readfile(std::string filename);

	static std::vector<Vertex> getVertices() {
		return vertices;
	}

	static std::vector<HalfEdge> getHalfEdges() {
		return halfEdges;
	}

	static std::vector<Face> getFaces() {
		return faces;
	}

private:
	static std::vector<Vertex> vertices;
	static std::vector<HalfEdge> halfEdges;
	static std::vector<Face> faces;
	static float max_x, min_x;
	static float max_y, min_y;
	static float max_z, min_z;

	static void updateBoundry(float x, float y, float z);
	static bool generateHalfEdge();
	static void triangulator();
};


#endif