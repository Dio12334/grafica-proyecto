#ifndef RAY_H
#define RAY_H

#include "../Math.h"

using Math::Point;
using Math::Vector;


struct Ray{

	Point<float> origin;
	Vector<float> direction;

	Ray() = default;
	Ray(Point<float> o, Vector<float> dir);
};

#endif

