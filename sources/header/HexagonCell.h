#pragma once

#include "Drawable.h"
#include "Character.h"

class HexagonCell : public Drawable
{
public:

	HexagonCell();
	HexagonCell(bool playground);
	~HexagonCell();

    bool pointIsInside(vec2 point);

    void setCharacter(Character* c);
    void removeCharacter();
    Character* getCharacter() const;

	void setPlayground(bool playground);
	bool isPlayground() const;

	bool isFree() const;

    void setScoringArea(int team);
    int getScoringArea();

private:

    Character* character;

	bool playground;
    int scoringArea;

private:

    static bool sameSide(vec2 p1, vec2 p2, vec2 a, vec2 b);
};