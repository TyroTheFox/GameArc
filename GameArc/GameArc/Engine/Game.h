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
#include "global.h"

extern Event* keyEvent;

class Game
{
public:
	Game();
	Game(string levelsFile);
	bool loadFromJSON(string levelsFile);
	void ChangeScene(string sceneName);
	IEngineCore* m_engineInterfacePtr;
	void init();
	void update(float dt);
	void render();

private:
	GameObject m_playerBackground;

	Camera m_camera;
	Camera* m_MainCamera;

	Scene* m_currentScene;
	std::map<string, Scene*> sceneList;

	PlayerComponent* activePlayer;

	DebugHelper* debugHelper;
};
