#ifndef CILINDER_H
#define CILINDER_H

#include "Object.h"

struct Cilinder: public Object{
	Vector<float> pb;
	float ra;

	Cilinder(Vector<float> posa, Vector<float> posb, Color color, float ka, float kd, float ks, float n, float idr, float rada):
		Object(posa, color, ka, kd, ks, n, idr), pb(posb), ra(rada){

		}

	bool intersect(const Ray& ray, float& minDistance, Vector<float>& normal) override;
	bool intersect(const Ray& ray) override;
};

#endif

