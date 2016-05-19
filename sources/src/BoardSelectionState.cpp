#include "BoardSelectionState.h"

#include <vector>

#include "AnimationState.h"
#include "AssetFactory.h"
//#include "AStar.h"
#include "DefaultState.h"
#include "KickState.h"
#include "Layer.h"
#include "ShipSelectionState.h"
#include "Util.h"

BoardSelectionState::BoardSelectionState(TestLevel* level) :
    GameState(level)
{
    Character* selectedChar = level->board.getHex(level->boardSelection).getCharacter();

    if (selectedChar->getTeam() == level->activeTeam) {
        calcMovePoints();

        //if (selectedChar->getRemainingActions() > 0) {
            calcKickAndTacklePoints();
        //}
    }
}

void BoardSelectionState::reset()
{
    level->resetAnimationBoard();
    level->resetBoardSelection();
    level->moveSprites.clear();
    level->kickSprites.clear();
    goToDefaultState();
}

void BoardSelectionState::shipClicked(int index, int ship)
{
    if ((ship == 1 && level->activeTeam == 1 && level->team1[index].isInShip()) ||
        (ship == 2 && level->activeTeam == 2 && level->team2[index].isInShip()) ) {
            level->resetAnimationBoard();
            level->resetBoardSelection();
            level->setShipSelection(index, ship);
            level->setAnimationShip("Idle");
            level->nextState = new ShipSelectionState(level);
    }
}

void BoardSelectionState::hexClicked(vec2i hex)
{
    if ( !level->board.getHex(hex).isPlayground() ) {
		reset();
        return;
    }

    if (level->youControlCharacter(level->boardSelection)) {
        yourCharacter(hex);
    } else {
        notYourCharacter(hex);
    }
}

void BoardSelectionState::yourCharacter(vec2i hex)
{
    Character* selected = level->board.getHex(level->boardSelection).getCharacter();

    if (level->board.getHex(hex).isFree()) {
        if (/*selected->getRemainingActions() > 0 &&*/ linearSearch(movePoints, hex)) {
            move(selected, hex);
        } else {
			reset();
        }
    } else { // there is a character / ball
        Character* target = level->board.getHex(hex).getCharacter();
        
        if (selected->isSameTeam(target)) {
            if (hex.x == level->boardSelection.x && hex.y == level->boardSelection.y) {
                reset();
            } else {
                level->resetAnimationBoard();
                level->setBoardSelection(hex);
                level->setAnimationBoard("Idle");
                calcMovePoints();
                calcKickAndTacklePoints();
            }
        } else if (target->getTeam() == 0) { // target is a ball
            if (level->canKick(selected->getPosition(), target->getPosition())) {
                int direction = level->board.getDirection(level->boardSelection, hex);
                goToKickState(hex, static_cast<Direction>(direction), selected->getSkill());

            } else {
                reset();
            }
        } else { // target is an opponent
            if (level->canTackle(selected->getPosition(), target->getPosition())) {
                tackle(selected, target, hex);
                selected->setMovedToKickPos(true); // quick hack to prevent kicking the ball
                                                    // after tackling =)
            } else {
                //reset();
                level->resetAnimationBoard();
                level->setBoardSelection(hex);
                level->setAnimationBoard("Idle");
                level->moveSprites.clear();
                level->kickSprites.clear();
            }
        }
        
        
        
        /*else if (target->getTeam() == 0) { // target is a ball
            int direction = level->board.getDirection(level->boardSelection, hex);
            
            if (direction != -1 && selected->getRemainingActions() > 0) { // ball is next to selected
                goToKickState(hex, static_cast<Direction>(direction), selected->getSkill());
            } else {
                reset();
            }
        } else { // target is opponent
            tackle(selected, target, hex);
        }*/
    }
}

