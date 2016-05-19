#include "Level.h"

#include "GameState.h"

#include "IwGL.h"

Level::Level()
{
    screenWidth = IwGLGetInt(IW_GL_WIDTH);
    screenHeight = IwGLGetInt(IW_GL_HEIGHT);
}


Level::~Level()
{
}