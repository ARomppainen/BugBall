#pragma once

#include <iostream>

class Game
{
public:

	Game();
	~Game();

	void start();
	void quitGame();

private:

	bool quit;
    uint64 deltaTime;

	int initialize();
	int eglInit();

	void mainLoop();
	void update();
	void render();
};