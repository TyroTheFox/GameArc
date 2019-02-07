#include "GLFW_EngineCore.h"
#include "Game.h"
#include <fstream>
#include <sstream>
#include <glm/detail/type_vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

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
}

bool GLFW_EngineCore::initWindow(int width, int height, std::string windowName)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	
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
	initShadows();
	//for (it = sceneObjects.begin(); it != sceneObjects.end(); ++it) {
	//	if(it->second->getComponent<ModelComponent>() != nullptr && it->second->getComponent<TransformComponent>() != nullptr)
	//	calculateShadows(gameptr, it->second->getComponent<ModelComponent>()->model, it->second->getComponent<TransformComponent>()->getModelMatrix());
	//}
	
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
		 
		std::map<std::string, GameObject*>::iterator it;
		std::map<std::string, GameObject*> sceneObjects = game.m_currentScene->getGameObjects();
		//if (sceneObjects["cube01"] != nullptr)
			//calculateShadows(gameptr, sceneObjects["cube01"]->getComponent<ModelComponent>()->model, sceneObjects["cube01"]->getComponent<TransformComponent>()->getModelMatrix());
		for (it = sceneObjects.begin(); it != sceneObjects.end(); ++it) {
			if (it->second->getComponent<ModelComponent>() != nullptr && it->second->getComponent<TransformComponent>() != nullptr)
				calculateShadows(gameptr, it->second->getComponent<ModelComponent>()->model, it->second->getComponent<TransformComponent>()->getModelMatrix());
		}

		if (game.m_MainCamera == nullptr) {
			setCamera(&game.m_camera);
		}
		else {
			setCamera(game.m_MainCamera);
		}
		game.render(); // prepare game to send info to the renderer in engine core
		glEnable(GL_FRAMEBUFFER_SRGB);
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
}

void GLFW_EngineCore::drawModel(Model* model, const glm::mat4& modelMatrix)
{
	Shader* temp;
	if (model->GetTextureSize() > 0) {
		temp = texturePhong;
	}
	else {
		temp = phong;
	}
	temp->use();
	
	// set the model component of our shader to the object model
	glUniformMatrix4fv(glGetUniformLocation(temp->ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	texturePhong->SetVector3f("material.ambient", model->modelColour.ambient, true);
	texturePhong->SetVector3f("material.diffuse", model->modelColour.diffuse, true);
	texturePhong->SetVector3f("material.specular", model->modelColour.specular, true);
	texturePhong->setFloat("material.shininess", model->modelColour.shininess);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	texturePhong->setInt("diffuseTexture0", 0);
	texturePhong->setInt("shadowMap", 1);
	model->render(temp->ID);
	glBindTexture(GL_TEXTURE_2D, 0);
	debugShadow->setFloat("near_plane", 1.0f);
	debugShadow->setFloat("far_plane", 100.0f);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	model->render(debugShadow->ID);
}

void GLFW_EngineCore::calculateLight(Light * light, int directionalLightTotal, int pointLightTotal, int spotLightTotal)
{
	texturePhong->use();
	texturePhong->setInt("noOfDirectionalLights", directionalLightTotal);

	texturePhong->use();
	texturePhong->setInt("noOfPointLights", pointLightTotal);

	texturePhong->use();
	texturePhong->setInt("noOfSpotLights", spotLightTotal);

	phong->use();
	phong->setBool("blinn", false);
	texturePhong->use();
	texturePhong->setBool("blinn", false);

	std::string stringID;
	std::string stringItem;

	switch(light->lType()){
		case Light::LightType::DIRECTIONAL:
			texturePhong->SetArrayVector3f("dirLights", "ambient", light->GetID(), light->lColour().ambient, true);
			texturePhong->SetArrayVector3f("dirLights", "diffuse", light->GetID(), light->lColour().diffuse, true);
			texturePhong->SetArrayVector3f("dirLights", "specular", light->GetID(), light->lColour().specular, true);
			texturePhong->SetArrayVector3f("dirLights", "direction", light->GetID(), light->direction(), true);

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
			texturePhong->SetArrayVector3f("spotLights", "direction", light->GetID(), light->direction(), true);
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

void GLFW_EngineCore::initShadows()
{	
	// configure depth map FBO
	// -----------------------
	glGenFramebuffers(1, &depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

	// create depth texture
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

	//float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "FRAMEBUFFER IS INCOMPLETE" << std::endl;
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLFW_EngineCore::calculateShadows(Game* game, Model* model, const glm::mat4& modelMatrix) {
	// 1. render depth of scene to texture (from light's perspective)
	// --------------------------------------------------------------
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 1.0f, far_plane = 50.0f;
	lightProjection = glm::ortho<float>(-50.0f, 50.0f, -50.0f, 50.0f, near_plane, far_plane);
	std::vector<Light*> gameLights = game->lightHandler->getLights();
	int i = 0;
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	//glClear(GL_DEPTH_BUFFER_BIT);
	for (Light* light : gameLights) {
		//if (light->lType() != Light::DIRECTIONAL) { continue; }
		//lightView = glm::lookAt(-light->direction(), glm::vec3(modelMatrix[3]), light->up());
		lightView = glm::lookAt(-light->direction(), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightSpaceMatrix = lightProjection * lightView * glm::mat4(1.0);
		// render scene from light's point of view
		
		simpleDepth->SetMatrix4("lightSpaceMatrix", lightSpaceMatrix, true);
		//simpleDepth->SetMatrix4("model", modelMatrix, true);
		simpleDepth->use();
		///Switch to rendering to 2D shadow texture
		model->render(simpleDepth->ID);
		
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, m_screenWidth, m_screenHeight);
}

void GLFW_EngineCore::renderShadows(Game& game)
{
	///Switch to rendering to 2D shadow texture
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	simpleDepth->use();
	game.render();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, m_screenWidth, m_screenHeight);
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
	texturePhong = new Shader("assets/shaders/phong2.vert", "assets/shaders/phong2.frag");
	simpleDepth = new Shader("assets/shaders/simpleShadow.vert", "assets/shaders/simpleShadow.frag");
	textWriterShader = new Shader("assets/shaders/TextWriter.vert", "assets/shaders/TextWriter.frag");
	Shader2D = new Shader("assets/shaders/Shader2D.vert", "assets/shaders/Shader2D.frag");

	debugShadow = new Shader("assets/shaders/debugShadow.vert", "assets/shaders/debugShadow.frag");
}

// a simple function to initialise a cube model in memory
void GLFW_EngineCore::initCubeModel()
{
	// set up vertex and normal data
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
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
	glUniform3fv(glGetUniformLocation(texturePhong->ID, "viewDir"), 1, glm::value_ptr(cam->position()));
	
}

void GLFW_EngineCore::drawCube(const glm::mat4& modelMatrix)
{
	glUseProgram(phong->ID);
	// set the model component of our shader to the cube model
	glUniformMatrix4fv(glGetUniformLocation(phong->ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// the only thing we can draw so far is the cube, so we know it is bound already
	// this will obviously have to change later
	glDrawArrays(GL_TRIANGLES, 0, 36);
}