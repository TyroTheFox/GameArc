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
	string cameraName;
	string switchToScene;
	EventHandler movementListener;

	//Movement Vectors
	glm::vec3 front, right, up;
	glm::vec3 intendedDirection;
public:
	float movementSpeed;

	EventCameraComponent() : camera(new CameraComponent), cameraName("default"){}
	EventCameraComponent(GameObject* p) : camera(new CameraComponent(p)), cameraName("default") {	}
	void OnSetUp() override {
		buildEvents();
		debug->AddConsoleCommand("setecrot", TextParser::InterpFunc([this](std::vector<std::string> s) {
			if (this->parent->name == s.at(0)) {
				if (s.size() > 1) {
					camera->GetCamera()->yaw(std::stof(s.at(1)));
				}
				if (s.size() > 2) {
					camera->GetCamera()->roll(std::stof(s.at(2)));
				}
				if (s.size() > 3) {
					camera->GetCamera()->pitch(std::stof(s.at(3)));
				}
				this->debug->WriteToConsole("Event Camera " + s.at(0) + " rotated");
			}
		}));
		debug->AddConsoleCommand("setectran", TextParser::InterpFunc([this](std::vector<std::string> s) {
			if (this->parent->name == s.at(0)) {
				if (s.size() > 1) {
					camera->GetCamera()->m_position.x = std::stof(s.at(1));
				}
				if (s.size() > 2) {
					camera->GetCamera()->m_position.y = std::stof(s.at(2));
				}
				if (s.size() > 3) {
					camera->GetCamera()->m_position.z = std::stof(s.at(3));
				}
				this->debug->WriteToConsole("Event Camera " + s.at(0) + " moved");
			}
		}));
	}
	void setParent(GameObject* p) {
		parent = p;
		camera->SetParent(parent);
	}

	void buildEvents() {
		EventHandler::Func movementMessage = [this]() { this->OnChangeScene(this->switchToScene); };
		movementListener = EventHandler(movementMessage, "ChangeScene");
		keyEvent->addHandler(movementListener);
	}

	void OnUpdate(float dt) override {
		//camera->OnUpdate(dt);
	}
	void OnRender(IEngineCore* m_engineInterfacePtr) override {}
	void OnChangeScene(const std::string m) {
		keyEvent->notifyHandlerWithMessage("NextScene", m);
		keyEvent->removeHandler("ChangeScene");
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