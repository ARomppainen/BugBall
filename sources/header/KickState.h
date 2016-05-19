#pragma once

#include "GameState.h"

class KickState : public GameState
{
public:

    KickState(TestLevel* level, vec2i ballPosition, Direction kickDirection, int skill);

    
    virtual void reset();
    virtual void shipClicked(int index, int ship);
    virtual void hexClicked(vec2i hex);

private:

    void calcKickPoints(int skill);
    void calcKickSprites();

    void switchDirectionLeft(vec2i& newPos);
    void switchDirectionRight(vec2i& newPos);
    void switchDirectionTopLeft(vec2i& newPos);
    void switchDirectionTopRight(vec2i& newPos);
    void switchDirectionBottomLeft(vec2i& newPos);
    void switchDirectionBottomRight(vec2i& newPos);

    void goToAnimationState();
    void goToBoardSelectionState();
    void goToDefaultState();

private:

    vec2i ballPosition;
    Direction kickDirection;
    std::vector<vec2i> kickPoints;
};