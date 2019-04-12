#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <chrono>
#include "IEngineCore.h"
#include "Game.h"
#include "Camera.h"
#include "Model.h"
#include "TextWriter.h"
#include "InputHandler.h"

#include "CamToTexture.h"

// GL includes
#include "Shader.h"
#include "HDRRenderTarget.h"
// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

#include "global.h"

///Forward Declared
class Game;
///Forward Declared
struct Character;
/** \class GLFWEngineCore
\file GLFW_EngineCore.h
\brief A rendering and utility core for wrapping GLFW opperation

A wrapper for GLFW opperations and set up to allow for easy use of the library in the rest of the game
*/
class GLFW_EngineCore : public IEngineCore
{
public:
	///Delta Time for updating objects
	float delta = 0;
	///Input handler class, used for processing key inputs and firing events based on that input
	static InputHandler* inputHandler;
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
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
	void drawLightCube(const glm::mat4& modelMatrix);
	///Draw a given model using a given transformation matrix
	void drawModel(Model* model, const glm::mat4& modelMatrix) override;
	void calculateLight(Light* light, int directionalLightTotal, int pointLightTotal, int spotLightTotal) override;
	void calculateShadows(Game* game) override;
	///Draw 2D text to the screen
	void drawText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, std::map<GLchar, Character> Characters, GLuint VAO, GLuint VBO) override;
	///Used for drawing 2D rectandles to the screen
	void draw2DRect(glm::vec2 position, glm::vec2 size, GLfloat rotate, glm::vec3 color, GLuint quadVAO) override;
private:
	///Game Pointer
	Game* gameptr;
	///GLFW window object
	GLFWwindow* m_window;
	///Default Shader
	Shader* phong; 
	///Texture Phong Shader
	Shader* texturePhong; 
	Shader* simpleDepth;
	Shader* pointDepth;
	Shader* debugShadow;
	Shader* debugBuffer;
	Shader* textureRender;
	Shader* lightBox;
	///Text Writer Shader
	Shader* textWriterShader;
	///2D Object Shader
	Shader* Shader2D;

	//Render Target for HDR
	HDRRenderTarget* hdrTarget;

	///Shadow Map FBO
	float near_plane = 1.0f, far_plane = 1000.0f;
	unsigned int cubeVBO, cubeVAO;
	GLuint vaoDebugTexturedRect;
	std::vector<Light*> sceneLights;
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
	void DisplayFramebufferTexture(GLuint textureID);
	///Sets default shaders up
	void setDefaultShaders();
	///Initialises internal cube model
	void initCubeModel();
	GLenum glCheckError_(const char *file, int line);

	CamToTexture* camTexture;
	void DrawToCamTexture(Game* game);
};

