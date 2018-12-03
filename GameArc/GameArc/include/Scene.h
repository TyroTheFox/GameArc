#pragma once
#include "Model.h"
#include "InputHandler.h"
#include "GameObject.h"

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <memory>
#include <functional>
//#include "json.h"

#include "TransformComponent.h"
#include "ColourComponent.h"
#include "ModelComponent.h"

using namespace std;

class Scene
{
public:
	Scene();

	vector<InputHandler*> inputHandlers;

	void loadLevelTxt(std::string levelFile);
	bool loadLevelJSON(std::string levelFile);

	std::map<std::string, GameObject*> getGameObjects();
	vector<InputHandler*> getInputHandlers();

	template<typename T> void attachComponent(GameObject* object, const Json::Value& p_component);

	vector<string> m_displayCubes;

private:
	GameObject m_playerBackground;

	//vector<GameObject*> m_gameObjects;
	std::map<std::string, GameObject*> m_gameObjects;
	std::map<std::string, std::function<void(GameObject* object, const Json::Value& p_component)>> m_componentJsonBuilders;
	int numElementsToRead;

	stringstream ss;

	vector<glm::vec3> translations;
	vector<glm::quat> rotations;
	vector<glm::vec3> scalars;

	//void buildLevel();
};
