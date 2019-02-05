#pragma once
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
#include "ModelHandler.h"
#include "InputHandler.h"
#include "LightHandler.h"
#include "global.h"
///Forward declared key event object
extern Event* keyEvent;

/** \class Game
\file Game.h
\brief A container and manager object for the game that's driven by the Engine Core

A container and manager object for scenes that's driven by the Engine Core. Updates and Sets Up all active game objects.
It's this that the player actually sees and interacts with.
*/

class Game
{
public:
	///Object manager
	ObjectManager* oM;
	///Debug helper object used for debug console opperations
	DebugHelper * debugHelper;
	///Engine core pointer used for updating and rendering objects
	IEngineCore* m_engineInterfacePtr;
	///Model Handler object pointer
	ModelHandler* modelHandler;
	///Input handler object pointer
	InputHandler* inputHandler;
	LightHandler* lightHandler;
	///Constructor
	Game();
	///Constructor
	Game(string levelsFile, DebugHelper* debug);
	///Deletes set up game objects
	void CleanUp();
	///Load scene data from JSON file
	bool loadFromJSON(string levelsFile);
	///Change currently displayed scene to a new one
	void ChangeScene(string sceneName); 
	///Initialise variables once Game Engine Core has been initialised
	void init(InputHandler* iH);
	///Update all scenes
	void update(float dt);
	///Render all scene contents
	void render();
private:
	///Background colour
	GameObject* m_playerBackground;
	///Default camera if no camera is found among game objects
	Camera m_camera;
	///Main current camera among objects in a scene
	Camera* m_MainCamera;
	///Currently rendered scene
	Scene* m_currentScene;
	///List of active scenes
	std::map<string, Scene*> sceneList;
	///Currently active player object
	PlayerComponent* activePlayer;
};
