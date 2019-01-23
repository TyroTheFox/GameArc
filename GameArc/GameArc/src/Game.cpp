#include "Game.h"
#include <fstream>
#include <sstream>

Game::Game() {

}

Game::Game(string levelsFile, DebugHelper* debug)
{
	debugHelper = debug;
	ModelHandler* modelHandler = new ModelHandler();
	oM = new ObjectManager(debugHelper, modelHandler);
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

	EventHandler::FuncMessage nextSceneMessage = [this](std::string message) { this->ChangeScene(message); };
	EventHandler nextSceneListener(nextSceneMessage, "ChangeScene");
	keyEvent->addHandler(nextSceneListener);

	debug->AddConsoleCommand("changescene", TextParser::InterpFunc([this](std::vector<std::string> s) { this->ChangeScene(s.at(0)); }));
}

void Game::init(InputHandler* iH)
{
	inputHandler = iH;
	// update the camera
	if (m_MainCamera == nullptr) {
		m_engineInterfacePtr->setCamera(&m_camera);
	}
	else {
		m_engineInterfacePtr->setCamera(m_MainCamera);
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
	// update the camera
	if (m_MainCamera == nullptr) {
		m_engineInterfacePtr->setCamera(&m_camera);
	}
	else {
		m_engineInterfacePtr->setCamera(m_MainCamera);
	}
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

	//for (GameObject* object : m_currentScene->getGameObjects()) {
	std::map<std::string, GameObject*>::iterator it;
	std::map<std::string, GameObject*> sceneObjects = m_currentScene->getGameObjects();
	for (it = sceneObjects.begin(); it != sceneObjects.end(); ++it)
	{
		it->second->renderAllComponents(m_engineInterfacePtr);
	}
	debugHelper->render();
}
