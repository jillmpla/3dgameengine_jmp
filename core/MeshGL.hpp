#pragma once
#include <GL/glew.h>
#include "MeshData.hpp"

class MeshGL
{
private:
	GLuint VBO = 0; //ID of vertex buffer
	GLuint EBO = 0; //ID of index buffer
	GLuint VAO = 0; //ID of vertex array object
	int indexCnt = 0; //Number of indices in the index buffer



public: 
	MeshGL(MeshData *data);

	~MeshGL();

	void draw();
};