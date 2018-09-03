#pragma once 
#include <string>
#include <SDL.h>
#include <glad/glad.h>
#include <memory>
#include "Shader.hpp"
#include "ProgramGL.hpp"

class Screen{
public:
	Screen(const char* title, int posX, int posY, int width, int height, Uint32 flags);
	void initSDL();
	void initGL();
	int getWidth();
	int getHeight();
	void Loop();
	void Close();
private:
	std::unique_ptr<ProgramGL> program;
	bool quitRequest;
	unsigned int VAO;
	struct 
	{
		std::string title;
		int posX;
		int posY;
		int width;
		int height;
		Uint32 flags;
		SDL_Window* ptr;
		SDL_GLContext context;
	}params;

	void EventHandler();
	void CloseError(const char *);
	void CloseError(const char * title, const char * msg);
	void CloseError(const char * title, std::exception& e);

};