#include "Ball.h"

Ball::Ball() :
    Character("BallBug", 1, 1, 1, 0, true)
{
    this->scale = 0.8f;
    //this->scale = 10.0f;
}

Ball::Ball(int movement) :
    Character("BallBug", 1, movement, 1, 0, true)
{
    this->scale = 0.8f;
    //this->scale = 10.0f;
}

Ball::~Ball()
{
    
}

void Ball::updateOffset()
{
    // äijä on kohdistettu original width / 2 kohtaan
    // transleittaa:
    // (original width / 2 - offset + width / 2) / mysterySize * skaala???

    float mysterySize = 128.0f;

    float offsetX = (coords.originalWidth / 2 - coords.offsetX - coords.width / 2) / mysterySize * scale;
    float offsetY = (coords.originalHeight / 2 - coords.offsetY - coords.height / 2) / mysterySize * scale;

    if (!flipX) {
        offsetX = -offsetX;
    }

    transform.setOffset(offsetX, offsetY, 0);
}