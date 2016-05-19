#include "TestLevel.h"

#include <algorithm>

#include "AnimationState.h"
#include "AssetFactory.h"
#include "AStar.h"
#include "DefaultState.h"
#include "HashSet.h"
#include "Input.h"
#include "Layer.h"
#include "Util.h"
#include "vec2iHash.h"
#include "SoundPlayer.h"
#include "KickRules.h"

#include "IwRandom.h"

TestLevel::TestLevel() :
    cameraVelocity(0, 0),
    cameraAcceleration(0, 0),
    boardSelection(-1, -1),
    ballSpawn(4, 6),
    team1Ship(vec3(-1.25, -1.5f, BOARD)), // -2
    team2Ship(vec3(12.25, -1.5f, BOARD)) // 13
{
    activeTeam = 1;

    team1Score = 0;
    team2Score = 0;

    state = NULL;
    nextState = NULL;
    camera = NULL;

	soundPlayer.playMusic("music.mp3");
}

TestLevel::~TestLevel()
{
	
}

void TestLevel::initialize()
{
    state = new DefaultState(this);
    initCameras();
    //board.createBoardGrassTiles();
    board.loadLevel("levels/grassland.xml");
    initCharacters();
    initUI();
    //initWater();

    team1placementHexes.push_back(vec2i(1, 2));
    team1placementHexes.push_back(vec2i(2, 2));
    team1placementHexes.push_back(vec2i(4, 2));
    team1placementHexes.push_back(vec2i(6, 2));
    team1placementHexes.push_back(vec2i(7, 2));

    team2placementHexes.push_back(vec2i(1, 9));
    team2placementHexes.push_back(vec2i(2, 10));
    team2placementHexes.push_back(vec2i(4, 10));
    team2placementHexes.push_back(vec2i(6, 10));
    team2placementHexes.push_back(vec2i(7, 9));

    IwRandSeed((int32)s3eTimerGetMs());

	//soundPlayer.playSound("music", "TestLevel");
}

void TestLevel::initCameras()
{
    camera = Camera::createOrhographicCamera(screenWidth / screenHeight);
    camera->transform.setPosition(3, -3, 0);
    camera->setZoom(3.3f);

    UIcamera = Camera::createOrhographicCamera(screenWidth / screenHeight);
    UIcamera->transform.setPosition(0, 0, 0);
    UIcamera->setZoom(2.5f);
}

void TestLevel::initCharacters()
{
    vec3 pos;

    ball = new Ball();
    ball->flipX = true;
    ball->shape = AssetFactory::getInstance().getShape("SPRITE");
    ball->material.texture = AssetFactory::getInstance().getTexture("BALL");
    pos = board.getHex(ballSpawn).transform.position;
    ball->transform.setPosition(pos.x, pos.y, CHARACTER + (pos.z - BOARD));
    ball->transform.setScale(0.5, 0.5, 1);
    ball->useAtlas = true;

	ball->setAnimation(ball->getName() + "Stop");

    board.setCharacter(ball, ballSpawn);

    initTeam1();
    initTeam2();

    charactersInBoard.push_back(ball);
}

