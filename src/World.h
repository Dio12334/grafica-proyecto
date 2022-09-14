#ifndef WORLD_H
#define WORLD_H

#include "../Math.h"
#include <vector>


struct World{

	class Camera* camera;
	std::vector<class Object*> objects;
	std::vector<class Light*> lights;
	std::vector<class Firefly*> fireflies;
	class Light* ambient;
	class SDL_Renderer* renderer;

	World() = default;
	void setCamera(class Camera*);
	void setAmbient(class Light*);
	void setRenderer(class SDL_Renderer*);
	void addLight(class Light*);
	void addObject(class Object*);
	void addFirefly(class Firefly*);
	void updatePositions(float deltaTime);
	void changeCameraPosition(Math::Vector<float> position);
	void createScenario();
	void render();
};

#endif

