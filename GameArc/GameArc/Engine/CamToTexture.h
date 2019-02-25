#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include "Model.h"
#include "Shader.h"

class CamToTexture {
public:
	CamToTexture(Shader* s, float screenWidth, float screenHeight);
	//CALL THIS FIRST
	void PrepareToRender();
	//THEN RENDER TO TEXTURE
	void RenderToTexture(Model * model, glm::mat4 modelMatrix);
	//THEN CALL THIS TO GO BACK TO NORMAL!!!
	void RevertToNormalRendering();
	GLuint getTextureID();
private:
	bool toTextureRenderingActivated = false;
	Shader * shader;
	glm::mat4 projection;
	float near_plane = 1.0f, far_plane = 1000.0f;
	const unsigned int m_width = 1024, m_height = 1024;
	float m_screenWidth, m_screenHeight;
	GLuint texID;
	GLuint fboID;
};