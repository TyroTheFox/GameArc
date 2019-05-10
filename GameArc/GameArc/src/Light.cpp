#include "../Engine/Light.h"

Light::Light()
{
	active = true;
	lightType = LightType::DIRECTIONAL;
	lightColour = LightColour();
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	CalculateDirection();
}

Light::Light(LightColour lC)
{
	active = true;
	lightType = LightType::DIRECTIONAL;
	lightColour = lC;
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	CalculateDirection();
}

Light::Light(LightColour lC, glm::vec3 o)
{
	active = true;
	lightType = LightType::DIRECTIONAL;
	lightColour = lC;
	m_orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	yaw(o.y);
	pitch(o.x);
	roll(o.z);
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	CalculateDirection();
}

Light::Light(LightColour lC, glm::vec3 p, glm::vec3 o)
{
	active = true;
	lightType = LightType::DIRECTIONAL;
	lightColour = lC;
	m_orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	yaw(o.y);
	pitch(o.x);
	roll(o.z);
	m_position = p;
	CalculateDirection();
}

Light::Light(PointLightData pLD, LightColour lC)
{
	active = true;
	lightType = LightType::POINT;
	lightColour = lC;
	pointLightData = pLD;
	m_position = glm::vec3(1.0f);
	m_orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	CalculateDirection();
}

Light::Light(PointLightData pLD, LightColour lC, glm::vec3 p)
{
	active = true;
	lightType = LightType::POINT;
	lightColour = lC;
	pointLightData = pLD;
	m_position = p;
	m_orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	CalculateDirection();
}

Light::Light(SpotLightData sLD, LightColour lC)
{
	active = true;
	lightType = LightType::SPOT;
	lightColour = lC;
	spotLightData = sLD;
	m_position = glm::vec3(1.0f);
	m_orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	CalculateDirection();
}

Light::Light(SpotLightData sLD, LightColour lC, glm::vec3 p)
{
	active = true;
	lightType = LightType::SPOT;
	lightColour = lC;
	spotLightData = sLD;
	m_position = p;
	m_orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	CalculateDirection();
}

Light::Light(SpotLightData sLD, LightColour lC, glm::vec3 p, glm::vec3 o)
{
	active = true;
	lightType = LightType::SPOT;
	lightColour = lC;
	spotLightData = sLD;
	m_orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	yaw(o.y);
	pitch(o.x);
	roll(o.z);
	m_position = p;
	CalculateDirection();
}

void Light::lookAt(const glm::vec3 & target)
{
	m_orientation = (glm::toQuat(glm::lookAt(m_position, target, glm::vec3(0, 1, 0))));
	CalculateDirection();
}

glm::quat Light::getLookAt(const glm::vec3 & target)
{
	return (glm::toQuat(glm::lookAt(m_position, target, glm::vec3(0, 1, 0))));
}

const glm::vec3 & Light::position()
{
	return m_position;
}

const glm::quat & Light::orientation()
{
	return m_orientation;
}

const glm::vec3 & Light::direction()
{
	return m_direction;
}

const glm::vec3 & Light::up()
{
	return m_up;
}

const LightColour& Light::lColour()
{
	return lightColour;
}

const Light::LightType& Light::lType()
{
	return lightType;
}

int Light::GetID()
{
	return ID;
}

void Light::CalculateDirection()
{
	matrix = glm::translate(glm::mat4_cast(inverse(m_orientation * glm::quat(0.0f, 0.0f, 1.0f, 0.0f))), -m_position);
	m_right = glm::vec3(matrix[0].x, matrix[1].x, matrix[2].x);
	m_direction = glm::vec3(matrix[0].z, matrix[1].z, matrix[2].z);
	m_up = glm::vec3(matrix[0].y, matrix[1].y, matrix[2].y);
}

glm::mat4 Light::GetMatrix()
{
	return glm::translate(glm::mat4_cast(inverse(m_orientation * glm::quat(0.0f, 0.0f, 1.0f, 0.0f))), -m_position);
}

void Light::translate(const glm::vec3 & v)
{
	m_position += v * m_orientation;
	CalculateDirection();
}

void Light::translate(float x, float y, float z)
{
	m_position += glm::vec3(x, y, z) * m_orientation;
	CalculateDirection();
}

void Light::rotate(float angle, const glm::vec3 & axis)
{
	m_orientation *= glm::angleAxis(angle, axis * m_orientation);
	CalculateDirection();
}

void Light::rotate(float angle, float x, float y, float z)
{
	m_orientation *= glm::angleAxis(angle, glm::vec3(x, y, z) * m_orientation);
	CalculateDirection();
}

void Light::pitch(float angle)
{
	rotate(angle, 1.0f, 0.0f, 0.0f);
}

void Light::yaw(float angle)
{
	rotate(angle, 0.0f, 1.0f, 0.0f);
}

void Light::roll(float angle)
{
	rotate(angle, 0.0f, 0.0f, 1.0f);
}