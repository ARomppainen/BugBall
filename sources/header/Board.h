#pragma once

#include <vector>

#include "IwGeomVec2.h"

#include "HexagonCell.h"
#include "Camera.h"

typedef CIwVec2 vec2i;

class Board
{
public:

	Board();
	Board(int height, int width);
	~Board();

    HexagonCell& getHex(int row, int column);
    HexagonCell& getHex(vec2i point);
    // returns array indices if hex found, (-1, -1) otherwise
    vec2i pointInsideHex(vec2 point);
    void draw(Camera& c);

    void moveCharacter(vec2i from, vec2i to);
    void setCharacter(Character* c, int row, int column);
    void setCharacter(Character* c, vec2i point);
    void removeCharacter(int row, int column);
    void removeCharacter(vec2i point);

    std::vector<vec2i> getAllNeighbors(vec2i point);
    std::vector<vec2i> getAllNeighbors(unsigned row, unsigned column);
    std::vector<vec2i> getBallMovePoints(vec2i point);
    std::vector<vec2i> getBallMovePoints(unsigned row, unsigned column);
    std::vector<vec2i> getFreeNeighbors(vec2i point);
    std::vector<vec2i> getFreeNeighbors(unsigned row, unsigned column);
    vec2i getNeighbor(unsigned row, unsigned column, Direction d); // returns (-1, -1) if none found

    // returns direction from coords p1 to p2
    // return -1 if the hexagons are not next to each other
    int getDirection(vec2i p1, vec2i p2);
    int distance(vec2i p1, vec2i p2);

    void createBoardGrassTiles();

    int loadLevel(const char* xmlname);

private:

	std::vector<std::vector<HexagonCell> > hexagonCells;
};