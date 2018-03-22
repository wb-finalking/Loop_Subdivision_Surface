#include "readObj.h"

std::vector<Vertex> ReadObj::vertices = std::vector<Vertex>();
std::vector<HalfEdge> ReadObj::halfEdges = std::vector<HalfEdge>();
std::vector<Face> ReadObj::faces = std::vector<Face>();
float ReadObj::min_x=FLT_MAX;
float ReadObj::max_x = -FLT_MAX;
float ReadObj::min_y = FLT_MAX;
float ReadObj::max_y = -FLT_MAX;
float ReadObj::min_z = FLT_MAX;
float ReadObj::max_z = -FLT_MAX;

bool  ReadObj::readfile(std::string filename)
{
	min_x = FLT_MAX;
	max_x = -FLT_MAX;
	min_y = FLT_MAX;
	max_y = -FLT_MAX;
	min_z = FLT_MAX;
	max_z = -FLT_MAX;
	std::ifstream file(filename);

	if (!file) {
		std::cerr << "Invalid file name!" << std::endl;
		return false;
	}

	vertices.clear();
	halfEdges.clear();
	faces.clear();

	vector<int> tmp;
	char buf[20];
	string sub_line;
	for (std::string line; std::getline(file, line);)
	{
		if (line[0] == 'v' && line[1] == ' ')
		{
			float x, y, z;
			sscanf_s(line.c_str(), "v %f %f %f", &x, &y, &z);
			updateBoundry( x,  z,  -y);
			vertices.push_back({ new Vector4(x, z, -y), -1 });
		}
		else if (line[0] == 'f' && line[1] == ' ')
		{
			int v;
			tmp.clear();
			sub_line = line.substr(2);
			istringstream stringin(sub_line);
			string tmp_str;
			int vert_size = vertices.size();
			while (stringin >> v)
			{
				tmp.push_back(v < 0 ? vert_size - v : v - 1);
				stringin >> tmp_str;	
			}
			if (tmp.size()>0)
				faces.push_back({ tmp });

		}

	}//for (std::string line; std::getline(file, line);)

	float width = max_x - min_x;
	float height = max_y - min_y;
	float depth = max_z - min_z;
	float center_x = (max_x + min_x) / 2;
	float center_y = (max_y + min_y) / 2;
	float center_z = (max_z + min_z) / 2;
	Vector4 tmp_vector(center_x, center_y, center_z);
	float max = width>height ? width : height;
	max = max>depth ? max : depth;

	float scale = 2 / max;
	for (int i = 0; i < vertices.size(); ++i)
	{
		*vertices[i].vector -= tmp_vector;
		*vertices[i].vector *= scale;
	}


	triangulator();

	return generateHalfEdge();
}

void ReadObj::triangulator()
{
	std::vector<Face> newfaces;
	for (Face f : faces)
	{
		if (f.verts.size() == 3)
		{
			newfaces.push_back(f);
		}
		else
		{
			Vector4 center_v(0,0,0,0);
			for (int i : f.verts)
				center_v += *vertices[i].vector;

			center_v = center_v / f.verts.size();
			int index_v = vertices.size();
			vertices.push_back({ new Vector4(center_v.x, center_v.y, center_v.z), -1 });
			for (int i = 0; i < f.verts.size(); i++)
			{
				int size = f.verts.size();
				int a = i%size;
				int b = (i + 1) % size;
				newfaces.push_back({ {f.verts[a] , f.verts[b] , index_v} });
			}
		}//if (f.verts.size() == 3)
	}//for (Face f : faces)
	faces.clear();
	faces = newfaces;
}

bool ReadObj::generateHalfEdge()
{
	std::vector<std::vector<int> > edgeMap(vertices.size(), std::vector<int>());
	halfEdges.resize(3 * faces.size(), {-1});

	for (int he_id = 0; he_id < 3 * faces.size(); ++he_id) {
		//std::cout << "Current HalfEdge num: " << he << std::endl;

		int face_id = he_id / 3;

		// vertex update
		int s_v = he_id % 3;//start vertex of this hafedge
		int e_v = (he_id + 1) % 3;//end vertex of this hafedge
		vertices[faces[face_id].verts[e_v]].halfEdge = he_id;

		// halfedge constr.
		int pair = -1;

		//search the hafedges containing e_v 
		for (std::vector<int>::iterator it = edgeMap[faces[face_id].verts[e_v]].begin(); it != edgeMap[faces[face_id].verts[e_v]].end(); ++it) {
			int tmp_he_id = *it;
			int tmp_face_id = tmp_he_id / 3;
			int tmp_e_v = (tmp_he_id + 1) % 3;
			
			//if find the halfedge whose end vertex is equal to the start vertex of this halfedge,the pair of hafedges has been found
			if (faces[face_id].verts[s_v] == faces[tmp_face_id].verts[tmp_e_v]) {
				pair = tmp_he_id;
				halfEdges[tmp_he_id].pair = he_id;
				edgeMap[faces[face_id].verts[e_v]].erase(it);
				break;
			}
		}

		if (pair == -1) {
			edgeMap[faces[face_id].verts[s_v]].push_back(he_id);
		}

		//halfEdges.push_back({ pair });
		halfEdges[he_id].pair = pair;

	}

	std::cerr << "Loading complete!" << std::endl;
	return true;
}

void ReadObj::updateBoundry(float x, float y, float z)
{
	min_x = x < min_x ? x : min_x;
	max_x = x > max_x ? x : max_x;
	min_y = y < min_y ? y : min_y;
	max_y = y > max_y ? y : max_y;
	min_z = z < min_z ? z : min_z;
	max_z = z > max_z ? z : max_z;
}