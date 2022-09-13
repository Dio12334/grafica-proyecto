#ifndef LIGHT_H
#define LIGHT_H

#include "../Math.h"
#include "Color.h"

struct Light{
	Math::Vector<float> position;
	Color color;
	Light(Math::Vector<float> pos, Color col);
};

#endif

