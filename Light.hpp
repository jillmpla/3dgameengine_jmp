#pragma once
#include "ModelData.hpp"

class Light
{
private:
	glm::vec3 pos = glm::vec3(0, 0, 0);
	glm::vec3 color = glm::vec3(1, 1, 1);
	bool isPointLight = true;

public:
	Light(glm::vec3 pos, glm::vec3 color, bool isPointLight);

	void Light::setPos(glm::vec3 pos2);

	bool Light::getIsPointLight();

	void Light::setIsPointLight(bool isPointLight);

	void Light::setColor(glm::vec3 color);

	glm::vec4 Light::getColor();

	glm::vec4 Light::getPos();
};