#include "MeshGL.hpp"

MeshGL::MeshGL(MeshData *data) {

	vector<Vertex>* vertices = data->getVertices();
	vector<unsigned int>* elements = data->getIndices();

	//Get the number of indices and store it in indexCnt
	indexCnt = elements->size();

	//Create and bind the VBO and store the vertex data in it
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices->size(), vertices->data(), GL_STATIC_DRAW);

	//Create and bind the VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Enable vertex attributes arrays 0 and 1
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//Bind the VBO and set up vertex attribute arrays for pos and color from struct Vertex
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos)); //pos
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color)); //color
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)); //normal

	//Create and bind the EBO and copy in the indices
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements->size() * sizeof(GLuint), elements->data(), GL_STATIC_DRAW);

	//Unbind the VAO
	glBindVertexArray(0);

}

MeshGL::~MeshGL() {

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &EBO);

}

void MeshGL::draw() {

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indexCnt, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);

}