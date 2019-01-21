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

class ObjectManager {
private:
	std::map<std::string, std::function<void(GameObject* object, const Json::Value& p_component)>> m_componentJsonBuilders;
	std::map<std::string, GameObject*> m_gameObjects;
	std::multimap<int, std::string> sceneIDRegister;
	int latestID = 0;
	DebugHelper* debug;
public:
	ObjectManager(DebugHelper* d);
	int getNewID();
	bool loadLevelJSON(std::string levelFile, int id);
	template<typename T> void attachComponent(GameObject* object, const Json::Value& p_component);
	std::map<std::string, GameObject*> getGameObjects(int id);
};