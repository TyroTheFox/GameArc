#include "Game.h"
#include <fstream>
#include <sstream>

Game::Game() {

}

Game::Game(string levelsFile, DebugHelper* debug)
{
	lightHandler = new LightHandler();
	testLight = lightHandler->createNewLight(
		LightColour(glm::vec3(1), glm::vec3(0.3f), glm::vec3(1)),
		glm::vec3(3.006, 0.812, -2.266)
	);
	//lightHandler->createNewLight(
	//	LightColour(glm::vec3(1), glm::vec3(0.4f), glm::vec3(1)),
	//	glm::vec3(-60, 10, 0)
	//);
	//lightHandler->createNewLight(
	//	LightColour(glm::vec3(1), glm::vec3(1), glm::vec3(1)),
	//	glm::vec3(-90, 20, 0)
	//);
	//lightHandler->createNewLight(
	//	LightColour(glm::vec3(1), glm::vec3(1), glm::vec3(1)),
	//	glm::vec3(-45, 20, 0)
	//);
	//for (int i = 0; i < 16; i++) {
	//	lightHandler->createNewLight(
	//		PointLightData(1, 0.022f, 0.0019f),
	//		LightColour(glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1)),
	//		glm::vec3(5.0f * i, 30.0f, 0.0f)
	//	);
	//	lightHandler->createNewLight(
	//		SpotLightData(1, 0.014f, 0.0007f, glm::cos(glm::radians(50.0f)), glm::cos(glm::radians(55.0f))),
	//		LightColour(glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1)),
	//		glm::vec3(-5.0f * i, 30.0f, 50.0f),
	//		glm::vec3(-90, 0, 0)
	//	);
	//}
	//lightHandler->createNewLight(
	//	PointLightData(1, 0.022f, 0.0019f),
	//	LightColour(glm::vec3(1, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0)),
	//	glm::vec3(0.0f, 30.0f, -100.0f)
	//);
	//lightHandler->createNewLight(
	//	PointLightData(1, 0.014f, 0.0007f),
	//	LightColour(glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0)),
	//	glm::vec3(200.0f, 30.0f, -100.0f)
	//);
	//lightHandler->createNewLight(
	//	PointLightData(1, 0.014f, 0.0007f),
	//	LightColour(glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1)),
	//	glm::vec3(-200.0f, 30.0f, -100.0f)
	//);

	//rotates BY, not TO
	spotLight1 = lightHandler->createNewLight(
		SpotLightData(1, 0.014f, 0.0007f, glm::cos(glm::radians(50.0f)), glm::cos(glm::radians(55.0f))),
		LightColour(glm::vec3(1, 1, 1), glm::vec3(0, 10, 0), glm::vec3(1, 1, 1)),
		glm::vec3(0, 10, 15.0f),
		glm::vec3(0, 3.142, 3.142f)
	);
	spotLight2 = lightHandler->createNewLight(
		SpotLightData(1, 0.014f, 0.0007f, glm::cos(glm::radians(50.0f)), glm::cos(glm::radians(55.0f))),
		LightColour(glm::vec3(1, 1, 1), glm::vec3(10, 0, 0), glm::vec3(1, 1, 1)),
		glm::vec3(10, 10, 15.0f),
		glm::vec3(0, 2.666, 3.142f)
	);
	spotLight3 = lightHandler->createNewLight(
		SpotLightData(1, 0.014f, 0.0007f, glm::cos(glm::radians(50.0f)), glm::cos(glm::radians(55.0f))),
		LightColour(glm::vec3(1, 1, 1), glm::vec3(0, 0, 10), glm::vec3(1, 1, 1)),
		glm::vec3(-10, 10, 15.0f),
		glm::vec3(0, -2.666, 3.142f)
	);
	//testLight = lightHandler->createNewLight(
	//	SpotLightData(1, 0.014f, 0.0007f, glm::cos(glm::radians(50.0f)), glm::cos(glm::radians(55.0f))),
	//	LightColour(glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1)),
	//	glm::vec3(0, 7.0f, 5),
	//	glm::vec3(0, 0, 0)
	//);

	//testLight = lightHandler->createNewLight(
	//	PointLightData(1, 0.014f, 0.0007f),
	//	LightColour(glm::vec3(1, 1, 1), glm::vec3(10, 0, 0), glm::vec3(1, 1, 1)),
	//	glm::vec3(0, 0.0f, -120)
	//);

	//pointLight1 = lightHandler->createNewLight(
	//	PointLightData(1, 0.014f, 0.0007f),
	//	LightColour(glm::vec3(1, 1, 1), glm::vec3(0, 0, 10), glm::vec3(1, 1, 1)),
	//	glm::vec3(-10, 5, 20.0f)
	//);
	//pointLight1->active = false;
	//pointLight2 = lightHandler->createNewLight(
	//	PointLightData(1, 0.014f, 0.0007f),
	//	LightColour(glm::vec3(1, 1, 1), glm::vec3(0, 10, 0), glm::vec3(1, 1, 1)),
	//	glm::vec3(0, 5, 20.0f)
	//);
	//pointLight2->active = false;
	//pointLight3 = lightHandler->createNewLight(
	//	PointLightData(1, 0.014f, 0.0007f),
	//	LightColour(glm::vec3(1, 1, 1), glm::vec3(10, 0, 0), glm::vec3(1, 1, 1)),
	//	glm::vec3(10, 5, 20.0f)
	//);
	//pointLight3->active = false;

	debugHelper = debug;
	ModelHandler* modelHandler = new ModelHandler();
	oM = new ObjectManager(debugHelper, modelHandler, lightHandler);
	loadFromJSON(levelsFile);

	std::map<std::string, GameObject*>::iterator it;
	std::map<std::string, GameObject*> sceneObjects = m_currentScene->getGameObjects();
	for (it = sceneObjects.begin(); it != sceneObjects.end(); ++it)
	{
		if (it->first == "background") {
			if (it->second->getComponent<ColourComponent>() != nullptr) {
				m_playerBackground = it->second;
			}
		}

		if (it->second->getComponent<PlayerComponent>() == nullptr) continue;

		activePlayer = it->second->getComponent<PlayerComponent>();
		m_MainCamera = activePlayer->GetCamera();
	}
	if (activePlayer == nullptr) {
		for (it = sceneObjects.begin(); it != sceneObjects.end(); ++it)
		{
			if (it->second->getComponent<CameraComponent>() != nullptr) {

				CameraComponent* temp = it->second->getComponent<CameraComponent>();
				m_MainCamera = temp->GetCamera();
				break;
			}
			if (it->second->getComponent<EventCameraComponent>() != nullptr) {

				EventCameraComponent* temp = it->second->getComponent<EventCameraComponent>();
				m_MainCamera = temp->GetCamera();
				break;
			}
		}
	}

	keyEvent->subscribeToEvent("ChangeScene", [this](std::string message) { this->ChangeScene(message); });

	debugHelper->AddConsoleCommand("changeScene", TextParser::InterpFunc([this](std::vector<std::string> s) { if (s.size() <= 0) { this->debugHelper->WriteErrorToConsole("Missing value"); return; }
		this->ChangeScene(s.at(0)); }));
	debugHelper->AddConsoleCommand("listScenes", TextParser::InterpFunc([this](std::vector<std::string> s) {
		std::string temp = "Scenes:";
		for (auto scene : sceneList) {
			temp += " " + scene.first;
		}
		this->debugHelper->WriteToConsole(temp);
	}));
	debugHelper->AddConsoleCommand("HDR", TextParser::InterpFunc([this](std::vector<std::string> s) { 
		if (s.size() <= 0) { this->debugHelper->WriteErrorToConsole("Missing value"); return; }
		if (s.at(0) == "true") {
			this->m_engineInterfacePtr->getDisplayedRenderTarget()->SetHDR(true);
		}
		if (s.at(0) == "false") {
			this->m_engineInterfacePtr->getDisplayedRenderTarget()->SetHDR(false);
		}
		if (s.at(0) != "true" && s.at(0) != "false") {
			this->debugHelper->WriteErrorToConsole("Incorrect Syntax, true or false only"); return;
		}
	}));

	debugHelper->AddConsoleCommand("Bloom", TextParser::InterpFunc([this](std::vector<std::string> s) {
		if (s.size() <= 0) { this->debugHelper->WriteErrorToConsole("Missing value"); return; }
		if (s.at(0) == "true") {
			this->m_engineInterfacePtr->getDisplayedRenderTarget()->SetBloom(true);
		}
		if (s.at(0) == "false") {
			this->m_engineInterfacePtr->getDisplayedRenderTarget()->SetBloom(false);
		}
		if (s.at(0) != "true" && s.at(0) != "false") {
			this->debugHelper->WriteErrorToConsole("Incorrect Syntax, true or false only"); return;
		}
	}));

	debugHelper->RunCommand("autoRotate OTTO");

	keyEvent->subscribeToEvent("rotYawL-", [this]() { this->testLight->yaw(-0.01); testLight->CalculateDirection(); });
	keyEvent->subscribeToEvent("rotYawL+", [this]() { this->testLight->yaw(0.01); testLight->CalculateDirection(); });
	keyEvent->subscribeToEvent("rotPitL-", [this]() { this->testLight->pitch(-0.01); testLight->CalculateDirection(); });
	keyEvent->subscribeToEvent("rotPitL+", [this]() { this->testLight->pitch(0.01); testLight->CalculateDirection(); });

	keyEvent->subscribeToEvent("expo+", [this]() { this->m_engineInterfacePtr->getDisplayedRenderTarget()->exposure += 0.1f; });
	keyEvent->subscribeToEvent("expo-", [this]() { this->m_engineInterfacePtr->getDisplayedRenderTarget()->exposure -= 0.1f; });

	keyEvent->subscribeToEvent("gamma-", [this]() { this->m_engineInterfacePtr->getDisplayedRenderTarget()->gamma -= 0.1f; });
	keyEvent->subscribeToEvent("gamma+", [this]() { this->m_engineInterfacePtr->getDisplayedRenderTarget()->gamma += 0.1f; });

	keyEvent->subscribeToEvent("switchLights", [this]() { 
		spotLight1->active = !spotLight1->active;
		spotLight2->active = !spotLight2->active;
		spotLight3->active = !spotLight3->active;

		pointLight1->active = !pointLight1->active;
		pointLight2->active = !pointLight2->active;
		pointLight3->active = !pointLight3->active;
	});
}

