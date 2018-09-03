#include <iostream>
#include "Screen.hpp"

int main(int argc, char ** args){
	Screen mainScreen(
					"Pruebas", 
					SDL_WINDOWPOS_CENTERED,	SDL_WINDOWPOS_CENTERED, 
					600, 600, 
					SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
	mainScreen.initSDL();
	mainScreen.initGL();
	mainScreen.Loop();
	mainScreen.Close();
	return 0;
}