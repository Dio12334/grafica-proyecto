#ifndef PLANE_H
#define PLANE_H

#include "Object.h"

struct Plane: public Object{
	
	Vector<float> N;
	float d;

	Plane(Point<float> position, Color col, float ka, float kd, float ks, float n, float idr, Vector<float> normal): Object(position, col, ka, kd, ks, n, idr), N(normal){
		d = - Math::dotProduct(position, N);
	}

	bool intersect(const Ray& ray, float &minDistance, Vector<float>& normal) override {
		/* minDistance = -(Math::dotProduct(N, ray.origin) + d) / Math::dotProduct(N, ray.direction); */
		/* if(minDistance > 0){ */
		/* 	normal = N; */
		/* 	return true; */
		/* } */
		/* return false; */

		float nd = Math::dotProduct(N, ray.direction);
		if(nd == 0) return false;
		minDistance = Math::dotProduct((N*d - ray.origin), N) / nd;
		if(minDistance < 0) return false;
		normal = nd > 0? -1.0F*N:N;
		return true;
	}

	bool intersect(const Ray& ray) override {
		/* float dis = -(Math::dotProduct(N, ray.origin) + d) / Math::dotProduct(N, ray.direction); */
		/* if(dis > 0) */	
		/* 	return true; */
		/* return false; */

		float nd = Math::dotProduct(N, ray.direction);
		if(nd == 0) return false;
		float minDistance = Math::dotProduct((N*d - ray.origin), N) / nd;
		if(minDistance < 0) return false;
		return true;
	}
};

#endif

