#include "Scene.h"

/*Credit to Dynamic JSON loader to Stefan Bauer*/

Scene::Scene()
{
	m_componentJsonBuilders["TransformComponent"] = [this](GameObject* object, const Json::Value& p_component) { attachComponent<TransformComponent>(object, p_component); };
	m_componentJsonBuilders["ModelComponent"] = [this](GameObject* object, const Json::Value& p_component) { attachComponent<ModelComponent>(object, p_component); };
	m_componentJsonBuilders["ColourComponent"] = [this](GameObject* object, const Json::Value& p_component) { attachComponent<ColourComponent>(object, p_component); };
}

void Scene::loadLevelTxt(std::string levelFile)
{
	//If not, we read in the OBJ file
	//This is far, far slower than Binaries
	fstream levelData(levelFile, ios_base::in);
	//If the OBJ file exists
	if (levelData.is_open()) {
		//Read in and process each line till there's nothing else in the file
		string s;
		// read the first line which has the number of elements
		std::getline(levelData, s);
		ss.str(s);
		ss.ignore(17); // ignore the first 20 chars
		ss >> numElementsToRead;
		// resize the correct size for the vector
		translations.resize(numElementsToRead);
		rotations.resize(numElementsToRead);
		scalars.resize(numElementsToRead);
		ss.clear();
		for (int i = 0; i < numElementsToRead; i++)
		{
			// use a stringstream to get integer values
			getline(levelData, s);
			//Translation
			float x, y, z, w;

			getline(levelData, s);
			ss.clear();
			ss.str(s);
			ss >> x >> y >> z;
			glm::vec3 translation(x, y, z);
			translations[i] = translation;

			//Rotation
			getline(levelData, s);
			ss.clear();
			ss.str(s);
			ss >> x >> y >> z >> w;
			glm::quat rotation(x, y, z, w);
			rotations[i] = rotation;

			//Scalar
			getline(levelData, s);
			ss.clear();
			ss.str(s);
			ss >> x >> y >> z;
			glm::vec3 scale(x, y, z);
			scalars[i] = scale;
		}
		levelData.close();

		//buildLevel();
	}
}



/*void Scene::loadLevelJSON(std::string levelFile)
{
	std::fstream jsonData;
	Json::Value root;
	Json::Reader reader;
	jsonData.open(levelFile.c_str());
	// check for errors
	if (!reader.parse(jsonData, root))
	{
		std::cout << "Failed to parse data from: "
			<< levelFile
			<< reader.getFormattedErrorMessages();
	}
	else {
		const Json::Value gameObjects = root["GameObjects"];
		// size() tells us how large the array is
		numElementsToRead = gameObjects.size();
		translations.resize(numElementsToRead);
		rotations.resize(numElementsToRead);
		scalars.resize(numElementsToRead);
		for (int i = 0; i < (int)gameObjects.size(); i++)
		{
			// get string
			std::cout << gameObjects[i]["name"].asString() << " loaded\n";
			float x, y, z, w;
			// get the position node
			const Json::Value posNode = gameObjects[i]["position"];
			if (posNode.type() != Json::nullValue) {
				x = posNode[0].asFloat();
				y = posNode[1].asFloat();
				z = posNode[2].asFloat();
				glm::vec3 pos(x, y, z);
				translations[i] = pos;
			}
			else {
				translations[i] = glm::vec3(0, 0, -5);
			}

			const Json::Value rotNode = gameObjects[i]["rotation"];
			if (rotNode.type() != Json::nullValue) {
				x = rotNode[0].asFloat(); // get float
				y = rotNode[1].asFloat();
				z = rotNode[2].asFloat();
				w = rotNode[3].asFloat();
				glm::quat rot(x, y, z, w);
				rotations[i] = rot;
			}
			else {
				rotations[i] = glm::quat(1, 0, 0, 0);
			}

			const Json::Value scaNode = gameObjects[i]["scale"];
			if (scaNode.type() != Json::nullValue) {
				x = scaNode[0].asFloat();
				y = scaNode[1].asFloat();
				z = scaNode[2].asFloat();
				glm::vec3 sca(x, y, z);
				scalars[i] = sca;
			}
			else{
				scalars[i] = glm::vec3(1, 1, 1);
			}
		}
	}
	buildLevel();
}*/

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

	for (int i = 0; i < gameObjects.size(); i++) {
		try {
			std::string objectName = gameObjects[i]["name"].asString();

			if (gameObjects[i].isMember("visible")) {
				if (gameObjects[i]["visible"].asBool()) {
					m_displayCubes.push_back(objectName);
				}
			}

			m_gameObjects[objectName] = new GameObject();

			const Json::Value& components = gameObjects[i]["components"];
			for (int j = 0; j < components.size(); j++) {
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

//vector<InputHandler*> Scene::getInputHandlers()
//{
//	return inputHandlers;
//}

/*void Scene::buildLevel()
{

	Model* myTestModel = new Model("assets/models/testObj.obj");
	for (int i = 0; i < numElementsToRead; i++) {
		GameObject* m_playerCube = new GameObject();
		m_playerBackground.addComponent(new RedComponent);
		m_playerBackground.addComponent(new GreenComponent);
		m_playerBackground.addComponent(new BlueComponent);

		m_playerCube->addComponent(new TransformComponent);

		m_playerCube->addComponent(new ModelComponent(myTestModel));

		// move the cube
		TransformComponent* temp = m_playerCube->getComponent<TransformComponent>();
		temp->m_position = translations[i];
		temp->m_orientation = rotations[i];
		temp->m_scale = scalars[i];
		//m_gameObjects.push_back(m_playerCube);
		inputHandlers.push_back(new InputHandler(m_playerCube));
	}
}*/
