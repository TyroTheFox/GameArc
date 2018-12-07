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
	GameObject * parent;
	CameraComponent() : camera(new Camera) {}
	CameraComponent(GameObject* p) : parent(p), camera(new Camera) {
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
			camera->m_position = pTransform->position();
			camera->m_orientation = pTransform->orientation();
		}
	}
	void OnUpdate(float dt) override {
		SetTranslationToParent();
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