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

#include "ModelHandler.h"

extern InputHandler* inputHandler;

class Game
{
public:
	Game();
	IEngineCore* m_engineInterfacePtr;

	InputHandler* m_inputHandler;

	void init();
	void update(float dt);
	void render();

private:
	GameObject m_playerBackground;

	Camera m_camera;

	Scene* m_currentScene;

	PlayerComponent* activePlayer;
};
