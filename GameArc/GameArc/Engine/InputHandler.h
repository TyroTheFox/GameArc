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
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "json.h"
///External Event handler object
extern Event* keyEvent;

/**
* Input Command
*
* Basis for Input Command Objects
*/
class InputCommand
{
public:
	///Constructor
	virtual ~InputCommand() {}
	///Fires only when key is lifted, when true
	bool onKeyUp = false;
	///Key pressed flag
	bool keyPressed = false;
	///Event class name
	std::string eventType;
	///Executed action
	virtual void execute() = 0;
};

/**
* Key Input Event
*
* Used for Key Events that send string messages
*/
class KeyInputEvent : public InputCommand
{
public:
	///Name of Event to call
	std::string eventName;
	///Message for event
	std::string message;
	///Constructor
	KeyInputEvent(std::string eN, std::string m) : eventName(eN), message(m) { eventType = "KeyInputEvent"; }
	///Constructor
	KeyInputEvent(std::string eN, std::string m, bool onUp) : eventName(eN), message(m) { onKeyUp = onUp; eventType = "KeyInputEvent"; }
	///Command to execute on key push
	void execute() override {
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
	///Name of Event to call
	std::string eventName;
	///Message for event
	int message;
	///Constructor
	KeyInputIntEvent(std::string eN, int m) : eventName(eN) { message = m; eventType = "KeyInputIntEvent"; }
	///Constructor
	KeyInputIntEvent(std::string eN, int m, bool onUp) : eventName(eN), message(m) { message = m; onKeyUp = onUp; eventType = "KeyInputIntEvent"; }
	///Command to execute on key push
	void execute() override {
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
	///Name of Event to call
	std::string eventName;
	///Constructor
	KeyInputFuncEvent(std::string eN) : eventName(eN){ eventType = "KeyInputFuncEvent"; }
	///Constructor
	KeyInputFuncEvent(std::string eN, bool onUp) : eventName(eN) { onKeyUp = onUp; eventType = "KeyInputFuncEvent"; }
	///Command to execute on key push
	void execute() override {
		keyEvent->notifyHandler(eventName);
	}
};

class InputHandler
{
private:
	///Default Event Name for mouse input
	std::string mouseEventName = "PlayerMouseXY";
	///Previous ticks mouse position
	glm::vec2 oldMouseXY = glm::vec2(0);
	///Disable main movement actions flag
	bool disableInput = false;
public:
	///Normal controls
	std::map<int, InputCommand*> m_controlMapping;
	///Debug controls that will work regardless of input flag
	std::map<int, InputCommand*> m_debugMapping;
	///Constructor, sets up default controls
	InputHandler(){
		m_controlMapping[(int)'W'] = new KeyInputEvent("PlayerMovement", "moveForwards");
		m_controlMapping[(int)'S'] = new KeyInputEvent("PlayerMovement", "moveBackwards");
		m_controlMapping[(int)'A'] = new KeyInputEvent("PlayerMovement", "moveLeft");
		m_controlMapping[(int)'D'] = new KeyInputEvent("PlayerMovement", "moveRight");
		m_controlMapping[(int)'E'] = new KeyInputEvent("PlayerMovement", "switchCamera");

		m_controlMapping[(int)'Q'] = new KeyInputEvent("ChangeScene", "CubeLevel");
		
		setUpDebugControls();
	}
	///Constructor, sets up controls according to file
	InputHandler(string inputFile)
	{
		loadFromJSON(inputFile);
		setUpDebugControls();
	}
	///Sets up debug controls
	void setUpDebugControls() {
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
	///Set input flag
	void setDisableInput(bool dI) {
		disableInput = dI;
	}
	///Loads each input key from file and sets up needed events
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
	///Interprets keybuffer and fires events depending on keys pushed
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
	///Takes in keyboard input and send through key event
	void handleConsoleInput(char c) {
		if (disableInput) {
			if (c != '`') {
				keyEvent->notifyHandlerWithint("DebugConsoleInput", c);
			}
		}
	}
	///Handles mouse input and sends through event system
	void handleMouse(const glm::vec2 mouseXY) {
		if (mouseXY != oldMouseXY) {
			//Send mouse event
			keyEvent->notifyHandlerWithVec2(mouseEventName, mouseXY);
			//Update old with new
			oldMouseXY = mouseXY;
		}
	}
};
