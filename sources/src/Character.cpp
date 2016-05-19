#include "Character.h"

#include "Board.h"

Character::Character(const std::string& name,
                     int strength, int movement, int skill,
                     int team, bool inBoard) :
    position(-1, -1),
    kickPosition(-1, -1)
{
    this->scale = 1.8f;

	this->name = name;
	this->strength = strength;
	this->movement = movement;
	this->skill = skill;
    this->team = team;

    this->actions = 1;
    this->remainingActions = actions;
    this->remainingMoves = movement;

    this->inBoard = inBoard;
    this->outOfBounds = false;

    if (inBoard) {
        this->inShip = false;
    } else {
        this->inShip = true;
    }

    this->movedToKickPos = false;

    vec3 t1col(0, 0, 1);
    vec3 t2col(1, 102.0f / 256.0f, 0);

    if (team == 1) {
        highlightHex.material.setColor(t1col);
    } else {
        highlightHex.material.setColor(t2col);
    }

	this->highlightHex.shape = AssetFactory::getInstance().getShape("SPRITE");
	this->highlightHex.material.texture = AssetFactory::getInstance().getTexture("UI_HEXATLAS");
	this->highlightHex.useAtlas = true;
	this->highlightHex.render = false;
	this->highlightHex.coords.x = 0;
	this->highlightHex.coords.y = 0;
	this->highlightHex.coords.width = 128;
	this->highlightHex.coords.height = 128;
}

Character::~Character()
{

}

void Character::updateOffset()
{
    // äijä on kohdistettu original width / 2 kohtaan
    // transleittaa:
    // (original width / 2 - offset + width / 2) / mysterySize * skaala???

    float mysterySize = 128.0f;
    float vakioY = 0.7f;

    float offsetX = (coords.originalWidth / 2 - coords.offsetX - coords.width / 2) / mysterySize * scale;
    float offsetY = (coords.originalHeight / 2 - coords.offsetY - coords.height / 2) / mysterySize * scale;

    if (!flipX) {
        offsetX = -offsetX;
    }

    transform.setOffset(offsetX, offsetY + vakioY, 0);
}

bool Character::isSameTeam(const Character* other)
{
    return this->team == other->team ? true : false;
}

void Character::spendMoves(int value)
{
    for (int i = 0; i < value; ++i) {
        spendMove();
    }
}

void Character::spendMove()
{
    --remainingMoves;

    if (remainingMoves == 0) {
        --remainingActions;

        if (remainingActions > 0) {
            remainingMoves = movement;
        }
    }

	//Color of hex
    /*
    if(getRemainingActions() <= 0 && getRemainingMoves() <= 0) {
        if (kickPosition.x == -1 || movedToKickPos) {
            vec3 col = *highlightHex.material.color;
		    highlightHex.material.setColor(col / 2);
        }
	}
    */

    /*
	if(getRemainingActions() > 0 && getRemainingMoves() > 0) {
		highlightHex.material.setColor(vec3(0, 1, 0));
	}
    */
}

void Character::spendAction()
{
    spendMoves(remainingMoves);
}

void Character::reset()
{
    remainingActions = actions;
    remainingMoves = movement;

    vec3 t1col(0, 0, 1);
    vec3 t2col(1, 102.0f / 256.0f, 0);

    if (team == 1) {
        highlightHex.material.setColor(t1col);
    } else {
        highlightHex.material.setColor(t2col);
    }

    kickPosition = vec2i(-1, -1);
    movedToKickPos = false;
}

void Character::swapHighlight()
{
    vec3 t1col(0, 0, 1.0f / 4);
    vec3 t2col(1.0f / 2, 102.0f / 256.0f / 2, 0);

    if (team == 1) {
        highlightHex.material.setColor(t1col);
    } else {
        highlightHex.material.setColor(t2col);
    }
}

//Getters and setters
std::string Character::getName(){ return name; }
vec2i Character::getPosition() { return position; }
vec2i Character::getKickPosition() { return kickPosition; }
int Character::getStrength(){ return strength; }
int Character::getMovement(){ return movement; }
int Character::getSkill(){ return skill; }
int Character::getTeam() { return team; }
int Character::getRemainingActions() { return remainingActions; }
int Character::getRemainingMoves() { return remainingMoves; }

bool Character::isMoving() { return moving; }
bool Character::isInBoard() { return inBoard; }
bool Character::isInShip() { return inShip; }
bool Character::isOutOfBounds() { return outOfBounds; }
bool Character::hasMovedToKickPos() { return movedToKickPos; }

void Character::setMoving(bool isMoving)
{
	this->moving = isMoving;
}
void Character::setBoard()
{
    inBoard = true;
    inShip = false;
    outOfBounds = false;
}

void Character::setShip()
{
    inBoard = false;
    inShip = true;
    outOfBounds = false;
}

void Character::setOutOfBounds()
{
    inBoard = false;
    inShip = false;
    outOfBounds = true;
}

void Character::setPosition(vec2i point)
{
    position = point;
}

void Character::setPosition(int x, int y)
{
    position = vec2i(x, y);
}

void Character::setTeam(int t)
{
    this->team = t;
}

void Character::setKickPosition(vec2i point)
{
    kickPosition = point;
}

void Character::setKickPosition(int x, int y)
{
    kickPosition = vec2i(x, y);
}

void Character::setMovedToKickPos(bool b)
{
    movedToKickPos = b;
}