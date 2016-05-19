#pragma once

//#include <iostream>

#include "IwGL.h"
#include "IwGeomFVec2.h"
#include "IwGeomFVec3.h"
#include "IwGeomFVec4.h"

#include "Matrix3.h"
#include "Matrix4.h"

typedef CIwFVec2 vec2;
typedef CIwFVec3 vec3;
typedef CIwFVec4 vec4;

class Program
{
public:
	Program(const char* vsFile, const char* fsFile);
	~Program();

    static GLuint createShader(GLenum type, const char* fileName);
    //static GLuint createShader2(GLenum type, const char* fileName);
    static int readFile(const char* fileName, char** buffer, bool binary);
    static void printShaderInfoLog(GLuint shader);

	void use() const;
	void stopUsing() const;

	GLint attrib(const GLchar* attribName) const;
	GLint uniform(const GLchar* uniformName) const;

	GLuint getId() const;

    void setUniform(const GLchar* name, int x);
    void setUniform(const GLchar* name, float x);
    void setUniform(const GLchar* name, int x, int y);
    void setUniform(const GLchar* name, float x, float y);
    void setUniform(const GLchar* name, vec2& vec);
    void setUniform(const GLchar* name, int x, int y, int z);
    void setUniform(const GLchar* name, float x, float y, float z);
    void setUniform(const GLchar* name, vec3& vec);
    void setUniform(const GLchar* name, int x, int y, int z, int w);
    void setUniform(const GLchar* name, float x, float y, float z, float w);
    void setUniform(const GLchar* name, vec4& vec);
    void setUniform(const GLchar* name, Matrix3& mat);
    void setUniform(const GLchar* name, Matrix4& mat);
    void setUniformMatrix2(const GLchar* name, float matrix[], int count, bool transpose);
    void setUniformMatrix3(const GLchar* name, float matrix[], int count, bool transpose);
    void setUniformMatrix4(const GLchar* name, float matrix[], int count, bool transpose);

    void setAttrib(const GLchar* name, float x);
    void setAttrib(const GLchar* name, float x, float y);
    void setAttrib(const GLchar* name, vec2& vec);
    void setAttrib(const GLchar* nane, float x, float y, float z);
    void setAttrib(const GLchar* nane, vec3& vec);
    void setAttrib(const GLchar* name, float x, float y, float z, float w);
    void setAttrib(const GLchar* name, vec4& vec);

private:

	GLuint id;

};