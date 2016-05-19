#pragma once

#include "GameState.h"
#include "TestLevel.h"

class ShipSelectionState : public GameState
{
public:

    ShipSelectionState(TestLevel* level);

    virtual void reset();
    virtual void shipClicked(int index, int ship);
    virtual void hexClicked(vec2i hex);

private:

    void init();

    void playCharacter(vec2i hex);

    void goToDefaultState();
    void goToBoardSelectionState();

private:

    int count; // the amount of players on the field from the selected team
};