void Game::init(InputHandler* iH)
{
	inputHandler = iH;
	// update the camera
	std::vector<Light*> gameLights = lightHandler->getLights();
	for (Light* light : gameLights) {
		m_engineInterfacePtr->calculateLight(light, lightHandler->getDirectionalLightCount(), lightHandler->getPointLightCount(), lightHandler->getSpotLightCount());
	}
	debugHelper->WriteToConsole("Game Set Up Successfully");
}

void Game::CleanUp() {
	delete debugHelper;
	delete modelHandler;
	std::map<string, Scene*>::iterator itr;
	for (itr = sceneList.begin(); itr != sceneList.end(); itr++)
	{
		delete (itr->second);
	}
	sceneList.clear();
	delete oM;
}

bool Game::loadFromJSON(string levelsFile) {
	std::ifstream inputstream(levelsFile);
	Json::Reader reader;
	Json::Value obj;
	reader.parse(inputstream, obj);

	const Json::Value& levels = obj["Levels"];
	if (!levels) {
		std::cout << "Exception thrown loading Levels from JSON(" << levelsFile << "), no value for Inputs." << std::endl;
		debugHelper->WriteToConsole("Exception thrown loading Levels from JSON(" + levelsFile + "), no value for Inputs.");
		return false;
	}

	for (unsigned int i = 0; i < levels.size(); i++) {
		try {
			Scene* temp = new Scene(debugHelper, oM);
			temp->loadLevelJSON(levels[i]["Address"].asString());
			string name = levels[i]["Name"].asString();
			sceneList[name] = temp;
			if (levels[i]["StartingScene"].asBool()) {
				m_currentScene = sceneList[name];
			}
		}
		catch (...) { 
			std::cout << "Exception thrown loading Levels from JSON(" << levelsFile << "), in parsing Input[" << std::to_string(i) << "]." << std::endl; 
			debugHelper->WriteToConsole("Exception thrown loading Levels from JSON(" + levelsFile + "), in parsing Input[" + std::to_string(i) + "].");
		}
	}
	return true;
}

