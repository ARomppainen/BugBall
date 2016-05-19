#include "Board.h"

#include "AssetFactory.h"
#include "Layer.h"
#include "Util.h"

#include "tinyxml2.h"

Board::Board()
{

}

Board::Board(int height, int width)
{
	hexagonCells.resize(height);

	for(int i = 0; i < height; i++)
	{
		hexagonCells[i].resize(width);
	}
}

Board::~Board()
{
}

HexagonCell& Board::getHex(int row, int column)
{
    return hexagonCells[row][column];
}

HexagonCell& Board::getHex(vec2i point)
{
    return hexagonCells[point.x][point.y];
}

vec2i Board::pointInsideHex(vec2 point)
{
    vec2i ret(-1, -1);

    for (unsigned i = 0; i < hexagonCells.size(); ++i) {
        for (unsigned j = 0; j < hexagonCells[i].size(); ++j) {
            if (hexagonCells[i][j].pointIsInside(point)) {
                ret = vec2i(i, j);
                break;
            }
        }
    }

    return ret;
}

void Board::draw(Camera& c)
{
    for (unsigned i = 0; i < hexagonCells.size(); ++i) {
        for (unsigned j = 0; j < hexagonCells[i].size(); ++j) {
            hexagonCells[i][j].draw(c);
        }
    }
}

void Board::moveCharacter(vec2i from, vec2i to)
{
    Character* c = getHex(from).getCharacter();
    setCharacter(c, to);
    removeCharacter(from);
}

void Board::setCharacter(Character* c, int row, int column)
{
    c->setPosition(row, column);

    if (getHex(row, column).isPlayground()) {
        hexagonCells[row][column].setCharacter(c);
    } else {
        c->setOutOfBounds();
        c->highlightHex.render = false;
        //c->setPosition(vec2i(-1, -1));
    }

    /*
    hexagonCells[row][column].setCharacter(c);

    c->setPosition(row, column);

    if ( !getHex(row, column).isPlayground() ) {
        c->setOutOfBounds();
    }
    */
}

void Board::setCharacter(Character* c, vec2i point)
{
    setCharacter(c, point.x, point.y);
}

void Board::removeCharacter(int row, int column)
{
    hexagonCells[row][column].removeCharacter();
}

void Board::removeCharacter(vec2i point)
{
    removeCharacter(point.x, point.y);
}

std::vector<vec2i> Board::getAllNeighbors(vec2i point)
{
    return getAllNeighbors(point.x, point.y);
}

std::vector<vec2i> Board::getAllNeighbors(unsigned row, unsigned column)
{
    std::vector<vec2i> ret;

    for (int i = TOPLEFT; i <= LEFT; ++i) {
        vec2i vec = getNeighbor(row, column, static_cast<Direction>(i));

        if (vec.x != -1) {
            ret.push_back(vec);
        }
    }

    return ret;
}

std::vector<vec2i> Board::getBallMovePoints(vec2i point)
{
    return getBallMovePoints(point.x, point.y);
}

std::vector<vec2i> Board::getBallMovePoints(unsigned row, unsigned column)
{
    std::vector<vec2i> ret;

    for (int i = TOPLEFT; i <= LEFT; ++i) {
        vec2i vec = getNeighbor(row, column, static_cast<Direction>(i));

        if (vec.x != -1) {
            if (hexagonCells[vec.x][vec.y].isFree()) {
                if (hexagonCells[vec.x][vec.y].isPlayground() ||
                    hexagonCells[vec.x][vec.y].getScoringArea() != 0) {
                        
                    ret.push_back(vec);
                }
            }
        }
    }

    return ret;
}

std::vector<vec2i> Board::getFreeNeighbors(vec2i point)
{
    return getFreeNeighbors(point.x, point.y);
}

std::vector<vec2i> Board::getFreeNeighbors(unsigned row, unsigned column)
{
    std::vector<vec2i> ret;

    for (int i = TOPLEFT; i <= LEFT; ++i) {
        vec2i vec = getNeighbor(row, column, static_cast<Direction>(i));

        if (vec.x != -1) {
            if (hexagonCells[vec.x][vec.y].isPlayground() &&
                hexagonCells[vec.x][vec.y].isFree()) {
                    ret.push_back(vec);
                }
        }
    }

    return ret;
}

vec2i Board::getNeighbor(unsigned row, unsigned column, Direction d)
{
    vec2i ret(-1, -1);

    switch(d) {
        case TOPLEFT:
            if (row > 0) {
                if (row % 2 == 0) {
                    if (column > 0) {
                        ret = vec2i(row - 1, column - 1);
                    }
                } else {
                    ret = vec2i(row - 1, column);
                }
            }
            break;

        case TOPRIGHT:
            if (row > 0) {
                if (row % 2 == 0) {
                    ret = vec2i(row - 1, column);
                } else {
                    if (column < hexagonCells[row].size() - 1) {
                        ret = vec2i(row - 1, column + 1);
                    }
                }
            }
            break;

        case RIGHT:
            if (column < hexagonCells[row].size() - 1) {
                ret = vec2i(row, column + 1);
            }
            break;

        case BOTTOMRIGHT:
            if (row < hexagonCells.size() - 1) {
                if (row % 2 == 0) {
                    ret = vec2i(row + 1, column);
                } else {
                    if (column < hexagonCells[row].size() - 1) {
                        ret = vec2i(row + 1, column + 1);
                    }
                }
            }
            break;

        case BOTTOMLEFT:
            if (row < hexagonCells.size() - 1) {
                if (row % 2 == 0) {
                    if (column > 0) {
                        ret = vec2i(row + 1, column - 1);
                    }
                } else {
                    ret = vec2i(row + 1, column);
                }
            }
            break;

        case LEFT:
            if (column > 0) {
                ret = vec2i(row, column - 1);
            }
            break;
    }

    return ret;
}

