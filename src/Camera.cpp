#include "Camera.h"
#include "Color.h"
#include "Light.h"
#include "Object.h"
#include <limits>
#include <algorithm>
#include <SDL2/SDL_image.h>
#include <sstream>
#include <iomanip>
#include <omp.h>

inline void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel){
	Uint32 * const target_pixel = (Uint32 *) ((Uint8 *) surface->pixels 
														+ y * surface->pitch
														+ x * surface->format->BytesPerPixel);
	*target_pixel = pixel;
}

Camera::Camera(std::size_t w, std::size_t h, float n, float f, Point<float> e, Point<float> c, Vector<float> u):
	w(w),
	h(h),
	near(n),
	fov(f),
	eye(e),
	center(c),
	up(u)
	{
		init();
	}

void Camera::init(){	
	f = near;
	a = 2*f*std::tan(fov/2.0);
	b = w/(h*1.0F) * a;
	ze = Math::normalize(eye - center);
	xe = Math::normalize(Math::crossProduct(up,ze));
	ye = Math::crossProduct(ze, xe);
}

Ray Camera::throwRay(std::size_t x, std::size_t y){
	Ray ray;
	ray.origin = eye;
	ray.direction = -f*ze.normal() + a*(y/(h*1.0) - 0.5F)*ye.normal() + b*(x/(w*1.0) - 0.5F)*xe.normal();
	ray.direction.normalize();
	return ray;
}


SDL_Texture* Camera::createTexture(SDL_Renderer* renderer, std::vector<Object*> objects, std::vector<Light*> lights, Light* ambient){
		
	Uint32 rmask, gmask, bmask, amask;
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
	#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
	#endif


	SDL_Surface* surf = SDL_CreateRGBSurface(0, w, h, 32, rmask, gmask, bmask, amask);

	// itero por todos los pixeles de la ventana
/* #pragma omp parallel for */
/* #pragma omp parallel for private(j) */
#pragma omp parallel for default(none) shared(objects, lights, ambient, surf) 
	for(std::size_t i = 0; i < w; ++i){
		for(std::size_t j = 0; j < h; ++j){
				
			Ray ray = throwRay(i, j);
			Color color = calculateColor(ray, objects, lights, ambient, 1);
			set_pixel(surf, i, j, color.toUint32());
		}
	}

	texture = SDL_CreateTextureFromSurface(renderer, surf);
	static int frame = 0;
	std::string title = "frames/frame_";
	std::ostringstream record;
	record << std::setw(2) << std::setfill('0') << frame++ << ".png";
	IMG_SavePNG(surf, (title + record.str()).c_str());
	return nullptr;
}

void Camera::render(SDL_Renderer* renderer){
	SDL_RenderCopy(renderer, texture ,nullptr, nullptr);
}

void Camera::changePosition(float x, float y, float z, SDL_Renderer* renderer, std::vector<Object*> objects, std::vector<Light*> lights, Light* ambient){
	eye.x = x; 
	eye.y = y;
	eye.z = z;
		
	init();
	createTexture(renderer, objects, lights, ambient);
}

