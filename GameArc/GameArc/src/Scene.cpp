#include "Scene.h"

/*Credit to Dynamic JSON loader to Stefan Bauer*/

Scene::Scene(ObjectManager* oM)
{
	objectManager = oM;
	ID = objectManager->getNewID();
	//m_componentJsonBuilders["TransformComponent"] = [this](GameObject* object, const Json::Value& p_component) { attachComponent<TransformComponent>(object, p_component); };
	//m_componentJsonBuilders["ModelComponent"] = [this](GameObject* object, const Json::Value& p_component) { attachComponent<ModelComponent>(object, p_component); };
	//m_componentJsonBuilders["ColourComponent"] = [this](GameObject* object, const Json::Value& p_component) { attachComponent<ColourComponent>(object, p_component); };
	//m_componentJsonBuilders["PlayerComponent"] = [this](GameObject* object, const Json::Value& p_component) { 
	//	attachComponent<PlayerComponent>(object, p_component); 
	//};
	//m_componentJsonBuilders["EventCameraComponent"] = [this](GameObject* object, const Json::Value& p_component) { 
	//	attachComponent<EventCameraComponent>(object, p_component);
	//	object->getComponent<EventCameraComponent>()->buildEvents();
	//};
	//m_componentJsonBuilders["TextUIComponent"] = [this](GameObject* object, const Json::Value& p_component) { attachComponent<TextUIComponent>(object, p_component); };
}

Scene::Scene(DebugHelper* d, ObjectManager* oM)
{
	debug = d;
	objectManager = oM;
	ID = objectManager->getNewID();
	//m_componentJsonBuilders["TransformComponent"] = [this](GameObject* object, const Json::Value& p_component) { attachComponent<TransformComponent>(object, p_component); };
	//m_componentJsonBuilders["ModelComponent"] = [this](GameObject* object, const Json::Value& p_component) { attachComponent<ModelComponent>(object, p_component); };
	//m_componentJsonBuilders["ColourComponent"] = [this](GameObject* object, const Json::Value& p_component) { attachComponent<ColourComponent>(object, p_component); };
	//m_componentJsonBuilders["PlayerComponent"] = [this](GameObject* object, const Json::Value& p_component) {
	//	attachComponent<PlayerComponent>(object, p_component);
	//};
	//m_componentJsonBuilders["EventCameraComponent"] = [this](GameObject* object, const Json::Value& p_component) {
	//	attachComponent<EventCameraComponent>(object, p_component);
	//	object->getComponent<EventCameraComponent>()->buildEvents();
	//};
	//m_componentJsonBuilders["TextUIComponent"] = [this](GameObject* object, const Json::Value& p_component) { attachComponent<TextUIComponent>(object, p_component); };

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

template<typename T> void Scene::attachComponent(GameObject* object, const Json::Value& p_component) {
	T * component = new T();
	component->debug = debug;
	component->BuildFromJson(p_component);
	object->addComponent<T>(component);
}

bool Scene::loadLevelJSON(std::string levelFile)
{
	if (objectManager->loadLevelJSON(levelFile, ID)) {
		m_gameObjects = objectManager->getGameObjects(ID);
		return true;
	}
	return false;
	/*std::ifstream inputstream(levelFile);
	Json::Reader reader;
	Json::Value obj;
	reader.parse(inputstream, obj);

	const Json::Value& gameObjects = obj["GameObjects"];
	if (!gameObjects) {
		std::cout << "Exception thrown in loadLevelFromJson(" << levelFile << "), no value for GameObjects." << std::endl;
		return false;
	}

	for (unsigned int i = 0; i < gameObjects.size(); i++) {
		try {
			std::string objectName = gameObjects[i]["name"].asString();

			m_gameObjects[objectName] = new GameObject();
			m_gameObjects[objectName]->name = objectName;
			const Json::Value& components = gameObjects[i]["components"];
			for (unsigned int j = 0; j < components.size(); j++) {
				try {
					std::string className = components[j]["class"].asString();

					m_componentJsonBuilders[className](m_gameObjects[objectName], components[j]);
				}
				catch (...) {
					std::cout << "Exception thrown in loadLevelFromJson(" << levelFile << "), in parsing GameObject[" << std::to_string(i) << "], in parsing components[" << std::to_string(j) << "]." << std::endl;
				}
			}
		}
		catch (...) { std::cout << "Exception thrown in loadLevelFromJson(" << levelFile << "), in parsing GameObject[" << std::to_string(i) << "]." << std::endl; }
	}
	return true;*/
}

std::map<std::string, GameObject*> Scene::getGameObjects()
{
	return m_gameObjects;
}

void Scene::setGameObjects(std::map<std::string, GameObject*> go) {
	m_gameObjects = go;
}