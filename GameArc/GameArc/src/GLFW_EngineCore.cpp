#include "GLFW_EngineCore.h"
#include "Game.h"
#include <fstream>
#include <sstream>
#include <glm/detail/type_vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
std::vector<int> GLFW_EngineCore::m_keyBuffer;
int GLFW_EngineCore::m_screenWidth;
int GLFW_EngineCore::m_screenHeight;
InputHandler* GLFW_EngineCore::inputHandler;

GLFW_EngineCore::~GLFW_EngineCore()
{
	// cleanup
	glfwTerminate();
	delete inputHandler;
	delete phong;
	delete texturePhong;
	delete textWriterShader;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (Light* light : sceneLights) {
		switch (light->lType()) {
			glDeleteFramebuffers(1, &light->depthMapFBO);
			glDeleteTextures(1, &light->depthMap);
		}
	}
}

bool GLFW_EngineCore::initWindow(int width, int height, std::string windowName)
{
	glfwInit();

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	m_screenWidth = width;
	m_screenHeight = height;

	inputHandler = new InputHandler("assets/inputs.json");

	m_window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	if (m_window == nullptr)
	{
		std::cout << "Failed to create GLFW m_window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialise GLAD" << std::endl;
		return false;
	}

	// callback functions
	glfwSetFramebufferSizeCallback(m_window, windowResizeCallbackEvent);
	glfwSetKeyCallback(m_window, keyCallbackEvent);
	glfwSetCharCallback(m_window, characterCallbackEvent);

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// make space for the keybuffer
	m_keyBuffer.resize(m_keyBufferSize);
	std::fill(m_keyBuffer.begin(), m_keyBuffer.end(), 0);

	// set thes to the given default ones
	setDefaultShaders();

	// set the drawable model as a cube
	// note: this will be changed later when we can draw many kinds of objects
	initCubeModel();
	// enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	//Create HDR Render Target
	hdrTarget = new HDRRenderTarget(
		m_screenWidth, m_screenHeight, 
		new Shader("assets/shaders/hdr.vert", "assets/shaders/hdr.frag"), 
		new Shader("assets/shaders/blur.vert", "assets/shaders/blur.frag"), 
		0.5f
	);

	shadowTarget = new ShadowRenderTarget(
		m_screenWidth, m_screenHeight,
		SHADOW_WIDTH, SHADOW_HEIGHT,
		0.00001f, 2000,
		new Shader("assets/shaders/simpleShadow.vert", "assets/shaders/simpleShadow.frag"),
		new Shader("assets/shaders/pointShadow.vert", "assets/shaders/pointShadow.frag", "assets/shaders/pointShadow.geom")
	);

	GLint flags = 0; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // makes sure errors are displayed synchronously
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
	return true;
}

bool GLFW_EngineCore::runEngine(Game& game)
{
	gameptr = &game;
	// for this example just give the game direct access to the engine
	// there are other ways this could be handled
	game.m_engineInterfacePtr = this;
	// message loop
	game.init(inputHandler);
	
	//camTexture = new CamToTexture(textureRender, m_screenWidth, m_screenHeight);
	//calculateShadows(gameptr);

	if (game.m_MainCamera == nullptr) {
		setCamera(&game.m_camera);
	}
	else {
		setCamera(game.m_MainCamera);
	}
	
	while (!glfwWindowShouldClose(m_window))
	{
		auto start = clock.now();

		inputHandler->handleInputs(m_keyBuffer);
		double xpos, ypos;
		glfwGetCursorPos(m_window, &xpos, &ypos);
		inputHandler->handleMouse(glm::vec2(xpos, ypos));
		game.update(delta); // update game logic
		//calculateShadows(gameptr);
		sceneLights = game.lightHandler->getLights();
		shadowTarget->SetUp(sceneLights);
		//DrawToCamTexture(&game);
		if (game.m_MainCamera == nullptr) {
			setCamera(&game.m_camera);
		}
		else {
			setCamera(game.m_MainCamera);
		}
		hdrTarget->ClearBuffer();
		game.render(); // prepare game to send info to the renderer in engine core
		hdrTarget->RenderToBuffer();
		for (Light* light : sceneLights) {
			if (light->lType() != Light::LightType::DIRECTIONAL) {
				
				lightBox->SetVector3f("lightColor", light->lColour().diffuse, true);
				//drawLightCube(glm::inverse(light->GetMatrix()));
				drawLightCube(light->GetMatrix());
				glm::mat4 marker = glm::toMat4(light->orientation());
				glm::vec3 orient = glm::vec3(0.0f);
				light->CalculateDirection();
				orient = light->direction() * 5.0f;
				marker *= glm::translate(light->GetMatrix(), light->position() + orient);
				marker *= glm::scale(marker, glm::vec3(0.5f));
				drawCube(marker);
			}
		}
		hdrTarget->UnbindBuffer();
		//glEnable(GL_FRAMEBUFFER_SRGB);
		hdrTarget->Render();
		// swap buffers
		glfwSwapBuffers(m_window);
		glfwPollEvents();
		
		auto finish = clock.now();
		std::chrono::duration<float> elapsed = finish - start;
		delta = elapsed.count();
	}
	game.CleanUp();

	return true;
}

