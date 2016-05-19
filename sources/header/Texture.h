#pragma once

#include "IwGL.h"
#include "IwImage.h"

typedef CIwImage Image;

class Texture
{
public:
	Texture(const char* fileName, int format);
	~Texture();

    GLuint getId() const;
    GLsizei getWidth() const;
    GLsizei getHeight() const;


private:

    GLuint id;
    GLsizei width;
    GLsizei height;
};