#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include <vector>
#include <SDL2/SDL.h>

void set_pixel(class SDL_Surface *surface, int x, int y, Uint32 pixel);


struct Camera{

	// Ancho y alto de pixeles
	std::size_t w, h;
	// Distancia de la camara hacia el plano de proyección
	float near;
	// field of view
	float fov; // radians
	// Posición de la camara
	Point<float> eye;
	// Punto donde apunta el centro de la camara
	Point<float> center;
	// Vector hacia arriba
	Vector<float> up;

	// valores xd
	float a, b, f;

	class SDL_Texture* texture;
	
	// Eje de coordenadas de la camara
	class Vector<float> xe, ye, ze;

	Camera() = default;
	Camera(std::size_t w, std::size_t h, float n, float f, Point<float> e, Point<float> c, Vector<float> u);

	void init();

	class Ray throwRay(std::size_t x, std::size_t y);

	class SDL_Texture* createTexture(class SDL_Renderer* renderer, std::vector<class Object*> objects, std::vector<class Light*> lights, class Light* ambient);

	void render(class SDL_Renderer* renderer);

	void changePosition(float x, float y, float z, class SDL_Renderer* renderer, std::vector<class Object*> objects, std::vector<class Light*> lights, class Light* ambient);
	
	class Color calculateColor(const Ray& ray, std::vector<class Object*> objects, std::vector<class Light*> lights, class Light* ambient, int prof);

	class Vector<float> refract(class Vector<float> I, class Vector<float> N, float ior);
	
	void fresnel(class Vector<float> I, class Vector<float> N, float &ior, float &kr);
};

#endif

