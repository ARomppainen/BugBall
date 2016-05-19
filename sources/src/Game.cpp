#include "game.h"

#include "s3e.h"
#include "IwSound.h"
#include "IwResManager.h"
#include "Iw2D.h"

#include "Input.h"
#include "LevelManager.h"
#include "MainMenu.h"
#include "TestLevel.h"
#include "AssetFactory.h"

Game::Game()
{
	quit = false;
	Iw2DInit();
}

Game::~Game()
{

}

int Game::initialize()
{
	return eglInit();
}

int Game::eglInit()
{
	if ( !IwGLInit() )
	{
		s3eDebugErrorShow(S3E_MESSAGE_CONTINUE, "eglInit failed");
			return 1;
	}

    // enable depth buffering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 0;
}

void Game::mainLoop()
{
    // initialize
    deltaTime = 0;

    // TODO: create a config file and move these there
    const uint64 TARGET_FRAME_MS = 16;
    const uint64 MAX_FRAME_MS = 100;

    // init assets
    AssetFactory::getInstance().init();

		//init resource manager
#ifdef IW_BUILD_RESOURCES
	IwGetResManager()->AddHandler(new CIwResHandlerWAV);
#endif

#ifdef _DEBUG
	IwGetResManager()->LoadGroup("TestLevel.group");
#endif

    // create level
    //LevelManager::getInstance().pushLevel(new MainMenu(this));
    LevelManager::getInstance().pushLevel(new TestLevel());
    LevelManager::getInstance().getActiveLevel()->initialize();

    // init input
    Input::getInstance();
	
	//Init sound and resmanager
	//IwGxInit();
	//Iw2DInit();
	//Iw2DSetAlphaMode(IW_2D_ALPHA_HALF);
	IwSoundInit();
	IwResManagerInit();


	IwGetResManager()->LoadGroup("TestLevel.group");

    // loop
    while( !quit )
    {
        //s3eDeviceYield(0); // S3E applications should yield frequently
        uint64 time = s3eTimerGetMs();

		if ( s3eDeviceCheckQuitRequest() ) {
			quit = true;
		}

		IwGetSoundManager()->Update();
		update();
		render();

        deltaTime = s3eTimerGetMs() - time;
        int yield = 0;

        if (deltaTime < TARGET_FRAME_MS) {
            yield = TARGET_FRAME_MS - deltaTime;
            deltaTime = TARGET_FRAME_MS;
        } else if (deltaTime > MAX_FRAME_MS) {
            deltaTime = MAX_FRAME_MS;
        }

        s3eDeviceYield(yield);
    }

    // clean up
    Input::getInstance().unregister();

	IwSoundTerminate();
	IwResManagerTerminate();
	Iw2DTerminate();
}

void Game::update()
{
    Input::getInstance().update();
    LevelManager::getInstance().getActiveLevel()->update(deltaTime);
}

void Game::render()
{
    int w = IwGLGetInt(IW_GL_WIDTH);
    int h = IwGLGetInt(IW_GL_HEIGHT);

    glViewport(0, 0, w, h);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    LevelManager::getInstance().getActiveLevel()->render();

    IwGLSwapBuffers();
}

void Game::start()
{
    if( initialize() == 0 ) {
        mainLoop();
    }
}

void Game::quitGame()
{
	quit = true;
}