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

	glm::vec3 POVOffset;

	//Movement Vectors
	glm::vec3 front, right, up;
	glm::vec3 intendedDirection;
public:
	float movementSpeed;

	PlayerComponent() : camera(new CameraComponent), movementSpeed(0.5f){}
	PlayerComponent(GameObject* p) : parent(p), camera(new CameraComponent(p)), movementSpeed(0.5f) {
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
		camera->transformOffset = POVOffset;
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
		EventHandler::FuncMessage movementMessage = [this](std::string message) { this->OnMessage(message); };
		EventHandler movementListener(movementMessage, "PlayerMovement");
		keyEvent->addHandler(movementListener);
		EventHandler::FuncVec2 mouseUpdate = [&](glm::vec2 vec) { camera->mouseXY = vec; };
		EventHandler mouseListener(mouseUpdate, "PlayerMouseXY");
		keyEvent->addHandler(mouseListener);
	}

	void OnUpdate(float dt) override {
		camera->OnUpdate(dt);
		if (glm::length(intendedDirection) != 0.0f) {
			if (transform != nullptr) {
				glm::vec3 forwards = camera->front * (intendedDirection.z * movementSpeed);
				glm::vec3 sideways = camera->right * (intendedDirection.x * movementSpeed);
				transform->m_position.x += forwards.x + sideways.x;
				transform->m_position.z += forwards.z + sideways.z;
			}
			intendedDirection *= 0;
		}
	}

	void OnMessage(const std::string m) override {
		if (m == "moveRight") {
			intendedDirection.x += 1;
		}
		else if (m == "moveLeft") {
			intendedDirection.x -= 1;
		}
		else if (m == "moveForwards") {
			intendedDirection.z -= 1;
		}
		else if (m == "moveBackwards") {
			intendedDirection.z += 1;
		}
	}

	void BuildFromJson(const Json::Value& componentJSON) override {
		try {
			if (componentJSON.isMember("POVOffset")) {
				const Json::Value& offset = componentJSON["POVOffset"];
				POVOffset.x = offset[0].asFloat();
				POVOffset.y = offset[1].asFloat();
				POVOffset.z = offset[2].asFloat();
			}
			else {
				POVOffset.x = 0;
				POVOffset.y = 0;
				POVOffset.z = 0;
			}
		}
		catch (...) {
			std::cout << "Exception thrown in parsing BuildFromJson in TransformComponent." << std::endl;
			throw;
		}

	}

	void BuildToJson(Json::Value& componentJSON) override {}
	Camera* GetCamera() {
		return camera->GetCamera();
	}
};