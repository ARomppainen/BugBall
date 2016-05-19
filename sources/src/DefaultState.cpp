#include "DefaultState.h"

#include "BoardSelectionState.h"
#include "ShipSelectionState.h"
#include "Util.h"

DefaultState::DefaultState(TestLevel* level) :
    GameState(level)
{

}

void DefaultState::shipClicked(int index, int ship)
{
    if ((ship == 1 && level->activeTeam == 1 && level->team1[index].isInShip()) ||
        (ship == 2 && level->activeTeam == 2 && level->team2[index].isInShip()) ) {
        level->setShipSelection(index, ship);
        level->setAnimationShip("Idle");
        level->nextState = new ShipSelectionState(level);
    }
}

void DefaultState::hexClicked(vec2i hex)
{
    if (level->board.getHex(hex).isPlayground()) {
        if ( !level->board.getHex(hex).isFree()) {
            if (level->board.getHex(hex).getCharacter()->getTeam() != 0) { // do not select ball
                level->setBoardSelection(hex);
                level->setAnimationBoard("Idle");
                level->nextState = new BoardSelectionState(level);
            }
        }
    }
}