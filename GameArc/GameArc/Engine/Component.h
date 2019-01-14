#ifndef _COMPONENT_H_
#define _COMPONENT_H_
#include <string>
#include "json.h"
class Component
{
public:
	bool active = true;
	virtual void OnUpdate(float dt) = 0;
	virtual void OnMessage(const std::string m) = 0;
	virtual void BuildFromJson(const Json::Value& componentJSON) = 0;
	virtual void BuildToJson(Json::Value& componentJSON) = 0;
};
#endif
