#include "AnimationState.h"

#include "BoardSelectionState.h"
#include "DefaultState.h"

AnimationState::AnimationState(TestLevel* level) :
    GameState(level)
{

}

void AnimationState::checkStateEnd()
{
    if (level->moves.empty()) {
        if (level->boardSelection.x == -1) {
            goToDefaultState();
        } else {
            goToBoardSelectionState();
        }
    }
}

void AnimationState::goToDefaultState()
{
    level->nextState = new DefaultState(level);
}

void AnimationState::goToBoardSelectionState()
{
    level->nextState = new BoardSelectionState(level);
}