int Board::getDirection(vec2i p1, vec2i p2)
{
    for (int i = TOPLEFT; i <= LEFT; ++i) {
        vec2i neighbor = getNeighbor(p1.x, p1.y, static_cast<Direction>(i));

        if (neighbor == p2) {
            return i;
        }
    }

    return -1;
}

// http://www.redblobgames.com/grids/hexagons/
int Board::distance(vec2i p1, vec2i p2)
{
    // convert to cube coordinates (odd-r to cube)
    int x1 = p1.y - (p1.x - (p1.x % 2)) / 2;
    int z1 = p1.x;
    int y1 = -x1 - z1;

    int x2 = p2.y - (p2.x - (p2.x % 2)) / 2;
    int z2 = p2.x;
    int y2 = -x2 - z2;

    return Util::max(Util::abs(x1-x2), Util::abs(y1-y2), Util::abs(z1-z2));
}

// OLD VERSION
void Board::createBoardGrassTiles()
{
    // create basic tiles
    for (int i = 0; i < 9; ++i) {
        hexagonCells.push_back(std::vector<HexagonCell>());

        for (int j = 0; j < 11; ++j) {
            hexagonCells[i].push_back(HexagonCell());
            //hexagonCells[i][j].camera = c;
            hexagonCells[i][j].shape =
                AssetFactory::getInstance().getShape("SPRITE");
            hexagonCells[i][j].material.texture =
                AssetFactory::getInstance().getTexture("TILES_GRASS");
            hexagonCells[i][j].useAtlas = true;
        }
    }

    // set non-renderable parts
    hexagonCells[0][0].render = false;
    hexagonCells[0][10].render = false;
    hexagonCells[1][10].render = false;

    hexagonCells[7][10].render = false;
    hexagonCells[8][0].render = false;
    hexagonCells[8][10].render = false;

    // set non-playground areas
    for (int i = 0; i < 11; ++i) { // top and bottom row
        hexagonCells[0][i].setPlayground(false);
        hexagonCells[8][i].setPlayground(false);
    }
    for (int i = 1; i < 8; ++i) { // left and right columns
        hexagonCells[i][0].setPlayground(false);
        hexagonCells[i][10].setPlayground(false);
    }
    for (int i = 1; i < 8; i += 2) { // 4 tiles on the right
        hexagonCells[i][9].setPlayground(false);
    }

    // set scoring areas
    for (int i = 3; i <= 5; ++i) {
        hexagonCells[i][0].setScoringArea(2);
        hexagonCells[i][10 - i % 2].setScoringArea(1);
    }

    // set atlas coordinates
    AtlasCoords coords;
    coords.height = 128;
    coords.width = 128;

    // set border tiles
    coords.y = 3 * coords.height;
    
    // top corners
    hexagonCells[0][1].coords = coords;
    hexagonCells[0][9].coords = coords;
    hexagonCells[0][9].flipX = true;

    coords.x = 3 * coords.width;

    // top mix
    hexagonCells[0][2].coords = coords;
    hexagonCells[0][8].coords = coords;
    hexagonCells[0][8].flipX = true;

    coords.x = 2 * coords.width;

    // top
    for (int i = 3 ; i < 8; ++i) {
        hexagonCells[0][i].coords = coords;
    }

    // top lower corners
    coords.x = 3 * coords.width;
    coords.y = 2 * coords.height;

    hexagonCells[1][0].coords = coords;
    hexagonCells[1][9].coords = coords;
    hexagonCells[1][9].flipX = true;

    // edge <
    coords.x = coords.width;
    coords.y = coords.height;

    for (int i = 2; i <= 6; i += 2) {
        hexagonCells[i][0].coords = coords;
        hexagonCells[i][10].coords = coords;
        hexagonCells[i][10].flipX = true;
    }

    // edge >
    coords.x = 0;

    for (int i = 3; i <= 5; i += 2) {
        hexagonCells[i][0].coords = coords;
        hexagonCells[i][9].coords = coords;
        hexagonCells[i][9].flipX = true;
    }

    // bottom higher corners
    coords.x = 2 * coords.width;

    hexagonCells[7][0].coords = coords;
    hexagonCells[7][9].coords = coords;
    hexagonCells[7][9].flipX = true;

    // bottom corners
    coords.x = 3 * coords.width;

    hexagonCells[8][1].coords = coords;
    hexagonCells[8][9].coords = coords;
    hexagonCells[8][9].flipX = true;

    // bottom mix
    coords.x = 2 * coords.width;
    coords.y = 2 * coords.height;

    hexagonCells[8][2].coords = coords;
    hexagonCells[8][8].coords = coords;
    hexagonCells[8][8].flipX = true;

    // bottom
    coords.x = coords.width;

    for (int i = 3; i < 8; ++i) {
        hexagonCells[8][i].coords = coords;
    }

    // set sand -> grass tiles
    coords.x = 2 * coords.width;
    coords.y = 0;

    for (int i = 1; i <= 7; i += 2) {
        hexagonCells[i][1].coords = coords;
        hexagonCells[i][8].coords = coords;
        hexagonCells[i][8].flipX = true;
    }

    // set grass -> sand tiles
    coords.x = 3 * coords.width;

    for (int i = 2; i <= 6; i += 2) {
        hexagonCells[i][2].coords = coords;
        hexagonCells[i][8].coords = coords;
        hexagonCells[i][8].flipX = true;
    }

    // set sand tiles
    coords.x = coords.width;

    for (int i = 2; i <= 6; i += 2) {
        hexagonCells[i][1].coords = coords;
        hexagonCells[i][9].coords = coords;
    }

    // set grass tiles
    coords.x = 0;

    for (int i = 1; i <= 7; i += 2) {
        for (int j = 2; j <= 7; ++j) {
            hexagonCells[i][j].coords = coords;
        }
    }

    for (int i = 2; i <= 6; i += 2) {
        for (int j = 3; j <= 7; ++j) {
            hexagonCells[i][j].coords = coords;
        }
    }

    // set transforms
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 11; ++j) {

            float x = j;
            if (i % 2 == 1) {
                x += 0.5f;
            }
            float y = i * -0.74f; // testing, original -0.75

            hexagonCells[i][j].transform.setPosition(x, y, BOARD); // old-z = 1.0f

            // Use this offset to interpolate characters "depth" when moving on board
            hexagonCells[i][j].transform.position.z += 0.9f - (i * 0.05f);
        }
    }
}

