#pragma once
#include "Model.h"

#include "IEngineCore.h"
#include "InputHandler.h"
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
class Game
{
public:
	Game();
	IEngineCore* m_engineInterfacePtr;

	InputHandler* m_inputHandler;
	//vector<InputHandler*> inputHandlers;

	void update();
	void render();
	//vector<InputHandler*> getInputHandlers();

private:
	GameObject m_playerBackground;

	Camera m_camera;

	Scene* m_currentScene;
};
