#pragma once

#include <iostream>

#include "IwGL.h"

class Shader
{
public:
	Shader(const char* source, GLenum shaderType);
	~Shader();

	static Shader shaderFromFile(GLenum shaderType, const std::string& filePath);

	GLuint getId();

    void deleteShader();
    void printShaderInfoLog(GLuint shader);

private:

	GLuint id;
};