int Board::loadLevel(const char* xmlname)
{
    tinyxml2::XMLDocument doc;

    if (doc.LoadFile(xmlname) != tinyxml2::XML_SUCCESS) {
        return 1;
    }

    tinyxml2::XMLNode* root = doc.FirstChildElement("level");

    int rows = root->FirstChildElement("rows")->IntAttribute("value");
    int columns = root->FirstChildElement("columns")->IntAttribute("value");

    for (int i = 0; i < rows; ++i) {
        hexagonCells.push_back(std::vector<HexagonCell>());

        for (int j = 0; j < columns; ++j) {
            hexagonCells[i].push_back(HexagonCell());
        }
    }

    std::vector<AtlasCoords> tiles;
    tiles.push_back(AtlasCoords());

    tinyxml2::XMLElement* tilesElement = root->FirstChildElement("tiles");

    std::string name = tilesElement->Attribute("name");
    int width = tilesElement->IntAttribute("width");
    int height = tilesElement->IntAttribute("height");

    for (tinyxml2::XMLElement* e = tilesElement->FirstChildElement("tile"); e ; e = e->NextSiblingElement("tile")) {
        AtlasCoords coords;
        coords.width = width;
        coords.height = height;
        coords.x = e->IntAttribute("x");
        coords.y = e->IntAttribute("y");
        tiles.push_back(coords);
    }

    int row = 0;
    int column = 0;
    for (tinyxml2::XMLElement* e = root->FirstChildElement("row"); e ; e = e->NextSiblingElement("row")) {
        
        column = 0;
        
        for (tinyxml2::XMLElement* e2 = e->FirstChildElement("hex"); e2 ; e2 = e2->NextSiblingElement("hex")) {
            hexagonCells[row][column].shape = AssetFactory::getInstance().getShape("SPRITE");
            hexagonCells[row][column].material.texture = AssetFactory::getInstance().getTexture(name);
            hexagonCells[row][column].useAtlas = true;

            hexagonCells[row][column].coords = tiles[e2->IntAttribute("tile")];

            if (e2->IntAttribute("draw") == 0) {
                hexagonCells[row][column].render = false;
            }

            if (e2->IntAttribute("playground") == 0) {
                hexagonCells[row][column].setPlayground(false);
            }

            hexagonCells[row][column].setScoringArea(e2->IntAttribute("goal"));

            if (e2->IntAttribute("flip") == 1) {
                hexagonCells[row][column].flipX = true;
            }

            ++column;
        }

        ++row;
    }

    // set transforms
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < column; ++j) {

            float x = j;
            if (i % 2 == 1) {
                x += 0.5f;
            }
            float y = i * -0.74f; // testing, original -0.75

            hexagonCells[i][j].transform.setPosition(x, y, BOARD);

            // Use this offset to interpolate characters "depth" when moving on board
            hexagonCells[i][j].transform.position.z += 0.9f - (i * 0.05f);
        }
    }

    return 0;
}