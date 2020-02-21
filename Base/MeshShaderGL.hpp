#pragma once
#include "ShaderGL.hpp"
#include "Camera.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "ModelData.hpp"
#include "Light.hpp"

class MeshShaderGL : public ShaderGL {
public:
	MeshShaderGL(std::string vertex, std::string fragment, bool isFilepath);
	void setModelTransform(glm::mat4 &modelMat);
	void setViewAndProjection(Camera *camera);
	void setLight(Light *light);
	void setShininess(float s);
	void setMaterialChoice(int choice);

private:
	GLint modelMatLoc = -1;
	glm::mat4 modelMat;

	GLint viewMat = -1;
	GLint projMat = -1;
	glm::mat4 viewMatrix = glm::mat4(1.0f);

	GLint normal = -1;
	GLint lightPosition = -1;
	GLint lightColor = -1;
	GLint shininess = -1;
	GLint material = -1;
};