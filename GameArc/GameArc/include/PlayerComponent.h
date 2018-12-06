#ifndef _PLAYERCOMPONENT_H_
#define _PLAYERCOMPONENT_H_

#pragma once
#include "Component.h"
#include "global.h"
#include "TransformComponent.h"
#include "GameObject.h"

extern Event* keyEvent;

class PlayerComponent : public Component
{
private:
	TransformComponent * transform;
	
public:
	GameObject * parent;
	PlayerComponent() {}
	PlayerComponent(GameObject* p) : parent(p) {
		if (p->getComponent<TransformComponent>() != nullptr) {
			transform = p->getComponent<TransformComponent>();
			buildEvents();
		}
	}
	void OnUpdate(float dt) override {}
	void OnMessage(const std::string m) override {}
	void BuildFromJson(const Json::Value& componentJSON) override {}
	void BuildToJson(Json::Value& componentJSON) override {}

	void buildEvents() {
		//If own transform not set
		if (transform == nullptr) {
			//has parent been set?
			if (parent == nullptr) {
				return;
			}
			//is there a transform comp to grab?
			if (parent->getComponent<TransformComponent>() == nullptr) {
				return;
			}
			//Get transform comp
			transform = parent->getComponent<TransformComponent>();
		}
		EventHandler::FuncMessage f = [=](std::string message) {transform->OnMessage(message);};
		EventHandler eh(f, "PlayerTransform");
		keyEvent->addHandler(eh);
	}
};
#endif