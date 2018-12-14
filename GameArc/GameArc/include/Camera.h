#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

class Camera
{
private:
	float m_yaw;
	float m_pitch;
	float m_roll;
public:
	glm::vec3 m_position;
	glm::quat m_orientation;
	float m_fov;

	float upPitchBound, lowPitchBound;
	
	Camera() : m_position(0), m_orientation(1,0,0,0), m_fov(45), m_yaw(0), m_pitch(0), upPitchBound(1.8f), lowPitchBound(1.8f){ yaw(m_yaw); pitch(m_pitch);}
	Camera(const glm::vec3& pos) : m_position(pos), m_orientation(1, 0, 0, 0),m_fov(45), m_yaw(0), m_pitch(0), upPitchBound(1.8f), lowPitchBound(1.8f) { yaw(m_yaw); pitch(m_pitch); }
	Camera(const glm::vec3& pos, const glm::quat& orient) : m_position(pos), m_orientation(orient), m_fov(45), m_yaw(0), m_pitch(0), upPitchBound(1.8f), lowPitchBound(1.8f) { yaw(m_yaw); pitch(m_pitch); }

	void lookAt(const glm::vec3& target) { m_orientation = (glm::toQuat(glm::lookAt(m_position, target, glm::vec3(0, 1, 0)))); }
	glm::quat getLookAt(const glm::vec3& target) { return (glm::toQuat(glm::lookAt(m_position, target, glm::vec3(0, 1, 0)))); }
	
	const glm::vec3& position() const { return m_position; }
	const glm::quat& orientation() const { return m_orientation; }

	glm::mat4 getViewMatrix() const { return glm::translate(glm::mat4_cast(m_orientation), m_position); }
	
	void translate(const glm::vec3 &v) { m_position += v * m_orientation; }
	void translate(float x, float y, float z) { m_position += glm::vec3(x, y, z) * m_orientation; }

	void rotate(float angle, const glm::vec3 &axis) { m_orientation *= glm::angleAxis(angle, axis * m_orientation); }
	void rotate(float angle, float x, float y, float z) { m_orientation *= glm::angleAxis(angle, glm::vec3(x, y, z) * m_orientation); }

	void mouseUpdate(float x, float y) { 
		
		m_yaw += x;

		float newPitch = m_pitch + y;
		if (newPitch < lowPitchBound && newPitch > -upPitchBound) {
			m_pitch = newPitch;
		}
		else {
			if (newPitch > lowPitchBound) m_pitch = lowPitchBound;
			if (newPitch < -upPitchBound) m_pitch = -upPitchBound;
		}

		yaw(m_yaw);
		pitch(m_pitch);
	}

	void yaw(float angle) { rotate(angle, 0.0f, 1.0f, 0.0f); }
	void pitch(float angle) { rotate(angle, 1.0f, 0.0f, 0.0f); }
	void roll(float angle) { rotate(angle, 0.0f, 0.0f, 1.0f); }

	void setFOV(float fov) { m_fov = fov; }
};
