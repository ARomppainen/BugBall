#include "LevelManager.h"

LevelManager* LevelManager::instance = 0;

LevelManager::LevelManager()
{

}

LevelManager& LevelManager::getInstance()
{
    if ( !instance ) {
        instance = new LevelManager();
    }

    return *instance;
}

void LevelManager::pushLevel(Level* level)
{
    levels.push(level);
}

void LevelManager::popLevel()
{
    levels.pop();
}

Level* LevelManager::getActiveLevel()
{
    return levels.top();
}