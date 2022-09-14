#include "World.h"
#include "Light.h"
#include "Object.h"
#include "Sphere.h"
#include "Camera.h"
#include "Firefly.h"

void World::addLight(class Light* light){
	lights.push_back(light);
	Object* source = new Sphere(1);
	source->position = light->position;
	source->isLightSource = true;
	source->color = light->color;
	addObject(source);
}
void World::addObject(class Object* object){
	objects.push_back(object);
}

void World::changeCameraPosition(Math::Vector<float> position){
	camera->changePosition(position.x, position.y, position.z, renderer, objects, lights, ambient);
}

void World::setCamera(class Camera * icamera){
	camera = icamera;
}

void World::setAmbient(class Light* light){
	ambient = light;
}

void World::createScenario(){
	camera->createTexture(renderer, objects, lights, ambient);
}

void World::render(){
	camera->render(renderer);
}

void World::setRenderer(class SDL_Renderer * irenderer){
	renderer = irenderer;
}


void World::addFirefly(class Firefly* firefly){
	fireflies.push_back(firefly);
	lights.push_back(firefly->light);
	objects.push_back(firefly->object);
}

void World::updatePositions(float deltaTime){
	for(auto firefly: fireflies)
		firefly->updatePosition(deltaTime);
}
