#include "Shader.h"

#include <stdio.h>

#include "s3e.h"

Shader::Shader(const char* source, GLenum shaderType)
    : id(0)
{
	id = glCreateShader(shaderType);
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);
    printShaderInfoLog(id);
}

Shader::~Shader(void)
{

}

GLuint Shader::getId()
{
	return id;
}

void Shader::deleteShader()
{
    glDeleteShader(id);
}

void Shader::printShaderInfoLog(GLuint shader)
{
    GLint length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    if (length)
    {
        char* buffer = (char*)malloc( sizeof(char) * length ) ;
        glGetShaderInfoLog(shader, length, NULL, buffer);
        printf("%s", buffer);
        free( buffer ) ;
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (success != GL_TRUE) {
            exit(1);
        }
    }
}