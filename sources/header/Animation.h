#pragma once

#include <iostream>
#include <vector>

#include "AtlasCoords.h"

class Animation
{
public:

    Animation();
    ~Animation();

    bool update(uint64 deltaTime, AtlasCoords& coords);
    void updateCoords(AtlasCoords& coords);
    bool isOver();

public:

    std::vector<AtlasCoords> frames;

    std::string name;
    std::string nextAnimation;

    uint64 timeElapsed;
    uint64 startTime;
    uint64 animationLength; // the length of a single loop
    uint64 duration; // the duration the animation plays
    
    //int frameCount;
    int currentFrame;
 
    float startX;
    float startY;
    float spriteWidth;
    float spriteHeight;
    float originalWidth;
    float originalHeight;
    float offsetX;
    float offsetY;

    //int columnCount;
    //int startColumn;

    bool reverse;
    bool loop;
};