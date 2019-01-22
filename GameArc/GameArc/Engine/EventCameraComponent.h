#pragma once
/**
* \class Event Camera Component
* \file EventCameraComponent.h
* \author Kieran Clare
* \brief Static camera made for title scenes
*
* Made for title scenes, this camera component can recieve event calls to change to a new scene
*/
#include "GameObject.h"
#include "Component.h"

#include "global.h"

#include "TransformComponent.h"
#include "CameraComponent.h"
///External event object
extern Event* keyEvent;

class EventCameraComponent : public Component
{
private:
	///Transform component
	TransformComponent * transform;
	///Camera component
	CameraComponent * camera;
	///Camera name
	string cameraName;
	///Name of scene to switch to
	string switchToScene;
	///Event handler for sending change scene message
	EventHandler movementListener;
public:
	///Constructor
	EventCameraComponent() : camera(new CameraComponent), cameraName("default"){}
	///Constructor
	EventCameraComponent(GameObject* p) : camera(new CameraComponent(p)), cameraName("default") {	}
	///Called when added to object, sets up component events
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
	///Sets camera parent
	void setParent(GameObject* p) {
		parent = p;
		camera->SetParent(parent);
	}
	///Builds camera events for changing scene
	void buildEvents() {
		EventHandler::Func movementMessage = [this]() { this->OnChangeScene(this->switchToScene); };
		movementListener = EventHandler(movementMessage, "ChangeScene");
		keyEvent->addHandler(movementListener);
	}
	///Called on update call, doesn't do anything
	void OnUpdate(float dt) override {
		//camera->OnUpdate(dt);
	}
	///Called on render call, doesn't do anything
	void OnRender(IEngineCore* m_engineInterfacePtr) override {}
	///Notifies event object with the next scene the Game class should switch to
	void OnChangeScene(const std::string m) {
		keyEvent->notifyHandlerWithMessage("NextScene", m);
		keyEvent->removeHandler("ChangeScene");
	}
	///Called on event call, does nothing
	void OnMessage(const std::string m) override {}
	///Builds component from JSON values
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
	///Return Camera object
	Camera* GetCamera() {
		return camera->GetCamera();
	}
};