void BoardSelectionState::move(Character *selected, vec2i hex)
{
    if (hex == selected->getKickPosition()) {
        selected->setMovedToKickPos(true);
    }

    level->makeMovePathfind(level->boardSelection, hex);
    int dist = level->board.distance(level->boardSelection, hex);
    selected->spendMoves(dist);

    if (selected->getRemainingActions() <= 0 && selected->getRemainingMoves() <= 0) {
        if ( !level->canKick(selected->getPosition(), level->ball->getPosition()) ) {
            if (selected->getKickPosition().x == -1 ||
                selected->hasMovedToKickPos()) {
                    selected->swapHighlight();
            }
        }
    }

    selected->setAnimation(selected->getName() + "Run", 300 * dist);
    selected->addFollowingAnimation(selected->getName() + "Stop");
    level->setBoardSelection(hex);
    goToAnimationState();
}

void BoardSelectionState::tackle(Character *selected, Character *target, vec2i hex)
{
    int direction = level->board.getDirection(level->boardSelection, hex);
    
    if (direction != -1 && selected->getRemainingActions() > 0) { // target is next to selected
        Direction dir = static_cast<Direction>(direction);

        // TODO: päivitä tää uuteen sääntöversioon!
        //int bonus = 0; // potkujuna pois!
        /*
        int bonus = level->calcTackleBonus(
        level->boardSelection,
        Util::getOppositeDirection(dir),
        selectedChar->getTeam());
        */
        
        int tackle;
        int base = selected->getStrength() - target->getStrength();

        if (base >= 1) {
            tackle = 2;
        } else if (base >= -1) {
            tackle = 1;
        } else {
            tackle = 0;
        }

        int minus = level->calcTackleBonus(hex, dir, target->getTeam());

        //tackle += (selected->getStrength() + bonus);
        //tackle -= (target->getStrength() + minus);
        tackle -= minus;

        if (tackle > 0) {
            if (minus > 0) {
                std::vector<vec2i> points;
                vec2i point = hex;
                
                for (int i = 0; i < minus; ++i) {
                    point = level->board.getNeighbor(point.x, point.y, dir);
                    points.push_back(point);
                }
                
                for (int i = points.size() - 1; i >= 0; --i) {
                    level->makeTackle(points[i], dir, tackle);
                }
            }
            
            int tackled = level->makeTackle(hex, dir, tackle);
            
            selected->spendAction();

            if (selected->getRemainingActions() <= 0 && selected->getRemainingMoves() <= 0) {
                if ( !level->canKick(selected->getPosition(), level->ball->getPosition()) ) {
                    if (selected->getKickPosition().x == -1 ||
                        selected->hasMovedToKickPos()) {
                            selected->swapHighlight();
                    }
                }
            }
            
            if (tackled > 0) {
                selected->setAnimation(selected->getName() + "Tackle");
                selected->addFollowingAnimation(selected->getName() + "Idle");
                level->makeMove(level->boardSelection, hex);
                level->setBoardSelection(hex);
                level->kickSprites.clear();
                level->moveSprites.clear();
            }
            
            goToAnimationState();
        
        } else {
            selected->spendAction();
            reset();
        }
    } else {
        level->resetAnimationBoard();
        level->setBoardSelection(hex);
        level->setAnimationBoard("Idle");
        level->moveSprites.clear();
        level->kickSprites.clear();
    }
}

void BoardSelectionState::notYourCharacter(vec2i hex)
{
    Character* selectedChar = level->board.getHex(level->boardSelection).getCharacter();

    if ( !level->board.getHex(hex).isFree() ) {
        Character* targetChar = level->board.getHex(hex).getCharacter();

        if (targetChar->getTeam() != 0 && selectedChar != targetChar) {
            level->resetAnimationBoard();
            level->setBoardSelection(hex);
            level->setAnimationBoard("Idle");

            if ( !selectedChar->isSameTeam(targetChar) ) {
                calcMovePoints();
                calcKickAndTacklePoints();
            }
        } else {
            reset();
        }
    } else {
        reset();
    }
}

void BoardSelectionState::goToAnimationState()
{
    level->nextState = new AnimationState(level);
}

