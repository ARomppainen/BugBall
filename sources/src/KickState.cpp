#include "KickState.h"

#include "AnimationState.h"
#include "AssetFactory.h"
#include "BoardSelectionState.h"
#include "DefaultState.h"
#include "Layer.h"
#include "ShipSelectionState.h"
#include "Util.h"

KickState::KickState(TestLevel* level, vec2i ballPosition, Direction kickDirection, int skill) :
    GameState(level),
    ballPosition(ballPosition),
    kickDirection(kickDirection)
{
    calcKickPoints(skill);
}

void KickState::calcKickPoints(int skill)
{
    vec2i pos = ballPosition;

    for (int i = 0; i < skill; ++i) {
         vec2i newPos = level->board.getNeighbor(pos.x, pos.y, kickDirection);

        if (level->board.getHex(newPos).getScoringArea() != 0) {
            kickPoints.push_back(newPos);
            break;
        } else {
            if (level->board.getHex(newPos).isPlayground()) {
                if (level->board.getHex(newPos).isFree()) {
                    kickPoints.push_back(newPos);
                } else {
                    break;
                }
            } else {
                switch (kickDirection) {
                    case LEFT:          switchDirectionLeft(newPos);        break;
                    case RIGHT:         switchDirectionRight(newPos);       break;
                    case TOPLEFT:       switchDirectionTopLeft(newPos);     break;
                    case TOPRIGHT:      switchDirectionTopRight(newPos);    break;
                    case BOTTOMLEFT:    switchDirectionBottomLeft(newPos);  break;
                    case BOTTOMRIGHT:   switchDirectionBottomRight(newPos); break;
                }

                newPos = level->board.getNeighbor(pos.x, pos.y, kickDirection);

                if (level->board.getHex(newPos).isFree()) {
                    kickPoints.push_back(newPos);
                } else {
                    break;
                }
            }
        }

        pos = newPos;
    }

    calcKickSprites();
}

void KickState::calcKickSprites()
{
    if (level->kickSprites.size() > 0) {
        level->kickSprites.clear();
    }

    for (size_t i = 0; i < kickPoints.size(); ++i) {
        Drawable d;

        if (level->board.getHex(kickPoints[i]).getScoringArea() != 0) {
            d.material.setColor(vec3(1, 1, 0));
            d.coords.x = 0;
            d.coords.y = 384;
            d.coords.width = 96;
            d.coords.height = 96;
            //d.transform.setScale(0.5, 1, 1);
        } else {
            //d.material.color = new vec3(0, 1, 0);
            d.material.setColor(vec3(0, 1, 0));
            d.coords.x = 0;
            d.coords.y = 128 * 2;
            d.coords.width = 128;
            d.coords.height = 64;
            d.transform.setScale(1, 0.5, 1);
        }

        d.shape = AssetFactory::getInstance().getShape("SPRITE");
        d.material.texture = AssetFactory::getInstance().getTexture("UI_HEXATLAS");
        d.useAtlas = true;

        vec3 point = level->board.getHex(kickPoints[i]).transform.position;
        d.transform.setPosition(point.x, point.y, CHARACTER_FRONT);

        level->kickSprites.push_back(d);
    }
}

void KickState::switchDirectionLeft(vec2i& newPos)
{
    int rows = 9; // TODO: add getter for this in board class

    if (newPos.x < rows / 2) {
        kickDirection = BOTTOMRIGHT;
    } else {
        kickDirection = TOPRIGHT;
    }
}

void KickState::switchDirectionRight(vec2i& newPos)
{
    int rows = 9;

    if (newPos.x < rows / 2) {
        kickDirection = BOTTOMLEFT;
    } else {
        kickDirection = TOPLEFT;
    }
}

void KickState::switchDirectionTopLeft(vec2i& newPos)
{
    vec2i pos(newPos.x + 1, newPos.y - 1);

    if (level->board.getHex(pos).isPlayground()) {
        kickDirection = BOTTOMLEFT;
    } else {
        kickDirection = RIGHT;
    }
}

void KickState::switchDirectionTopRight(vec2i& newPos)
{
    vec2i pos(newPos.x + 1, newPos.y);
    
    if (level->board.getHex(pos).isPlayground()) {
        kickDirection = BOTTOMRIGHT;
    } else {
        kickDirection = LEFT;
    }
}

void KickState::switchDirectionBottomLeft(vec2i& newPos)
{
    vec2i pos(newPos.x - 1, newPos.y - 1);

    if (level->board.getHex(pos).isPlayground()) {
        kickDirection = TOPLEFT;
    } else {
        kickDirection = RIGHT;
    }
}

void KickState::switchDirectionBottomRight(vec2i& newPos)
{
    vec2i pos(newPos.x - 1, newPos.y);

    if (level->board.getHex(pos).isPlayground()) {
        kickDirection = TOPRIGHT;
    } else {
        kickDirection = LEFT;
    }
}

void KickState::reset()
{
    level->resetAnimationBoard();
    level->resetBoardSelection();
    goToDefaultState();
}

void KickState::shipClicked(int index, int ship)
{
    if ((ship == 1 && level->activeTeam == 1 && level->team1[index].isInShip()) ||
        (ship == 2 && level->activeTeam == 2 && level->team2[index].isInShip()) ) {

            level->ball->setAnimation(level->ball->getName() + "UnCurl");
            level->ball->addFollowingAnimation(level->ball->getName() + "Stop");
            level->resetAnimationBoard();
            level->resetBoardSelection();
            level->setShipSelection(index, ship);
            level->setAnimationShip("Idle");
            level->nextState = new ShipSelectionState(level);
            level->kickSprites.clear();
    }
}

void KickState::hexClicked(vec2i hex)
{
    if (linearSearch(kickPoints, hex)) {
        Character* c = level->board.getHex(level->boardSelection).getCharacter();
        c->spendAction();

        c->setKickPosition(level->ball->getPosition());    // UUSI SYSTEEMI!!!

        // ÄIJÄ EI ENÄÄ LIIKU POTKUPISTEESEEN!!!
        level->makeKick(ballPosition, hex, kickPoints);
        c->setAnimation(c->getName() + "Kick");
        c->addFollowingAnimation(c->getName() + "Idle");
        //level->setBoardSelection(ballPosition);

        int score = level->board.getHex(hex).getScoringArea();

        if (score != 0) {
            level->goal(score);
        }

        goToAnimationState();

    } else if ( !level->board.getHex(hex).isFree() ) {
        level->ball->setAnimation(level->ball->getName() + "UnCurl");
        level->ball->addFollowingAnimation(level->ball->getName() + "Stop");
        level->resetAnimationBoard();
        level->setBoardSelection(hex);
        level->setAnimationBoard("Idle");
        goToBoardSelectionState();
    } else {
        level->ball->setAnimation(level->ball->getName() + "UnCurl");
        level->ball->addFollowingAnimation(level->ball->getName() + "Stop");
        reset();
    }
}

void KickState::goToAnimationState()
{
    level->kickSprites.clear();
    level->nextState = new AnimationState(this->level);
}

void KickState::goToBoardSelectionState()
{
    level->kickSprites.clear();
    level->nextState = new BoardSelectionState(this->level);
}

void KickState::goToDefaultState()
{
    level->kickSprites.clear();
    level->nextState = new DefaultState(this->level);
}