#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <memory>
#include "glad/glad.h"
#include "Shader.hpp"

ShaderException::ShaderException(const char* _msg):msg("SHADER: "){
    msg += _msg;
}
ShaderException::ShaderException(const char* _msg, const char* _desc):msg("SHADER: "){
    msg += _msg;
    msg += "\t\t";
    msg += _desc;
}
const char* ShaderException::what(){
    return msg.c_str();
}

Shader::Shader(const char* PATH, unsigned int _flag):flag(_flag){
    std::ifstream ShaderFile;
    ShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        ShaderFile.open(PATH);
        std::stringstream ShaderStream;
        ShaderStream << ShaderFile.rdbuf();
        ShaderFile.close();

        Code = std::make_unique<std::string>(ShaderStream.str());
    }
    catch(std::ifstream::failure e)
    {
        throw ShaderException("FILE_NOT_SUCCESFULLY_READ");
    }
}

void Shader::Compile(){
    int success;
    char infoLog[512];
    auto ShaderCode = Code->c_str();

    id = glCreateShader(flag);
    glShaderSource(id, 1, &ShaderCode, NULL);
    glCompileShader(id);
    // print compile errors if any
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        throw ShaderException("COMPILATION_FAILED\n",infoLog);
    };
}

const unsigned int& Shader::getID() const{
    return id;
}
void Shader::Delete(){
    Code.reset();
    glDeleteShader(id);
}
