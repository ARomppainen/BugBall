#pragma once

#include <vector>

#include "GameState.h"
#include "HashSet.h"
#include "vec2iHash.h"

class BoardSelectionState : public GameState
{
public:

    BoardSelectionState(TestLevel* level);

    virtual void reset();
    virtual void shipClicked(int index, int ship);
    virtual void hexClicked(vec2i hex);

private:

    void yourCharacter(vec2i hex);
    void notYourCharacter(vec2i hex);

    void move(Character *selected, vec2i hex);
    void tackle(Character *selected, Character *target, vec2i hex);

    void goToAnimationState();
    void goToDefaultState();
    void goToKickState(vec2i ballPosition, Direction d, int skill);

    //void calcKickPoints();
    //void calcTacklePoints();
    void calcKickAndTacklePoints();
    void calcMovePoints();
    void calcMovePoints(HashSet<vec2iHash>& set, vec2i point, int moves);
    void calcMoveSprites();

private:

    std::vector<vec2i> movePoints;
};