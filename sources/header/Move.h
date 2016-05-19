#pragma once

#include <vector>

#include "Character.h"

class Move
{
public:
    Move(Character* object, uint64 duration, vec3 start, vec3 end);
    Move(Character* object, uint64 duration, std::vector<vec3> points);
    Move(Character* object, uint64 duration, uint64 delay, vec3 start, vec3 end);
    Move(Character* object, uint64 duration, uint64 delay, std::vector<vec3> point);
    ~Move();

    void update(uint64 deltaTime);
    vec3 currentPosition() const;
    bool isFinished() const;

private:

    Character* object;

    uint64 delay;
    uint64 duration;
    uint64 elapsed;
    std::vector<vec3> points;
};