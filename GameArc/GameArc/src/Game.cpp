#include "Game.h"
#include <fstream>
#include <sstream>

Game::Game() {

}

Game::Game(string levelsFile, DebugHelper* debug)
{
	debugHelper = debug;
	loadFromJSON(levelsFile);

	//Scene* temp = new Scene();
	//temp->loadLevelJSON("assets/levels/myCubeLevel.json");
	//sceneList["CubeLevel"] = temp;

	//temp = new Scene();
	//temp->loadLevelJSON("assets/levels/scene1.json");
	//sceneList["scene1"] = temp;

	//m_currentScene = sceneList["scene1"];

	std::map<std::string, GameObject*>::iterator it;
	std::map<std::string, GameObject*> sceneObjects = m_currentScene->getGameObjects();
	for (it = sceneObjects.begin(); it != sceneObjects.end(); ++it)
	{
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
}

void Game::init()
{
	//debugHelper = new DebugHelper(m_engineInterfacePtr);
	// update the camera
	if (m_MainCamera == nullptr) {
		m_engineInterfacePtr->setCamera(&m_camera);
	}
	else {
		m_engineInterfacePtr->setCamera(m_MainCamera);
	}
	debugHelper->WriteToConsole("Game Set Up Successfully");
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
			Scene* temp = new Scene();
			temp->debug = debugHelper;
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
	//if (activePlayer != nullptr) {
	//	activePlayer->OnUpdate(dt);
	//	//m_engineInterfacePtr->setCamera(activePlayer->GetCamera());
	//}
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
	if (m_playerBackground.getComponent<RedComponent>())
		redValue = m_playerBackground.getComponent<RedComponent>()->m_colourValue;
	if (m_playerBackground.getComponent<GreenComponent>())
		greenValue = m_playerBackground.getComponent<GreenComponent>()->m_colourValue;
	if (m_playerBackground.getComponent<BlueComponent>())
		blueValue = m_playerBackground.getComponent<BlueComponent>()->m_colourValue;

	// e.g. pass object details to the engine to render the next frame
	m_engineInterfacePtr->renderColouredBackground(redValue, greenValue, blueValue);

	//for (GameObject* object : m_currentScene->getGameObjects()) {
	std::map<std::string, GameObject*>::iterator it;
	std::map<std::string, GameObject*> sceneObjects = m_currentScene->getGameObjects();
	for (it = sceneObjects.begin(); it != sceneObjects.end(); ++it)
	{
		it->second->renderAllComponents(m_engineInterfacePtr);
		// draw the cube
		//m_engineInterfacePtr->drawCube(object->getComponent<TransformComponent>()->getModelMatrix());
		if (it->second->getComponent<ModelComponent>() == nullptr && it->second->getComponent<TransformComponent>() == nullptr) continue;
		if (it->second->getComponent<ModelComponent>()->active) {
			Model* temp = it->second->getComponent<ModelComponent>()->model;
			m_engineInterfacePtr->drawModel(temp, it->second->getComponent<TransformComponent>()->getModelMatrix());
		}
	}
	debugHelper->render();
}
