#pragma once

#include "IwGeomFVec3.h"

#include "Texture.h"

typedef CIwFVec3 vec3;

class Material
{
public:

    Material();
    Material(const Material& other);
    ~Material();

    void setColor(vec3 color);

    Material& operator=(const Material& other);

    Texture* texture;
    vec3* color;
};