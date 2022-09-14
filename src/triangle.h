#ifndef TRIANGLE_H
#define TRIANGLE_H


#include "Object.h"
#include <cstdint>

struct Triangle: public Object{

	Point<float> p1,p2,p3;
    Vector<float> N;

	Triangle() = default;

	Triangle(Point<float> p1_, Point<float>  p2_, Point<float>  p3_): Object(),  p1(p1_), p2(p2_), p3(p3_) {
        auto temp_vec = Math::crossProduct(p2 - p1, p3 - p2);
        N =  temp_vec / (temp_vec.length()); 
	}
	
    Triangle(Point<float> p1_, Point<float>  p2_, Point<float>  p3_, Vector<float> pos, Color color, float ka, float kd, float ks, float n , float idr, float rad):Object(pos, color, ka, kd, ks, n, idr), 
     p1(p1_), p2(p2_), p3(p3_) {
	    auto temp_vec = Math::crossProduct(p2 - p1, p3 - p2);
        N =  temp_vec / (temp_vec.length()); 
	}
	
	bool intersect(const Ray& ray, float &minDistance, Vector<float>& normal) override{
		

        Vector<float> v0v1 =  p2 - p1;
        Vector<float> v0v2 =  p3 - p1;

        Vector<float> n = Math::crossProduct(v0v1, v0v2);
        float area = n.length();
        
        // checking ray and plane are parallel
        float NdotRay = Math::dotProduct(n, ray.origin);
        
        if( abs(NdotRay) < 0.001)
            return false;

        //checking if point is inside triangle

        float d = -Math::dotProduct(n , p1);
        auto t =   -(Math::dotProduct(n, ray.origin) + d) / NdotRay;

        if(t <  0) return false;

        Vector<float>  P = ray.origin + t * ray.direction;
        Vector<float>  C;

        //edge 0
        Vector<float> edge0 = p2 - p1;
        Vector<float> vp0 = P - p1;
        C = Math::crossProduct(edge0, vp0);
        if(Math::dotProduct(n, C) < 0) return false;

        //edge 1
        Vector<float> edge1 = p3 - p2;
        Vector<float> vp1 = P - p2;
        C = Math::crossProduct(edge1, vp1);
        if(Math::dotProduct(n, C) < 0) return false;

        //edge 2
        Vector<float> edge2 = p1 - p3;
        Vector<float> vp2 = P - p3;
        C = Math::crossProduct(edge2, vp2);
        if(Math::dotProduct(n, C) < 0) return false;

        minDistance = t;
        normal = n;

		return true;
	}


	bool intersect(const Ray& ray) override{
                Vector<float> v0v1 =  p2 - p1;
        Vector<float> v0v2 =  p3 - p1;

        Vector<float> n = Math::crossProduct(v0v1, v0v2);
        float area = n.length();
        
        // checking ray and plane are parallel
        float NdotRay = Math::dotProduct(n, ray.origin);
        
        if( abs(NdotRay) < 0.001)
            return false;

        //checking if point is inside triangle

        float d = -Math::dotProduct(n , p1);
        auto t =   -(Math::dotProduct(n, ray.origin) + d) / NdotRay;

        if(t <  0) return false;

        Vector<float>  P = ray.origin + t * ray.direction;
        Vector<float>  C;

        //edge 0
        Vector<float> edge0 = p2 - p1;
        Vector<float> vp0 = P - p1;
        C = Math::crossProduct(edge0, vp0);
        if(Math::dotProduct(n, C) < 0) return false;

        //edge 1
        Vector<float> edge1 = p3 - p2;
        Vector<float> vp1 = P - p2;
        C = Math::crossProduct(edge1, vp1);
        if(Math::dotProduct(n, C) < 0) return false;

        //edge 2
        Vector<float> edge2 = p1 - p3;
        Vector<float> vp2 = P - p3;
        C = Math::crossProduct(edge2, vp2);
        if(Math::dotProduct(n, C) < 0) return false;

		return true;
	}
};



#endif