void TestLevel::initTeam1()
{
    for (int i = 0; i <= 3; ++i) {
        Character ch("RastaDwarf", 3, 3, 3, 1, false);
        ch.shape = AssetFactory::getInstance().getShape("SPRITE");
        ch.material.texture = AssetFactory::getInstance().getTexture("ISLE_ELVES");
        ch.transform.setScale(0.8, 0.8, 1);
        ch.useAtlas = true;

		ch.setAnimation(ch.getName() + "Stop");

        ch.flipX = true;
        vec3 p = team1Ship.getSprite(i).transform.position;
        ch.transform.setPosition(p.x, p.y, CHARACTER);
        team1.push_back(ch);
    }

    for (int i = 4; i <= 5; ++i) {
        Character ch("SportyElf", 2, 3, 4, 1, false);
        ch.shape = AssetFactory::getInstance().getShape("SPRITE");
        ch.material.texture = AssetFactory::getInstance().getTexture("ISLE_ELVES");
        ch.transform.setScale(0.8, 0.8, 1);
        ch.useAtlas = true;

		ch.setAnimation(ch.getName() + "Stop");

        ch.flipX = true;
        vec3 p = team1Ship.getSprite(i).transform.position;
        ch.transform.setPosition(p.x, p.y, CHARACTER);
        team1.push_back(ch);
    }

    for (int i = 6; i <= 7; ++i) {
        Character ch("BeardElf", 4, 2, 3, 1, false);
        ch.shape = AssetFactory::getInstance().getShape("SPRITE");
        ch.material.texture = AssetFactory::getInstance().getTexture("ISLE_ELVES");
        ch.transform.setScale(0.8, 0.8, 1);
        ch.useAtlas = true;

        ch.setAnimation(ch.getName() + "Stop");

        ch.flipX = true;
        vec3 p = team1Ship.getSprite(i).transform.position;
        ch.transform.setPosition(p.x, p.y, CHARACTER);
        team1.push_back(ch);
    }
}

void TestLevel::initTeam2()
{
    
    for (int i = 0; i <= 3; ++i) {
        Character ch("RastaDwarf", 3, 3, 3, 2, false);
        ch.shape = AssetFactory::getInstance().getShape("SPRITE");
        ch.material.texture = AssetFactory::getInstance().getTexture("ISLE_ELVES");
        ch.transform.setScale(0.8, 0.8, 1);
        ch.useAtlas = true;

        ch.setAnimation(ch.getName() + "Stop");

        ch.setTeam(2);
        vec3 p = team2Ship.getSprite(i).transform.position;
        ch.transform.setPosition(p.x, p.y, CHARACTER);
        team2.push_back(ch);
    }

    for (int i = 4; i <= 5; ++i) {
        Character ch("SportyElf", 2, 3, 4, 2, false);
        ch.shape = AssetFactory::getInstance().getShape("SPRITE");
        ch.material.texture = AssetFactory::getInstance().getTexture("ISLE_ELVES");
        ch.transform.setScale(0.8, 0.8, 1);
        ch.useAtlas = true;

		ch.setAnimation(ch.getName() + "Stop");

        ch.setTeam(2);
        vec3 p = team2Ship.getSprite(i).transform.position;
        ch.transform.setPosition(p.x, p.y, CHARACTER);
        team2.push_back(ch);
    }

    for (int i = 6; i <= 7; ++i) {
        Character ch("BeardElf", 4, 2, 3, 2, false);
        ch.shape = AssetFactory::getInstance().getShape("SPRITE");
        ch.material.texture = AssetFactory::getInstance().getTexture("ISLE_ELVES");
        ch.transform.setScale(0.8, 0.8, 1);
        ch.useAtlas = true;

		ch.setAnimation(ch.getName() + "Stop");

        ch.setTeam(2);
        vec3 p = team2Ship.getSprite(i).transform.position;
        ch.transform.setPosition(p.x, p.y, CHARACTER);
        team2.push_back(ch);
    }
}

void TestLevel::initUI()
{
    float x, y;

    x = UIcamera->getAspectRatio() * UIcamera->getZoom() - 0.5f;
    y = 0.5f - UIcamera->getZoom();

    endTurnButton.shape = AssetFactory::getInstance().getShape("SPRITE");
    endTurnButton.material.texture = AssetFactory::getInstance().getTexture("UI_HEXATLAS");
	endTurnButton.useAtlas = true;
	endTurnButton.coords.x = 256;
	endTurnButton.coords.y = 256;
	endTurnButton.coords.width = 128;
	endTurnButton.coords.height = 128;
    endTurnButton.transform.setPosition(x, y, UI);

    endTurnButton.setCommand(new EndTurnCommand(this));
}

