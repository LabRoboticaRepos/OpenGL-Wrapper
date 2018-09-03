#pragma once
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <memory>
#include <string>
#include "glad/glad.h"


class ShaderException: public std::exception{
	public:
		ShaderException(const char* msg);
		ShaderException(const char* _msg, const char* _desc);
		virtual const char* what();
	private:
		std::string msg;
};

class Shader{
	public:
		Shader(const char* PATH, unsigned int _flag);
	    void Compile();
	    void Delete();
	    const unsigned int& getID() const;
	private:
		const std::string filePATH;
		std::unique_ptr<std::string> Code;

		unsigned int id;
		unsigned int flag;
};