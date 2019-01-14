#define USE_GLFW 1
#ifdef USE_GLFW
#include <Engine/GLFW_EngineCore.h>
#endif

#include <Engine/Game.h>
#include <Engine/global.h>

extern Event* keyEvent;
extern InputHandler* inputHandler;
extern ModelHandler* modelHandler;

int main()
{
	inputHandler = new InputHandler("assets/inputs.json");
	keyEvent = new Event();
	modelHandler = new ModelHandler();
	IEngineCore* engineCore;

#ifdef USE_GLFW
	engineCore = new GLFW_EngineCore();
#endif

	if (!engineCore->initWindow(800, 600, "TransformAndData"))
		return -1;

	Game myFirstGame("assets/levels.json");

	engineCore->runEngine(myFirstGame);

	return 0;
}

