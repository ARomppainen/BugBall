#pragma once

#include "GameState.h"

class DefaultState : public GameState
{
public:

    DefaultState(TestLevel* level);

    virtual void shipClicked(int index, int ship);
    virtual void hexClicked(vec2i hex);
};