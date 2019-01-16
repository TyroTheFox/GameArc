#pragma once
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

class Game;
struct Character;
extern InputHandler* inputHandler;

class GLFW_EngineCore : public IEngineCore
{
public:
	~GLFW_EngineCore() override;

	bool initWindow(int width, int height, std::string windowName) override;
	bool runEngine(Game& game) override;
	void renderColouredBackground(float r, float g, float b) override;

	void setCamera(const Camera* cam) override;
	void drawCube(const glm::mat4& modelMatrix) override;
	void drawModel(Model* model, const glm::mat4& modelMatrix) override;
	void drawText(std::string text, Shader * shader, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, std::map<GLchar, Character> Characters, GLuint VAO, GLuint VBO) override;

private:
	GLFWwindow* m_window;
	GLuint m_defaultShaderProgram;
	
	static int m_screenWidth;
	static int m_screenHeight;
	static std::vector<int> m_keyBuffer;
	static const int m_keyBufferSize = 400;

	std::chrono::steady_clock clock;
	float delta = 0;

	static void mouseMoveCallbackEvent(GLFWwindow* window, double xPos, double yPos);
	static void keyCallbackEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void windowResizeCallbackEvent(GLFWwindow* window, int width, int height);
	
	
	void setDefaultShaders();
	void initCubeModel();


};

