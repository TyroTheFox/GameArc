#pragma once
/**
* \class Input Handler
* \file InputHandler.h
* \author Kieran Clare
* \brief Mediates between Game Engine Core and Event System to fire events on key presses
*
* Interprets key inputs from the Game Engine Core and turns them into Events that are then handled by the Key Event object
*/
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

extern Event* keyEvent;///External Event handler object

/**
* Input Command
*
* Basis for Input Command Objects
*/
class InputCommand
{
public:
	virtual ~InputCommand() {}///Constructor
	bool onKeyUp = false;///Fires only when key is lifted, when true
	bool keyPressed = false;///Key pressed flag
	std::string eventType;///Event class name
	virtual void execute() = 0;///Executed action
};

/**
* Key Input Event
*
* Used for Key Events that send string messages
*/
class KeyInputEvent : public InputCommand
{
public:
	std::string eventName;///Name of Event to call
	std::string message;///Message for event
	KeyInputEvent(std::string eN, std::string m) : eventName(eN), message(m) { eventType = "KeyInputEvent"; }///Constructor
	KeyInputEvent(std::string eN, std::string m, bool onUp) : eventName(eN), message(m) { onKeyUp = onUp; eventType = "KeyInputEvent"; }///Constructor
	void execute() override {///Command to execute on key push
		keyEvent->notifyHandlerWithMessage(eventName, message);
	}
};

/**
* Key Input Int Event
*
* Used for Key Events that send int messages
*/
class KeyInputIntEvent : public InputCommand
{
public:
	std::string eventName;///Name of Event to call
	int message;///Message for event
	KeyInputIntEvent(std::string eN, int m) : eventName(eN) { message = m; eventType = "KeyInputIntEvent"; }///Constructor
	KeyInputIntEvent(std::string eN, int m, bool onUp) : eventName(eN), message(m) { message = m; onKeyUp = onUp; eventType = "KeyInputIntEvent"; }///Constructor
	void execute() override {///Command to execute on key push
		keyEvent->notifyHandlerWithint(eventName, message);
	}
};

/**
* Key Input Func Event
*
* Used for Key Events that don't need messages
*/
class KeyInputFuncEvent : public InputCommand
{
public:
	std::string eventName;///Name of Event to call
	KeyInputFuncEvent(std::string eN) : eventName(eN){ eventType = "KeyInputFuncEvent"; }///Constructor
	KeyInputFuncEvent(std::string eN, bool onUp) : eventName(eN) { onKeyUp = onUp; eventType = "KeyInputFuncEvent"; }///Constructor
	void execute() override {///Command to execute on key push
		keyEvent->notifyHandler(eventName);
	}
};

class InputHandler
{
private:
	std::string mouseEventName = "PlayerMouseXY";///Default Event Name for mouse input

	glm::vec2 oldMouseXY = glm::vec2(0);///Previous ticks mouse position

	bool disableInput = false;///Disable main movement actions flag
public:
	std::map<int, InputCommand*> m_controlMapping;///Normal controls
	std::map<int, InputCommand*> m_debugMapping;///Debug controls that will work regardless of input flag
	InputHandler(){///Constructor, sets up default controls
		m_controlMapping[(int)'W'] = new KeyInputEvent("PlayerMovement", "moveForwards");
		m_controlMapping[(int)'S'] = new KeyInputEvent("PlayerMovement", "moveBackwards");
		m_controlMapping[(int)'A'] = new KeyInputEvent("PlayerMovement", "moveLeft");
		m_controlMapping[(int)'D'] = new KeyInputEvent("PlayerMovement", "moveRight");
		m_controlMapping[(int)'E'] = new KeyInputEvent("PlayerMovement", "switchCamera");

		m_controlMapping[(int)'Q'] = new KeyInputEvent("ChangeScene", "CubeLevel");
		
		setUpDebugControls();
	}
	InputHandler(string inputFile)///Constructor, sets up controls according to file
	{
		loadFromJSON(inputFile);
		setUpDebugControls();
	}

	void setUpDebugControls() {///Sets up debug controls
		KeyInputFuncEvent* keyEvent = new KeyInputFuncEvent("ToggleDebugConsole");
		keyEvent->onKeyUp = true;
		m_debugMapping[(int)'`'] = keyEvent;
		//Debug - Enter
		keyEvent = new KeyInputFuncEvent("DebugEnterHit");
		keyEvent->onKeyUp = true;
		m_debugMapping[257] = keyEvent;
		//Debug - Backspace
		keyEvent = new KeyInputFuncEvent("DebugBackspaceHit");
		keyEvent->onKeyUp = true;
		m_debugMapping[259] = keyEvent;
		//Debug - Right Shift
		KeyInputIntEvent* keyIntEvent = new KeyInputIntEvent("DebugShiftHit", 1);
		m_debugMapping[344] = keyIntEvent;
	}

	void setDisableInput(bool dI) {///Set input flag
		disableInput = dI;
	}

	bool loadFromJSON(string inputFile) {///Loads each input key from file and sets up needed events
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

	void handleInputs(const std::vector<int>& keyBuffer)///Interprets keybuffer and fires events depending on keys pushed
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

	void handleConsoleInput(char c) {///Takes in keyboard input and send through key event
		if (disableInput) {
			if (c != '`') {
				keyEvent->notifyHandlerWithint("DebugConsoleInput", c);
			}
		}
	}

	void handleMouse(const glm::vec2 mouseXY) {///Handles mouse input and sends through event system
		if (mouseXY != oldMouseXY) {
			//Send mouse event
			keyEvent->notifyHandlerWithVec2(mouseEventName, mouseXY);
			//Update old with new
			oldMouseXY = mouseXY;
		}
	}
};
