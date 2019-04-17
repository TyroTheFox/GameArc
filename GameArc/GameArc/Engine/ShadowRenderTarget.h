#pragma once
#include "RenderTarget.h"
#include "Light.h"
#include "GameObject.h"
#include "ModelComponent.h"
#include <vector>

class ShadowRenderTarget : public RenderTarget {
private:
	float near_plane;
	float far_plane;
	unsigned int shadowWidth;
	unsigned int shadowHeight;
	Shader* simpleDepth;
	Shader* pointDepth;
	glm::mat4 lightProjection;
public:
	ShadowRenderTarget(unsigned int width, unsigned int height, 
		unsigned int shadowwidth, unsigned int shadowheight, 
		float nearPlane, float farPlane, 
		Shader* ShaderSimple, Shader* ShaderPoint
	) {
		ScreenWidth = width;
		ScreenHeight = height;
		shadowWidth = shadowwidth;
		shadowHeight = shadowheight;
		near_plane = nearPlane;
		far_plane = farPlane;
		this->simpleDepth = ShaderSimple;
		this->pointDepth = ShaderSimple;
	}

	void SetUp() override {}

	void SetUp(std::vector<Light*> sceneLights) {
		float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		for (Light* light : sceneLights) {
			if (light->depthMapFBO == 0) {
				switch (light->lType()) {
				case Light::DIRECTIONAL:
				case Light::SPOT:
					lightProjection = glm::ortho(-(GLfloat)(shadowWidth * 0.1), (GLfloat)(shadowWidth * 0.1), -(GLfloat)(shadowHeight * 0.1), (GLfloat)(shadowHeight * 0.1), near_plane, far_plane);
					glGenFramebuffers(1, &light->depthMapFBO);
					// create depth texture
					glGenTextures(1, &light->depthMap);
					glBindTexture(GL_TEXTURE_2D, light->depthMap);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
					// attach depth texture as FBO's depth buffer
					glBindFramebuffer(GL_FRAMEBUFFER, light->depthMapFBO);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, light->depthMap, 0);
					glDrawBuffer(GL_NONE);
					glReadBuffer(GL_NONE);
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
					glBindTexture(GL_TEXTURE_2D, 0);

					light->lightSpaceMatrix = lightProjection * light->GetMatrix();
					break;
				case Light::POINT:
					glGenFramebuffers(1, &light->depthMapFBO);
					glGenTextures(1, &light->depthMap);
					glBindTexture(GL_TEXTURE_CUBE_MAP, light->depthMap);
					for (unsigned int i = 0; i < 6; ++i) {
						glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
					}
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
					glBindFramebuffer(GL_FRAMEBUFFER, light->depthMapFBO);
					glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, light->depthMap, 0);
					glDrawBuffer(GL_NONE);
					glReadBuffer(GL_NONE);
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
					glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
					break;
				}
			}
		}
	}

	void ClearBuffer() override {
		glViewport(0, 0, ScreenWidth, ScreenHeight);
	}

	void RenderToBuffer() override {}

	void RenderToBuffer(Light* light) {
		light->lightSpaceMatrix = lightProjection * light->GetMatrix();
		glCullFace(GL_FRONT);
		glViewport(0, 0, shadowWidth, shadowHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, light->depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void UnbindBuffer() override {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, ScreenWidth, ScreenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_BACK);
	}

	void Render() override {}

	void RenderDirectional(ModelComponent* mC, glm::mat4* modelMatrix, glm::mat4* lightSpaceMatrix) {
		simpleDepth->use();
		simpleDepth->SetMatrix4("lightSpaceMatrix", *lightSpaceMatrix, true);
		Model* model = mC->model;
		simpleDepth->SetMatrix4("model", *modelMatrix, true);
		model->renderWithoutTextures(simpleDepth->ID);
	}

	void RenderPoint(ModelComponent* mC, glm::mat4* modelMatrix, glm::vec3& position) {
		pointDepth->use();
		lightProjection = glm::perspective(glm::radians(90.0f), (float)shadowWidth / (float)shadowHeight, near_plane, far_plane);
		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(lightProjection * glm::lookAt(position, position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(lightProjection * glm::lookAt(position, position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(lightProjection * glm::lookAt(position, position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		shadowTransforms.push_back(lightProjection * glm::lookAt(position, position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		shadowTransforms.push_back(lightProjection * glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(lightProjection * glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		for (unsigned int i = 0; i < 6; ++i) {
			pointDepth->SetArrayMatrix4("shadowMatrices", i, shadowTransforms[i], true);
		}
		pointDepth->setFloat("far_plane", far_plane);
		pointDepth->SetVector3f("lightPos", position, true);
		pointDepth->SetMatrix4("model", *modelMatrix, true);
		Model* model = mC->model;
		model->renderWithoutTextures(pointDepth->ID);
	}
};