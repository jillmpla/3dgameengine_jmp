#include "ModelGL.hpp"
#include "ModelData.hpp"


ModelGL::ModelGL() {
}

ModelGL::ModelGL(ModelData *data) {
	for (int x = 0; x < data->getMeshCnt(); x++) {
		meshes.push_back(new MeshGL(data->getMesh(x)));
	}

}

ModelGL::~ModelGL() {
	for (int x = 0; x < meshes.size(); x++) {
		delete meshes.at(x);
	}

	meshes.clear();

}

void ModelGL::draw(MeshShaderGL *shader) {

	shader->setModelTransform(modelMatrix); //call setModelTransform() on shader object and pass in model matrix

	for (int x = 0; x < meshes.size(); x++) {
		meshes[x]->draw();
	}

}

void ModelGL::translate(glm::vec3 offset) {

	//Add offset to center position
	centerofModel = offset + centerofModel;
	//Create a transformation T that translates by offset
	glm::mat4 T = glm::translate(glm::mat4(1.0f), offset);
	//Set model matrix equal to T*current model matrix
	modelMatrix = T * modelMatrix;
}

void ModelGL::rotate(float degrees, glm::vec3 axis) {

	//convert degrees to radians
	degrees = glm::radians(degrees);

	//create a transformation T that:
	//-translates by negative center(moves model to origin)
	//-rotates around the given axis by the desired angle
	//-translates back to the center
	glm::mat4 Tn = glm::translate(glm::mat4(1.0f), -centerofModel);
	glm::mat4 R = glm::rotate(glm::mat4(1.0f), degrees, axis);
	glm::mat4 Tc = glm::translate(glm::mat4(1.0f), centerofModel);
	glm::mat4 T = Tc*R*Tn;
	
	//Set model matrix equal to T*current model matrix
	modelMatrix = T * modelMatrix;
}

void ModelGL::reset() {
	centerofModel = glm::vec3(0, 0, 0);
	modelMatrix = glm::mat4(1.0f);
}
