#pragma once
#include "../core/ModelGL.hpp"
#include "../core/ModelData.hpp"

class Collide
{
private:
	glm::vec3 maxXYZ;
	glm::vec3 minXYZ;
	ModelData* modelDataTemp = NULL;

public:
	Collide(ModelData* a);

};