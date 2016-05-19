#include "ShipSelectionState.h"

#include "AssetFactory.h"
#include "BoardSelectionState.h"
#include "DefaultState.h"
#include "Layer.h"

#define MAX_PLAYERS 5

ShipSelectionState::ShipSelectionState(TestLevel* level) :
    GameState(level)
{
    init();
}

void ShipSelectionState::init()
{
    int size;

    if (level->selectedShip == 1) {
        size = level->team1placementHexes.size();
    } else { // if (level->selectedShip == 2)
        size = level->team2placementHexes.size();
    }

    // laske kenttäpelaajat, jos 5 -> kentälle ei voi enää pelata

    count = 0;

    if (level->selectedShip == 1) {
        for (size_t i = 0; i < level->team1.size(); ++i) {
            if (level->team1[i].isInBoard()) {
                ++count;
            }
        }
    } else {
        for (size_t i = 0; i < level->team2.size(); ++i) {
            if (level->team2[i].isInBoard()) {
                ++count;
            }
        }
    }

    if (count >= MAX_PLAYERS) {
        return;
    }

    for (int i = 0; i < size; ++i) {
        if ((level->selectedShip == 1 && !level->board.getHex(level->team1placementHexes[i]).isFree()) ||
            (level->selectedShip == 2 && !level->board.getHex(level->team2placementHexes[i]).isFree())) {
                continue;
        }

        Drawable d;

        d.shape = AssetFactory::getInstance().getShape("SPRITE");
        d.material.texture = AssetFactory::getInstance().getTexture("UI_HEXATLAS");
        d.material.color = new vec3(1, 1, 1);
        d.useAtlas = true;
        d.coords.x = 0;
        d.coords.y = 0;
        d.coords.width = 128;
        d.coords.height = 128;

        vec3 point;

        if (level->selectedShip == 1) {
            point = level->board.getHex(level->team1placementHexes[i]).transform.position;
        } else {
            point = level->board.getHex(level->team2placementHexes[i]).transform.position;
        }

        d.transform.setPosition(point.x, point.y, CHARACTER_BEHIND);
        level->moveSprites.push_back(d);
    }
}

void ShipSelectionState::reset()
{
    level->moveSprites.clear();
    level->resetAnimationShip();
    level->resetShipSelection();
    goToDefaultState();
}

void ShipSelectionState::shipClicked(int index, int ship)
{
    if ((ship == 1 && level->activeTeam == 1 && level->team1[index].isInShip()) ||
        (ship == 2 && level->activeTeam == 2 && level->team2[index].isInShip()) ) {

            level->resetAnimationShip();
            level->setShipSelection(index, ship);
            level->setAnimationShip("Idle");
            level->nextState = new ShipSelectionState(level);
    }
}

void ShipSelectionState::hexClicked(vec2i hex)
{
    if ( !level->board.getHex(hex).isPlayground() ) {
        reset();
        return;
    }

    if (level->board.getHex(hex).isFree()) {
        if ((level->selectedShip == 1 && linearSearch(level->team1placementHexes, hex)) ||
            (level->selectedShip == 2 && linearSearch(level->team2placementHexes, hex))) {
                if (count < MAX_PLAYERS) {
                    playCharacter(hex);
                    level->resetShipSelection();
                    level->setBoardSelection(hex);
                    goToBoardSelectionState();
                } else {
                    reset();
                }
        } else {
            reset();
        }
    } else {
        level->resetAnimationShip();
        level->resetShipSelection();
        level->setBoardSelection(hex);
        level->setAnimationBoard("Idle");
        goToBoardSelectionState();
    }
}

void ShipSelectionState::playCharacter(vec2i hex)
{
    Character* selected;

    if (level->selectedShip == 1) {
        selected = &level->team1[level->shipSelection];

    } else if (level->selectedShip == 2) {
        selected = &level->team2[level->shipSelection];
    }

    vec3 pos = level->board.getHex(hex).transform.position;
    selected->transform.setPosition(pos.x, pos.y, CHARACTER + (pos.z - BOARD));

    selected->setBoard();
    level->board.setCharacter(selected, hex);
    selected->spendAction();

    if (selected->getRemainingActions() <= 0 && selected->getRemainingMoves() <= 0) {
        if ( !level->canKick(selected->getPosition(), level->ball->getPosition()) ) {
            if (selected->getKickPosition().x == -1 ||
                selected->hasMovedToKickPos()) {
                    selected->swapHighlight();
            }
        }
    }

    selected->highlightHex.render = true;
    level->charactersInBoard.push_back(selected);
}

void ShipSelectionState::goToBoardSelectionState()
{
    level->nextState = new BoardSelectionState(this->level);
}

void ShipSelectionState::goToDefaultState()
{
    level->nextState = new DefaultState(this->level);
}