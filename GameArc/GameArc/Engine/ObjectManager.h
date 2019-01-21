#pragma once
/**
* \class Object Manager
* \file ObjectManager.h
* \brief A factory object that creates each component and game object from JSON files
*
* Partly based on the basic fundermentals provided by Stefan Bauer, this factory object dynamically reads in the JSON file
* and constructs game objects with the use of lambda functions to call the correct functions for each required component
*/
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
	std::map<std::string, std::function<void(GameObject* object, const Json::Value& p_component)>> m_componentJsonBuilders;///List of lambda functions that will construct a given component
	std::map<std::string, GameObject*> m_gameObjects;///Stored game objects this Game instance
	std::multimap<int, std::string> sceneIDRegister;///Register of which game objects belong to which scene
	int latestID = 0;///Latest Scene ID made
	DebugHelper* debug;///Debug handler object
public:
	ObjectManager(DebugHelper* d);///Constructor
	int getNewID();///Get new Scene ID
	bool loadLevelJSON(std::string levelFile, int id);///Load Level from file
	template<typename T> void attachComponent(GameObject* object, const Json::Value& p_component);///Attach component of given type to given game object
	std::map<std::string, GameObject*> getGameObjects(int id);///Return list of created game objects registered to given ID
};