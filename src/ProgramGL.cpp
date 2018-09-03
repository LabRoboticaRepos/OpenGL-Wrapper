#include <memory>
#include <stdexcept>
#include <SDL.h>
#include "glad/glad.h"
#include "ProgramGL.hpp"
#include "Shader.hpp"

ProgramGLException::ProgramGLException(const char* _msg):msg("PROGRAM: "){msg += _msg;}
ProgramGLException::ProgramGLException(const char* _msg, const char* _desc):msg("PROGRAM: "){
    msg += _msg;
    msg += "\t\t";
    msg += _desc;
}
const char* ProgramGLException::what(){return msg.c_str();}


ProgramGL::ShadersPaths::ShadersPaths(const char* _vertex, const char* _fragment, const char* _geometry = nullptr):vertex(_vertex),fragment(_fragment), geometry(_geometry){

}
ProgramGL::ProgramGL(ShadersPaths& shadersPaths){
	id = glCreateProgram();
	if(shadersPaths.vertex != nullptr){
		shaders.vertex = std::make_unique<Shader>(shadersPaths.vertex, GL_VERTEX_SHADER);
		shaders.vertex->Compile();
	}
	
	if(shadersPaths.geometry != nullptr){
		shaders.geometry = std::make_unique<Shader>(shadersPaths.geometry, GL_GEOMETRY_SHADER);
		shaders.geometry->Compile();
	}
	
	if(shadersPaths.fragment != nullptr){
		shaders.fragment = std::make_unique<Shader>(shadersPaths.fragment, GL_FRAGMENT_SHADER);
		shaders.fragment->Compile();
	}
}
void ProgramGL::attachShaders(){
	if(shaders.vertex != nullptr){
		glAttachShader(id, shaders.vertex->getID());
	}
	if(shaders.geometry != nullptr){
		glAttachShader(id, shaders.geometry->getID());
	}
	if(shaders.fragment != nullptr){
		glAttachShader(id, shaders.fragment->getID());
	}
}
void ProgramGL::link(){
	int success;
	char infoLog[512];

    glLinkProgram(id);
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        throw ProgramGLException("LINKING_FAILED\n", infoLog);
    }

	if(shaders.vertex != nullptr){
	    shaders.vertex->Delete();
		shaders.vertex.reset();
	}
	if(shaders.geometry != nullptr){
		shaders.geometry->Delete();
		shaders.geometry.reset();
	}
	if(shaders.fragment != nullptr){
		shaders.fragment->Delete();
		shaders.fragment.reset();
	}
}
void ProgramGL::use(){
	glUseProgram(id);
}