void TestLevel::initWater()
{
    Drawable waterSprite;
    waterSprite.shape = AssetFactory::getInstance().getShape("SPRITE");
    waterSprite.material.texture = AssetFactory::getInstance().getTexture("WATER");
    waterSprite.useAtlas = true;
    waterSprite.coords.x = 0;
    waterSprite.coords.y = 0;
    waterSprite.coords.width = 128;
    waterSprite.coords.height = 128;

    // top line
    for (int i = -2; i <= 14; ++i) {
        Drawable d(waterSprite);
        d.transform.setPosition(i, 0, WATER);
        waterSprites.push_back(d);
    }

    for (int i = -2; i <= 1; ++i) {
        for (int j = -1; j >= -5; j--) {
            Drawable d(waterSprite);
            d.transform.setPosition(i, j, WATER);
            waterSprites.push_back(d);
        }
    }

    for (int i = 11; i <= 14; ++i) {
        for (int j = -1; j >= -5; --j) {
            Drawable d(waterSprite);
            d.transform.setPosition(i, j, WATER);
            waterSprites.push_back(d);
        }
    }

    Drawable d1(waterSprite);
    d1.transform.setPosition(2, -1, WATER);
    waterSprites.push_back(d1);

    Drawable d2(waterSprite);
    d2.transform.setPosition(10, -1, WATER);
    waterSprites.push_back(d2);

    // bottom line
    for (int i = -2; i <= 14; ++i) {
        Drawable d(waterSprite);
        d.transform.setPosition(i, -6, WATER);
        waterSprites.push_back(d);
    }
}

void TestLevel::update(uint64 deltaTime)
{
    updateState();
    updateInput();
    updateCamera(deltaTime);
    updateMoves(deltaTime);
    updateAnimations(deltaTime);
	updateHighlightHex();
}

void TestLevel::updateState()
{
    state->checkStateEnd();

    if (nextState != NULL) {
        delete state;
        state = nextState;
        nextState = NULL;
    }
}

void TestLevel::updateInput()
{
    while(Input::getInstance().hasEvents()) {
        Event e = Input::getInstance().popEvent();

        if (e.motion) {
            motion(e);
        } else {
            click(e);
        }
    }

    accelerateCamera();
}

void TestLevel::motion(const Event& e)
{
    if (touch.pressed && touch.id == e.id) {
        touch.x = e.x;
        touch.y = e.y;
    }

    /*
    if (prev.x != -1) {
        float aX = 0.008f * (prev.x - e.x);
        float aY = 0.008f * (e.y - prev.y);

        cameraAcceleration += vec2(aX, aY);
    }

    prev = vec2i(e.x, e.y);
    */
}

void TestLevel::click(const Event& e)
{
    if ( !touch.pressed ) { // new touch
        if (e.pressed) {
            touch.pressed = true;
            touch.id = e.id;
            touch.origX = e.x;
            touch.origY = e.y;
            touch.x = e.x;
            touch.y = e.y;
            touch.prevX = e.x;
            touch.prevY = e.y;
        }
    } else if (touch.id == e.id) {
        if (e.motion) {
            touch.x = e.x;
            touch.y = e.y;

        } else if ( !e.pressed ) {
            accelerateCamera();
            state->handleClick(vec2i(touch.origX, touch.origY), vec2i(touch.x, touch.y));
            touch.reset();
        }
    }
}

void TestLevel::accelerateCamera()
{
    float multi = 0.008f;

    float aX = multi * (touch.prevX - touch.x);
    float aY = multi * (touch.y - touch.prevY);

    cameraAcceleration += vec2(aX, aY);

    touch.prevX = touch.x;
    touch.prevY = touch.y;
}

