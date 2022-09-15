#include "Display.h"
#include <SDL2/SDL_timer.h>
#include <algorithm>
#include "Sphere.h"
#include "Light.h"
#include "Plane.h"
#include "Random.h"
#include "Cilinder.h"
#include "World.h"
#include "Firefly.h"
#include <cmath>

Display::Display(): isRunning(false), window(nullptr), renderer(nullptr), ticksCount(0), world(nullptr){}

bool Display::initialize(std::size_t width, std::size_t height){

    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if(sdlResult != 0){
        SDL_Log("No se pudo inicializar SDL: %s", SDL_GetError());
        return false;
    }
    window = SDL_CreateWindow("WorldGen", 
			SDL_WINDOWPOS_CENTERED, 
			SDL_WINDOWPOS_CENTERED, 
			width, 
			height, 
			SDL_WINDOW_SHOWN);

    if(!window){
        SDL_Log("Falla en la creación de la ventana: %s", SDL_GetError());
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!renderer){
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}
	isRunning = true;
   
	Random::init();
	world = new World();
	world->setRenderer(renderer);
	//set(vec3 _color, float _kd, float _ks=0, float _n=0, float _idr=0, float _kr=0
	world->addObject(
		new Plane(
				Point<float>(0, 0, 0), // position
				Color(1.0F, 1.0F, 1.0F),  // color
				0.01F,  // ka
				0.01F, // kd
				0.5F, // ks
				1.F, //kr
				20, // n
				0, // idr 
				Vector<float>(0,0,1).normalize() // d
				)
		);

	int begX = -7, endX = 9;
	int begY = 6, endY = -6;

	/* int begX = 1, endX = 1; */
	/* int begY = 1, endY = 1; */
	for(int x = begX; x <= endX; x += 3){
		for(int y = begY; y >= endY; y -= 3){
			world->addObject(new Sphere(
						Point<float>(x, y, 2), // position
						Color(Random::getVector(Vector<float>(0,0,0), Vector<float>(1,1,1))), //color;
						Random::getFloat(0, 0.01F),  // ka
						Random::getFloat(0,1), // kd
						Random::getFloat(0,1), // ks
						Random::getFloat(0,1), // kr
						Random::getInt(0, 100), //n
						Random::getFloat(0,0), // idr
						1)); // radius
		}
	}

	for(int i = 0; i < 5; ++i)
		world->addFirefly(new Firefly(Point<float>(1, 1, 10)));

	/* world->addObject(new Cilinder( */
	/* 					Point<float>(0,0,0), // position */
	/* 					Point<float>(0,0,20), // pb */
	/* 					Color(Random::getVector(Vector<float>(0,0,0), Vector<float>(1,1,1))), //color; */
	/* 					Random::getFloat(0, 1.F),  // ka */
	/* 					Random::getFloat(0,1), // kd */
	/* 					Random::getFloat(0,1), // ks */
	/* 					Random::getFloat(0,1), //kr */
	/* 					Random::getInt(0, 100), //n */
	/* 					Random::getFloat(1,2), // idr normalmente mayor a 1 */
	/* 					10)); // radius */

	world->addLight(		
			new Light(Point<float>(0,0,30), Color(1, 1, 1))
		//new Light{Point<float>(0,0,100), Color(1,0,0)}
			);

	world->setAmbient(new Light(Point<float>(30, 30, 30), Color(0.5,0.5,0.5)));
	world->setCamera(new Camera(width, height, 4, Math::toRadians(60.0F), Point<float>(30, 30, 10), Point<float>(0,0,10), Vector<float>(0,0,-1)));
    world->createScenario();
	return true;
}

void Display::runLoop(){
    while(isRunning){
        // std::cout<<"input\n";
        processInputs();
        // std::cout<<"diplay\n";
        updateDisplay();
         //std::cout<<"out\n";s
        generateOutput();
    }
}

void Display::shutdown(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void Display::processInputs(){
    SDL_Event event;
	while(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_QUIT:
				isRunning = false;
				break;
		}
	}

    const Uint8* state = SDL_GetKeyboardState(NULL);

    if(state[SDL_SCANCODE_ESCAPE])
        isRunning = false; 
	else if(state[SDL_SCANCODE_A]){
		float x, y, z;
		std::cout << "Set new position: ";
		std::cin >> x >> y >> z;
		Vector<float> input(x,y,z);
		world->changeCameraPosition(input);
	}
}

void Display::updateDisplay(){

	 /* while(!SDL_TICKS_PASSED(SDL_GetTicks(), ticksCount + 16)); */ 
	float deltaTime = (SDL_GetTicks() - ticksCount)/1000.0f;
	/* if(deltaTime > 0.0416) */
	/* 	deltaTime = 0.0416; */
	float angularVelocity = 0.33f;
	float radius = Math::length(world->camera->eye - world->camera->center);
	static float angle = 0.0f;
	angle += deltaTime*angularVelocity;
	if(angle >= 2*M_PI)
		angle = 0.0f;
	/* float cameraZ = world->camera->eye.z; */
	/* Vector<float> forwardVector = Math::normalize(world->camera->eye - world->camera->center); */
	/* float angle = std::asin(forwardVector.y) + angularVelocity*deltaTime; */
	/* Vector<float> newPos = radius * Vector<float>(std::cos(angle), std::sin(angle), 0); */
	/* newPos.z = cameraZ; */
	/* world->camera->eye = newPos; */
	world->camera->eye.x = std::cos(angle) * radius;
	world->camera->eye.y = std::sin(angle) * radius;
	world->camera->init();
	world->updatePositions(deltaTime);
	
	ticksCount = SDL_GetTicks();
}

void Display::generateOutput(){
	/* static int frames = 0; */
	/*  if(frames > 100) */ 
	/* 	 isRunning = false; */ 
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
	world->createScenario();
	world->render();
	SDL_RenderPresent(renderer);
	/* ++frames; */
	
}
