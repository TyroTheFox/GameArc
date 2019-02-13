#include "Game.h"
#include <fstream>
#include <sstream>

Game::Game() {

}

Game::Game(string levelsFile, DebugHelper* debug)
{
	lightHandler = new LightHandler();
	testLight = lightHandler->createNewLight(
		LightColour(glm::vec3(1), glm::vec3(1), glm::vec3(1)),
		glm::vec3(0.0f, 100.0f, 0.0f),
		glm::vec3(0, 0, 0)
	);
	//lightHandler->createNewLight(
	//	LightColour(glm::vec3(1), glm::vec3(1), glm::vec3(1)),
	//	glm::vec3(-45, 20, 0)
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
	//lightHandler->createNewLight(
	//	SpotLightData(1, 0.014f, 0.0007f, glm::cos(glm::radians(50.0f)), glm::cos(glm::radians(55.0f))),
	//	LightColour(glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1)),
	//	glm::vec3(0, -10.0f, 0),
	//	glm::vec3(0, -170, 0)
	//);
	//testLight = lightHandler->createNewLight(
	//	SpotLightData(1, 0.014f, 0.0007f, glm::cos(glm::radians(50.0f)), glm::cos(glm::radians(55.0f))),
	//	LightColour(glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1)),
	//	glm::vec3(0, -10.0f, 0),
	//	glm::vec3(0, 0, 0)
	//);

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
	keyEvent->subscribeToEvent("rotYawL-", [this]() { this->testLight->yaw(-0.01); testLight->CalculateDirection(); });
	keyEvent->subscribeToEvent("rotYawL+", [this]() { this->testLight->yaw(0.01); testLight->CalculateDirection(); });
	keyEvent->subscribeToEvent("rotPitL-", [this]() { this->testLight->pitch(-0.01); testLight->CalculateDirection(); });
	keyEvent->subscribeToEvent("rotPitL+", [this]() { this->testLight->pitch(0.01); testLight->CalculateDirection(); });
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
		m_engineInterfacePtr->calculateLight(light, lightHandler->getDirectionalLightCount(), lightHandler->getPointLightCount(), lightHandler->getSpotLightCount());
		m_engineInterfacePtr->drawCube(glm::inverse(light->GetMatrix()));
		
		glm::mat4 marker = glm::toMat4(light->orientation());
		glm::vec3 orient = glm::vec3(0.0f);
		light->CalculateDirection();
		orient = light->direction() * 5.0f;
		marker *= glm::translate(light->GetMatrix(), light->position() + orient); 
		marker *= glm::scale(marker, glm::vec3(0.5f));
		
		//std::cout << "Direction: " << light->direction().x * 5.0f << " " << light->direction().y * 5.0f << " " << light->direction().z * 5.0f << " " << std::endl;
		m_engineInterfacePtr->drawCube(marker);
	}

	std::map<std::string, GameObject*>::iterator it;
	std::map<std::string, GameObject*> sceneObjects = m_currentScene->getGameObjects();
	for (it = sceneObjects.begin(); it != sceneObjects.end(); ++it)
	{
		it->second->renderAllComponents(m_engineInterfacePtr);
	}

	debugHelper->render();
}
