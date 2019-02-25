#include "../Engine/CamToTexture.h"
#include <glm/detail/type_vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

CamToTexture::CamToTexture(Shader* s, float screenWidth, float screenHeight)
{
	shader = s;
	projection = glm::ortho(-(GLfloat)(m_width * 0.1), (GLfloat)(m_width * 0.1), -(GLfloat)(m_height * 0.1), (GLfloat)(m_height * 0.1), near_plane, far_plane);
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	glGenFramebuffers(1, &fboID);
	// create depth texture
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindFramebuffer(GL_FRAMEBUFFER, fboID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_RGBA, GL_TEXTURE_2D, texID, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CamToTexture::PrepareToRender()
{
	shader->use();
	glViewport(0, 0, m_width, m_height);
	glBindFramebuffer(GL_FRAMEBUFFER, fboID);
	glClear(GL_COLOR_BUFFER_BIT);
	toTextureRenderingActivated = true;
}

void CamToTexture::RenderToTexture(Model * model, glm::mat4 modelMatrix)
{
	shader->SetMatrix4("projection", projection, true);
	shader->SetMatrix4("model", modelMatrix, true);
	model->render(shader->ID);
}

void CamToTexture::RevertToNormalRendering()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_screenWidth, m_screenHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	toTextureRenderingActivated = false;
}

GLuint CamToTexture::getTextureID()
{
	return texID;
}