void TestLevel::updateCamera(uint64 deltaTime)
{
    cameraVelocity += cameraAcceleration;
    cameraAcceleration = vec2(0, 0);

    // if camera has velocity -> move
    if (!Util::equals(cameraVelocity.GetLength(), 0.0f, 0.005f)) {

        // update camera position
        camera->transform.Translate(cameraVelocity.x, 0, 0);
        //camera->transform.Translate(cameraVelocity.x, cameraVelocity.y, 0);
        camera->viewModified = true;

        vec2 friction = -cameraVelocity;
        friction.Normalise();
        friction *= deltaTime * 0.006f;
        // TODO: add this mystery multiplier to a better location...

        if (friction.GetLength() >= cameraVelocity.GetLength()) {
            cameraVelocity = vec2(0, 0);
        } else {
            cameraVelocity += friction;
        }

        // Camera bounds if board has less than 5 characters
        
        int charCount = getCharCountOnBoard();

		if(getActiveTeam() == 1 && charCount < 5){	//Team1 bounds
			if (camera->transform.position.x < 3) {
				camera->transform.position.x = 3;
			} else if (camera->transform.position.x > 6.0) {
				camera->transform.position.x = 6.0;
			}
		} else if(getActiveTeam() == 2 && charCount < 5){	//Team2 bounds
			if (camera->transform.position.x > 9) {
				camera->transform.position.x = 9;
			} else if (camera->transform.position.x < 6.0) {
				camera->transform.position.x = 6.0;
			}
		}
        
        /*
		if(camera->transform.position.x < 6.2 && getCharCountOnBoard() >= 5 || camera->transform.position.x > 6.2 && getCharCountOnBoard() >= 5)
			camera->transform.position.x = 6.2;
        //*/

        if (charCount >= 5) {
            camera->transform.position.x = 6.0f;
        }

        /*
        if (camera->transform.position.y > -1) {
            camera->transform.position.y = -1;
        } else if (camera->transform.position.y < -5) {
            camera->transform.position.y = -5;
        }
        */
    }
}

void TestLevel::updateMoves(uint64 deltaTime)
{
    if (moves.size() > 0) {
        // p‰ivitell‰‰n
        for (unsigned i = 0; i < moves.size(); ++i) {
            moves[i].update(deltaTime);
        }

        std::vector<Move> remainingMoves;

        for (unsigned i = 0; i < moves.size(); ++i) {
            if (!moves[i].isFinished()) {
                remainingMoves.push_back(moves[i]);
            }
        }
        moves = remainingMoves;	
	}
}

void TestLevel::updateAnimations(uint64 deltaTime)
{
    for (size_t i = 0; i < waterSprites.size(); ++i) {
        waterSprites[i].update(deltaTime);
    }

	ball->update(deltaTime);

	for (size_t i = 0; i < team1.size(); ++i) {
		team1[i].update(deltaTime);
	}

	for (size_t i = 0; i < team2.size(); ++i) {
		team2[i].update(deltaTime);
	}
}

void TestLevel::updateHighlightHex(){
    for (size_t i = 0; i < team1.size(); ++i) {
        team1[i].highlightHex.transform.setPosition(
            team1[i].transform.position.x,
            team1[i].transform.position.y,
            CHARACTER_BEHIND);
    }

    for (size_t i = 0; i < team2.size(); ++i) {
        team2[i].highlightHex.transform.setPosition(
            team2[i].transform.position.x,
            team2[i].transform.position.y,
            CHARACTER_BEHIND);
    }
    
    /*
    for(unsigned i = 0; i < team1.size(); ++i){
		if(getActiveTeam() == 1)
			team1[i].highlightHex.transform.setPosition(team1[i].transform.position.x, team1[i].transform.position.y, CHARACTER_BEHIND); 
			
		if(!team1[i].isMoving() && team1[i].isInBoard() && getActiveTeam() == 1)
				team1[i].highlightHex.render = true;
	}

	for(unsigned i = 0; i < team2.size(); ++i){
		if(getActiveTeam() == 2)
			team2[i].highlightHex.transform.setPosition(team2[i].transform.position.x, team2[i].transform.position.y, CHARACTER_BEHIND); 
			
		if(!team2[i].isMoving() && team2[i].isInBoard() && getActiveTeam() == 2)
				team2[i].highlightHex.render = true;
	}
    */
}

