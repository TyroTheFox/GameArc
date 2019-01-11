#pragma once
#include "global.h"
#include "EventHandler.h"
#include <map>
#include "GameObject.h"
#include <vector>
#include "TransformComponent.h"
#include <string>

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
	InputHandler()
	{
		// the idea will be to map the keys directly from a config file that can be loaded in
		// and changed on the fly
		m_controlMapping[(int)'W'] = new KeyInputEvent("PlayerMovement", "moveForwards");
		m_controlMapping[(int)'S'] = new KeyInputEvent("PlayerMovement", "moveBackwards");
		m_controlMapping[(int)'A'] = new KeyInputEvent("PlayerMovement", "moveLeft");
		m_controlMapping[(int)'D'] = new KeyInputEvent("PlayerMovement", "moveRight");
		m_controlMapping[(int)'E'] = new KeyInputEvent("PlayerMovement", "switchCamera");
		/*m_controlMapping[(int)'W'] = new DecreaseRotateX;
		m_controlMapping[(int)'D'] = new IncreaseRotateY;
		m_controlMapping[(int)'A'] = new DecreaseRotateY;
		m_controlMapping[(int)'Q'] = new IncreaseRotateZ;
		m_controlMapping[(int)'E'] = new DecreaseRotateZ;

		m_controlMapping[(int)'H'] = new IncreaseTranslateX;
		m_controlMapping[(int)'F'] = new DecreaseTranslateX;
		m_controlMapping[(int)'T'] = new IncreaseTranslateY;
		m_controlMapping[(int)'G'] = new DecreaseTranslateY;
		m_controlMapping[(int)'R'] = new IncreaseTranslateZ;
		m_controlMapping[(int)'Y'] = new DecreaseTranslateZ;*/

		/*m_controlMapping[0x25] = new IncreaseTranslateX;
		m_controlMapping[0x27] = new DecreaseTranslateX;
		m_controlMapping[0x21] = new IncreaseTranslateY;
		m_controlMapping[0x22] = new DecreaseTranslateY;
		m_controlMapping[0x26] = new IncreaseTranslateZ;
		m_controlMapping[0x28] = new DecreaseTranslateZ;*/

		/*	m_controlMapping[(int)'J'] = new IncreaseScaleX;
		m_controlMapping[(int)'L'] = new DecreaseScaleX;
		m_controlMapping[(int)'I'] = new IncreaseScaleY;
		m_controlMapping[(int)'K'] = new DecreaseScaleY;
		m_controlMapping[(int)'U'] = new IncreaseScaleZ;
		m_controlMapping[(int)'O'] = new DecreaseScaleZ;*/
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
