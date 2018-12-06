#define USE_GLFW 1
#ifdef USE_GLFW
#include "GLFW_EngineCore.h"
#endif

#include "Game.h"
#include "include\global.h"

extern Event* keyEvent;
extern InputHandler* inputHandler;

int main(int argc, char* argv[])
{
	IEngineCore* engineCore;
	inputHandler = new InputHandler();
	keyEvent = new Event();

#ifdef USE_GLFW
	engineCore = new GLFW_EngineCore;
#endif

	if (!engineCore->initWindow(800, 600, "TransformAndData"))
		return -1;

	Game myFirstGame;

	engineCore->runEngine(myFirstGame);

	return 0;
}
