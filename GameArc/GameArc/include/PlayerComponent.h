#pragma once
#include "GameObject.h"
#include "Component.h"

#include "global.h"

#include "TransformComponent.h"
#include "CameraComponent.h"

extern Event* keyEvent;

class PlayerComponent : public Component
{
private:
	TransformComponent * transform;
	CameraComponent * camera;
	GameObject * parent;
public:
	PlayerComponent() : camera(new CameraComponent){}
	PlayerComponent(GameObject* p) : parent(p), camera(new CameraComponent(p)) {
		if (parent->getComponent<TransformComponent>() != nullptr) {
			transform = parent->getComponent<TransformComponent>();
			buildEvents();
		}
	}
	void setParent(GameObject* p) {
		parent = p;
		if (parent->getComponent<TransformComponent>() != nullptr) {
			transform = parent->getComponent<TransformComponent>();
		}
		camera->SetParent(parent);
	}
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
		EventHandler::FuncMessage f = [=](std::string message) {transform->OnMessage(message); };
		EventHandler eh(f, "PlayerTransform");
		keyEvent->addHandler(eh);
	}
	void OnUpdate(float dt) override {
		camera->SetTranslationToParent();
	}
	void OnMessage(const std::string m) override {}
	void BuildFromJson(const Json::Value& componentJSON) override {}
	void BuildToJson(Json::Value& componentJSON) override {}
	Camera* GetCamera() {
		return camera->GetCamera();
	}
};