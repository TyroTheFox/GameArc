#pragma once
#include <string>
#include <iostream>
#include "Model.h"
#include "TextWriter.h"
#include <glm/mat4x4.hpp>

class Camera;
class Game;
class Model;
struct Character;

class IEngineCore
{
public:

	virtual ~IEngineCore() {}

	virtual bool initWindow(int width, int height, std::string windowName) = 0;
	virtual bool runEngine(Game& game) = 0;

	// some simple drawing tools - will want to move these to a proper renderer class later
	virtual void renderColouredBackground(float r, float g, float b) = 0;
	virtual	void setCamera(const Camera* cam) = 0;
	virtual void drawCube(const glm::mat4& modelMatrix) = 0;
	virtual void drawModel(Model* model, const glm::mat4& modelMatrix) = 0;
	virtual void drawText(std::string text, Shader * shader, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, std::map<GLchar, Character> Characters, GLuint VAO, GLuint VBO) = 0;
};
