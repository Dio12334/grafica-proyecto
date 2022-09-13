#ifndef OBJECT_H
#define OBJECT_H


#include "Ray.h"
#include "Color.h"
#include <cstdint>
#include <SDL2/SDL.h>

struct Object{

	Point<float> position;
	Color color;
	float ka, kd, ks;
	float n;
	float idr;
	bool isLightSource = false;
	Object() = default;
	Object(Point<float> po, Color col, float ika, float ikd, float iks, float in, float iidr): 
		position(po), 
		color(col), 
		ka(ika), 
		kd(ikd), 
		ks(iks), 
		n(in),
		idr(iidr),
		isLightSource(false)
	{

	}

	virtual bool intersect(const Ray&, float &minDistance, Vector<float>& normal) = 0;
	virtual bool intersect(const Ray&) = 0;
	virtual ~Object(){}
};

#endif