void Game::ChangeScene(string sceneName) {
	if (sceneList.count(sceneName) == 0) {
		return;
	}
	m_currentScene = sceneList[sceneName];
	std::map<std::string, GameObject*>::iterator it;
	std::map<std::string, GameObject*> sceneObjects = m_currentScene->getGameObjects();
	for (it = sceneObjects.begin(); it != sceneObjects.end(); ++it)
	{
		if (it->first == "background") {
			if (it->second->getComponent<ColourComponent>() != nullptr) {
				m_playerBackground = it->second;
			}
		}

		if (it->second->getComponent<PlayerComponent>() == nullptr) continue;

		activePlayer = it->second->getComponent<PlayerComponent>();
		m_MainCamera = activePlayer->GetCamera();
	}
	if (activePlayer == nullptr) {
		for (it = sceneObjects.begin(); it != sceneObjects.end(); ++it)
		{
			if (it->second->getComponent<CameraComponent>() != nullptr) {

				CameraComponent* temp = it->second->getComponent<CameraComponent>();
				m_MainCamera = temp->GetCamera();
				break;
			}
		}
	}
	debugHelper->WriteToConsole("SceneChanged to " + sceneName);
}

void Game::update(float dt)
{
	debugHelper->update(dt);
	inputHandler->setDisableInput(debugHelper->displayConsole);
	std::map<std::string, GameObject*>::iterator it;
	std::map<std::string, GameObject*> sceneObjects = m_currentScene->getGameObjects();
	for (it = sceneObjects.begin(); it != sceneObjects.end(); ++it)
	{
		it->second->updateAllComponents(dt);
	}
	testLight->yaw(0.001f);
}

