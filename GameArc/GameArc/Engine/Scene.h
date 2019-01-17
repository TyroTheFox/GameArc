#pragma once
#include "Model.h"
#include "GameObject.h"

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <memory>
#include <functional>
#include "json.h"

#include "TransformComponent.h"
#include "ColourComponent.h"
#include "ModelComponent.h"
#include "PlayerComponent.h"
#include "EventCameraComponent.h"
#include "TextUIComponent.h"

#include "DebugHelper.h"

using namespace std;

class Scene
{
public:
	DebugHelper* debug;
	vector<string> m_displayCubes;
	//vector<InputHandler*> inputHandlers;
	Scene();
	Scene(DebugHelper* d);
	bool loadLevelJSON(std::string levelFile);

	std::map<std::string, GameObject*> getGameObjects();
	void setGameObjects(std::map<std::string, GameObject*> go);

	template<typename T> void attachComponent(GameObject* object, const Json::Value& p_component);

private:
	GameObject m_playerBackground;

	std::map<std::string, GameObject*> m_gameObjects;
	std::map<std::string, std::function<void(GameObject* object, const Json::Value& p_component)>> m_componentJsonBuilders;
	int numElementsToRead;

	stringstream ss;

	vector<glm::vec3> translations;
	vector<glm::quat> rotations;
	vector<glm::vec3> scalars;
};
