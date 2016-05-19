#pragma once

#include <vector>

#include "TestLevel.h"

#include "IwGeomVec2.h"

typedef CIwVec2 vec2i;

class GameState
{
public:

    GameState(TestLevel* level);
    virtual ~GameState();

    virtual void checkStateEnd();
    virtual void handleClick(vec2i down, vec2i up);

    virtual void hexClicked(vec2i hex);
    virtual void shipClicked(int index, int ship);
    virtual void reset();

    bool linearSearch(std::vector<vec2i>& points, vec2i point);

protected:

    TestLevel* level;
};