#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>
/** \class Camera
\file Camera.h
\author Kieran Clare
\brief An object that creates and maintains the view matrix for use in later calculations

Creates and maintains the view matrix for use in the Engine Core to procude the illusion of a camera viewport within the game world
*/
class Camera
{
private:
	///Current Camera Yaw
	float m_yaw;
	///Current Camera Roll
	float m_roll;
	///Current Camera Pitch
	float m_pitch;
public:
	///Current Position
	glm::vec3 m_position;
	///Current Orientation
	glm::quat m_orientation;
	///Current Field of View
	float m_fov;
	///Upper and lower bounds for pitch rotation when looking with the mouse
	float upPitchBound, lowPitchBound;
	///Constructor
	Camera() : m_position(0), m_orientation(1,0,0,0), m_fov(45), m_yaw(0), m_pitch(0), upPitchBound(1.8f), lowPitchBound(1.8f){ yaw(m_yaw); pitch(m_pitch);}
	///Constructor
	Camera(const glm::vec3& pos) : m_position(pos), m_orientation(1, 0, 0, 0),m_fov(45), m_yaw(0), m_pitch(0), upPitchBound(1.8f), lowPitchBound(1.8f) { yaw(m_yaw); pitch(m_pitch); }
	///Constructor
	Camera(const glm::vec3& pos, const glm::quat& orient) : m_position(pos), m_orientation(orient), m_fov(45), m_yaw(0), m_pitch(0), upPitchBound(1.8f), lowPitchBound(1.8f) { yaw(m_yaw); pitch(m_pitch); }
	///Sets orientation according to a look at function for a given target
	void lookAt(const glm::vec3& target) { m_orientation = (glm::toQuat(glm::lookAt(m_position, target, glm::vec3(0, 1, 0)))); }
	///Returns the look at rotation for a given target
	glm::quat getLookAt(const glm::vec3& target) { return (glm::toQuat(glm::lookAt(m_position, target, glm::vec3(0, 1, 0)))); }
	///Returns current position
	const glm::vec3& position() const { return m_position; }
	///Returns current orientation
	const glm::quat& orientation() const { return m_orientation; }
	///Returns view matrix
	glm::mat4 getViewMatrix() const { return glm::translate(glm::mat4_cast(m_orientation), m_position); }
	///Translate by vector
	void translate(const glm::vec3 &v) { m_position += v * m_orientation; }
	///Translate by variables
	void translate(float x, float y, float z) { m_position += glm::vec3(x, y, z) * m_orientation; }
	///Rotate by vector and angle
	void rotate(float angle, const glm::vec3 &axis) { m_orientation *= glm::angleAxis(angle, axis * m_orientation); }
	///Rotate by variables
	void rotate(float angle, float x, float y, float z) { m_orientation *= glm::angleAxis(angle, glm::vec3(x, y, z) * m_orientation); }
	///Update camera orientation with mouse values
	void mouseUpdate(float x, float y) { 
		
		m_yaw += x;

		float newPitch = m_pitch + y;
		if (newPitch < upPitchBound && newPitch > -lowPitchBound) {
			m_pitch = newPitch;
		}
		else {
			if (newPitch > upPitchBound) m_pitch = upPitchBound;
			if (newPitch < -lowPitchBound) m_pitch = -lowPitchBound;
		}

		yaw(m_yaw);
		pitch(m_pitch);
	}
	///Updates camera yaw (y axis rotation)
	void yaw(float angle) { rotate(angle, 0.0f, 1.0f, 0.0f); }
	///Updates camera pitch (x axis rotation)
	void pitch(float angle) { rotate(angle, 1.0f, 0.0f, 0.0f); }
	///Updates camera roll (z axis rotation)
	void roll(float angle) { rotate(angle, 0.0f, 0.0f, 1.0f); }
	///Sets camera field of view
	void setFOV(float fov) { m_fov = fov; }
};
