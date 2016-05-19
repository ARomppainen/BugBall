#pragma once

#include "GameState.h"

class AnimationState : public GameState
{
public:

    AnimationState(TestLevel* level);

    virtual void checkStateEnd();

private:

    void goToDefaultState();
    void goToBoardSelectionState();
};