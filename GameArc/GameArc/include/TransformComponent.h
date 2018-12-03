#ifndef _TRANSCOMPONENT_H_
#define _TRANSCOMPONENT_H_
#pragma once
#include "Component.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

class TransformComponent : public Component
{
public:
	glm::vec3 m_position;
	glm::quat m_orientation;
	glm::vec3 m_scale;

	float increaseStep = 0.1f;

	TransformComponent() : m_position(0), m_orientation(1, 0, 0, 0), m_scale(1.0f) {}
	TransformComponent(const glm::vec3& pos) : m_position(pos), m_orientation(1, 0, 0, 0), m_scale(1.0f) {}
	TransformComponent(const glm::vec3& pos, const glm::quat& orient) : m_position(pos), m_orientation(orient), m_scale(1.0f) {}
	TransformComponent(const glm::vec3& pos, const glm::quat& orient, const glm::vec3& scale) : m_position(pos), m_orientation(orient), m_scale(scale) {}

	void OnUpdate(float dt) override{}
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

	void BuildToJson(Json::Value& componentJSON) override
	{
		componentJSON["class"] = "TransformComponent";

		if (m_position != glm::vec3(0)) {
			Json::Value outPosition(Json::arrayValue);
			outPosition.append(Json::Value(m_position.x));
			outPosition.append(Json::Value(m_position.y));
			outPosition.append(Json::Value(m_position.z));
			componentJSON["position"] = outPosition;
		}

		if (m_orientation != glm::quat(1, 0, 0, 0)) {
			Json::Value outOrientation(Json::arrayValue);
			outOrientation.append(Json::Value(m_orientation.w));
			outOrientation.append(Json::Value(m_orientation.x));
			outOrientation.append(Json::Value(m_orientation.y));
			outOrientation.append(Json::Value(m_orientation.z));
			componentJSON["orientation"] = outOrientation;
		}

		if (m_scale != glm::vec3(1.0f)) {
			Json::Value outScale(Json::arrayValue);
			outScale.append(Json::Value(m_scale.x));
			outScale.append(Json::Value(m_scale.y));
			outScale.append(Json::Value(m_scale.z));
			componentJSON["scale"] = outScale;
		}
	}

	const glm::vec3& position() const { return m_position; }
	const glm::quat& orientation() const { return m_orientation; }
	const glm::vec3& scale() const { return m_scale; }

	glm::mat4 getModelMatrix()
	{
		glm::mat4 transMatrix = glm::translate(glm::mat4(1.0f), m_position);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);
		glm::mat4 rotMatrix = glm::mat4_cast(m_orientation);
		return transMatrix * rotMatrix * scaleMatrix;
	}

	void translate(const glm::vec3 &v) { m_position += v; }
	void translate(float x, float y, float z) { m_position += glm::vec3(x, y, z); }

	void rotate(float angle, const glm::vec3 &axis) { m_orientation *= glm::angleAxis(angle, axis * m_orientation); }
	void rotate(float angle, float x, float y, float z) { m_orientation *= glm::angleAxis(angle, glm::vec3(x, y, z) * m_orientation); }

	void scaleUp(const glm::vec3 &v) { m_scale += v; }
	void scaleUp(float x, float y, float z) { m_scale += glm::vec3(x, y, z); }

	void yaw(float angle) { rotate(angle, 0.0f, 1.0f, 0.0f); }
	void pitch(float angle) { rotate(angle, 1.0f, 0.0f, 0.0f); }
	void roll(float angle) { rotate(angle, 0.0f, 0.0f, 1.0f); }
};
#endif