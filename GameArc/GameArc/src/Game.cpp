#include "Game.h"
#include <fstream>
#include <sstream>

Game::Game()
{
	m_engineInterfacePtr = nullptr;
	m_currentScene = new Scene();
	m_currentScene->loadLevelJSON("assets/levels/myCubeLevel.json");
}

void Game::update()
{
	
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

	// update the camera (probably don't need to do this each frame)
	m_engineInterfacePtr->setCamera(&m_camera);

	//for (GameObject* object : m_currentScene->getGameObjects()) {
	std::map<std::string, GameObject*>::iterator it;
	std::map<std::string, GameObject*> sceneObjects = m_currentScene->getGameObjects();
	for (it = sceneObjects.begin(); it != sceneObjects.end(); ++it)
	{
		// draw the cube
		//m_engineInterfacePtr->drawCube(object->getComponent<TransformComponent>()->getModelMatrix());
		if (it->second->getComponent<ModelComponent>() == nullptr && it->second->getComponent<TransformComponent>() == nullptr) continue;
		Model* temp = it->second->getComponent<ModelComponent>()->model;
		m_engineInterfacePtr->drawModel(temp, it->second->getComponent<TransformComponent>()->getModelMatrix());
	}
}

vector<InputHandler*> Game::getInputHandlers()
{
	return m_currentScene->getInputHandlers();
}

