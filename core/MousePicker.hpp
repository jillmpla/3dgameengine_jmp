#pragma once
#include "Camera.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" 

class MousePicker
{
private:
	glm::vec3 currentRay;
	glm::mat4 projMatriX;
	glm::mat4 viewMatriX;
	Camera *camera;
	glm::vec3 clickPosInWorld;
	glm::vec3 direction;
	double x;
	double y;
	int width;
	int height;
	bool theHitBool;
	glm::vec3 currentRayAt;
	glm::vec4 clipCoords;
	glm::vec3 calculateMouseRay();
	glm::vec2 getNormalizedDevCoor(double x1, double y1);
	glm::vec4 toEye(glm::vec4 clips);
	glm::vec3 MousePicker::toWorldCoor(glm::vec4 eyeSpaceCoords);

public:
	MousePicker(Camera *cam);
	glm::vec3 getCurrentRay();
	void update();
	void MousePicker::saveCoordX(double x);
	void MousePicker::saveCoordY(double y);
	void updateBufferW(int width);
	void updateBufferH(int height);
	void intersectionXYPlane(float *worldPos);
	//glm::vec3 MousePicker::getClickPosInWorld();
	//glm::vec3 MousePicker::getDirection();

};