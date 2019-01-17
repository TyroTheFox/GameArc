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
	bool onKeyUp = false;
	bool keyPressed = false;
	virtual void execute() = 0;
};

class KeyInputEvent : public InputCommand
{
public:
	std::string eventName;
	std::string message;
	KeyInputEvent(std::string eN, std::string m) : eventName(eN), message(m) {}
	KeyInputEvent(std::string eN, std::string m, bool onUp) : eventName(eN), message(m) { onKeyUp = onUp; }
	void execute() override {
		keyEvent->notifyHandlerWithMessage(eventName, message);
	}
};

class KeyInputFuncEvent : public InputCommand
{
public:
	std::string eventName;
	KeyInputFuncEvent(std::string eN) : eventName(eN){}
	KeyInputFuncEvent(std::string eN, bool onUp) : eventName(eN) { onKeyUp = onUp; }
	void execute() override {
		keyEvent->notifyHandler(eventName);
	}
};

class InputHandler
{
private:
	std::map<int, InputCommand*> m_controlMapping;
	std::map<int, InputCommand*> m_debugMapping;

	std::string mouseEventName = "PlayerMouseXY";

	glm::vec2 oldMouseXY = glm::vec2(0);

	bool disableInput = false;
public:
	InputHandler(){
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

		KeyInputFuncEvent* keyEvent = new KeyInputFuncEvent("ToggleDebugConsole");
		keyEvent->onKeyUp = true;
		m_debugMapping[(int)'`'] = keyEvent;
		keyEvent = new KeyInputFuncEvent("DebugEnterHit");
		keyEvent->onKeyUp = true;
		m_debugMapping[257] = keyEvent;
		keyEvent = new KeyInputFuncEvent("DebugBackspaceHit");
		keyEvent->onKeyUp = true;
		m_debugMapping[259] = keyEvent;
	}

	void setDisableInput(bool dI) {
		disableInput = dI;
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
				bool onKeyUp = false;
				if (inputs[i].isMember("OnKeyUp")) {
					onKeyUp = inputs[i]["OnKeyUp"].asBool();
				}
				if (inputs[i].isMember("EventMessage")) {
					string eventMessage = inputs[i]["EventMessage"].asString();
					KeyInputEvent* keyEvent = new KeyInputEvent(eventName, eventMessage);
					keyEvent->onKeyUp = onKeyUp;
					m_controlMapping[(int)key] = keyEvent;
				}
				else {
					KeyInputFuncEvent* keyEvent = new KeyInputFuncEvent(eventName);
					keyEvent->onKeyUp = onKeyUp;
					m_controlMapping[(int)key] = keyEvent;
				}
			}
			catch (...) { std::cout << "Exception thrown loading Inputs from JSON(" << inputFile << "), in parsing Input[" << std::to_string(i) << "]." << std::endl; }
		}
		return true;
	}

	void registerInput(char key, InputCommand* e) {
		m_controlMapping[(int)key] = e;
	}
	void handleInputs(const std::vector<int>& keyBuffer)
	{
		if (!disableInput) {
			for (const auto& mapEntry : m_controlMapping)
			{
				if (keyBuffer[mapEntry.first] == 1 || keyBuffer[mapEntry.first] == 2)
				{
					if (mapEntry.second->onKeyUp) {
						mapEntry.second->keyPressed = true;
					}
					else {
						mapEntry.second->execute();
					}
				}

				if (keyBuffer[mapEntry.first] == 0 && mapEntry.second->keyPressed) {
					//Check for Release (0)
					mapEntry.second->execute();
					mapEntry.second->keyPressed = false;
				}
			}
		}
		for (const auto& mapEntry : m_debugMapping)
		{
			if (keyBuffer[mapEntry.first] == 1 || keyBuffer[mapEntry.first] == 2)
			{
				if (mapEntry.second->onKeyUp) {
					mapEntry.second->keyPressed = true;
				}
				else {
					mapEntry.second->execute();
				}
			}

			if (keyBuffer[mapEntry.first] == 0 && mapEntry.second->keyPressed) {
				//Check for Release (0)
				mapEntry.second->execute();
				mapEntry.second->keyPressed = false;
			}
		}
	}

	void handleConsoleInput(char c) {
		if (disableInput) {
			if (c != '`') {
				keyEvent->notifyHandlerWithint("DebugConsoleInput", c);
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
