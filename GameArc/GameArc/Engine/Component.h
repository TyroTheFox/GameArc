#pragma once
/**
* \class Component
* \file Component.h
* \author Kieran Clare
* \brief Base Component Template
*
* The base template for all components that contains all generic component variables
*/
#include <string>
#include "json.h"
#include "DebugHelper.h"
#include "IEngineCore.h"

class DebugHelper;
class GameObject;
class IEngineCore;

class Component
{
public:
	///Whether component is active or not
	bool active = true;
	///Debug class for displaying internal variables
	DebugHelper* debug;
	///The Game Object the Component is attached to
	GameObject* parent;
	///Called after the Component has been attached to the parent Game Object
	virtual void OnSetUp() = 0;
	///Called every update tick to update component variables
	virtual void OnUpdate(float dt) = 0;
	///Called every update tick to draw component variables
	virtual void OnRender(IEngineCore* m_engineInterfacePtr) = 0;
	///Called on event fires to alter component variables
	virtual void OnMessage(const std::string m) = 0;
	///Build component from JSON elements
	virtual void BuildFromJson(const Json::Value& componentJSON) = 0;
};
