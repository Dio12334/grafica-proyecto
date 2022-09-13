#ifndef COLOR_H
#define COLOR_H

#include "../Math.h"
#include <SDL2/SDL.h>

struct Color: public Math::Vector<float>{

	Color() = default;

	Color(float ir, float ig, float ib);

	Color(Math::Vector<float> other);

	Uint32 toUint32();
};


#endif

