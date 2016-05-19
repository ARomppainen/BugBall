#include "Util.h"

#include <cmath>

int Util::abs(int v)
{
    return v < 0 ? -v : v;
}

int Util::max(int v1, int v2)
{
    return v1 > v2 ? v1 : v2;
}

int Util::max(int v1, int v2, int v3)
{
    int m = max(v1, v2);
    return m > v3 ? m : v3;
}

int Util::getHash(const std::string& str)
{
    int hash = 0;
    int nMinus1 = str.size() - 1;

    for (int i = 0; i <= nMinus1; ++i) {
        hash += str[i] * (int)pow(31, nMinus1 - i);
    }

    return hash;
}

float Util::lerp(float v1, float v2, float t)
{
    return v1 + (v2 - v1) * t;
}

vec3 Util::lerp(vec3 v1, vec3 v2, float t)
{
    return v1 + (v2 - v1) * t;
}

bool Util::equals(float f1, float f2, float delta)
{
    if (f1 < f2 + delta && f1 > f2 - delta) {
        return true;
    }

    return false;
}

vec2 Util::worldCoords(vec2 point,
                       float width,
                       float height,
                       float zoom,
                       const vec3& position)
{
    float relativeX = point.x / width;
    float relativeY = point.y / height;

    float ar = width / height;

    float x = lerp(
        -ar * zoom + position.x,
         ar * zoom + position.x,
         relativeX);
    
    float y = Util::lerp(
         zoom + position.y,
        -zoom + position.y,
         relativeY);

    return vec2(x, y);
}

bool Util::pointInsideSprite(vec2 p, const Transform& t)
{
    float halfWidth = 0.5 * t.scale.x;
    float haldHeight = 0.5 * t.scale.y;

    if (p.x < t.position.x + halfWidth &&
        p.x > t.position.x - halfWidth &&
        p.y < t.position.y + haldHeight &&
        p.y > t.position.y - haldHeight) {
            return true;
        }

    return false;
}

Direction Util::getOppositeDirection(Direction d)
{
    Direction dir;

    switch(d)
    {
    case TOPLEFT:       dir =  BOTTOMRIGHT; break;
    case TOPRIGHT:      dir =  BOTTOMLEFT;  break;
    case RIGHT:         dir =  LEFT;        break;
    case BOTTOMLEFT:    dir =  TOPRIGHT;    break;
    case BOTTOMRIGHT:   dir =  TOPLEFT;     break;
    case LEFT:          dir =  RIGHT;       break;
    }

    return dir;
}