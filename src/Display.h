#ifndef DISPLAY_H
#define DISPLAY_H

#include "Camera.h"
#include <SDL2/SDL.h>
#include <iostream>

class Display{

    public:

        Display();
        bool initialize(std::size_t width, std::size_t height);
        void runLoop();
        void shutdown();

    private:

        void processInputs();
        void updateDisplay();
        void generateOutput(); 
        bool isRunning;
        SDL_Window* window;
        SDL_Renderer* renderer;
        int screenSize;
		Camera camera;
		std::size_t ticksCount;
		
		class World* world;

};


int random(int low, int high);

#endif

