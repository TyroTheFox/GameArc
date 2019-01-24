#pragma once
#include "GameObject.h"

#include "TransformComponent.h"
#include "CameraComponent.h"
#include "ModelComponent.h"

#include "global.h"
///External event handler class for key input
extern Event* keyEvent;
/**
\file PlayerComponent.h
\author Kieran Clare
\brief The component handling the player's in-game character

The game's playable character. It handles most player controls, camera and the player's model though does need
components to work with.
*/
class PlayerComponent : public Component
{
private:
	///Transform component
	TransformComponent * transform;
	///Camera component
	CameraComponent * camera;
	///Model component
	ModelComponent * model;

	///Movement Vectors
	glm::vec3 front, right, up;
	///Direction to move into next
	glm::vec3 intendedDirection;
public:
	///Base movement speed
	float movementSpeed;
	///Constructor
	PlayerComponent() : camera(new CameraComponent), movementSpeed(0.5f){	} 
	///Called when added to object, initialises all needed components and adds to parent
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

		debug->AddConsoleCommand("setPlayerSpeed", TextParser::InterpFunc([this](std::vector<std::string> s) {
			if (s.size() <= 0) { this->debug->WriteErrorToConsole("Missing value"); return; }
			this->movementSpeed = std::stof(s.at(0)); 
			this->debug->WriteToConsole("Player Movement Speed set to " + s.at(0)); 
		}));
	}
	///Sets component parent to a new object
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
	///Builds the player's control events needed for movement
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
		keyEvent->subscribeToEvent("PlayerMovement", [this](std::string message) { this->OnMessage(message); });
		keyEvent->subscribeToEvent("PlayerMouseXY", [&](glm::vec2 vec) { camera->mouseXY = vec; });
	}
	///Called every update tick, updates camera and player position
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
	///Called every render call
	void OnRender(IEngineCore* m_engineInterfacePtr) override {}
	///Called every event fire, interprets given action messages
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
	///Builds object from JSON, though isn't necessary right now
	void BuildFromJson(const Json::Value& componentJSON) override {
		try {
		}
		catch (...) {
			std::cout << "Exception thrown in parsing BuildFromJson in PlayerComponent." << std::endl;
			throw;
		}

	}
	///Returns Camera object
	Camera* GetCamera() {
		return camera->GetCamera();
	}
};