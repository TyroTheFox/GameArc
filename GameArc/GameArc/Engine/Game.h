#pragma once
/**
* \class Game
* \file Game.h
* \brief A container and manager object for the game that's driven by the Engine Core
*
* A container and manager object for scenes that's driven by the Engine Core. Updates and Sets Up all active game objects.
* It's this that the player actually sees and interacts with.
*/
#include "Model.h"

#include "IEngineCore.h"
#include "GameObject.h"
#include "Camera.h"
#include "Scene.h"

#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#include "TransformComponent.h"
#include "ColourComponent.h"
#include "ModelComponent.h"
#include "DebugHelper.h"
#include "ObjectManager.h"
#include "global.h"

extern Event* keyEvent;///Forward declared key event object

class Game
{
public:
	ObjectManager* oM;///Object manager
	DebugHelper * debugHelper;///Debug helper object used for debug console opperations
	IEngineCore* m_engineInterfacePtr;///Engine core pointer used for updating and rendering objects
	Game();///Constructor
	Game(string levelsFile, DebugHelper* debug);///Constructor
	bool loadFromJSON(string levelsFile);///Load scene data from JSON file
	void ChangeScene(string sceneName); ///Change currently displayed scene to a new one
	void init();///Initialise variables once Game Engine Core has been initialised
	void update(float dt);///Update all scenes
	void render();///Render all scene contents

private:
	GameObject* m_playerBackground;///Background colour

	Camera m_camera;///Default camera if no camera is found among game objects
	Camera* m_MainCamera;///Main current camera among objects in a scene

	Scene* m_currentScene;///Currently rendered scene
	std::map<string, Scene*> sceneList;///List of active scenes

	PlayerComponent* activePlayer;///Currently active player object
};
