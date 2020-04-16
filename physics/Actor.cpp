#include "Actor.hpp"

Actor::Actor(ModelData* incoming, ModelGL* incoming1, Collide* incoming2) {
	d = incoming;
	g = incoming1;
	c = incoming2;
}

ModelGL* Actor::returnModelGL() {
	return g;
}

ModelData* Actor::returnModelData() {
	return d;
}