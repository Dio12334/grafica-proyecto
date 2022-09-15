#include <iostream>
#include "Display.h"


int main(int argc, char **argv){
	Display display;
		
	if(display.initialize(1366, 768))
		display.runLoop();
	display.shutdown();
    return 0;
}
