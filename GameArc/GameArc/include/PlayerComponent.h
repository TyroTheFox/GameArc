#ifndef _PLAYERCOMPONENT_H_
#define _PLAYERCOMPONENT_H_

#pragma once
#include "Component.h"
#include "global.h"
#include "TransformComponent.h"
#include "GameObject.h"

class KeyEventReciever;

extern EventHandler* eventHandler;

class PlayerComponent : public Component
{
private:
	GameObject* parent;
	TransformComponent * transform;
	KeyEventReciever keyEventReceiver;
public:
	PlayerComponent() {}
	PlayerComponent(GameObject* p) : parent(p) {
		if (p->getComponent<TransformComponent>() != nullptr) {
			transform = p->getComponent<TransformComponent>();
			eventHandler->subscribe<KeyEvent, KeyEventReciever>(&keyEventReceiver);
		}
	}
	void OnUpdate(float dt) override {
	}
	void OnMessage(const std::string m) override {}
	void BuildFromJson(const Json::Value& componentJSON) override {}
	void BuildToJson(Json::Value& componentJSON) override {}
};

class KeyEventReciever : public Receiver<KeyEvent>
{
public:

	virtual void receive(const KeyEvent& keyevent) /*override*/
	{
		std::cout << "Collision event occured with " << keyevent.message << '\n';
	}
};
#endif