#include "Animation.h"

Animation::Animation()
{
    name = "";
    nextAnimation = "";

    timeElapsed = 0;
    startTime = 0;
    animationLength = 0;
    
    //frameCount = 0;
    currentFrame = 0;
 
    startX = 0;
    startY = 0;
    spriteWidth = 0;
    spriteHeight = 0;

    //columnCount = 0;
    //startColumn = 0;

    loop = false;
    reverse = false;
}

Animation::~Animation()
{

}

bool Animation::update(uint64 deltaTime, AtlasCoords& coords)
{
    timeElapsed += deltaTime;
    int newFrame;

    if (timeElapsed >= startTime) {
        uint64 effectiveTime = timeElapsed - startTime;
        //float frameTime = (float)animationLength / (float)frameCount;
        float frameTime = (float)animationLength / (float)frames.size();

        if (!reverse) {
            int frame = 1 + effectiveTime / frameTime;

            if ((unsigned int)frame > frames.size()) {
                if (loop) { // loop
                    while ((unsigned int)frame > frames.size()) {
                        frame -= frames.size();
                    }
                    newFrame = frame;

                } else {
                    newFrame = frames.size();
                }
            } else { // if (frame <= frameCount)
                newFrame = frame;
            }
        } else { // if (reverse)
            int frame = frames.size() - effectiveTime / frameTime;

            if (frame < 1) {
                if (loop) {
                    while (frame < 1) {
                        frame += frames.size();
                    }
                    newFrame = frame;

                } else {
                    newFrame = 1;
                }
            } else { // if (frame >= 1)
                newFrame = frame;
            }
        }
    } else { // if (timeElapsed < StartTime)
        newFrame = 0;
    }

    if (newFrame != currentFrame) {
        currentFrame = newFrame;
        //updateCoords(coords);
        updateCoords(coords);
        return true;
    }

    return false;
}

/*
void Animation::updateCoords(AtlasCoords& coords)
{
    if (currentFrame > 0 && currentFrame <= frameCount) {
        float originX = startX - (startColumn - 1) * spriteWidth;
        float originY = startY;

        int row = (currentFrame + startColumn - 2) / columnCount;
        int column = (currentFrame + startColumn - 2) % columnCount;

        coords.x = originX + column * spriteWidth;
        coords.y = originY + row * spriteHeight;
        coords.width = spriteWidth;
        coords.height = spriteHeight;
    }
}
*/

void Animation::updateCoords(AtlasCoords& coords)
{
    if (currentFrame > 0 && (unsigned int)currentFrame <= frames.size()) {
        AtlasCoords frame = frames[currentFrame - 1];

        coords.x = frame.x;
        coords.y = frame.y;
        coords.width = frame.width;
        coords.height = frame.height;

        coords.offsetX = frame.offsetX;
        coords.offsetY = frame.offsetY;
        coords.originalWidth = frame.originalWidth;
        coords.originalHeight = frame.originalHeight;
    }
}

bool Animation::isOver()
{
    return timeElapsed > duration + startTime;
}