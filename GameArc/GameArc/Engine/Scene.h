#pragma once
/**
* \class Scene
* \file Scene.h
* \author Kieran Clare
* \brief Handles encapsulation of different objects into managable, arangable groups
*
* Encapsulates and manages game objects within to allow for quick and easy switching between 'screens' of the game
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

#include "ObjectManager.h"
#include "DebugHelper.h"

using namespace std;

class Scene
{
private:
	///Factory object for game objects
	ObjectManager * objectManager;
	///Stores game objects and allows for easy access
	std::map<std::string, GameObject*> m_gameObjects;
public:
	///ID used to get only registered objects
	int ID;
	///Debug variable for debug menu actions and displaying necessary information
	DebugHelper* debug;
	///Constructor
	Scene(ObjectManager* oM);
	///Constructor
	Scene(DebugHelper* d, ObjectManager* oM);
	///Load in and build level objects from JSON
	bool loadLevelJSON(std::string levelFile);
	///Returns game objects map
	std::map<std::string, GameObject*> getGameObjects();
	///Sets game objects map to that provided
	void setGameObjects(std::map<std::string, GameObject*> go);
};
