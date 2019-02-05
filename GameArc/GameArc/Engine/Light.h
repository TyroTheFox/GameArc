#pragma once

#include "IEngineCore.h"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

struct LightColour {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	LightColour() : ambient(glm::vec3(1.0f, 1.0f, 1.0f)), diffuse(glm::vec3(1.0f, 1.0f, 1.0f)), specular(glm::vec3(1.0f, 1.0f, 1.0f)) {};
	LightColour(glm::vec3 a, glm::vec3 d, glm::vec3 s) : ambient(a), diffuse(d), specular(s) {};
};

struct PointLightData {
	float constant;
	float linear;
	float quadratic;
	PointLightData() : constant(0), linear(0), quadratic(0) {};
	PointLightData(float c, float l, float q) : constant(c), linear(l), quadratic(q) {};
};

struct SpotLightData {
	float constant;
	float linear;
	float quadratic;
	float cutOff;
	float outerCutOff;
	SpotLightData() : constant(0), linear(0), quadratic(0), cutOff(0), outerCutOff(1) {};
	SpotLightData(float c, float l, float q, float cO, float oCO) : constant(c), linear(l), quadratic(q), cutOff(cO), outerCutOff(oCO) {};
};

class Light {
public:
	enum LightType { DIRECTIONAL, POINT, SPOT };
private:
	glm::vec3 m_position;
	glm::quat m_orientation;
	glm::vec3 m_direction;
	LightType lightType;
	LightColour lightColour;
public:
	int ID;
	PointLightData pointLightData;
	SpotLightData spotLightData;
	Light();
	Light(LightType lT, LightColour lC);
	Light(LightType lT, LightColour lC, glm::vec3 p);
	Light(LightType lT, LightColour lC, glm::vec3 p, glm::vec3 o);

	Light(PointLightData pLD, LightColour lC);
	Light(PointLightData pLD, LightColour lC, glm::vec3 p);
	Light(PointLightData pLD, LightColour lC, glm::vec3 p, glm::vec3 o);

	Light(SpotLightData sLD, LightColour lC);
	Light(SpotLightData sLD, LightColour lC, glm::vec3 p);
	Light(SpotLightData sLD, LightColour lC, glm::vec3 p, glm::vec3 o);

	///Sets orientation according to a look at function for a given target
	void lookAt(const glm::vec3& target);
	///Returns the look at rotation for a given target
	glm::quat getLookAt(const glm::vec3& target);
	///Returns current position
	const glm::vec3& position();
	///Returns current orientation
	const glm::quat& orientation();
	const glm::vec3& direction();

	///Returns current position
	const LightColour& lColour();
	///Returns current orientation
	const Light::LightType& lType();

	int GetID();

	void CalculateDirection();

	///Translate by vector
	void translate(const glm::vec3 &v);
	///Translate by variables
	void translate(float x, float y, float z);
	///Rotate by vector and angle
	void rotate(float angle, const glm::vec3 &axis);
	///Rotate by variables
	void rotate(float angle, float x, float y, float z);
	///Updates camera yaw (y axis rotation)
	void yaw(float angle);
	///Updates camera pitch (x axis rotation)
	void pitch(float angle);
	///Updates camera roll (z axis rotation)
	void roll(float angle);
};