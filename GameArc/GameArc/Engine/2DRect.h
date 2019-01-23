#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "IEngineCore.h"
#include "Shader.h"

class IEngineCore;
/** \class Rect
\file 2DRect.h.h
\brief A onject that creates a 2D coloured square

Based on the Learn OpenGL code, it draws 2D coloured squares to the screen
*/
class Rect
{
private:
	///Engine Core Pointer, used for drawing
	IEngineCore* m_engineInterfacePtr;
	///VAO Object for a 2D object
	GLuint quadVAO;
public:
	///Constructor, sets up values
	Rect(IEngineCore* enginePtr);
	///Deconstructor
	~Rect();
	///Draws a 2D rectangle to the screen
	void DrawRect(glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
};