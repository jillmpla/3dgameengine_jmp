#pragma once
#include "ModelData.hpp"
#include "MeshShaderGL.hpp"
#include "MeshGL.hpp"


class ModelGL
{
private:
	vector<MeshGL*> meshes; //list of all meshes in the model

	glm::mat4 modelMatrix = glm::mat4(1.0f); //for the model matrix, default = identity matrix

	glm::vec3 centerofModel = glm::vec3(0, 0, 0); //store center of model, default = (0, 0, 0)


public:
	ModelGL(ModelData *data);

	~ModelGL();

	void draw(MeshShaderGL *shader);

	void translate(glm::vec3 offset);

	void rotate(float degrees, glm::vec3 axis);

	void reset();
};