#ifndef SPHERE_H
#define SPHERE_H

#include "Object.h"
#include <cstdint>


struct Sphere: public Object{

	float radius;

	Sphere() = default;
	Sphere(float rad): Object(), radius(rad){

	}
	Sphere(Vector<float> pos, Color color, float ka, float kd, float ks, float kr, float n , float idr, float rad):Object(pos, color, ka, kd, ks, kr, n, idr), radius(rad){
	
	}
	
	bool intersect(const Ray& ray, float &minDistance, Vector<float>& normal) override{
		float a = ray.direction.squaredLength();
		float b = Math::dotProduct(2*ray.direction, ray.origin - position);
		float c = Math::squaredLength(ray.origin - position) - radius * radius;
		
		float det = b*b - 4*a*c;
		if(det > 0){
			float t1 = (-b - sqrt(det)) / (2*a);
			float t2 = (-b + sqrt(det)) / (2*a);
			float t = std::min(t1, t2);
			if(t <= 0) return false;
			
			minDistance = t;
			Point<float> pi = ray.origin + t*ray.direction;
			normal = Math::normalize(pi - position);
			return true;
		}

		return false;
	}


	bool intersect(const Ray& ray) override{
		float a = ray.direction.squaredLength();
		float b = Math::dotProduct(2*ray.direction, ray.origin - position);
		float c = Math::squaredLength(ray.origin - position) - radius * radius;
		
		float det = b*b - 4*a*c;
		if(det > 0){
			float t1 = (-b - sqrt(det)) / (2*a);
			float t2 = (-b + sqrt(det)) / (2*a);
			float t = std::min(t1, t2);
			if(t <= 0) return false;
			
			return true;
		}

		return false;
	}
};

#endif

