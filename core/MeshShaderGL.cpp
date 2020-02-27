#include "MeshShaderGL.hpp"

MeshShaderGL::MeshShaderGL(std::string vertex, std::string fragment, bool isFilepath) : ShaderGL(vertex, fragment, isFilepath) 
{
	modelMatLoc = glGetUniformLocation(programID, "modelMat");
	viewMat = glGetUniformLocation(programID, "viewMat");
	projMat = glGetUniformLocation(programID, "projMat");
	normal = glGetUniformLocation(programID, "normMat");
	lightPosition = glGetUniformLocation(programID, "light.pos");
	lightColor = glGetUniformLocation(programID, "light.color");
	shininess = glGetUniformLocation(programID, "shiny");
	material = glGetUniformLocation(programID, "matChoice");
}

void MeshShaderGL::setModelTransform(glm::mat4 &modelMat) 
{
	this->modelMat = modelMat;
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(this->modelMat));

	//compute normal matrix and pass to shader
	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(viewMatrix*modelMat)));
	glUniformMatrix3fv(normal, 1, GL_FALSE, glm::value_ptr(normalMatrix));
}

void MeshShaderGL::setViewAndProjection(Camera *camera) {
	viewMatrix = camera->getViewMatrix();
	glUniformMatrix4fv(viewMat, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(projMat, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));

	//compute normal matrix and pass to shader
	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(viewMatrix*modelMat)));
	glUniformMatrix3fv(normal, 1, GL_FALSE, glm::value_ptr(normalMatrix));
}

void MeshShaderGL::setLight(Light *light) {
	glm::vec4 viewLit = viewMatrix*light->getPos();
	glUniform4fv(lightPosition, 1, glm::value_ptr(viewLit));
	glUniform4fv(lightColor, 1, glm::value_ptr(light->getColor()));
}

void MeshShaderGL::setShininess(float s) {
	glUniform1f(shininess, s);                      //for floats
}

void MeshShaderGL::setMaterialChoice(int choice) {
	glUniform1i(material, choice);                 //for ints
}