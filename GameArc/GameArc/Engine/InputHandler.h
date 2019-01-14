#pragma once
#include "global.h"
#include "EventHandler.h"
#include <map>
#include "GameObject.h"
#include <vector>
#include "TransformComponent.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "json.h"

extern Event* keyEvent;

class InputCommand
{
public:
	virtual ~InputCommand() {}
	virtual void execute() = 0;
};

class KeyInputEvent : public InputCommand
{
public:
	std::string eventName;
	std::string message;
	KeyInputEvent(std::string eN, std::string m) : eventName(eN), message(m) {}
	void execute() override {
		keyEvent->notifyHandlerWithMessage(eventName, message);
	}
};

struct InputHandler
{
private:
	GameObject* m_playerCube;

	std::map<int, InputCommand*> m_controlMapping;

	std::string mouseEventName = "PlayerMouseXY";

	glm::vec2 oldMouseXY = glm::vec2(0);
public:
	InputHandler() {
		m_controlMapping[(int)'W'] = new KeyInputEvent("PlayerMovement", "moveForwards");
		m_controlMapping[(int)'S'] = new KeyInputEvent("PlayerMovement", "moveBackwards");
		m_controlMapping[(int)'A'] = new KeyInputEvent("PlayerMovement", "moveLeft");
		m_controlMapping[(int)'D'] = new KeyInputEvent("PlayerMovement", "moveRight");
		m_controlMapping[(int)'E'] = new KeyInputEvent("PlayerMovement", "switchCamera");

		m_controlMapping[(int)'Q'] = new KeyInputEvent("ChangeScene", "CubeLevel");
	}
	InputHandler(string inputFile)
	{
		loadFromJSON(inputFile);
	}

	bool loadFromJSON(string inputFile) {
		std::ifstream inputstream(inputFile);
		Json::Reader reader;
		Json::Value obj;
		reader.parse(inputstream, obj);

		const Json::Value& inputs = obj["Inputs"];
		if (!inputs) {
			std::cout << "Exception thrown loading Inputs from JSON(" << inputFile << "), no value for Inputs." << std::endl;
			return false;
		}

		for (unsigned int i = 0; i < inputs.size(); i++) {
			try {
				char key = *inputs[i]["Key"].asCString();
				string eventName = inputs[i]["EventName"].asString();
				string eventMessage = inputs[i]["EventMessage"].asString();
				m_controlMapping[(int)key] = new KeyInputEvent(eventName, eventMessage);
			}
			catch (...) { std::cout << "Exception thrown loading Inputs from JSON(" << inputFile << "), in parsing Input[" << std::to_string(i) << "]." << std::endl; }
		}
		return true;
	}

	void registerInput(char key, InputCommand* e) {
		m_controlMapping[(int)key] = e;
	}
	void handleInputs(const std::vector<bool>& keyBuffer)
	{
		for (const auto& mapEntry : m_controlMapping)
		{
			if (keyBuffer[mapEntry.first])
			{
				mapEntry.second->execute();
			}
		}

	}

	void handleMouse(const glm::vec2 mouseXY) {
		if (mouseXY != oldMouseXY) {
			//Send mouse event
			keyEvent->notifyHandlerWithVec2(mouseEventName, mouseXY);
			//Update old with new
			oldMouseXY = mouseXY;
		}
	}
};
