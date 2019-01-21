#pragma once
/**
* \class GLFW Engine Core
* \file GLFW_EngineCore.h
* \brief A rendering and utility core for wrapping GLFW opperation
*
* A wrapper for GLFW opperations and set up to allow for easy use of the library in the rest of the game
*/
#include "IEngineCore.h"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <chrono>
#include "Camera.h"
#include "Model.h"
#include "TextWriter.h"
// GL includes
#include "Shader.h"
// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

#include "global.h"

///Forward Declared
class Game;
struct Character;
extern InputHandler* inputHandler;

class GLFW_EngineCore : public IEngineCore
{
public:
	float delta = 0;///Delta Time for updating objects
	~GLFW_EngineCore() override;///Deconstructor

	bool initWindow(int width, int height, std::string windowName) override;///Sets up game window and other variables
	bool runEngine(Game& game) override;///Starts game update and render loops
	void renderColouredBackground(float r, float g, float b) override;///Changes the background colour

	void setCamera(const Camera* cam) override;///Sets camera
	void drawCube(const glm::mat4& modelMatrix) override;///Draws a cube from the test model
	void drawModel(Model* model, const glm::mat4& modelMatrix) override;///Draw a given model using a given transformation matrix
	void drawText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, std::map<GLchar, Character> Characters, GLuint VAO, GLuint VBO) override;///Draw 2D text to the screen

private:
	GLFWwindow* m_window;///GLFW window object
	Shader* phong; ///Default Shader
	Shader* texturePhong; ///Texture Phong Shader
	Shader* textWriterShader; ///Text Writer Shader
	
	static int m_screenWidth; ///Width of the Window
	static int m_screenHeight;///Height of the Window
	static std::vector<int> m_keyBuffer;///Key Buffer for player input
	static const int m_keyBufferSize = 400;///Number of keys avaiable in the buffer

	std::chrono::steady_clock clock;///Clock object used for calcuating delta valyes

	static void keyCallbackEvent(GLFWwindow* window, int key, int scancode, int action, int mods);///Key call back used for player controls
	static void characterCallbackEvent(GLFWwindow * window, unsigned int codepoint);///Keyboard call back used for Debug Console input
	static void windowResizeCallbackEvent(GLFWwindow* window, int width, int height); ///Called when window is resized
	
	void setDefaultShaders();///Sets default shaders up
	void initCubeModel();///Initialises internal cube model


};

