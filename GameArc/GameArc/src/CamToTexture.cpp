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
	glBindFramebuffer(GL_FRAMEBUFFER, fboID);
	// create depth texture
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texID, 0);
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) { return; }
		
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CamToTexture::PrepareToRender()
{
	shader->use();
	shader->SetVector3f("spriteColor", glm::vec3(1, 1, 1), true);
	glViewport(0, 0, m_width, m_height);
	glBindFramebuffer(GL_FRAMEBUFFER, fboID);
	toTextureRenderingActivated = true;
}

void CamToTexture::RenderToTexture(Model * model, glm::mat4 modelMatrix)
{
	if (!toTextureRenderingActivated) { return; }
	shader->SetMatrix4("projection", projection, true);
	shader->SetMatrix4("model", modelMatrix, true);
	model->render(shader->ID);
}

void CamToTexture::RevertToNormalRendering()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_screenWidth, m_screenHeight);
	toTextureRenderingActivated = false;
}

GLuint CamToTexture::getTextureID()
{
	return texID;
}
