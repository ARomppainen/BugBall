#pragma once

#include <vector>

#include "Camera.h"
#include "Drawable.h"

#include "IwGeomFVec3.h"

typedef CIwFVec3 vec3;

class Ship
{
public:

    Ship(vec3 pos);
    ~Ship();

    void draw(Camera& c);

    int pointIsInside(vec2 point);
    Drawable& getSprite(int index);

private:

    std::vector<Drawable> tiles;
    vec3 position;
    int width;
    int height;
};