#pragma once

#include <iostream>
#include "Util.h"
#include "IwGeomVec2.h"

typedef CIwVec2 vec2i;

class vec2iHash
{
public:

    vec2iHash(vec2i v) :
        vec(v)
    {

    }

    vec2i vec;

    int getHash() const
    {
        std::string s;
        s.push_back(vec.x);
        s.push_back(':');
        s.push_back(vec.y);
        return Util::getHash(s);
    }

    bool operator==(const vec2iHash& rhs) const
    {
        return (this->vec.x == rhs.vec.x &&
                this->vec.y == rhs.vec.y);
    }
};