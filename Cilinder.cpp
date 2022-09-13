#include "Cilinder.h"

bool Cilinder::intersect(const Ray& ray, float& minDistance, Vector<float>& normal){
	Vector<float> ro = ray.origin;
	Vector<float> rd = ray.direction;
	Vector<float> ca = pb - position;
	Vector<float> oc = ro - position;
	
	float caca = ca.squaredLength();
	float card = Math::dotProduct(ca, rd);
	float caoc = Math::dotProduct(ca, oc);
	float a = caca - card*card;
    float b = caca * Math::dotProduct(oc, rd) - caoc*card;
    float c = caca * oc.squaredLength() - caoc*caoc - ra*ra*caca;
    float h = b*b - a*c;
    if( h < 0.0 ) return false; //no intersection
    h = std::sqrt(h);
    minDistance = (-b-h)/a;
    //if (t <= 0) return false;
    // body
    float y = caoc + minDistance*card;
    if ( y > 0.0 && y < caca && minDistance > 0) {
        normal = (oc + minDistance*rd - ca*y/caca)/ra;
        normal.normalize();
        return true;
    }
    // caps
    minDistance = (((y<0.0)?0.0:caca) - caoc)/card;
    if (minDistance <= 0) return false;
    if( std::abs(b+a*minDistance)<h ) {
        normal =  ca * Math::sgn(y) / caca;
        normal.normalize();
        return true;
    }
    return false; //no intersection
}


bool Cilinder::intersect(const Ray& ray){
	Vector<float> ro = ray.origin;
	Vector<float> rd = ray.direction;
	Vector<float> ca = pb - position;
	Vector<float> oc = ro - position;
	
	float caca = ca.squaredLength();
	float card = Math::dotProduct(ca, rd);
	float caoc = Math::dotProduct(ca, oc);
	float a = caca - card*card;
    float b = caca * Math::dotProduct(oc, rd) - caoc*card;
    float c = caca * oc.squaredLength() - caoc*caoc - ra*ra*caca;
    float h = b*b - a*c;
    if( h < 0.0 ) return false; //no intersection
    h = std::sqrt(h);
    float minDistance = (-b-h)/a;
    //if (t <= 0) return false;
    // body
    float y = caoc + minDistance*card;
    if ( y > 0.0 && y < caca && minDistance > 0) {
        return true;
    }
    // caps
    minDistance = (((y<0.0)?0.0:caca) - caoc)/card;
    if (minDistance <= 0) return false;
    if( std::abs(b+a*minDistance)<h ) {
        return true;
    }
    return false; //no intersection
}
