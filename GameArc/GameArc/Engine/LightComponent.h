#pragma once
#include <iostream>
#include "Component.h"
#include "Light.h"
#include "LightHandler.h"
#include "TransformComponent.h"
#include "GameObject.h"
/**
\brief Handles the contained camera object
\file LightComponent.h
\author Kieran Clare
*/
class LightComponent : public Component
{
private:
	Light* light;
	Light::LightType lightType;
	int ID;	
public:
	LightHandler* lightHandler;
	///Front movement vector
	glm::vec3 front = glm::vec3(0.0f, 0.0f, 1.0f);
	///Right movement vector
	glm::vec3 right;
	///Up movement vector
	glm::vec3 up;
	///Movement matrix
	glm::mat4 viewMatrix;
	///Attached game object
	GameObject * parent;
	///Constructor
	LightComponent() : light(new Light()) {}
	///Called when attached to object, sets up debug functions
	void OnSetUp() override {
	}
	///Returns camera object
	Light* GetLight() {
		return light;
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
			light->m_position = pTransform->position();
			light->m_orientation = pTransform->orientation();
			light->CalculateDirection();
		}
	}
	///Gets direction vectors from translation matrix
	void ComputeDirectionVector()
	{
		viewMatrix = light->GetMatrix();
		right = glm::vec3(viewMatrix[0].x, viewMatrix[1].x, viewMatrix[2].x);
		up = glm::vec3(viewMatrix[0].y, viewMatrix[1].y, viewMatrix[2].y);
		front = glm::vec3(viewMatrix[0].z, viewMatrix[1].z, viewMatrix[2].z);
		light->m_up = up;
		light->m_direction = front;
	}
	///Called on update tick, updates all camera values and handles mouse movement
	void OnUpdate(float dt) override {
		SetTranslationToParent();
		ComputeDirectionVector();
	}
	///Called every render call, not used
	void OnRender(IEngineCore* m_engineInterfacePtr) override {
		m_engineInterfacePtr->drawCube(light->GetMatrix());
	}
	///Called on event fire, not used
	void OnMessage(const std::string m) override {}
	///Builds component from JSON values
	void BuildFromJson(const Json::Value& componentJSON) override {
		try {
			Light::LightType lightType = Light::DIRECTIONAL;
			LightColour lightColour;
			SpotLightData spotLightData;
			PointLightData pointLightData;
			glm::vec3 p;
			glm::vec3 o;
			if (componentJSON.isMember("SpotLightData")) {
				const Json::Value& sLD = componentJSON["SpotLightData"];
				spotLightData = SpotLightData(
					sLD[0].asFloat(), 
					sLD[1].asFloat(), 
					sLD[2].asFloat(), 
					glm::cos(glm::radians(sLD[3].asFloat())), 
					glm::cos(glm::radians(sLD[4].asFloat()))
				);
				lightType = Light::SPOT;
			}
			if (componentJSON.isMember("PointLightData")) {
				const Json::Value& pLD = componentJSON["PointLightData"];
				pointLightData = PointLightData(
					pLD[0].asFloat(),
					pLD[1].asFloat(),
					pLD[2].asFloat()
				);
				lightType = Light::POINT;
			}
			if (componentJSON.isMember("Ambient")) {
				const Json::Value& a = componentJSON["Ambient"];
				lightColour.ambient = glm::vec3(
					a[0].asFloat(),
					a[1].asFloat(),
					a[2].asFloat()
				);
			}
			else {
				lightColour.ambient = glm::vec3(1.0f);
			}
			if (componentJSON.isMember("Diffuse")) {
				const Json::Value& d = componentJSON["Diffuse"];
				lightColour.diffuse = glm::vec3(
					d[0].asFloat(),
					d[1].asFloat(),
					d[2].asFloat()
				);
			}
			else {
				lightColour.diffuse = glm::vec3(1.0f);
			}
			if (componentJSON.isMember("Specular")) {
				const Json::Value& s = componentJSON["Specular"];
				lightColour.specular = glm::vec3(
					s[0].asFloat(),
					s[1].asFloat(),
					s[2].asFloat()
				);
			}
			else {
				lightColour.specular = glm::vec3(1.0f);
			}
			if (componentJSON.isMember("position")) {
				const Json::Value& position = componentJSON["position"];
				p.x = position[0].asFloat();
				p.y = position[1].asFloat();
				p.z = position[2].asFloat();
			}
			else {
				p.x = 0;
				p.y = 0;
				p.z = 0;
			}
			if (componentJSON.isMember("rotation")) {
				const Json::Value& rotation = componentJSON["rotation"];
				o = glm::vec3(rotation[0].asFloat(), rotation[1].asFloat(), rotation[2].asFloat());
			}
			if (componentJSON.isMember("orientation")) {
				const Json::Value& orientation = componentJSON["orientation"];
				o.x = orientation[0].asFloat();
				o.y = orientation[1].asFloat();
				o.z = orientation[2].asFloat();
			}
			else {
				o.x = 0;
				o.y = 0;
				o.z = 0;
			}

			switch (lightType) {
			case Light::DIRECTIONAL:
				lightHandler->createNewLight(
					lightColour,
					o
				);
				break;
			case Light::POINT:
				lightHandler->createNewLight(
					pointLightData,
					lightColour,
					p
				);
				break;
			case Light::SPOT:
				lightHandler->createNewLight(
					spotLightData,
					lightColour,
					p,
					o
				);
				break;
			}
		}
		catch (...) {
			std::cout << "Exception thrown in parsing BuildFromJson in LightComponent." << std::endl;
			throw;
		}
	}
};