void TestLevel::setShipSelection(int index, int ship)
{
    /*
    Character *c1 = 0;

    if (selectedShip == 1) {
        c1 = &team1[shipSelection];
    } else if (selectedShip == 2) {
        c1 = &team2[shipSelection];
    }

    if (c1) {
        c1->setAnimation(c1->getName() + "Stop");
    }
    //*/

    shipSelection = index;
    selectedShip = ship;

    /*
    Character *c2;

    if (selectedShip == 1) {
        c2 = &team1[shipSelection];
    } else if (selectedShip == 2) {
        c2 = &team2[shipSelection];
    }

    c2->setAnimation(c2->getName() + "Idle");
    //*/
}

void TestLevel::resetShipSelection()
{
    /*
    Character *c = 0;

    if (selectedShip == 1) {
        c = &team1[shipSelection];
    } else if (selectedShip == 2) {
        c = &team2[shipSelection];
    }

    if (c) {
        c->setAnimation(c->getName() + "Stop");
    }
    //*/

    shipSelection = -1;
    selectedShip = -1;
}

void TestLevel::setBoardSelection(vec2i point)
{
    boardSelection = point;
}

void TestLevel::resetBoardSelection()
{
    boardSelection = vec2i(-1, -1);
}

void TestLevel::resetAnimationBoard()
{
    if (boardSelection.x != -1) {
        Character *c = board.getHex(boardSelection).getCharacter();
        c->setAnimation(c->getName() + "Stop");
    }
}

void TestLevel::resetAnimationShip()
{
    if (selectedShip == 1) {
        team1[shipSelection].setAnimation(team1[shipSelection].getName() + "Stop");
    } else if (selectedShip == 2) {
        team2[shipSelection].setAnimation(team2[shipSelection].getName() + "Stop");
    }
}

void TestLevel::setAnimationBoard(std::string animationName)
{
    if (boardSelection.x != -1) {
        Character *c = board.getHex(boardSelection).getCharacter();
        c->setAnimation(c->getName() + animationName);
    }
}

void TestLevel::setAnimationShip(std::string animationName)
{
    if (selectedShip == 1) {
        team1[shipSelection].setAnimation(team1[shipSelection].getName() + animationName);
    } else if (selectedShip == 2) {
        team2[shipSelection].setAnimation(team2[shipSelection].getName() + animationName);
    }
}

bool TestLevel::youControlCharacter(vec2i point)
{
    return board.getHex(point).getCharacter()->getTeam() == activeTeam ? true : false;
}

int TestLevel::getActiveTeam()
{
    return activeTeam;
}

void TestLevel::setActiveTeam(int team)
{
    activeTeam = team;
}

int TestLevel::getCharCountOnBoard(){
	 int count = 0;

	 if (getActiveTeam() == 1) {
        for (size_t i = 0; i < team1.size(); ++i) {
            if (team1[i].isInBoard()) {
                ++count;
            }
        }
    } else {
        for (size_t i = 0; i < team2.size(); ++i) {
            if (team2[i].isInBoard()) {
                ++count;
            }
        }
    }

	return count;
}

void TestLevel::endTurn()
{
    ball->flipX = !ball->flipX;

    // remove unnecessary tooltips
    kickSprites.clear();
    moveSprites.clear();

    // reset selections and animations
    resetAnimationBoard();
    resetAnimationShip();
    resetBoardSelection();
    resetShipSelection();

    // reset actions for characters
    if (activeTeam == 1) {
        for (size_t i = 0; i < team1.size(); ++i) {
            team1[i].reset();
        }

    } else { // if active team == 2
        for (size_t i = 0; i < team2.size(); ++i) {
            team2[i].reset();
        }
    }

    // switch active team
    activeTeam = (activeTeam == 1) ? 2 : 1;

    if (board.getHex(ball->getPosition()).getScoringArea() != 0) {
        resetBug();

        if (ball->getPosition() == vec2i(4, 5)) {
            makeRandomBugMove();
            nextState = new AnimationState(this);
        }

    } else {
        makeRandomBugMove();
        nextState = new AnimationState(this);
    }

    // TODO: make camera move smooth
    // center the camera
    //camera->transform.setPosition(5, -3, 0);
    //camera->viewModified = true;
	if(getActiveTeam() == 1 && getCharCountOnBoard() < 5){
		camera->transform.setPosition(3, -3, 0);
	} 
	else if(getActiveTeam() == 2 && getCharCountOnBoard() < 5){
		camera->transform.setPosition(9, -3, 0);
	} else {
		camera->transform.setPosition(6.2, -3, 0);

	}
	camera->viewModified = true;
}

