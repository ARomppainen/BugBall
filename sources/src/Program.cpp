#include "Program.h"

#include <stdio.h>
#include <string.h>

//#include <fstream>
//#include <sstream>

Program::Program(const char* vsFile, const char* fsFile)
    : id(0)
{
	id = glCreateProgram();

    GLuint vs = createShader(GL_VERTEX_SHADER, vsFile);
    GLuint fs = createShader(GL_FRAGMENT_SHADER, fsFile);

    glAttachShader(id, vs);
    glAttachShader(id, fs);

	glLinkProgram(id);

    int linked = 0;
    glGetProgramiv(id, GL_LINK_STATUS, &linked);

    if (!linked)
    {
        GLint infoLen = 0;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLen);

        char* infoLog = NULL;
        if (infoLen > 1)
        {
            infoLog = (char*) malloc(sizeof(char) * infoLen);
            glGetProgramInfoLog(id, infoLen, NULL, infoLog );
        }

        s3eDebugErrorPrintf("Failed to link program: %s", infoLog);

        free(infoLog);

        glDeleteProgram(id);
        //return 1;
        printf("GLSL PROGRAM LINKING FAILED, id: %i\n", id);

        //exit(1);
    }

    //glDetachShader(id, vertexShader.getId());
    //glDetachShader(id, fragmentShader.getId());

    //vertexShader.deleteShader();
    //fragmentShader.deleteShader();
}


Program::~Program()
{
    if (id != 0) {
        glDeleteProgram(id);
    }
}

/*
GLuint Program::createShader2(GLenum type, const char* fileName)
{
    std::ifstream file;
    file.open(fileName, std::ios::in || std::ios::binary);

    if (!file.is_open()) {
        return 0;
    }

    std::stringstream ss;
    ss << file.rdbuf();
    const char* code = ss.str().c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar**)&code, NULL);
    glCompileShader(shader);
    printShaderInfoLog(shader);

    return shader;
}
*/

GLuint Program::createShader(GLenum type, const char* fileName)
{
    char* buffer = NULL;
    int len = 0;

    char** tmp = new char*;

    len = readFile(fileName, tmp, false);

    buffer = *tmp;
    len = strlen(buffer);

    if (len == 0) {
        s3eDebugErrorPrintf("failed to load shader: %s", fileName);
        return 0;
    }

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar**)&buffer, &len);
    glCompileShader(shader);
    printShaderInfoLog(shader);

    delete tmp;

    return shader;
}

int Program::readFile(const char* fileName, char** buffer, bool binary)
{
    FILE* fp = fopen(fileName, binary ? "rb" : "r");
    
    if (fp == NULL)
        return 0;

    fseek(fp, 0, SEEK_END);
    int len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    (*buffer) = new char[len + 1];

    len = fread(*buffer, 1, len, fp);

    fclose(fp);

    (*buffer)[len] = 0;

    return len;
}

void Program::printShaderInfoLog(GLuint shader)
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
            printf("SHADER COMPILATION FAILED... SOMEHOW...\n");
            exit(1);
        }
    }
}

GLuint Program::getId() const
{
    return id;
}

void Program::use() const
{
    glUseProgram(id);
}

void Program::stopUsing() const
{
    glUseProgram(0);
}

GLint Program::attrib(const GLchar* attribName) const
{
    GLint attrib = glGetAttribLocation(id, attribName);
    
    return attrib;
}

GLint Program::uniform(const GLchar* uniformName) const
{
    GLint uniform = glGetUniformLocation(id, uniformName);

    return uniform;
}

void Program::setUniform(const GLchar* name, int x)
{
    int location = uniform(name);
    glUniform1i(location, x);
}

void Program::setUniform(const GLchar* name, float x)
{
    int location = uniform(name);
    glUniform1f(location, x);
}

void Program::setUniform(const GLchar* name, int x, int y)
{
    int location = uniform(name);
    glUniform2i(location, x, y);
}

void Program::setUniform(const GLchar* name, float x, float y)
{
    int location = uniform(name);
    glUniform2f(location, x, y);
}

void Program::setUniform(const GLchar* name, vec2& vec)
{
    int location = uniform(name);
    glUniform2f(location, vec.x, vec.y);
}

void Program::setUniform(const GLchar* name, int x, int y, int z)
{
    int location = uniform(name);
    glUniform3i(location, x, y, z);
}

void Program::setUniform(const GLchar* name, float x, float y, float z)
{
    int location = uniform(name);
    glUniform3f(location, x, y, z);
}

void Program::setUniform(const GLchar* name, vec3& vec)
{
    int location = uniform(name);
    glUniform3f(location, vec.x, vec.y, vec.z);
}

void Program::setUniform(const GLchar* name, int x, int y, int z, int w)
{
    int location = uniform(name);
    glUniform4i(location, x, y, z, w);
}

void Program::setUniform(const GLchar* name, float x, float y, float z, float w)
{
    int location = uniform(name);
    glUniform4f(location, x, y, z, w);
}

void Program::setUniform(const GLchar* name, vec4& vec)
{
    int location = uniform(name);
    glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}

void Program::setUniform(const GLchar* name, Matrix3& mat)
{
    int location = uniform(name);
    glUniformMatrix3fv(location, 1, false, mat.getValues());
}

void Program::setUniform(const GLchar* name, Matrix4& mat)
{
    int location = uniform(name);
    glUniformMatrix4fv(location, 1, false, mat.getValues());
}

void Program::setUniformMatrix2(const GLchar* name, float matrix[], int count, bool transpose)
{
    int location = uniform(name);
    glUniformMatrix2fv(location, count, transpose, matrix);
}

void Program::setUniformMatrix3(const GLchar* name, float matrix[], int count, bool transpose)
{
    int location = uniform(name);
    glUniformMatrix3fv(location, count, transpose, matrix);
}

void Program::setUniformMatrix4(const GLchar* name, float matrix[], int count, bool transpose)
{
    int location = uniform(name);
    glUniformMatrix4fv(location, count, transpose, matrix);
}

void Program::setAttrib(const GLchar* name, float x)
{
    int location = attrib(name);
    glVertexAttrib1f(location, x);
}

void Program::setAttrib(const GLchar* name, float x, float y)
{
    int location = attrib(name);
    glVertexAttrib2f(location, x, y);
}

void Program::setAttrib(const GLchar* name, vec2& vec)
{
    int location = attrib(name);
    glVertexAttrib2f(location, vec.x, vec.y);
}

void Program::setAttrib(const GLchar* name, float x, float y, float z)
{
    int location = attrib(name);
    glVertexAttrib3f(location, x, y, z);
}

void Program::setAttrib(const GLchar* name, vec3& vec)
{
    int location = attrib(name);
    glVertexAttrib3f(location, vec.x, vec.y, vec.z);
}

void Program::setAttrib(const GLchar* name, float x, float y, float z, float w)
{
    int location = attrib(name);
    glVertexAttrib4f(location, x, y, z, w);
}

void Program::setAttrib(const GLchar* name, vec4& vec)
{
    int location = attrib(name);
    glVertexAttrib4f(location, vec.x, vec.y, vec.z, vec.w);
}