#pragma once

#include <iostream>

class Game; // forward declaration

class Level
{
public:
	Level();
	virtual ~Level();

	virtual void initialize() = 0;
	virtual void render() = 0;
    virtual void update(uint64 deltaTime) = 0;

    float screenWidth;
    float screenHeight;
};