void TestLevel::resetBug()
{
    if (board.getHex(ballSpawn).isFree()) {
        board.setCharacter(ball, ballSpawn);
        vec3 v = board.getHex(ballSpawn).transform.position;
        ball->transform.setPosition(v.x, v.y, CHARACTER + (v.z - BOARD));
    } else {
        std::vector<vec2i> points;
        std::vector<vec2i> neighbors = board.getAllNeighbors(4, 5);
        HashSet<vec2iHash> searched(50);
        searched.add(vec2iHash(vec2i(4, 5)));

        while (true) {
            for (size_t i = 0; i < neighbors.size(); ++i) {
                if (board.getHex(neighbors[i]).isFree() &&
                    board.getHex(neighbors[i]).isPlayground()) {

                    points.push_back(neighbors[i]);
                } else {
                    searched.add(vec2iHash(neighbors[i]));
                }
            }

            if (points.size() > 0) {
                break;
            }

            std::vector<vec2i> newNeighbors;

            for (size_t i = 0; i < neighbors.size(); ++i) {
                std::vector<vec2i> nbs = board.getAllNeighbors(neighbors[i]);

                if ( !searched.contains(vec2iHash(nbs[i])) ) {
                    newNeighbors.push_back(nbs[i]);
                }
            }

            neighbors = newNeighbors;
        }

        std::vector<vec2i> path;
        path.push_back(vec2i(4, 5));
        path.push_back(points[IwRandRange(points.size())]);

        std::vector<vec3> movePath;

        for (size_t i = 0; i < path.size(); ++i) {
            vec3 v = board.getHex(path[i]).transform.position;
            movePath.push_back(vec3(v.x, v.y, ball->transform.position.z));
        }

        moves.push_back(Move(ball, 1000, movePath));
        board.setCharacter(ball, path[1]);
        nextState = new AnimationState(this);
    }
}

void TestLevel::makeRandomBugMove()
{
    std::vector<vec2i> points = board.getBallMovePoints(ball->getPosition());

    if (points.size() > 0) {
        int index = IwRandRange(points.size());

        makeMove(ball->getPosition(), points[index]);
    }
}

void TestLevel::goal(int team)
{
    float x;
    float y;

    if (team == 1) {
        x = (0.5f + team1Score) - UIcamera->getAspectRatio() * UIcamera->getZoom();
        ++team1Score;

    } else if (team == 2) {
        x = UIcamera->getAspectRatio() * UIcamera->getZoom() - (0.5f + team2Score);
        ++team2Score;
    }

    y = UIcamera->getZoom() - 0.5;

    Drawable d;
    d.shape = AssetFactory::getInstance().getShape("SPRITE");
    d.material.texture = AssetFactory::getInstance().getTexture("UI_FLAG");
    d.transform.setPosition(x, y, UI);
    goalSprites.push_back(d);
}

bool TestLevel::canTackle(vec2i from, vec2i to)
{
    if (board.getHex(from).isFree() ||
        board.getHex(to).isFree()) {
            return false;
        }

    Character* actor = board.getHex(from).getCharacter();
    Character* target = board.getHex(to).getCharacter();

    if (actor->getTeam() != activeTeam ||
        actor->getRemainingActions() <= 0 ||
        actor->getTeam() == target->getTeam() ||
        target->getTeam() == 0) {
            return false;
        }

    int direction = board.getDirection(from, to);

    if (direction == -1) {
        return false;
    }

    Direction dir = static_cast<Direction>(direction);

    int tackle;
    int base = actor->getStrength() - target->getStrength();

    if (base >= 1) {
        tackle = 2;
    } else if (base >= -1) {
        tackle = 1;
    } else {
        tackle = 0;
    }

    int minus = calcTackleBonus(to, dir, target->getTeam());

    tackle -= minus;

    if (tackle <= 0) {
        return false;
    }

    int dist = minus + 1;
    vec2i neighbor = to;

    for (int i = 0; i < dist; ++i) {
        neighbor = board.getNeighbor(neighbor.x, neighbor.y, dir);
    }

    if (board.getHex(neighbor).isFree()) {
        return true;
    } else {
        return false;
    }
}

