#include "Scene.h"

/*Credit to Dynamic JSON loader to Stefan Bauer*/

Scene::Scene()
{
	m_componentJsonBuilders["TransformComponent"] = [this](GameObject* object, const Json::Value& p_component) { attachComponent<TransformComponent>(object, p_component); };
	m_componentJsonBuilders["ModelComponent"] = [this](GameObject* object, const Json::Value& p_component) { attachComponent<ModelComponent>(object, p_component); };
	m_componentJsonBuilders["ColourComponent"] = [this](GameObject* object, const Json::Value& p_component) { attachComponent<ColourComponent>(object, p_component); };
	m_componentJsonBuilders["PlayerComponent"] = [this](GameObject* object, const Json::Value& p_component) { attachComponent<PlayerComponent>(object, p_component); 
		object->getComponent<PlayerComponent>()->setParent(object);
		object->getComponent<PlayerComponent>()->buildEvents();
	};
	m_componentJsonBuilders["EventCameraComponent"] = [this](GameObject* object, const Json::Value& p_component) { attachComponent<EventCameraComponent>(object, p_component);
		object->getComponent<EventCameraComponent>()->setParent(object);
		object->getComponent<EventCameraComponent>()->buildEvents();
	};
}

template<typename T> void Scene::attachComponent(GameObject* object, const Json::Value& p_component) {
	T * component = new T();
	component->BuildFromJson(p_component);
	object->addComponent<T>(component);
}

bool Scene::loadLevelJSON(std::string levelFile)
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

			if (gameObjects[i].isMember("visible")) {
				if (gameObjects[i]["visible"].asBool()) {
					m_displayCubes.push_back(objectName);
				}
			}

			m_gameObjects[objectName] = new GameObject();

			const Json::Value& components = gameObjects[i]["components"];
			for (unsigned int j = 0; j < components.size(); j++) {
				try {
					std::string className = components[j]["class"].asString();

					m_componentJsonBuilders[className](m_gameObjects[objectName], components[j]);
				}
				catch (...) { std::cout << "Exception thrown in loadLevelFromJson(" << levelFile << "), in parsing GameObject[" << std::to_string(i) << "], in parsing components[" << std::to_string(j) << "]." << std::endl; }
			}
		}
		catch (...) { std::cout << "Exception thrown in loadLevelFromJson(" << levelFile << "), in parsing GameObject[" << std::to_string(i) << "]." << std::endl; }
	}
	return true;
}

std::map<std::string, GameObject*> Scene::getGameObjects()
{
	return m_gameObjects;
}