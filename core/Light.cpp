#include "Light.hpp"

Light::Light(glm::vec3 pos1, glm::vec3 color1, bool isPointLight1) {
	pos = pos1;
	color = color1;
	isPointLight = isPointLight1;
}

void Light::setPos(glm::vec3 pos2) {
	pos = pos2;
}

bool Light::getIsPointLight() {
	return isPointLight;
}

void Light::setIsPointLight(bool isPointLight1) {
	this->isPointLight = isPointLight1;
}

void Light::setColor(glm::vec3 color1) {
	color = color1;
}

glm::vec4 Light::getColor() {
	glm::vec4 temp(color, 1.0);
	return temp;
}

glm::vec4 Light::getPos() {
	if (isPointLight == true) {
		glm::vec4 temp1(pos, 1.0);
		return temp1;
	}
	else {
		glm::vec4 temp2(pos, 0.0);
		return temp2;
	}
}
