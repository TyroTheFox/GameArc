#define USE_GLFW 1
#ifdef USE_GLFW
#include <Engine/GLFW_EngineCore.h>
#endif
#include <Engine/DebugHelper.h>
#include <Engine/Game.h>
#include <Engine/ModelHandler.h>
#include <Engine/global.h>

extern Event* keyEvent;

int windowWidth = 800, windowHeight = 600;

int main()
{
	IEngineCore* engineCore;
	keyEvent = new Event();

#ifdef USE_GLFW
	engineCore = new GLFW_EngineCore();
#endif


	if (!engineCore->initWindow(windowWidth, windowHeight, "GameArc"))
		return -1;
	DebugHelper* debugHelper = new DebugHelper(engineCore);
	Game myFirstGame("assets/levels.json", debugHelper);

	engineCore->runEngine(myFirstGame);

	delete engineCore;

	return 0;
}

