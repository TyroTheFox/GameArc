#pragma once

#include "Component.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
/**
\file TransformComponent.h
\author Kieran Clare
\brief Handles Game Object transformations

Handles transformation of the game object in 3D space
*/
class TransformComponent : public Component
{
public:
	///Current position in 3D space
	glm::vec3 m_position;
	///Current orientation in 3D space
	glm::quat m_orientation;
	///Current size
	glm::vec3 m_scale;
	///Default rate of movement for all transformations
	float increaseStep = 0.1f;
	///Constructor
	TransformComponent() : m_position(0), m_orientation(1, 0, 0, 0), m_scale(1.0f) {}
	///Constructor
	TransformComponent(const glm::vec3& pos) : m_position(pos), m_orientation(1, 0, 0, 0), m_scale(1.0f) {}
	///Constructor
	TransformComponent(const glm::vec3& pos, const glm::quat& orient) : m_position(pos), m_orientation(orient), m_scale(1.0f) {}
	///Constructor
	TransformComponent(const glm::vec3& pos, const glm::quat& orient, const glm::vec3& scale) : m_position(pos), m_orientation(orient), m_scale(scale) {}
	///Called when component is attached to object
	void OnSetUp() override {	}
	///Called on update tick
	void OnUpdate(float dt) override{}
	///Called on render instance
	void OnRender(IEngineCore* m_engineInterfacePtr) override {}
	///Called on event fire, handles transformation calls
	void OnMessage(const std::string m) override
	{
		if (m == "increaseRotateX") {
			rotate(increaseStep, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else if (m == "decreaseRotateX") {
			rotate(-increaseStep, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		else if (m == "increaseRotateY") {
			rotate(increaseStep, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else if (m == "decreaseRotateY") {
			rotate(-increaseStep, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else if (m == "increaseRotateZ") {
			rotate(increaseStep, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		else if (m == "decreaseRotateZ") {
			rotate(-increaseStep, glm::vec3(0.0f, 0.0f, 1.0f));
		}

		if (m == "increaseTranslateX") {
			translate(increaseStep, 0, 0);
		}
		else if (m == "decreaseTranslateX") {
			translate(-increaseStep, 0, 0);
		}
		else if (m == "increaseTranslateY") {
			translate(0, increaseStep, 0);
		}
		else if (m == "decreaseTranslateY") {
			translate(0, -increaseStep, 0);
		}
		else if (m == "increaseTranslateZ") {
			translate(0, 0, increaseStep);
		}
		else if (m == "decreaseTranslateZ") {
			translate(0, 0, -increaseStep);
		}

		if (m == "increaseScaleX") {
			scaleUp(increaseStep, 0, 0);
		}
		else if (m == "decreaseScaleX") {
			scaleUp(-increaseStep, 0, 0);
		}
		else if (m == "increaseScaleY") {
			scaleUp(0, increaseStep, 0);
		}
		else if (m == "decreaseScaleY") {
			scaleUp(0, -increaseStep, 0);
		}
		else if (m == "increaseScaleZ") {
			scaleUp(0, 0, increaseStep);
		}
		else if (m == "decreaseScaleZ") {
			scaleUp(0, 0, -increaseStep);
		}
	}
	///Build from JSON variables
	void BuildFromJson(const Json::Value& componentJSON) override
	{
		try {
			if (componentJSON.isMember("position")) {
				const Json::Value& position = componentJSON["position"];
				m_position.x = position[0].asFloat();
				m_position.y = position[1].asFloat();
				m_position.z = position[2].asFloat();
			}
			if (componentJSON.isMember("orientation")) {
				const Json::Value& orientation = componentJSON["orientation"];
				m_orientation.w = orientation[0].asFloat();
				m_orientation.x = orientation[1].asFloat();
				m_orientation.y = orientation[2].asFloat();
				m_orientation.z = orientation[3].asFloat();
			}
			if (componentJSON.isMember("scale")) {
				const Json::Value& scale = componentJSON["scale"];
				m_scale.x = scale[0].asFloat();
				m_scale.y = scale[1].asFloat();
				m_scale.z = scale[2].asFloat();
			}
		}
		catch (...) {
			std::cout << "Exception thrown in parsing BuildFromJson in TransformComponent." << std::endl;
			throw;
		}
	}
	///Get position
	const glm::vec3& position() const { return m_position; }
	///Get orientation
	const glm::quat& orientation() const { return m_orientation; }
	///Get scale
	const glm::vec3& scale() const { return m_scale; }
	///Get Model Matrix for engine use
	glm::mat4 getModelMatrix()
	{
		glm::mat4 transMatrix = glm::translate(glm::mat4(1.0f), m_position);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);
		glm::mat4 rotMatrix = glm::mat4_cast(m_orientation);
		return transMatrix * rotMatrix * scaleMatrix;
	}
	///Transform by vector
	void translate(const glm::vec3 &v) { m_position += v; }
	///Transform by variables
	void translate(float x, float y, float z) { m_position += glm::vec3(x, y, z); }
	///Rotate by vector
	void rotate(float angle, const glm::vec3 &axis) { m_orientation *= glm::angleAxis(angle, axis * m_orientation); }
	///Rotate by variables
	void rotate(float angle, float x, float y, float z) { m_orientation *= glm::angleAxis(angle, glm::vec3(x, y, z) * m_orientation); }
	///Scale by vector
	void scaleUp(const glm::vec3 &v) { m_scale += v; }
	///Scale by variables
	void scaleUp(float x, float y, float z) { m_scale += glm::vec3(x, y, z); }
	///Change yaw by world y
	void yaw(float angle) { rotate(angle, 0.0f, 1.0f, 0.0f); }
	///Change yaw by world x
	void pitch(float angle) { rotate(angle, 1.0f, 0.0f, 0.0f); }
	///Change roll by world z
	void roll(float angle) { rotate(angle, 0.0f, 0.0f, 1.0f); }
};