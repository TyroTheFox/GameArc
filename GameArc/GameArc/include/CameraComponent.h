#pragma once
#include <iostream>
#include "Component.h"
#include "Camera.h"
#include "GameObject.h"

class CameraComponent : public Component
{
private:
	Camera * camera;
	TransformComponent * transform;

	//Direction Vector the Camera is facing in
	float m_directionX{ 0 };
	float m_directionY{ 0 };
	float m_directionZ{ 0 };

	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;
public:
	glm::vec2 mouseXY;
	GameObject * parent;
	glm::vec3 transformOffset;
	CameraComponent() : camera(new Camera), transformOffset(glm::vec3(0)) {}
	CameraComponent(GameObject* p) : parent(p), camera(new Camera), transformOffset(glm::vec3(0)) {
		SetTranslationToParent();
	}
	Camera* GetCamera() {
		return camera;
	}
	void SetParent(GameObject* p) {
		parent = p;
		SetTranslationToParent();
	}
	void SetTranslationToParent() {
		if (parent->getComponent<TransformComponent>() != nullptr) {
			TransformComponent* pTransform = parent->getComponent<TransformComponent>();
			camera->m_position = -pTransform->position();
			camera->m_orientation = pTransform->orientation();
		}
	}
	void SetToParentedOffset() {
		if (parent->getComponent<TransformComponent>() != nullptr) {
			TransformComponent* pTransform = parent->getComponent<TransformComponent>();
			camera->m_position = -pTransform->position() + transformOffset;
			camera->m_orientation = pTransform->orientation();
		}
	}

	void ComputeDirectionVector()
	{
		//Here, we figure out which way is forward, relative to the Camera
		//First, we get the Pitch of the Camera (Tilt Up and Down) 
		glm::vec3 m_angleXYZ = glm::eulerAngles(camera->orientation()) * 3.14159f / 180.f;
		float pitchRadians = m_angleXYZ.y;
		//Then we get the Y direction from the Sine of the Pitch
		m_directionY = sin(pitchRadians);
		//Then we get the Cosine of the Pitch
		float m_cosPitch = cos(pitchRadians);
		//Then the Yaw (Turn Left and Right)
		float yawRadians = glm::radians(m_angleXYZ.z);
		//Then we use both to find the X and Z directions
		m_directionX = sin(yawRadians) * m_cosPitch;
		m_directionZ = cos(yawRadians) * m_cosPitch;
		front = glm::vec3(m_directionX, m_directionY, m_directionZ);
		front = glm::normalize(front);
		glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::cross(right, front);
	}

	void OnUpdate(float dt) override {
		//SetTranslationToParent();
		camera->rotate(mouseXY.x, glm::vec3(0.0f, 0.0f, 1.0f));
		camera->rotate(mouseXY.y, glm::vec3(0.0f, 1.0f, 0.0f));
		ComputeDirectionVector();
		camera->lookAt(front);
	}
	void OnMessage(const std::string m) override {}
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
			std::cout << "Exception thrown in parsing BuildFromJson in TransformComponent." << std::endl;
			throw;
		}
	}
	void BuildToJson(Json::Value& componentJSON) override {
		componentJSON["class"] = "CameraComponent";

		if (camera->m_position != glm::vec3(0)) {
			Json::Value outPosition(Json::arrayValue);
			outPosition.append(Json::Value(camera->m_position.x));
			outPosition.append(Json::Value(camera->m_position.y));
			outPosition.append(Json::Value(camera->m_position.z));
			componentJSON["position"] = outPosition;
		}

		if (camera->m_orientation != glm::quat(1, 0, 0, 0)) {
			Json::Value outOrientation(Json::arrayValue);
			outOrientation.append(Json::Value(camera->m_orientation.w));
			outOrientation.append(Json::Value(camera->m_orientation.x));
			outOrientation.append(Json::Value(camera->m_orientation.y));
			outOrientation.append(Json::Value(camera->m_orientation.z));
			componentJSON["orientation"] = outOrientation;
		}

		if (camera->m_fov != 45.0f) {
			Json::Value outFOV(Json::arrayValue);
			outFOV.append(Json::Value(camera->m_fov));
			componentJSON["FOV"] = outFOV;
		}
	}
};