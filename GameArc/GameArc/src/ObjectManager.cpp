#include "../Engine/ObjectManager.h"

ObjectManager::ObjectManager(DebugHelper* d)
{
	debug = d;
	m_componentJsonBuilders["TransformComponent"] = [this](GameObject* object, const Json::Value& p_component) { attachComponent<TransformComponent>(object, p_component); };
	m_componentJsonBuilders["ModelComponent"] = [this](GameObject* object, const Json::Value& p_component) { attachComponent<ModelComponent>(object, p_component); };
	m_componentJsonBuilders["ColourComponent"] = [this](GameObject* object, const Json::Value& p_component) { attachComponent<ColourComponent>(object, p_component); };
	m_componentJsonBuilders["PlayerComponent"] = [this](GameObject* object, const Json::Value& p_component) {
		attachComponent<PlayerComponent>(object, p_component);
	};
	m_componentJsonBuilders["EventCameraComponent"] = [this](GameObject* object, const Json::Value& p_component) {
		attachComponent<EventCameraComponent>(object, p_component);
		object->getComponent<EventCameraComponent>()->buildEvents();
	};
	m_componentJsonBuilders["TextUIComponent"] = [this](GameObject* object, const Json::Value& p_component) { attachComponent<TextUIComponent>(object, p_component); };
}

int ObjectManager::getNewID() {
	int temp = latestID;
	latestID++;
	return temp;
}

bool ObjectManager::loadLevelJSON(std::string levelFile, int id)
{
	std::ifstream inputstream(levelFile);
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
			sceneIDRegister.insert(std::pair<int, std::string>(id, objectName));;
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
	return true;
}

template<typename T> void ObjectManager::attachComponent(GameObject* object, const Json::Value& p_component) {
	T * component = new T();
	component->debug = debug;
	component->BuildFromJson(p_component);
	object->addComponent<T>(component);
}

std::map<std::string, GameObject*> ObjectManager::getGameObjects(int id)
{
	std::map<std::string, GameObject*> temp; 
	for (auto& object : sceneIDRegister) {
		if (object.first == id) {
			temp[object.second] = m_gameObjects[object.second];
		}
	}
	return temp;
}