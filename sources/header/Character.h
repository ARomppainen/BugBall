#pragma once

#include <iostream>

#include "Direction.h"
#include "Drawable.h"

#include "IwGeomVec2.h"

typedef CIwVec2 vec2i;

class Board; //forward declaration

class Character : public Drawable
{
public:

	//Character(int strength, int movement, int skill, int team, bool inBoard);

	Character(const std::string& name,
        int strength, int movement, int skill,
        int team, bool inBoard);

	virtual ~Character();

    virtual void updateOffset();

    bool isSameTeam(const Character* other);

    void spendMoves(int value);
    void spendMove();
    void spendAction();
    void reset();
    void swapHighlight();

	//Getters and setters
	std::string getName();
    vec2i getPosition();
    vec2i getKickPosition();
	int getStrength();
	int getMovement();
	int getSkill();
    int getTeam();
    int getRemainingActions();
    int getRemainingMoves();
	
	bool isMoving();
    bool isInBoard();
    bool isInShip();
    bool isOutOfBounds();
    bool hasMovedToKickPos();

	void setMoving(bool isMoving);
    void setBoard();
    void setShip();
    void setOutOfBounds();
    void setPosition(vec2i point);
    void setPosition(int x, int y);
    void setTeam(int t);
    void setKickPosition(vec2i point);
    void setKickPosition(int x, int y);
    void setMovedToKickPos(bool b);

	Drawable highlightHex;

protected:
	std::string name;
	
	int strength;
	int movement;
	int skill;

    int team;

    int actions;
    int remainingActions;
    int remainingMoves;

    bool inBoard;
    bool inShip;
    bool outOfBounds;
	bool moving;
    vec2i position;

    vec2i kickPosition;
    bool movedToKickPos;
};