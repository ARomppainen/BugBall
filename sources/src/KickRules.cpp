#include "KickRules.h"

KickRules::KickRules()
{

}

void KickRules::calcKickPoints(Character *ch,
                               Ball *ball,
                               Board *board,
                               Direction dir,
                               std::vector<vec2i> *points)
{
    vec2i pos = ball->getPosition();
    Direction kickDirection = dir;

    for (int i = 0; i < ch->getSkill(); ++i) {
        vec2i newPos = board->getNeighbor(pos.x, pos.y, kickDirection);

        if (board->getHex(newPos).getScoringArea() != 0) {
            points->push_back(newPos);
            break;
        } else if (board->getHex(newPos).isPlayground()) {
            if (board->getHex(newPos).isFree()) {
                points->push_back(newPos);
            } else {
                break;
            }
        } else {
            switch (kickDirection) {
                case LEFT:          kickDirection = switchDirectionLeft(newPos, board);        break;
                case RIGHT:         kickDirection = switchDirectionRight(newPos, board);       break;
                case TOPLEFT:       kickDirection = switchDirectionTopLeft(newPos, board);     break;
                case TOPRIGHT:      kickDirection = switchDirectionTopRight(newPos, board);    break;
                case BOTTOMLEFT:    kickDirection = switchDirectionBottomLeft(newPos, board);  break;
                case BOTTOMRIGHT:   kickDirection = switchDirectionBottomRight(newPos, board); break;
            }

            newPos = board->getNeighbor(pos.x, pos.y, kickDirection);

            if (board->getHex(newPos).isFree()) {
                points->push_back(newPos);
            } else {
                break;
            }
        }
    }
}

Direction KickRules::switchDirectionLeft(vec2i& newPos, Board *board)
{
    int rows = 9;

    if (newPos.x < rows / 2) {
        return BOTTOMRIGHT;
    } else {
        return TOPRIGHT;
    }
}

Direction KickRules::switchDirectionRight(vec2i& newPos, Board *board)
{
    int rows = 9;

    if (newPos.x < rows / 2) {
        return BOTTOMLEFT;
    } else {
        return TOPLEFT;
    }
}

Direction KickRules::switchDirectionTopLeft(vec2i& newPos, Board *board)
{
    vec2i pos(newPos.x + 1, newPos.y - 1);

    if (board->getHex(pos).isPlayground()) {
        return BOTTOMLEFT;
    } else {
        return RIGHT;
    }
}

Direction KickRules::switchDirectionTopRight(vec2i& newPos, Board *board)
{
    vec2i pos(newPos.x + 1, newPos.y);
    
    if (board->getHex(pos).isPlayground()) {
        return BOTTOMRIGHT;
    } else {
        return LEFT;
    }
}

Direction KickRules::switchDirectionBottomLeft(vec2i& newPos, Board *board)
{
    vec2i pos(newPos.x - 1, newPos.y - 1);

    if (board->getHex(pos).isPlayground()) {
        return TOPLEFT;
    } else {
        return RIGHT;
    }
}

Direction KickRules::switchDirectionBottomRight(vec2i& newPos, Board *board)
{
    vec2i pos(newPos.x - 1, newPos.y);

    if (board->getHex(pos).isPlayground()) {
        return TOPRIGHT;
    } else {
        return LEFT;
    }
}