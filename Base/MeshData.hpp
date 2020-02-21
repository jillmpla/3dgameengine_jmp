#ifndef MESH_DATA_H
#define MESH_DATA_H

#include <vector>
#include "glm/glm.hpp"
using namespace std;

struct Vertex {
	glm::vec3 pos;
	glm::vec4 color;
	glm::vec3 normal;
	glm::vec2 texcoords;
	glm::vec4 tangent;

	Vertex() {};

	Vertex(glm::vec3 p, glm::vec4 c, glm::vec3 n,
		glm::vec2 tx, glm::vec4 tn) {
		pos = p;
		color = c;
		normal = n;
		texcoords = tx;
		tangent = tn;
	};
};

class MeshData {
private:
	vector<Vertex> vertices;
	vector<unsigned int> elements;
		
	void recomputeTangents();

public:
	MeshData(vector<Vertex> verts, vector<unsigned int> indices);
	~MeshData();

	vector<Vertex>* getVertices();
	vector<unsigned int>* getIndices();
};

#endif
