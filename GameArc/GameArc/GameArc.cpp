#define USE_GLFW 1
#ifdef USE_GLFW
#include "GLFW_EngineCore.h"
#endif

#include "Game.h"
#include "Engine/global.h"

extern Event* keyEvent;
extern InputHandler* inputHandler;

int main(int argc, char* argv[])
{
//	inputHandler = new InputHandler("assets/inputs.json");
//	keyEvent = new Event();
//	modelHandler = new ModelHandler();
//	IEngineCore* engineCore;
//
//#ifdef USE_GLFW
//	engineCore = new GLFW_EngineCore;
//#endif
//
//	if (!engineCore->initWindow(800, 600, "TransformAndData"))
//		return -1;
//
//	Game myFirstGame;
//
//	engineCore->runEngine(myFirstGame);
//
//	return 0;
}

//TODO
// 1) Map Controls Externally - DONE
// 2) Update Event Camera to change scenes through JSON entirely - DONE
// 3) Set Up Tests - DONE
// 4) Drawing Debug Objects
// 5) On-Screen Debug Messages - DONE
// 6) Debug Menu - DONE
// 7) Doxygen Comment Everything - DONE

// Optional
// 1) Skybox
// 2) Textures (Basic) - DONE