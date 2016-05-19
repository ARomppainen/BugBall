#include "HexagonCell.h"

#include <vector>

HexagonCell::HexagonCell()
{
	playground = true;
    character = NULL;
    scoringArea = 0;
}

HexagonCell::HexagonCell(bool playground)
{
    this->playground = playground;
    character = NULL;
    scoringArea = 0;
}

HexagonCell::~HexagonCell()
{
}

// http://www.blackpawn.com/texts/pointinpoly/default.html
bool HexagonCell::pointIsInside(vec2 p)
{
    // TODO: update these values if the hexes are not completely symmetrical
    // TODO: improvement, get corner data drom HexagonCell-object / somewhere else
    std::vector<vec2> corners;
    corners.push_back(vec2(
        ( 0.5f      * this->transform.scale.x) + this->transform.position.x,
        ( 0.288675f * this->transform.scale.y) + this->transform.position.y));
    corners.push_back(vec2(
        (0.0f * this->transform.scale.x) + this->transform.position.x,
        (0.5f * this->transform.scale.y) + this->transform.position.y));
    corners.push_back(vec2(
        (-0.5f      * this->transform.scale.x) + this->transform.position.x,
        ( 0.288675f * this->transform.scale.y) + this->transform.position.y));
    corners.push_back(vec2(
        (-0.5f      * this->transform.scale.x) + this->transform.position.x,
        (-0.288675f * this->transform.scale.y) + this->transform.position.y));
    corners.push_back(vec2(
        ( 0.0f * this->transform.scale.x) + this->transform.position.x,
        (-0.5f * this->transform.scale.y) + this->transform.position.y));
    corners.push_back(vec2(
        ( 0.5f      * this->transform.scale.x) + this->transform.position.x,
        (-0.288675f * this->transform.scale.y) + this->transform.position.y));

    bool ret;

    ret = sameSide(p, vec2(this->transform.position.x, this->transform.position.y), corners[5], corners[0]);

    if ( ret ) {
        for (int i = 0; i < 5; ++i) {
            ret = sameSide(p, vec2(this->transform.position.x, this->transform.position.y), corners[i], corners[i+1]);

            if ( !ret) {
                break;
            }
        }
    }

    return ret;
}

bool HexagonCell::sameSide(vec2 p1, vec2 p2, vec2 a, vec2 b)
{
    vec3 v1(a.x - b.x, a.y - b.y, 0);   // vector from point b to point a
    vec3 v2(a.x - p1.x, a.y - p1.y, 0); // vector from point p1 to point a
    vec3 v3(a.x - p2.x, a.y - p2.y, 0); // vector from point p2 to point a

    vec3 cp1 = v1.Cross(v2);
    vec3 cp2 = v1.Cross(v3);

    if (cp1.Dot(cp2) >= 0) {
        return true;
    }

    return false;
}

void HexagonCell::setCharacter(Character* c)
{
    character = c;
}

void HexagonCell::removeCharacter()
{
    character = NULL;
}

Character* HexagonCell::getCharacter() const
{
    return character;
}

void HexagonCell::setPlayground(bool playground)
{
	this->playground = playground;
}

bool HexagonCell::isPlayground() const
{
	return playground;
}

bool HexagonCell::isFree() const
{
	if (character == NULL) {
        return true;
    }

    return false;
}

void HexagonCell::setScoringArea(int team)
{
    scoringArea = team;
}

int HexagonCell::getScoringArea()
{
    return scoringArea;
}