void BoardSelectionState::goToDefaultState()
{
    level->nextState = new DefaultState(level);
}

void BoardSelectionState::goToKickState(vec2i ballPosition, Direction d, int skill)
{
    level->ball->setAnimation(level->ball->getName() + "Curl", 500);
    //level->ball->addFollowingAnimation(level->ball->getName() + "Stop");
    level->nextState = new KickState(this->level, ballPosition, d, skill);
}

// pois tää puolivillainen suunnitelma...
/*
void BoardSelectionState::calcKickPoints()
{

}

void BoardSelectionState::calcTacklePoints()
{
    Character *selected = level->board.getHex(level->boardSelection).getCharacter();
    std::vector<vec2i> neighbors = level->board.getAllNeighbors(level->boardSelection);

    for (size_t i = 0; i < neighbors.size(); ++i) {
        if ( !level->board.getHex(neighbors[i]).isFree() ) {
            Character *target = level->board.getHex(neighbors[i]).getCharacter();

            if (target->getTeam() != 0 && target->getTeam() != selected->getTeam()) {

            }
        }
    }
}
*/

void BoardSelectionState::calcKickAndTacklePoints()
{
    if (level->kickSprites.size() > 0) {
        level->kickSprites.clear();
    }

    Character* selected = level->board.getHex(level->boardSelection).getCharacter();
    std::vector<vec2i> nbs = level->board.getAllNeighbors(level->boardSelection);

    for (size_t i = 0; i < nbs.size(); ++i) {
        if ( !level->board.getHex(nbs[i]).isFree() ) {
            Character* target = level->board.getHex(nbs[i]).getCharacter();

            //if (c->getTeam() == 0) { // ball
            if (level->canKick(selected->getPosition(), target->getPosition())) {
                Drawable d;

                d.shape = AssetFactory::getInstance().getShape("SPRITE");
                d.material.texture = AssetFactory::getInstance().getTexture("UI_HEXATLAS");
                d.material.color = new vec3(0, 1, 0);
                d.useAtlas = true;
                d.coords.x = 0;
                d.coords.y = 128 * 2;
                d.coords.width = 128;
                d.coords.height = 64;
                d.transform.setScale(1, 0.5, 1);

                vec3 point = level->board.getHex(nbs[i]).transform.position;
                d.transform.setPosition(point.x, point.y, CHARACTER_FRONT);
                //level->moveSprites.push_back(d);
                level->kickSprites.push_back(d);

            } //else if (c->getTeam() != selectedChar->getTeam()) { // opponent
            else if (level->canTackle(selected->getPosition(), target->getPosition())) {
                Drawable d;

                d.shape = AssetFactory::getInstance().getShape("SPRITE");
                d.material.texture = AssetFactory::getInstance().getTexture("UI_HEXATLAS");
                d.material.color = new vec3(1, 0, 0);
                d.useAtlas = true;
                d.coords.x = 0;
                d.coords.y = 128 * 2;
                d.coords.width = 128;
                d.coords.height = 64;
                d.transform.setScale(1, 0.5, 1);

                vec3 point = level->board.getHex(nbs[i]).transform.position;
                d.transform.setPosition(point.x, point.y, CHARACTER_FRONT);
                //level->moveSprites.push_back(d);
                level->kickSprites.push_back(d);
            }
        }
    }
}