void GLFW_EngineCore::renderColouredBackground(float r, float g, float b)
{
	phong->use();
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//-----------------------------Private functions------------------------------
void GLFW_EngineCore::keyCallbackEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_UNKNOWN || key > m_keyBufferSize)
	{
		return;
	}
	
	//m_keyBuffer[key] = ((action == GLFW_PRESS || action == GLFW_REPEAT));

	m_keyBuffer[key] = action;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void GLFW_EngineCore::characterCallbackEvent(GLFWwindow* window, unsigned int codepoint)
{
	char c = codepoint;
	inputHandler->handleConsoleInput(c);
}

void GLFW_EngineCore::windowResizeCallbackEvent(GLFWwindow* window, int width, int height)
{
	// change the opengl viewport to match the new m_window size
	m_screenWidth = width;
	m_screenHeight = height;
	glViewport(0, 0, width, height);
	glfwSwapBuffers(window);
}

void GLFW_EngineCore::windowMaximiseCallbackEvent(GLFWwindow * window, int maximized)
{
	glfwGetWindowSize(window, &m_screenWidth, &m_screenHeight);
	glViewport(0, 0, m_screenWidth, m_screenHeight);
	glfwSwapBuffers(window);
}

void GLFW_EngineCore::drawModel(Model* model, const glm::mat4& modelMatrix)
{
	Shader* temp;
	hdrTarget->RenderToBuffer();
	if (model->GetTextureSize() > 0) {
		temp = texturePhong;
		temp->use();
		temp->SetVector3f("material.ambient", model->modelColour.ambient, true);
		temp->SetVector3f("material.diffuse", model->modelColour.diffuse, true);
		temp->SetVector3f("material.specular", model->modelColour.specular, true);
		temp->setFloat("material.shininess", model->modelColour.shininess);
		temp->setFloat("heightScale", model->modelColour.mapHeight);

		int i = 0;
		int offset = 0;
		for (Light* light : sceneLights) {
			temp->setFloat("far_plane", far_plane);
			switch (light->lType()) {
			case Light::LightType::DIRECTIONAL:
				offset = model->GetTextureSize() + i;
				temp->setInt("shadowMapDIR", offset);
				glActiveTexture(GL_TEXTURE0 + offset);
				glBindTexture(GL_TEXTURE_2D, light->depthMap);
				temp->SetMatrix4("DirectionalLightMatrix", light->lightSpaceMatrix, true);
				break;
			case Light::LightType::POINT:
				offset = model->GetTextureSize() + i;
				temp->SetArrayInt("shadowMapPOINT", light->ID, offset);
				glActiveTexture(GL_TEXTURE0 + offset);
				glBindTexture(GL_TEXTURE_CUBE_MAP, light->depthMap);
				break;
			case Light::LightType::SPOT:
				offset = model->GetTextureSize() + i;
				temp->SetArrayInt("shadowMapSPOT", light->ID, offset);
				glActiveTexture(GL_TEXTURE0 + offset);
				glBindTexture(GL_TEXTURE_2D, light->depthMap);
				temp->SetArrayMatrix4("SpotLightMatrix", light->ID, light->lightSpaceMatrix, true);
				break;
			}
			i++;
		}
		
		glCullFace(GL_BACK);
		glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else {
		temp = phong;
	}
	temp->use();

	
	// set the model component of our shader to the object model
	
	glUniformMatrix4fv(glGetUniformLocation(temp->ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	model->render(temp->ID);

	if (model->GetTextureSize() > 0) {
		int i = 0;
		int offset = 0;
		for (Light* light : sceneLights) {
			switch (light->lType()) {
			case Light::LightType::DIRECTIONAL:
			case Light::LightType::SPOT:
				offset = model->GetTextureSize() + i;
				glActiveTexture(GL_TEXTURE0 + offset);
				glBindTexture(GL_TEXTURE_2D, 0);
				break;
			case Light::LightType::POINT:
				offset = model->GetTextureSize() + i;
				glActiveTexture(GL_TEXTURE0 + offset);
				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
				break;
			}
			i++;
		}
		
		glDisable(GL_BLEND);
	}
	
	//debugShadow->use();
	//debugShadow->setFloat("near_plane", 1.0f);
	//debugShadow->setFloat("far_plane", 100.0f);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, depthMap);
	//model->render(debugShadow->ID);
	//DisplayFramebufferTexture(depthMap);
}

void GLFW_EngineCore::calculateLight(Light * light, int directionalLightTotal, int pointLightTotal, int spotLightTotal)
{
	texturePhong->use();
	texturePhong->setInt("noOfDirectionalLights", directionalLightTotal);

	texturePhong->use();
	texturePhong->setInt("noOfPointLights", pointLightTotal);

	texturePhong->setInt("noOfSpotLights", spotLightTotal);

	texturePhong->setBool("blinn", true);

	std::string stringID;
	std::string stringItem;

	texturePhong->SetVector3f("lightPos", light->position(), true);

	switch(light->lType()){
		case Light::LightType::DIRECTIONAL:
			texturePhong->SetVector3f("dirLight.ambient", light->lColour().ambient, true);
			texturePhong->SetVector3f("dirLight.diffuse", light->lColour().diffuse, true);
			texturePhong->SetVector3f("dirLight.specular", light->lColour().specular, true);
			texturePhong->SetVector3f("dirLight.direction", -light->direction(), true);

			break;
		case Light::LightType::POINT:
			texturePhong->SetArrayVector3f("pointLights", "position", light->GetID(), light->position(), true);
			texturePhong->SetArrayVector3f("pointLights", "ambient", light->GetID(), light->lColour().ambient, true);
			texturePhong->SetArrayVector3f("pointLights", "diffuse", light->GetID(), light->lColour().diffuse, true);
			texturePhong->SetArrayVector3f("pointLights", "specular", light->GetID(), light->lColour().specular, true);
			texturePhong->SetArrayFloat("pointLights", "constant", light->GetID(), light->pointLightData.constant);
			texturePhong->SetArrayFloat("pointLights", "linear", light->GetID(), light->pointLightData.linear);
			texturePhong->SetArrayFloat("pointLights", "quadratic", light->GetID(), light->pointLightData.quadratic);

			break;
		case Light::LightType::SPOT:
			texturePhong->SetArrayVector3f("spotLights", "position", light->GetID(), light->position(), true);
			texturePhong->SetArrayVector3f("spotLights", "direction", light->GetID(), -light->direction(), true);
			texturePhong->SetArrayVector3f("spotLights", "ambient", light->GetID(), light->lColour().ambient, true);
			texturePhong->SetArrayVector3f("spotLights", "diffuse", light->GetID(), light->lColour().diffuse, true);
			texturePhong->SetArrayVector3f("spotLights", "specular", light->GetID(), light->lColour().specular, true);
			texturePhong->SetArrayFloat("spotLights", "constant", light->GetID(), light->spotLightData.constant);
			texturePhong->SetArrayFloat("spotLights", "linear", light->GetID(), light->spotLightData.linear);
			texturePhong->SetArrayFloat("spotLights", "quadratic", light->GetID(), light->spotLightData.quadratic);
			texturePhong->SetArrayFloat("spotLights", "cutOff", light->GetID(), light->spotLightData.cutOff);
			texturePhong->SetArrayFloat("spotLights", "outerCutOff", light->GetID(), light->spotLightData.outerCutOff);

			break;
	}
}

void GLFW_EngineCore::calculateShadows(Game* game) {
	glm::mat4 projection, lightView;

	std::map<std::string, GameObject*>::iterator it;
	std::map<std::string, GameObject*> sceneObjects = game->m_currentScene->getGameObjects();
	shadowTarget->SetUp(sceneLights);
	for (Light* light : sceneLights) { 
		shadowTarget->RenderToBuffer(light);
		switch (light->lType()) {
		case Light::DIRECTIONAL:
		case Light::SPOT:
			for (it = sceneObjects.begin(); it != sceneObjects.end(); ++it) {
				if (it->second->getComponent<ModelComponent>() != nullptr && it->second->getComponent<TransformComponent>() != nullptr && it->second->getComponent<ModelComponent>()->active) {
					glm::mat4 matrix = it->second->getComponent<TransformComponent>()->getModelMatrix();
					shadowTarget->RenderDirectional(
						it->second->getComponent<ModelComponent>(), 
						&it->second->getComponent<TransformComponent>()->getModelMatrix(), 
						&light->lightSpaceMatrix
					);
				}
			}
			break;
		case Light::POINT:
			for (it = sceneObjects.begin(); it != sceneObjects.end(); ++it) {
				if (it->second->getComponent<ModelComponent>() != nullptr && it->second->getComponent<TransformComponent>() != nullptr && it->second->getComponent<ModelComponent>()->active) {
					glm::vec3 position = light->position();
					shadowTarget->RenderPoint(
						it->second->getComponent<ModelComponent>(),
						&it->second->getComponent<TransformComponent>()->getModelMatrix(),
						position
					);
				}
			}
			break;
		}
	}
	shadowTarget->UnbindBuffer();
}

void GLFW_EngineCore::DisplayFramebufferTexture(GLuint textureID)
{
	debugBuffer->use();
	GLuint VBO;
	GLfloat vertices[] = {
		// Pos      // Tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &vaoDebugTexturedRect);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(vaoDebugTexturedRect);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
	glUseProgram(debugBuffer->ID);
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(m_screenWidth), 0.0f, static_cast<GLfloat>(m_screenHeight));
	glUniformMatrix4fv(glGetUniformLocation(debugBuffer->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f));
	model = glm::scale(model, glm::vec3(200.0f, 100.0f, 1.0f));
	debugBuffer->SetMatrix4("model", model, true);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glBindVertexArray(vaoDebugTexturedRect);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glUseProgram(0);
}

HDRRenderTarget* GLFW_EngineCore::getDisplayedRenderTarget()
{
	return hdrTarget;
}

void GLFW_EngineCore::drawText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color,
								std::map<GLchar, Character> Characters, GLuint VAO, GLuint VBO)
{
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(m_screenWidth), 0.0f, static_cast<GLfloat>(m_screenHeight));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);

	// Activate corresponding render state	
	textWriterShader->use();
	glUniformMatrix4fv(glGetUniformLocation(textWriterShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3f(glGetUniformLocation(textWriterShader->ID, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos,     ypos,       0.0, 1.0 },
		{ xpos + w, ypos,       1.0, 1.0 },

		{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos + w, ypos,       1.0, 1.0 },
		{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_BLEND);
}

void GLFW_EngineCore::draw2DRect(glm::vec2 position, glm::vec2 size, GLfloat rotate, glm::vec3 color, GLuint quadVAO) {
	// Prepare transformations
	Shader2D->use();

	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(m_screenWidth), 0.0f, static_cast<GLfloat>(m_screenHeight));
	glUniformMatrix4fv(glGetUniformLocation(Shader2D->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));

	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

	model = glm::scale(model, glm::vec3(size, 1.0f));

	Shader2D->SetMatrix4("model", model, true);

	// Render textured quad
	Shader2D->SetVector3f("Color", color, true);

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

// loading some default shaders to get things up and running
void GLFW_EngineCore::setDefaultShaders()
{
	///Use Default Phong for Showing Errors Loading Materials and Objects Without Materials
	phong = new Shader("assets/shaders/defaultShader.vert", "assets/shaders/defaultShader.frag");
	///Normal Blin-Phong Shader for proper rendering
	texturePhong = new Shader("assets/shaders/phong2.vert", "assets/shaders/phong2.frag");//6
	simpleDepth = new Shader("assets/shaders/simpleShadow.vert", "assets/shaders/simpleShadow.frag");//9
	pointDepth = new Shader("assets/shaders/pointShadow.vert", "assets/shaders/pointShadow.frag", "assets/shaders/pointShadow.geom");//13
	textWriterShader = new Shader("assets/shaders/TextWriter.vert", "assets/shaders/TextWriter.frag");//16
	Shader2D = new Shader("assets/shaders/Shader2D.vert", "assets/shaders/Shader2D.frag");//19
	textureRender = new Shader("assets/shaders/TextureRender.vert", "assets/shaders/TextureRender.frag");

	lightBox = new Shader("assets/shaders/LightBox.vert", "assets/shaders/LightBox.frag");

	debugShadow = new Shader("assets/shaders/debugShadow.vert", "assets/shaders/debugShadow.frag");
	debugBuffer = new Shader("assets/shaders/debugging.vert", "assets/shaders/debugging.frag");
}

// a simple function to initialise a cube model in memory
void GLFW_EngineCore::initCubeModel()
{
	float vertices[] = {
		// back face
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
		// front face
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		// left face
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		// right face
			1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
		// bottom face
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		// top face
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
	};
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	// fill buffer
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// link vertex attributes
	glBindVertexArray(cubeVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);	
}

void GLFW_EngineCore::setCamera(const Camera* cam)
{
	phong->use();
	// set the view and projection components of our shader to the camera values
	glm::mat4 projection = glm::perspective(glm::radians(cam->m_fov), (float)m_screenWidth / (float)m_screenHeight, 0.1f, 1000.0f);
	glUniformMatrix4fv(glGetUniformLocation(phong->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glUniformMatrix4fv(glGetUniformLocation(phong->ID, "view"), 1, GL_FALSE, glm::value_ptr(cam->getViewMatrix()));

	// be sure to activate shader when setting uniforms/drawing objects
	glUniform3fv(glGetUniformLocation(phong->ID, "viewPos"), 1, glm::value_ptr(cam->position()));
	texturePhong->use();
	// set the view and projection components of our shader to the camera values
	glUniformMatrix4fv(glGetUniformLocation(texturePhong->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glUniformMatrix4fv(glGetUniformLocation(texturePhong->ID, "view"), 1, GL_FALSE, glm::value_ptr(cam->getViewMatrix()));

	// be sure to activate shader when setting uniforms/drawing objects
	glUniform3fv(glGetUniformLocation(texturePhong->ID, "viewPos"), 1, glm::value_ptr(cam->position()));

	lightBox->use();
	// set the view and projection components of our shader to the camera values
	glUniformMatrix4fv(glGetUniformLocation(lightBox->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glUniformMatrix4fv(glGetUniformLocation(lightBox->ID, "view"), 1, GL_FALSE, glm::value_ptr(cam->getViewMatrix()));

	// be sure to activate shader when setting uniforms/drawing objects
	glUniform3fv(glGetUniformLocation(lightBox->ID, "viewPos"), 1, glm::value_ptr(cam->position()));
}

void GLFW_EngineCore::drawCube(const glm::mat4& modelMatrix)
{
	glUseProgram(phong->ID);
	glBindVertexArray(cubeVAO);
	// set the model component of our shader to the cube model
	glUniformMatrix4fv(glGetUniformLocation(phong->ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// the only thing we can draw so far is the cube, so we know it is bound already
	// this will obviously have to change later
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void GLFW_EngineCore::drawLightCube(const glm::mat4& modelMatrix)
{
	lightBox->use();
	glBindVertexArray(cubeVAO);
	// set the model component of our shader to the cube model
	glUniformMatrix4fv(glGetUniformLocation(lightBox->ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// the only thing we can draw so far is the cube, so we know it is bound already
	// this will obviously have to change later
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

GLenum GLFW_EngineCore::glCheckError_(const char *file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case 0x0503:                error = "STACK_OVERFLOW"; break;
		case 0x0504:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}

void GLFW_EngineCore::DrawToCamTexture(Game * game)
{
	camTexture->PrepareToRender();

	std::map<std::string, GameObject*>::iterator it;
	std::map<std::string, GameObject*> sceneObjects = game->m_currentScene->getGameObjects();
	for (it = sceneObjects.begin(); it != sceneObjects.end(); ++it)
	{
		if (it->second->getComponent<ModelComponent>() != nullptr && it->second->getComponent<TransformComponent>() != nullptr) {
			camTexture->RenderToTexture(it->second->getComponent<ModelComponent>()->model, it->second->getComponent<TransformComponent>()->getModelMatrix());
		}
	}

	camTexture->RevertToNormalRendering();

	DisplayFramebufferTexture(camTexture->getTextureID());
}

#define glCheckError() glCheckError_(__FILE__, __LINE__)

void APIENTRY glDebugOutput(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar *message,
	const void *userParam)
{
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return; // ignore these non-significant error codes

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}

void DrawToCamTexture(Game * game)
{
	
}
