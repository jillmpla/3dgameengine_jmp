#include "MousePicker.hpp"
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

MousePicker::MousePicker(Camera *cam) {
	camera = cam;
	projMatriX = camera->getProjectionMatrix();
	viewMatriX = camera->getViewMatrix();
}

glm::vec3 MousePicker::getCurrentRay() {
	return currentRay;
}

//needs to be called every frame
void MousePicker::update() {
	viewMatriX = camera->getViewMatrix();
	projMatriX = camera->getProjectionMatrix();
	currentRay = calculateMouseRay();
}

//get x coord from mousecallback
void MousePicker::saveCoordX(double x) {
	this->x = x;
}

//get y coord from mousecallback
void MousePicker::saveCoordY(double y) {
	this->y = y;
}

//get width from mousecallback
void MousePicker::updateBufferW(int width) {
	this->width = width;
}

//get height from mousecallback
void MousePicker::updateBufferH(int height) {
	this->height = height;
}

glm::vec3 MousePicker::calculateMouseRay() {
	//screen coordinates
	double x1 = x;
	double y1 = y;
	glm::vec2 normalCoords = getNormalizedDevCoor(x1, y1);
	//clip space
	clipCoords = glm::vec4(normalCoords.x, normalCoords.y, -1.0f, 1.0f);
	//eye space
	glm::vec4 eyeSpaceCoords = toEye(clipCoords);
	//world space
	glm::vec3 worldSpaceRay = toWorldCoor(eyeSpaceCoords);
	worldSpaceRay.z = -worldSpaceRay.z;
	return worldSpaceRay;
}

//clip space to eye space/4d eye coordinates
glm::vec4 MousePicker::toEye(glm::vec4 clips) {
	glm::mat4 invProj = glm::inverse(projMatriX);
	glm::vec4 eyeCoor = invProj * clips;
	glm::vec4 eyeCoorFinal = glm::vec4(eyeCoor.x, eyeCoor.y, -camera->getNearPlane(), 1.0f);
	return eyeCoorFinal;
}

//eye space to world space
glm::vec3 MousePicker::toWorldCoor(glm::vec4 eyeSpaceCoords) {
	glm::mat4 invView = glm::inverse(viewMatriX);
	glm::vec4 rWorld = invView * eyeSpaceCoords;
	glm::vec3 finalWorld = glm::vec3(rWorld.x, rWorld.y, rWorld.z);
	return finalWorld;
}

glm::vec2 MousePicker::getNormalizedDevCoor(double x1, double y1) {
	double x2 = (2.0f * x1) / ((float)width) - 1.0f;
	double y2 = (2.0f * y1) / ((float)height) - 1.0f;
	glm::vec2 newVec = glm::vec2(x2, -y2);
	return newVec;
}

