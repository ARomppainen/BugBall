#pragma once

#include "Level.h"

#include <stack>

class LevelManager
{
public:

    static LevelManager& getInstance();

    Level* getActiveLevel();

    void pushLevel(Level* level);
    void popLevel();

private:

    LevelManager();

    std::stack<Level*> levels;

    static LevelManager* instance;
};