// WANHA WERSIO
/*
void BoardSelectionState::calcKickAndTacklePoints()
{
    if (level->kickSprites.size() > 0) {
        level->kickSprites.clear();
    }

    Character* selectedChar = level->board.getHex(level->boardSelection).getCharacter();
    std::vector<vec2i> nbs = level->board.getAllNeighbors(level->boardSelection);

    for (size_t i = 0; i < nbs.size(); ++i) {
        if ( !level->board.getHex(nbs[i]).isFree() ) {
            Character* c = level->board.getHex(nbs[i]).getCharacter();

            if (c->getTeam() == 0) { // ball
                Drawable d;

                d.shape = AssetFactory::getInstance().getShape("SPRITE");
                d.material.texture = AssetFactory::getInstance().getTexture("UI_HEXATLAS");
                d.material.color = new vec3(0, 1, 0);
                d.useAtlas = true;
                d.coords.x = 0;
                d.coords.y = 128 * 2;
                d.coords.width = 128;
                d.coords.height = 64;
                d.transform.setScale(1, 0.5, 1);

                vec3 point = level->board.getHex(nbs[i]).transform.position;
                d.transform.setPosition(point.x, point.y, CHARACTER_FRONT);
                //level->moveSprites.push_back(d);
                level->kickSprites.push_back(d);

            } else if (c->getTeam() != selectedChar->getTeam()) { // opponent
                Drawable d;

                d.shape = AssetFactory::getInstance().getShape("SPRITE");
                d.material.texture = AssetFactory::getInstance().getTexture("UI_HEXATLAS");
                d.material.color = new vec3(1, 0, 0);
                d.useAtlas = true;
                d.coords.x = 0;
                d.coords.y = 128 * 2;
                d.coords.width = 128;
                d.coords.height = 64;
                d.transform.setScale(1, 0.5, 1);

                vec3 point = level->board.getHex(nbs[i]).transform.position;
                d.transform.setPosition(point.x, point.y, CHARACTER_FRONT);
                //level->moveSprites.push_back(d);
                level->kickSprites.push_back(d);
            }
        }
    }
}
*/

void BoardSelectionState::calcMovePoints()
{
    if (movePoints.size() > 0) {
        movePoints.clear();
    }

    // TODO: improve implementation

    HashSet<vec2iHash> set(50);
    Character* ch = level->board.getHex(level->boardSelection).getCharacter();

    std::vector<vec2i> points = level->board.getFreeNeighbors(level->boardSelection);

    if (ch->getRemainingMoves() > 0) {
        for (size_t i = 0; i < points.size(); ++i) {
            vec2iHash hp(points[i]);

            set.add(vec2iHash(points[i]));
            calcMovePoints(set, points[i], ch->getRemainingMoves() - 1);
        }
    }
    
    if (ch->getKickPosition().x != -1 && !ch->hasMovedToKickPos()) {
        if (level->board.getHex(ch->getKickPosition()).isFree()) {
            //set.add(vec2iHash(ch->getKickPosition()));
            movePoints.push_back(ch->getKickPosition());
        }
    }

    std::vector<vec2iHash> elements = set.elements();

    for (size_t i = 0; i < elements.size(); ++i) {
        movePoints.push_back(elements[i].vec);
    }

    calcMoveSprites();
}

void BoardSelectionState::calcMovePoints(HashSet<vec2iHash>& set, vec2i point, int moves)
{
    if (moves <= 0) {
        return;
    }

    std::vector<vec2i> points = level->board.getFreeNeighbors(point);

    for (size_t i = 0; i < points.size(); ++i) {
        vec2iHash hp(points[i]);

        set.add(vec2iHash(points[i]));
        calcMovePoints(set, points[i], moves - 1);
    }
}

void BoardSelectionState::calcMoveSprites()
{
    if (level->moveSprites.size() > 0) {
        level->moveSprites.clear();
    }

    for (size_t i = 0; i < movePoints.size(); ++i) {
        Drawable d;

        d.shape = AssetFactory::getInstance().getShape("SPRITE");
        d.material.texture = AssetFactory::getInstance().getTexture("UI_HEXATLAS");
        d.material.color = new vec3(1, 1, 1);
        d.useAtlas = true;
        d.coords.x = 0;
        d.coords.y = 0;
        d.coords.width = 128;
        d.coords.height = 128;
        
        vec3 point = level->board.getHex(movePoints[i]).transform.position;
        d.transform.setPosition(point.x, point.y, CHARACTER_BEHIND);

        level->moveSprites.push_back(d);
    }
}