Color Camera::calculateColor(const Ray &ray, std::vector<class Object *> objects, std::vector<class Light *> lights, class Light *ambient, int prof){
	
	Color result(0.0F, 0.0F, 0.0F);

	float minDistance = std::numeric_limits<float>::max(), distance;
	bool existIntersection = false;

	Object* intersectedObject = nullptr;
	Vector<float> surfaceNormal, L;

	if(prof > 7)
		return result;

	for(auto object: objects){
		Vector<float> currentNormal;
		if(object->intersect(ray, distance, currentNormal)){
			if(distance < minDistance){
				minDistance = distance;
				existIntersection = true;
				surfaceNormal = currentNormal;
				intersectedObject = object;
			}
		}
	}

	if(existIntersection && !intersectedObject->isLightSource){
		Point<float> intersectionPoint = ray.origin + minDistance*ray.direction;
		Vector<float> iluminations;
		Vector<float> difusa, especular;
		Vector<float> V = Math::normalize(-1.0F*ray.direction);

		Color reflexivo(0,0,0);
		Color refractivo(0,0,0);
		
		float kr = intersectedObject->ks;
		float kt = 0;
		
		for(auto light: lights){
			L = Math::normalize(light->position - intersectionPoint);
			Ray shadowRay(intersectionPoint, L);
			bool existShadow = false;

			for(auto shadowObject: objects){
				if(!shadowObject->isLightSource && shadowObject != intersectedObject && shadowObject->intersect(shadowRay)){
					existShadow = true;
					break;
				}
			}
			if(!existShadow){
				float factorDifuso = Math::dotProduct(surfaceNormal, L);
				if(factorDifuso > 0)
					difusa = light->color * intersectedObject->kd * factorDifuso;

				Vector<float> R = Math::normalize(2*(Math::dotProduct(L, surfaceNormal))*surfaceNormal - L);
				
				float factorEspecular = Math::dotProduct(R, V);
				if(factorEspecular > 0)
					especular = light->color * intersectedObject->ks * std::pow(factorEspecular, intersectedObject->n);
				iluminations += difusa + especular;
			}

			bool outside = Math::dotProduct(ray.direction, surfaceNormal) < 0;
			Vector<float> bias = 0.001 * surfaceNormal;

			if(intersectedObject->idr > 0){
				fresnel(ray.direction, surfaceNormal, intersectedObject->idr, kr);
				if(kr < 1){
					kt = 1 - kr;
					Ray rayoRefractivo;
					rayoRefractivo.origin = outside ? intersectionPoint - bias: intersectionPoint + bias;
					rayoRefractivo.direction = Math::normalize(refract(ray.direction, surfaceNormal, intersectedObject->idr));
					refractivo = calculateColor(rayoRefractivo, objects, lights, ambient, prof + 1);
					
				}
			}
			if(kr > 0){
				Ray rayoReflexivo;
				rayoReflexivo.origin = outside ? intersectionPoint - bias: intersectionPoint + bias;
				rayoReflexivo.direction = Math::normalize(2 * Math::dotProduct(V, surfaceNormal) * surfaceNormal - V );
				reflexivo = calculateColor(rayoReflexivo, objects, lights, ambient, prof + 1);
			}
		}
		
		iluminations += intersectedObject->ka * ambient->color;
		result = intersectedObject->color * iluminations;
		result = result + reflexivo*kr + refractivo*kt;
		result.maxToOne();
	}
	else if(existIntersection && intersectedObject->isLightSource){
		result = intersectedObject->color;
	}
	return result;
}

Vector<float> Camera::refract(class Vector<float> I, class Vector<float> N, float ior){
	float cosi = Math::clamp(-1, 1, Math::dotProduct(I, N));
	float etai = 1, etat = ior;
	Vector<float> n = N;
	if(cosi < 0)
		cosi = -cosi;
	else{
		std::swap(etai, etat);
		n = -1.0F*N;
	}
	float eta = etai / etat;
	float k = 1 - eta * eta * (1 - cosi * cosi);
	return k < 0? Vector<float>(0, 0, 0) : eta * I + (eta * cosi - std::sqrt(k)) * n;
}

void Camera::fresnel(class Vector<float> I, class Vector<float> N, float &ior, float &kr){
	float cosi = Math::clamp(-1.0F, 1.0F, Math::dotProduct(I, N));
	float etai = 1, etat = ior;
	if(cosi > 0)
		std::swap(etai, etat);
	float sint = etai / etat * std::sqrt(std::max(0.0F, 1 - cosi * cosi));
	if(sint >= 1)
		kr = 1;
	else{
		float cost = std::sqrt(std::max(0.0F, 1 - sint * sint));
		cosi = std::abs(cosi);
		float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
        kr = (Rs * Rs + Rp * Rp) / 2;
	}
}
