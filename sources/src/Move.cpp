#include "Move.h"

#include "Layer.h"
#include "Util.h"

Move::Move(Character* object, uint64 duration, vec3 start, vec3 end)
{
    object->setMoving(true);

    this->object = object;
    this->duration = duration;
    this->elapsed = 0;
    this->delay = 0;

    points.push_back(start);
    points.push_back(end);

    for (size_t i = 0; i < points.size(); ++i) {
        points[i].z = CHARACTER + (points[i].z - BOARD);
    }
}

Move::Move(Character* object, uint64 duration, std::vector<vec3> points)
{
    object->setMoving(true);

    this->object = object;
    this->duration = duration;
    this->elapsed = 0;
    this->delay = 0;

    this->points = points;

    for (size_t i = 0; i < points.size(); ++i) {
        points[i].z = CHARACTER + (points[i].z - BOARD);
    }
}

Move::Move(Character* object, uint64 duration, uint64 delay, vec3 start, vec3 end)
{
    object->setMoving(true);

    this->object = object;
    this->duration = duration;
    this->elapsed = 0;
    this->delay = delay;

    points.push_back(start);
    points.push_back(end);

    for (size_t i = 0; i < points.size(); ++i) {
        points[i].z = CHARACTER + (points[i].z - BOARD);
    }
}

Move::Move(Character* object, uint64 duration, uint64 delay, std::vector<vec3> points)
{
    object->setMoving(true);

    this->object = object;
    this->duration = duration;
    this->elapsed = 0;
    this->delay = delay;

    this->points = points;

    for (size_t i = 0; i < points.size(); ++i) {
        points[i].z = CHARACTER + (points[i].z - BOARD);
    }
}

Move::~Move()
{

}

void Move::update(uint64 deltaTime)
{
    elapsed += deltaTime;

    if (elapsed >= delay) {

        uint64 actual = elapsed - delay;

        if (actual >= duration) {
            elapsed = duration + delay;
            actual = duration;
		
		    object->setMoving(false);
        } /*else {
		    object->setMoving(true);
		    //object->highlightHex.render = false;
	    }*/

        int moves = points.size() - 1;
        float moveTime = (float)duration / (float)moves;
        int move = actual / moveTime;
        float current = actual - move * moveTime;

        object->transform.setPosition(Util::lerp(points[move], points[move+1], current / moveTime));
    }
}

vec3 Move::currentPosition() const
{
    return object->transform.position;
}

bool Move::isFinished() const
{
    return elapsed >= duration + delay ? true : false;
}