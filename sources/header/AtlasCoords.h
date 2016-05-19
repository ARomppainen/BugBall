#pragma once

struct AtlasCoords
{
    AtlasCoords()
        : x(0), y(0), 
        width(0), height(0),
        offsetX(0), offsetY(0),
        originalWidth(0), originalHeight(0) {};

    float x;
    float y;
    float width;
    float height;

    float offsetX;
    float offsetY;
    float originalWidth;
    float originalHeight;
};