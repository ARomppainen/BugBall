#pragma once

#include <vector>

#include "IwGeomFVec2.h"
#include "IwGeomFVec3.h"

#include "Ball.h"
#include "Board.h"
#include "Button.h"
#include "Camera.h"
#include "Character.h"
#include "Input.h"
#include "Level.h"
#include "Move.h"
#include "Ship.h"
#include "SoundPlayer.h"

class GameState; // forward declaration

typedef CIwFVec2 vec2;
typedef CIwFVec3 vec3;

class TestLevel : public Level
{
public:

	TestLevel();
	virtual ~TestLevel();

	virtual void initialize();
	virtual void update(uint64 deltaTime);
    virtual void render();

    void setShipSelection(int index, int ship);
    void resetShipSelection();

    void setBoardSelection(vec2i point);
    void resetBoardSelection();

    void resetAnimationShip();
    void resetAnimationBoard();

    void setAnimationShip(std::string animationName);
    void setAnimationBoard(std::string animationName);

    bool youControlCharacter(vec2i point);

    int getActiveTeam();
    void setActiveTeam(int team);
	int getCharCountOnBoard();

    void endTurn();
    void resetBug();
    void makeRandomBugMove();

    void goal(int team);

    bool canTackle(vec2i from, vec2i to);
    bool canKick(vec2i from, vec2i to);

    int calcTackleBonus(vec2i point, Direction d, int team);

    void makeMove(vec2i from, vec2i to);
    void makeMove(vec2i from, vec2i to, uint64 delay);
    void makeMovePathfind(vec2i from, vec2i to);
    void makeKick(vec2i from, vec2i to, std::vector<vec2i>& kickPoints);
    int makeTackle(vec2i from, Direction d, int hexes);

private:

	void initCharacters();
	void initCameras();
	void initUI();
    void initWater();

    void initTeam1();
    void initTeam2();

    void updateState();
    void updateInput();
    void accelerateCamera();
    void updateCamera(uint64 deltaTime);
    void updateMoves(uint64 deltaTime);
    void updateAnimations(uint64 deltaTime);
	void updateHighlightHex();

    void motion(const Event& e);
    void click(const Event& e);

    GameState* state;

public: // for convenience with states =)

    int activeTeam;

    GameState* nextState;

    Board board;

    Camera* UIcamera;
    Camera* camera;
    vec2 cameraVelocity;
    vec2 cameraAcceleration;

    // variables to track touch screen presses / motion
    Touch touch; // 11.4.2014 use this variable instead (Touch is defined at Input.h)
    //vec2i prev;
    //vec2i down;
    //vec2i up;

    // selected hex with a character in board, (-1, -1) if no selection
    vec2i boardSelection;
    int shipSelection; // (-1 , 0..7)
    int selectedShip; // (-1, 1, 2)

    vec2i ballSpawn;
    Ball* ball;

    std::vector<Character> team1;
    std::vector<Character> team2;

    int team1Score;
    int team2Score;

    std::vector<Move> moves;

    Ship team1Ship;
    Ship team2Ship;

	SoundPlayer soundPlayer;

    Button endTurnButton;

    std::vector<vec2i> team1placementHexes;
    std::vector<vec2i> team2placementHexes;

    std::vector<Drawable> moveSprites;  // layer: CHARACTER_BEHIND
    std::vector<Drawable> kickSprites;  // layer: CHARACTER_FRONT
    std::vector<Drawable> goalSprites;  // layer: UI
	//std::vector<Drawable> hexSprites;
    std::vector<Drawable> waterSprites;

    // This pointer array is used to render the characters in playing field.
    // This includes initially the ball, and subsequently all the characters that are played
    // from the ship.
    // Because they can have varying z-values, the array is sorted before rendering
    // to make alpha blending correct.
    std::vector<Drawable*> charactersInBoard;
};

class EndTurnCommand : public Command
{
public:

    EndTurnCommand(TestLevel* level);

    void execute();

private:

    TestLevel* level;
};