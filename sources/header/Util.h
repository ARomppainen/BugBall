#pragma once

#include <iostream>

#include "Direction.h"
#include "Transform.h"

#include "IwGeomFVec2.h"
#include "IwGeomFVec3.h"

typedef CIwFVec3 vec3;
typedef CIwFVec2 vec2;

class Util
{
public:

    static int abs(int v);
    static int max(int v1, int v2);
    static int max(int v1, int v2, int v3);

    static int getHash(const std::string& str);

    static float lerp(float v1, float v2, float t);
    static vec3 lerp(vec3 v1, vec3 v2, float t);

    static bool equals(float f1, float f2, float delta);

    static vec2 worldCoords(
        vec2 point,
        float width,
        float height,
        float zoom,
        const vec3& position);

    static bool pointInsideSprite(vec2 p, const Transform& t);

    static Direction getOppositeDirection(Direction d);

private:
    Util();
};