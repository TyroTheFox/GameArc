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
#include "InputHandler.h"
// GL includes
#include "Shader.h"
// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

#include "global.h"

///Forward Declared
class Game;
struct Character;

class GLFW_EngineCore : public IEngineCore
{
public:
	///Delta Time for updating objects
	float delta = 0;
	///Input handler class, used for processing key inputs and firing events based on that input
	static InputHandler* inputHandler;
	///Deconstructor
	~GLFW_EngineCore() override;
	///Sets up game window and other variables
	bool initWindow(int width, int height, std::string windowName) override;
	///Starts game update and render loops
	bool runEngine(Game& game) override;
	///Changes the background colour
	void renderColouredBackground(float r, float g, float b) override;
	///Sets camera
	void setCamera(const Camera* cam) override;
	///Draws a cube from the test model
	void drawCube(const glm::mat4& modelMatrix) override;
	///Draw a given model using a given transformation matrix
	void drawModel(Model* model, const glm::mat4& modelMatrix) override;
	///Draw 2D text to the screen
	void drawText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, std::map<GLchar, Character> Characters, GLuint VAO, GLuint VBO) override;

private:
	///GLFW window object
	GLFWwindow* m_window;
	///Default Shader
	Shader* phong; 
	///Texture Phong Shader
	Shader* texturePhong; 
	///Text Writer Shader
	Shader* textWriterShader; 
	///Width of the Window
	static int m_screenWidth; 
	///Height of the Window
	static int m_screenHeight;
	///Key Buffer for player input
	static std::vector<int> m_keyBuffer;
	///Number of keys avaiable in the buffer
	static const int m_keyBufferSize = 400;
	///Clock object used for calcuating delta valyes
	std::chrono::steady_clock clock;
	///Key call back used for player controls
	static void keyCallbackEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
	///Keyboard call back used for Debug Console input
	static void characterCallbackEvent(GLFWwindow * window, unsigned int codepoint);
	///Called when window is resized
	static void windowResizeCallbackEvent(GLFWwindow* window, int width, int height); 
	///Sets default shaders up
	void setDefaultShaders();
	///Initialises internal cube model
	void initCubeModel();
};