bool TestLevel::canKick(vec2i from, vec2i to)
{
    if (board.getHex(from).isFree() ||
        board.getHex(to).isFree()) {
            return false;
        }

    Character* actor = board.getHex(from).getCharacter();
    Character* target = board.getHex(to).getCharacter();

    if (actor->getTeam() != activeTeam ||
        actor->getKickPosition().x != -1 ||
        target->getTeam() != 0) {
            return false;
        }

    int direction = board.getDirection(from, to);

    if (direction == -1) {
        return false;
    }

    Direction dir = static_cast<Direction>(direction);

    std::vector<vec2i> points;

    KickRules::calcKickPoints(actor, ball, &board, dir, &points);

    if (points.size() > 0) {
        return true;
    } else {
        return false;
    }
}

int TestLevel::calcTackleBonus(vec2i point, Direction d, int team)
{
    vec2i current = point;
    int bonus = 0;

    while(true)
    {
        current = board.getNeighbor(current.x, current.y, d);

        if (board.getHex(current).isFree()) {
            break;
        } else if (board.getHex(current).getCharacter()->getTeam() == team){
            ++bonus;
        }
    }

    return bonus;
}

void TestLevel::makeMove(vec2i from, vec2i to)
{
    Character* c = board.getHex(from).getCharacter();

    std::vector<vec2i> path;
    path.push_back(from);
    path.push_back(to);

    std::vector<vec3> movePath;

    for (size_t i = 0; i < path.size(); ++i) {
        vec3 v = board.getHex(path[i]).transform.position;
        movePath.push_back(vec3(v.x, v.y, c->transform.position.z));
    }

    moves.push_back(Move(c, 300 * (path.size() - 1), movePath));
    board.moveCharacter(from, to);
}

void TestLevel::makeMove(vec2i from, vec2i to, uint64 delay)
{
    Character* c = board.getHex(from).getCharacter();

    std::vector<vec2i> path;
    path.push_back(from);
    path.push_back(to);

    std::vector<vec3> movePath;

    for (size_t i = 0; i < path.size(); ++i) {
        vec3 v = board.getHex(path[i]).transform.position;
        movePath.push_back(vec3(v.x, v.y, c->transform.position.z));
    }
	
    moves.push_back(Move(c, 300 * (path.size() - 1), delay, movePath));
    board.moveCharacter(from, to);
}

void TestLevel::makeMovePathfind(vec2i from, vec2i to)
{
    Character* c = board.getHex(from).getCharacter();

    std::vector<vec2i> path = AStar::findPath(from, to, board);
    std::vector<vec3> movePath;

    for (size_t i = 0; i < path.size(); ++i) {
        vec3 v = board.getHex(path[i]).transform.position;
        movePath.push_back(vec3(v.x, v.y, c->transform.position.z));
    }

    moves.push_back(Move(c, 300 * (path.size() - 1), movePath));
    board.moveCharacter(from, to);
}

