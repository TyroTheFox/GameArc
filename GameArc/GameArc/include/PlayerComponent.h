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
	GameObject* parent;
	TransformComponent * transform;
public:
	PlayerComponent() {}
	PlayerComponent(GameObject* p) : parent(p) {
		if (p->getComponent<TransformComponent>() != nullptr) {
			transform = p->getComponent<TransformComponent>();
			Event<GameObject*> e;
			e += [parent](){ parent->getComponent<TransformComponent>()->rotate(5.0f, glm::vec3(0.0f, 1.0f, 0.0f)); };
			InputHandler::instance()->registerAction("Rotate", e);
		}
	}
	void OnUpdate(float dt) override {
		InputHandler::instance()->callAction("Rotate");
	}
	void OnMessage(const std::string m) override {}
	void BuildFromJson(const Json::Value& componentJSON) override {}
	void BuildToJson(Json::Value& componentJSON) override {}
};
#endif