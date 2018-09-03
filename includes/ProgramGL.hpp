#pragma once 
#include <memory>
#include <stdexcept>
#include "Shader.hpp"
class ProgramGLException: public std::exception{
	public:
		ProgramGLException(const char* msg);
		ProgramGLException(const char* _msg, const char* _desc);
		virtual const char* what();
	private:
		std::string msg;
};

class ProgramGL{
	public:
		struct ShadersPaths{
			const char* vertex;
			const char* geometry;
			const char* fragment;
			ShadersPaths(const char*,const char*,const char*);
		};
		ProgramGL(ShadersPaths& shadersPaths);

		void attachShaders();
		void link();
		void use();
	private:
		struct {
			std::unique_ptr<Shader> vertex;
			std::unique_ptr<Shader> geometry;
			std::unique_ptr<Shader> fragment;
		}shaders;

		unsigned int id;
		void attachShader(unsigned int shaderConst);
};