void TestLevel::makeKick(vec2i from, vec2i to, std::vector<vec2i>& kickPoints)
{

	soundPlayer.playSound("kick", "TestLevel");

    std::vector<vec2i> path;
    path.push_back(from);

    for (size_t i = 0; i < kickPoints.size(); ++i) {
        path.push_back(kickPoints[i]);

        if (kickPoints[i] == to) {
            break;
        }
    }

    std::vector<vec3> movePath;

    for (size_t i = 0; i < path.size(); ++i) {
        vec3 v = board.getHex(path[i]).transform.position;
        movePath.push_back(vec3(v.x, v.y, ball->transform.position.z));
    }

    //ball->setAnimation(ball->getName() + "Curl", 500); // TODO: delay on hatusta heitetty...
    //ball->addFollowingAnimation(ball->getName() + "Roll", 200 * (path.size() - 1));
    ball->setAnimation(ball->getName() + "Roll", 200 * (path.size() - 1), 500);
    ball->addFollowingAnimation(ball->getName() + "UnCurl");
    ball->addFollowingAnimation(ball->getName() + "Stop");

    moves.push_back(Move(ball, 200 * (path.size() - 1), 500, movePath)); // TODO: t‰‰ delay on hatusta...
    board.moveCharacter(from, to);
}

int TestLevel::makeTackle(vec2i from, Direction d, int hexes)
{
	soundPlayer.playSound("tackle", "TestLevel");

	if (hexes <= 0) {
        return 0;
    }

    Character* c = board.getHex(from).getCharacter();
    
    int length = 0;

    vec2i to = from;
    for (int i = 0; i < hexes; ++i) {
        vec2i next = board.getNeighbor(to.x, to.y, d);

        if ( !board.getHex(next).isFree() ) {
            break;
        }

        to = next;
        ++length;

        if ( !board.getHex(next).isPlayground() ) {
            break;
        }
    }

    if (length > 0) {
        std::vector<vec2i> path;
        path.push_back(from);
        path.push_back(to);

        std::vector<vec3> movePath;
        for (size_t i = 0; i < path.size(); ++i) {
            vec3 v = board.getHex(path[i]).transform.position;
            movePath.push_back(vec3(v.x, v.y, c->transform.position.z));
        }

        moves.push_back(Move(c, 200 * length, movePath));
        board.moveCharacter(from, to);
		c->setAnimation(c->getName() + "Hurt", 200 * length);
		c->addFollowingAnimation(c->getName() + "Stop");
    }

    return length;
}

void TestLevel::render()
{
    // BACKGROUND
    // WATER
    for (size_t i = 0; i < waterSprites.size(); ++i) {
        waterSprites[i].draw(*camera);
    }

    // BOARD
    board.draw(*camera);
    team1Ship.draw(*camera);
    team2Ship.draw(*camera);

    // CHARACTER BEHIND
	for (size_t i = 0; i < team1.size(); ++i){
        if ( !team1[i].isMoving() ) {
            team1[i].highlightHex.draw(*camera);
        }
	}

	for (size_t i = 0; i < team2.size(); ++i){
        if ( !team2[i].isMoving() ) {
            team2[i].highlightHex.draw(*camera);
        }
	}

    for (size_t i = 0; i < moveSprites.size(); ++i) {
        moveSprites[i].draw(*camera);
    }

    // CHARACTER
    //ball->draw(*camera);

    // draw the characters in ships
    for (size_t i = 0; i < team1.size(); ++i) {
        if (team1[i].isInShip()) {
            team1[i].draw(*camera);
        }
    }

    for (size_t i = 0; i < team2.size(); ++i) {
        if (team2[i].isInShip()) {
            team2[i].draw(*camera);
        }
    }

    // draw the characters in board

    std::sort(charactersInBoard.begin(), charactersInBoard.end(), Drawable::compareDepth);

    for (size_t i = 0; i < charactersInBoard.size(); ++i) {
        charactersInBoard[i]->draw(*camera);
    }

    // CHARACTER FRONT

    for (size_t i = 0; i < kickSprites.size(); ++i) {
        kickSprites[i].draw(*camera);
    }

    // UI
    endTurnButton.draw(*UIcamera);

    for (size_t i = 0; i < goalSprites.size(); ++i) {
        goalSprites[i].draw(*UIcamera);
    }

    // FOREGROUND
}

/**************************************************************/

EndTurnCommand::EndTurnCommand(TestLevel* level)
{
    this->level = level;
}

void EndTurnCommand::execute()
{
    level->endTurn();
}
