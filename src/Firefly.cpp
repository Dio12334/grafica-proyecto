#include "Firefly.h"
#include "Light.h"
#include "Object.h"
#include "Sphere.h"
#include "Random.h"

Firefly::Firefly(Math::Vector<float> position): 
	light(new Light(position, Color(1.f, 0.843137f, 0.f))), 
	object(new Sphere(0.5f))
{
	object->position = position;
	object->isLightSource = true;
	object->color = light->color;
}

void Firefly::updatePosition(float deltaTime){
	float velocity = 0.9f;
	Math::Vector<float> direction(Random::getFloat(-1,1), Random::getFloat(-1,1), Random::getFloat(-1,1));
	object->position += velocity*deltaTime*direction;	
	light->position += velocity*deltaTime*direction;
}
