#pragma once
#pragma once
#include "GameObject.h"
#include "Component.h"

#include "global.h"

#include "TransformComponent.h"
#include "CameraComponent.h"

extern Event* keyEvent;

class EventCameraComponent : public Component
{
private:
	TransformComponent * transform;
	CameraComponent * camera;
	GameObject * parent;
	string cameraName;
	string switchToScene;

	//Movement Vectors
	glm::vec3 front, right, up;
	glm::vec3 intendedDirection;
public:
	float movementSpeed;

	EventCameraComponent() : camera(new CameraComponent), cameraName("default"){}
	EventCameraComponent(GameObject* p) : parent(p), camera(new CameraComponent(p)), cameraName("default") {
		if (parent->getComponent<TransformComponent>() != nullptr) {
			transform = parent->getComponent<TransformComponent>();
			buildEvents();
		}
	}
	void setParent(GameObject* p) {
		parent = p;
		if (parent->getComponent<TransformComponent>() != nullptr) {
			transform = parent->getComponent<TransformComponent>();
			camera->SetParent(parent);
		}
	}

	void buildEvents() {
		EventHandler::FuncMessage movementMessage = [this](std::string message) { this->OnChangeScene(this->switchToScene); };
		EventHandler movementListener(movementMessage, "ChangeScene");
		keyEvent->addHandler(movementListener);
	}

	void OnUpdate(float dt) override {
		camera->OnUpdate(dt);
	}

	void OnChangeScene(const std::string m) {
		keyEvent->notifyHandlerWithMessage("NextScene", m);
	}

	void OnMessage(const std::string m) override {}

	void BuildFromJson(const Json::Value& componentJSON) override {
		try {
			if (componentJSON.isMember("position")) {
				const Json::Value& position = componentJSON["position"];
				camera->GetCamera()->m_position.x = position[0].asFloat();
				camera->GetCamera()->m_position.y = position[1].asFloat();
				camera->GetCamera()->m_position.z = position[2].asFloat();
			}
			else {
				camera->GetCamera()->m_position.x = 0;
				camera->GetCamera()->m_position.y = 0;
				camera->GetCamera()->m_position.z = 0;
			}
			if (componentJSON.isMember("orientation")) {
				const Json::Value& orientation = componentJSON["orientation"];
				camera->GetCamera()->m_orientation.w = orientation[0].asFloat();
				camera->GetCamera()->m_orientation.x = orientation[1].asFloat();
				camera->GetCamera()->m_orientation.y = orientation[2].asFloat();
				camera->GetCamera()->m_orientation.z = orientation[3].asFloat();
			}
			else {
				camera->GetCamera()->m_orientation.w = 1;
				camera->GetCamera()->m_orientation.x = 0;
				camera->GetCamera()->m_orientation.y = 0;
				camera->GetCamera()->m_orientation.z = 0;
			}
			if (componentJSON.isMember("startingYaw")) {
				const Json::Value& angle = componentJSON["startingYaw"];
				float a = angle.asFloat();
				camera->GetCamera()->yaw(a);
			}
			if (componentJSON.isMember("startingRoll")) {
				const Json::Value& angle = componentJSON["startingRoll"];
				float a = angle.asFloat();
				camera->GetCamera()->roll(a);
			}
			if (componentJSON.isMember("startingPitch")) {
				const Json::Value& angle = componentJSON["startingPitch"];
				float a = angle.asFloat();
				camera->GetCamera()->pitch(a);
			}
			if (componentJSON.isMember("FOV")) {
				const Json::Value& FOV = componentJSON["FOV"];
				camera->GetCamera()->m_fov = FOV.asFloat();
			}
			else {
				camera->GetCamera()->m_fov = 45.0f;
			}
			if (componentJSON.isMember("SwitchTo")) {
				const Json::Value& SwitchTo = componentJSON["SwitchTo"];
				switchToScene = SwitchTo.asString();
			}
			else {
				switchToScene = "";
			}
		}
		catch (...) {
			std::cout << "Exception thrown in parsing BuildFromJson in EventCameraComponent." << std::endl;
			throw;
		}

	}

	void BuildToJson(Json::Value& componentJSON) override {}
	Camera* GetCamera() {
		return camera->GetCamera();
	}
};