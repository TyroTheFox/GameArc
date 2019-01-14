#ifndef _MODELCOMPONENT_H_
#define _MODELCOMPONENT_H_
#pragma once
#include "Component.h"
#include "Model.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class ModelComponent : public Component
{
public:
	Model* model;
	ModelComponent() {}
	ModelComponent(Model* m) : model(m) {}
	void OnUpdate(float dt) override {}
	void OnMessage(const std::string m) override {}
	void BuildFromJson(const Json::Value& componentJSON) override {
		try {
			if (componentJSON.isMember("fileName")) {
				const Json::Value& fileName = componentJSON["fileName"];
				model = new Model(fileName.asCString());
			}
			if (componentJSON.isMember("active")) {
				const Json::Value& a = componentJSON["active"];
				active = a.asBool();
			}
		}
		catch (...) {
			std::cout << "Exception thrown in parsing BuildFromJson in TransformComponent." << std::endl;
			throw;
		}
	}	
	void BuildToJson(Json::Value& componentJSON) override {}
private:
};
#endif