void Game::render()
{
	float redValue = 0, greenValue = 0, blueValue = 0;
	if (m_playerBackground != nullptr) {
		if (m_playerBackground->getComponent<ColourComponent>()) {
			redValue = m_playerBackground->getComponent<ColourComponent>()->m_red;
			greenValue = m_playerBackground->getComponent<ColourComponent>()->m_green;
			blueValue = m_playerBackground->getComponent<ColourComponent>()->m_blue;
		}
	}
	// e.g. pass object details to the engine to render the next frame
	m_engineInterfacePtr->renderColouredBackground(redValue, greenValue, blueValue);
	
	m_engineInterfacePtr->calculateShadows(this);

	std::vector<Light*> gameLights = lightHandler->getLights();
	for (Light* light : gameLights) {
		if (light->active) {
			m_engineInterfacePtr->calculateLight(light, lightHandler->getDirectionalLightCount(), lightHandler->getPointLightCount(), lightHandler->getSpotLightCount());
			light->CalculateDirection();
		}
	}

	std::map<std::string, GameObject*>::iterator it;
	std::map<std::string, GameObject*> sceneObjects = m_currentScene->getGameObjects();
	for (it = sceneObjects.begin(); it != sceneObjects.end(); ++it)
	{
		it->second->renderAllComponents(m_engineInterfacePtr);
	}

	debugHelper->render();
}
