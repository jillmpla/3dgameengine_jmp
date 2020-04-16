#pragma once
#include "../core/ModelGL.hpp"
#include "../core/ModelData.hpp"
#include "Collide.hpp"

class Actor
{
private:
	ModelData* d;
	ModelGL* g;
	Collide* c;

public:
	Actor::Actor(ModelData* incoming, ModelGL* incoming1, Collide* incoming2);
	ModelData* Actor::returnModelData();
	ModelGL* Actor::returnModelGL();

};