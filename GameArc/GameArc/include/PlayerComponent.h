#ifndef _PLAYERCOMPONENT_H_
#define _PLAYERCOMPONENT_H_

#pragma once
#include "Component.h"
#include "EventHandler.h"
#include "TransformComponent.h"
#include "GameObject.h"

class KeyEventReciever : public Receiver<KeyEvent>
{
public:

	virtual void receive(const KeyEvent& keyevent) /*override*/
	{
		std::cout << "Collision event occured with " << keyevent.e1 << " and " << keyevent.e2 << '\n';
	}
};

class PlayerComponent : public Component
{
private:
	TransformComponent * transform;
public:
	GameObject* parent;

	PlayerComponent() {}
	PlayerComponent(GameObject* p) : parent(p) {
		if (p->getComponent<TransformComponent>() != nullptr) {
			transform = p->getComponent<TransformComponent>();
			KeyEventReciever ker;
			EventHandler::instance()->subscribe<KeyEvent>(&ker);
		}
	}
	void OnUpdate(float dt) override {

	}
	void OnMessage(const std::string m) override {}
	void BuildFromJson(const Json::Value& componentJSON) override {}
	void BuildToJson(Json::Value& componentJSON) override {}
};
#endif