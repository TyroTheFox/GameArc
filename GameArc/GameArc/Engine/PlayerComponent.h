#pragma once
#include "GameObject.h"
#include "Component.h"

#include "global.h"

#include "TransformComponent.h"
#include "CameraComponent.h"
#include "ModelComponent.h"

extern Event* keyEvent;

class PlayerComponent : public Component
{
private:
	TransformComponent * transform;
	CameraComponent * camera;
	ModelComponent * model;

	//Movement Vectors
	glm::vec3 front, right, up;
	glm::vec3 intendedDirection;
public:
	float movementSpeed;

	PlayerComponent() : camera(new CameraComponent), movementSpeed(0.5f){	}
	void OnSetUp() override {
		if (parent->getComponent<TransformComponent>() != nullptr) {
			transform = parent->getComponent<TransformComponent>();
		}
		camera->debug = debug;
		camera->SetParent(parent);
		camera->OnSetUp();
		if (parent->getComponent<ModelComponent>() != nullptr) {
			model = parent->getComponent<ModelComponent>();
		}
		buildEvents();

		debug->AddConsoleCommand("setplayerspeed", TextParser::InterpFunc([this](std::vector<std::string> s) {
			if (s.size() <= 0) { this->debug->WriteToConsole("Missing value"); return; }
			this->movementSpeed = std::stof(s.at(0)); 
			this->debug->WriteToConsole("Player Movement Speed set to " + s.at(0)); 
		}));
	}

	void setParent(GameObject* p) {
		parent = p;
		if (parent->getComponent<TransformComponent>() != nullptr) {
			transform = parent->getComponent<TransformComponent>();
		}
		camera->SetParent(parent);
		if (parent->getComponent<ModelComponent>() != nullptr) {
			model = parent->getComponent<ModelComponent>();
		}
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
		if (!camera->firstPersonCamera) {
			model->active = true;
		}
		else
		{
			model->active = false;
		}
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
	void OnRender(IEngineCore* m_engineInterfacePtr) override {}
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

		if (m == "switchCamera") {
			camera->firstPersonCamera = !camera->firstPersonCamera;
		}
	}

	void BuildFromJson(const Json::Value& componentJSON) override {
		try {
		}
		catch (...) {
			std::cout << "Exception thrown in parsing BuildFromJson in PlayerComponent." << std::endl;
			throw;
		}

	}

	void BuildToJson(Json::Value& componentJSON) override {}
	Camera* GetCamera() {
		return camera->GetCamera();
	}
};