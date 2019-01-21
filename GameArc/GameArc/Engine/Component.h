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
	bool active = true;///Whether component is active or not
	DebugHelper* debug;///Debug class for displaying internal variables
	GameObject* parent;
	virtual void OnSetUp() = 0;
	virtual void OnUpdate(float dt) = 0;
	virtual void OnRender(IEngineCore* m_engineInterfacePtr) = 0;
	virtual void OnMessage(const std::string m) = 0;
	virtual void BuildFromJson(const Json::Value& componentJSON) = 0;
	virtual void BuildToJson(Json::Value& componentJSON) = 0;
};
