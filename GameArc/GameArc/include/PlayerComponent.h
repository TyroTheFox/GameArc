#ifndef _PLAYERCOMPONENT_H_
#define _PLAYERCOMPONENT_H_

#pragma once
#include "Component.h"
#include "InputHandler.h"
#include "TransformComponent.h"
#include "GameObject.h"

InputHandler * InputHandler::s_instance = 0;

class PlayerComponent : public Component
{
private:
	GameObject * parent;
	TransformComponent * transform;
public:
	PlayerComponent() {}
	PlayerComponent(GameObject* p) : parent(p) {
		if (p->getComponent<TransformComponent>() != nullptr) {
			transform = p->getComponent<TransformComponent>();
			EventCallback<TransformComponent, std::string>* callback = new EventCallback<TransformComponent, std::string>(transform, &TransformComponent::OnMessage);

			InputHandler::instance()->registerAction("Jump", callback);
		}
	}
	void OnUpdate(float dt) override {
		InputHandler::instance()->callAction("Jump");
	}
	void OnMessage(const std::string m) override {}
	void BuildFromJson(const Json::Value& componentJSON) override {}
	void BuildToJson(Json::Value& componentJSON) override {}
};
#endif