#include "Scene.h"

Scene::Scene(ObjectManager* oM)
{
	objectManager = oM;
	ID = objectManager->getNewID();
}

Scene::Scene(DebugHelper* d, ObjectManager* oM)
{
	debug = d;
	objectManager = oM;
	ID = objectManager->getNewID();

	debug->AddConsoleCommand("delete", TextParser::InterpFunc([this](std::vector<std::string> s) {
		if (s.size() <= 0) { this->debug->WriteToConsole("Missing value"); return; }
		std::string name = s.at(0);
		std::map<std::string, GameObject*>::iterator it;
		std::map<std::string, GameObject*> objects = this->getGameObjects();
		it = objects.find(name);
		if (it != objects.end()) {
			objects.erase(name);
			this->setGameObjects(objects);
			this->debug->WriteToConsole("Deleted " + name);
		}
		else {
			this->debug->WriteToConsole("Cannot find " + name);
		}
	}));
}

Scene::~Scene() {
}

bool Scene::loadLevelJSON(std::string levelFile)
{
	if (objectManager->loadLevelJSON(levelFile, ID)) {
		m_gameObjects = objectManager->getGameObjects(ID);
		return true;
	}
	return false;
}

std::map<std::string, GameObject*> Scene::getGameObjects()
{
	return m_gameObjects;
}

void Scene::setGameObjects(std::map<std::string, GameObject*> go) {
	m_gameObjects = go;
}