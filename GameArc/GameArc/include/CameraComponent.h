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
public:
	//Direction Vector the Camera is facing in
	float m_directionX{ 0 };
	float m_directionY{ 0 };
	float m_directionZ{ 0 };

	glm::vec3 front = glm::vec3(0.0f, 0.0f, 1.0f);;
	glm::vec3 right;
	glm::vec3 up;
	glm::mat4 viewMatrix;

	glm::vec2 mouseXY, lastMouseXY;
	float sensitivity = 0.01f;

	GameObject * parent;
	glm::vec3 transformOffset;
	CameraComponent() : camera(new Camera), transformOffset(glm::vec3(0)), lastMouseXY(glm::vec2(300, 400)){}
	CameraComponent(GameObject* p) : parent(p), camera(new Camera), transformOffset(glm::vec3(0)), lastMouseXY(glm::vec2(300, 400)) {
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
		viewMatrix = camera->getViewMatrix();
		right = glm::vec3(viewMatrix[0].x, viewMatrix[1].x, viewMatrix[2].x);
		up = glm::vec3(viewMatrix[0].y, viewMatrix[1].y, viewMatrix[2].y);
		front = glm::vec3(viewMatrix[0].z, viewMatrix[1].z, viewMatrix[2].z);
	}

	void OnUpdate(float dt) override {
		SetToParentedOffset();
		//camera->rotate(mouseXY.x * 0.01f, glm::vec3(0.0f, 1.0f, 0.0f));
		//camera->rotate(mouseXY.y * 0.01f, glm::vec3(1.0f, 0.0f, 0.0f));

		float xoffset = mouseXY.x - lastMouseXY.x;
		float yoffset = mouseXY.y - lastMouseXY.y; // reversed since y-coordinates range from bottom to top
		lastMouseXY.x = mouseXY.x;
		lastMouseXY.y = mouseXY.y;

		xoffset *= sensitivity;
		yoffset *= sensitivity;

		camera->mouseUpdate(xoffset, yoffset);
		ComputeDirectionVector();
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