#include <iostream>
#include <exception>
#include <stdlib.h>
#include <cstring>
#include <SDL.h>
#include "glad/glad.h"
#include "ProgramGL.hpp"
#include "Shader.hpp"
#include "Screen.hpp"

Screen::Screen(const char* title, int posX, int posY, int width, int height, Uint32 flags){
	params.title = std::string(title);
	params.posX  = posX;
	params.posY = posY;
	params.width = width;
	params.height = height;
	params.flags = flags;
}
void Screen::initSDL(){
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) 
		CloseError("SDL:SDL_FAIL_INIT", SDL_GetError());
	
	params.ptr = SDL_CreateWindow(params.title.c_str(), params.posX, params.posY, params.width, params.height, params.flags);
	if(params.ptr == nullptr)
		CloseError("SDL:SDL_FAIL_CREATE_WINDOW", SDL_GetError());

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


	params.context = SDL_GL_CreateContext(params.ptr);
	if(params.context == nullptr) 
		CloseError("SDL:SDL_FAIL_CREATE_CONTEXT", SDL_GetError()); 

	SDL_GL_SetSwapInterval(1);

	if(!gladLoadGLLoader(SDL_GL_GetProcAddress))
		CloseError("GLAD:GLAD_FAIL_INIT", "");;
	
	glViewport( 0, 0, params.width, params.height);
}
void Screen::initGL(){
	ProgramGL::ShadersPaths paths("shaders/vertex.vs","shaders/fragment.fs", nullptr);

	program = std::make_unique<ProgramGL>(paths);
	program->attachShaders();
	try{
		program->link();
	}catch(ProgramGLException e){
		CloseError(e.what());
	}

	float vertices[] = {
        // positions         // colors
         0.5f, +0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
         0.0f,  -0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
    };

    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Screen::Loop(){
	quitRequest = false;
	while(!quitRequest){
		EventHandler();

	    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
  
		program->use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

		SDL_GL_SwapWindow(params.ptr);
	}
}

void Screen::EventHandler(){
	static SDL_Event event; 
	while(SDL_PollEvent(&event)){;
		switch(event.type){
			case SDL_QUIT:
				quitRequest = true;
		}
	}

}
void Screen::Close(){
	SDL_Quit();
}
void Screen::CloseError(const char * msg){
	std::cerr << "ERROR:\n\t\t" << msg << std::endl;
	SDL_Quit();
	exit(-1);
}
void Screen::CloseError(const char* title, const char * msg){
	std::cerr << "ERROR:\n\t\t" << title << " :: " << msg << std::endl;
	SDL_Quit();
	exit(-1);
}
void Screen::CloseError(const char* title, std::exception& e){
	std::cerr << "ERROR:\n\t\t"<< title << " :: " << e.what() << std::endl;
	SDL_Quit();
	exit(-1);
}