#ifndef FIREFLY_H
#define FIREFLY_H
#include "../Math.h"

struct Firefly{
	struct Light* light;
	struct Object* object;

	Firefly(Math::Vector<float> position);
	void updatePosition(float deltaTime);
};


#endif

