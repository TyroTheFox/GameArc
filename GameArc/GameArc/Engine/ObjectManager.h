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

#include "ModelHandler.h"
#include "DebugHelper.h"

/**
* \class Object Manager
* \file ObjectManager.h
* \brief A factory object that creates each component and game object from JSON files
*
* Partly based on the basic fundermentals provided by Stefan Bauer, this factory object dynamically reads in the JSON file
* and constructs game objects with the use of lambda functions to call the correct functions for each required component
*/

class ObjectManager {
private:
	///List of lambda functions that will construct a given component
	std::map<std::string, std::function<void(GameObject* object, const Json::Value& p_component)>> m_componentJsonBuilders;
	///Stored game objects this Game instance
	std::map<std::string, GameObject*> m_gameObjects;
	///Register of which game objects belong to which scene
	std::multimap<int, std::string> sceneIDRegister;
	///Latest Scene ID made
	int latestID = 0;
	///Debug handler object pointer
	DebugHelper* debug;
	///Model Handler object pointer
	ModelHandler* modelHandler;
public:
	///Constructor
	ObjectManager(DebugHelper* d, ModelHandler* mH);
	///Get new Scene ID
	int getNewID();
	///Load Level from file
	bool loadLevelJSON(std::string levelFile, int id);
	///Attach component of given type to given game object
	template<typename T> void attachComponent(GameObject* object, const Json::Value& p_component);
	///Return list of created game objects registered to given ID
	std::map<std::string, GameObject*> getGameObjects(int id);
};