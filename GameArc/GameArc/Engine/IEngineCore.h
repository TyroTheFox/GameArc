#pragma once

#include <string>
#include <iostream>
#include "Model.h"
#include "TextWriter.h"
#include <glm/mat4x4.hpp>

///Forward Declared
class Camera;
///Forward Declared
class Game;
///Forward Declared
class Model;
///Forward Declared
struct Character;
/** \class IEngineCore
* \file IEngineCore.h
* \brief A basis template for engine cores
*
* A template to base all engine cores on and allow for the engine core to be swapped out whenever needed
*/
class IEngineCore
{
public:
	///Deconstructor
	virtual ~IEngineCore() {}
	///Initialise the game window and variables
	virtual bool initWindow(int width, int height, std::string windowName) = 0;
	///Begin game update and render loops
	virtual bool runEngine(Game& game) = 0;
	///Draws a coloured background
	virtual void renderColouredBackground(float r, float g, float b) = 0;
	///Sets current camera object
	virtual	void setCamera(const Camera* cam) = 0;
	///Draws a cube based on internal test cube
	virtual void drawCube(const glm::mat4& modelMatrix) = 0;
	///Draws a given model using a given transformation matrix
	virtual void drawModel(Model* model, const glm::mat4& modelMatrix) = 0;
	///Draws 2D text to the screen
	virtual void drawText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, std::map<GLchar, Character> Characters, GLuint VAO, GLuint VBO) = 0;
	///Used for drawing 2D rectandles to the screen
	virtual void draw2DRect(glm::vec2 position, glm::vec2 size, GLfloat rotate, glm::vec3 color, GLuint quadVAO) = 0;
};
