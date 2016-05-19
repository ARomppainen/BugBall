#pragma once

#include <vector>
#include "Ball.h"
#include "Board.h"
#include "Character.h"
#include "IwGeomVec2.h"

typedef CIwVec2 vec2i;

class KickRules
{
public:

    static void calcKickPoints(
        Character *ch,
        Ball *ball,
        Board *board,
        Direction dir,
        std::vector<vec2i> *points);

private:

    static Direction switchDirectionLeft(vec2i& newPos, Board *board);
    static Direction switchDirectionRight(vec2i& newPos, Board *board);
    static Direction switchDirectionTopLeft(vec2i& newPos, Board *board);
    static Direction switchDirectionTopRight(vec2i& newPos, Board *board);
    static Direction switchDirectionBottomLeft(vec2i& newPos, Board *board);
    static Direction switchDirectionBottomRight(vec2i& newPos, Board *board);

    KickRules();
};