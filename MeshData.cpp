#include "MeshData.hpp"

// Code based off of: http://www.terathon.com/code/tangent.html
void MeshData::recomputeTangents() {
	// Zero out all tangents
	vector<glm::vec3> allB;
	for (int i = 0; i < vertices.size(); i++) {
		vertices[i].tangent = glm::vec4(0, 0, 0, 0);
		allB.push_back(glm::vec3(0, 0, 0));
	}

	// For each triangle...
	for (int t = 0; t < elements.size(); t += 3) {
		// Get indices
		int i0 = elements.at(t);
		int i1 = elements.at(t + 1);
		int i2 = elements.at(t + 2);

		// Get vertices of triangle
		Vertex v0 = vertices[i0];
		Vertex v1 = vertices[i1];
		Vertex v2 = vertices[i2];

		// Get side vectors
		glm::vec3 q1 = v1.pos - v0.pos;
		glm::vec3 q2 = v2.pos - v0.pos;

		// Get uv vectors
		glm::vec2 duv1 = v1.texcoords - v0.texcoords;
		glm::vec2 duv2 = v2.texcoords - v0.texcoords;

		// Build linear system
		glm::mat2x3 Qt;
		Qt[0] = q1;
		Qt[1] = q2;
		glm::mat3x2 Q = glm::transpose(Qt);

		glm::mat2x2 ST;
		ST[0][0] = duv2.t;
		ST[1][0] = -duv1.t;
		ST[0][1] = -duv2.s;
		ST[1][1] = duv1.s;

		float factor = 1.0f / (duv1.s * duv2.t - duv2.s * duv1.t);

		glm::mat3x2 TB = factor * (ST * Q);

		// Get tangent and bitangent
		glm::mat2x3 TBt = glm::transpose(TB);
		glm::vec3 T = TBt[0];
		glm::vec3 B = TBt[1];

		// Normalize and add to tangent for vector
		T = glm::normalize(T);
		glm::vec4 T4 = glm::vec4(T, 0.0);
		vertices[i0].tangent += T4;
		vertices[i1].tangent += T4;
		vertices[i2].tangent += T4;

		// Add bitangent
		B = glm::normalize(B);
		allB[i0] += B;
		allB[i1] += B;
		allB[i2] += B;
	}

	// For each vertex...
	for (int i = 0; i < vertices.size(); i++) {
		// Get normal, current tangent, and bitangent
		glm::vec3 N = vertices[i].normal;
		glm::vec3 T = glm::normalize(glm::vec3(vertices[i].tangent));
		glm::vec3 B = glm::normalize(allB[i]);

		// Do Gram-Schmidt orthogonalization
		glm::vec3 Tp = glm::normalize(T - (glm::dot(N, T)*N));

		// Get handedness
		float hand = 1.0;
		if (glm::dot(glm::cross(N, T), B) < 0.0) {
			hand = -1.0;
		}

		// Store tangent (with handedness)
		vertices[i].tangent = glm::vec4(Tp, hand);
	}
}

MeshData::MeshData(vector<Vertex> verts,
	vector<unsigned int> indices) {

	vertices.insert(vertices.end(), verts.begin(), verts.end());
	elements.insert(elements.end(), indices.begin(), indices.end());

	recomputeTangents();
}

MeshData::~MeshData() {
	vertices.clear();
	elements.clear();
}

vector<Vertex>* MeshData::getVertices() {
	return &vertices;
}

vector<unsigned int>* MeshData::getIndices() {
	return &elements;
}
