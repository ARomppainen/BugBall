#include "GameState.h"

#include "Util.h"

GameState::GameState(TestLevel* level)
{
    this->level = level;
    level->moveSprites.clear();
}

GameState::~GameState()
{

}

void GameState::checkStateEnd()
{
    // do nothing
}

void GameState::handleClick(vec2i down, vec2i up)
{
    // coords in ui space
    vec2 uiCoords1 = Util::worldCoords(
        vec2(down.x, down.y),
        level->screenWidth,
        level->screenHeight,
        level->UIcamera->getZoom(),
        level->UIcamera->transform.position);

    vec2 uiCoords2 = Util::worldCoords(
        vec2(up.x, up.y),
        level->screenWidth,
        level->screenHeight,
        level->UIcamera->getZoom(),
        level->UIcamera->transform.position);

    // handle ui elements

    // end turn button
    if (Util::pointInsideSprite(uiCoords1, level->endTurnButton.transform) &&
        Util::pointInsideSprite(uiCoords2, level->endTurnButton.transform)) {
            level->endTurnButton.click();

            //level->test.render = level->test.render ? false : true;

            return;
    }

    // coords in game space
    vec2 gameCoords1 = Util::worldCoords(
        vec2(down.x, down.y),
        level->screenWidth,
        level->screenHeight,
        level->camera->getZoom(),
        level->camera->transform.position);

    vec2 gameCoords2 = Util::worldCoords(
        vec2(up.x, up.y),
        level->screenWidth,
        level->screenHeight,
        level->camera->getZoom(),
        level->camera->transform.position);

    int r1, r2;

    r1 = level->team1Ship.pointIsInside(gameCoords1);
    r2 = level->team1Ship.pointIsInside(gameCoords2);

    if (r1 != -1 && r1 == r2) {
        shipClicked(r1, 1);
        return;
    }

    r1 = level->team2Ship.pointIsInside(gameCoords1);
    r2 = level->team2Ship.pointIsInside(gameCoords2);

    if (r1 != -1 && r1 == r2) {
        shipClicked(r1, 2);
        return;
    }

    vec2i ret1 = level->board.pointInsideHex(gameCoords1);
    vec2i ret2 = level->board.pointInsideHex(gameCoords2);

    if (ret1.x != -1 && ret1 == ret2) {
        hexClicked(ret1);
    } else if (level->touch.dist() < 10) {
        reset();
    }
}

void GameState::hexClicked(vec2i hex)
{
    // do nothing
}

void GameState::shipClicked(int index, int ship)
{
    // do nothing
}

void GameState::reset()
{
    // do nothing
}

bool GameState::linearSearch(std::vector<vec2i>& points, vec2i point)
{
    bool contains = false;

    for (unsigned i = 0; i < points.size(); ++i) {
        if (points[i] == point) {
            contains = true;
            break;
        }
    }

    return contains;
}