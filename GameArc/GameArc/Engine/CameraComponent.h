#pragma once
#include <iostream>
#include "Component.h"
#include "Camera.h"
#include "GameObject.h"
/**
\brief Handles the contained camera object
\file CameraComponent.h
\author Kieran Clare

Handles the contained camera object so that a camera can be placed on a game object
*/
class CameraComponent : public Component
{
private:
	Camera * camera;///Camera object
public:
	///Front movement vector
	glm::vec3 front = glm::vec3(0.0f, 0.0f, 1.0f);
	///Right movement vector
	glm::vec3 right;
	///Up movement vector
	glm::vec3 up;
	///Movement matrix
	glm::mat4 viewMatrix;
	///Mouse movement offset
	float xoffset;
	///Mouse movement offset
	float yoffset;
	///First person camera switch (switch to 3rd person if false)
	bool firstPersonCamera;
	///Distance from central point when in 3rd person mode
	float offsetFactor;
	///Current tick mouse positions
	glm::vec2 mouseXY;
	///Previous tick mouse positions
	glm::vec2 lastMouseXY;
	///Mouse movement sensitivity
	float sensitivity = 0.01f;
	///Attached game object
	GameObject * parent;
	///Constructor
	CameraComponent() : camera(new Camera), offsetFactor(30.0f), lastMouseXY(glm::vec2(300, 400)), firstPersonCamera(true) {}
	///Constructor
	CameraComponent(GameObject* p) : parent(p), offsetFactor(30.0f), camera(new Camera), lastMouseXY(glm::vec2(300, 400)), firstPersonCamera(true) {
		SetTranslationToParent();
	}
	///Called when attached to object, sets up debug functions
	void OnSetUp() override{
		debug->AddConsoleCommand("setFPC", TextParser::InterpFunc([this](std::vector<std::string> s) { if (s.size() <= 0) { this->debug->WriteErrorToConsole("Missing value"); return; }
			this->firstPersonCamera = std::stoi(s.at(0)); this->debug->WriteToConsole("Set First Person Camera to " + s.at(0)); }));
		debug->AddConsoleCommand("setCamOffset", TextParser::InterpFunc([this](std::vector<std::string> s) { if (s.size() <= 0) { this->debug->WriteErrorToConsole("Missing value"); return; }
			this->offsetFactor = std::stof(s.at(0)); this->debug->WriteToConsole("Set Distance From Player in Third Person Mode to " + s.at(0)); }));
		debug->AddConsoleCommand("setMouseSen", TextParser::InterpFunc([this](std::vector<std::string> s) { if (s.size() <= 0) { this->debug->WriteErrorToConsole("Missing value"); return; }
			this->sensitivity = std::stof(s.at(0)); this->debug->WriteToConsole("Set Mouse Sensitivity to " + s.at(0)); }));
		debug->AddConsoleCommand("setFOV", TextParser::InterpFunc([this](std::vector<std::string> s) { if (s.size() <= 0) { this->debug->WriteErrorToConsole("Missing value"); return; }
			this->GetCamera()->setFOV(std::stof(s.at(0))); this->debug->WriteToConsole("Set Camera FOV to " + s.at(0)); }));
	}
	///Returns camera object
	Camera* GetCamera() {
		return camera;
	}
	///Sets parent game object
	void SetParent(GameObject* p) {
		parent = p;
		SetTranslationToParent();
	}
	///Sets camera object position to parent's position
	void SetTranslationToParent() {
		if (parent->getComponent<TransformComponent>() != nullptr) {
			TransformComponent* pTransform = parent->getComponent<TransformComponent>();
			camera->m_position = -pTransform->position();
			camera->m_orientation = pTransform->orientation();
		}
	}
	///Moves camera object to offseted position durring 3rd person mode
	void SetToParentedOffset() {
		camera->m_position -= front * offsetFactor;
	}
	///Gets direction vectors from translation matrix
	void ComputeDirectionVector()
	{
		viewMatrix = camera->getViewMatrix();
		right = glm::vec3(viewMatrix[0].x, viewMatrix[1].x, viewMatrix[2].x);
		up = glm::vec3(viewMatrix[0].y, viewMatrix[1].y, viewMatrix[2].y);
		front = glm::vec3(viewMatrix[0].z, viewMatrix[1].z, viewMatrix[2].z);
	}
	///Called on update tick, updates all camera values and handles mouse movement
	void OnUpdate(float dt) override {
		SetTranslationToParent();

		xoffset = mouseXY.x - lastMouseXY.x;
		yoffset = mouseXY.y - lastMouseXY.y; // reversed since y-coordinates range from bottom to top
		lastMouseXY.x = mouseXY.x;
		lastMouseXY.y = mouseXY.y;

		xoffset *= sensitivity;
		yoffset *= sensitivity;

		camera->mouseUpdate(xoffset, yoffset);
		ComputeDirectionVector();
		if (!firstPersonCamera) {
			camera->upPitchBound = 1.6f;
			camera->lowPitchBound = 0.1f;
			SetToParentedOffset();
		}
		else {
			camera->upPitchBound = 1.8f;
			camera->lowPitchBound = 1.8f;
		}
	}
	///Called every render call, not used
	void OnRender(IEngineCore* m_engineInterfacePtr) override {}
	///Called on event fire, not used
	void OnMessage(const std::string m) override {}
	///Builds component from JSON values
	void BuildFromJson(const Json::Value& componentJSON) override {
		try {
			if (componentJSON.isMember("position")) {
				const Json::Value& position = componentJSON["position"];
				camera->m_position.x = position[0].asFloat();
				camera->m_position.y = position[1].asFloat();
				camera->m_position.z = position[2].asFloat();
			}
			else {
				camera->m_position.x = 0;
				camera->m_position.y = 0;
				camera->m_position.z = 0;
			}
			if (componentJSON.isMember("orientation")) {
				const Json::Value& orientation = componentJSON["orientation"];
				camera->m_orientation.w = orientation[0].asFloat();
				camera->m_orientation.x = orientation[1].asFloat();
				camera->m_orientation.y = orientation[2].asFloat();
				camera->m_orientation.z = orientation[3].asFloat();
			}
			else {
				camera->m_orientation.w = 1;
				camera->m_orientation.x = 0;
				camera->m_orientation.y = 0;
				camera->m_orientation.z = 0;
			}
			if (componentJSON.isMember("FOV")) {
				const Json::Value& FOV = componentJSON["FOV"];
				camera->m_fov = FOV[0].asFloat();
			}
			else {
				camera->m_fov = 45.0f;
			}
		}
		catch (...) {
			std::cout << "Exception thrown in parsing BuildFromJson in CameraComponent." << std::endl;
			throw;
		}
	}
};