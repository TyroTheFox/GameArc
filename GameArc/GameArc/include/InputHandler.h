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

//class DecreaseRotateX : public InputCommand
//{
//public:
//	void execute(GameObject& model) override {
//		if (model.getComponent<TransformComponent>())
//			model.getComponent<TransformComponent>()->OnMessage("decreaseRotateX");
//	}
//};
//
//class IncreaseRotateY : public InputCommand
//{
//public:
//	void execute(GameObject& model) override {
//		if (model.getComponent<TransformComponent>())
//			model.getComponent<TransformComponent>()->OnMessage("increaseRotateY");
//	}
//};
//
//class DecreaseRotateY : public InputCommand
//{
//public:
//	void execute(GameObject& model) override {
//		if (model.getComponent<TransformComponent>())
//			model.getComponent<TransformComponent>()->OnMessage("decreaseRotateY");
//	}
//};
//
//class IncreaseRotateZ : public InputCommand
//{
//public:
//	void execute(GameObject& model) override {
//		if (model.getComponent<TransformComponent>())
//			model.getComponent<TransformComponent>()->OnMessage("increaseRotateZ");
//	}
//};
//
//class DecreaseRotateZ : public InputCommand
//{
//public:
//	void execute(GameObject& model) override {
//		if (model.getComponent<TransformComponent>())
//			model.getComponent<TransformComponent>()->OnMessage("decreaseRotateZ");
//	}
//};
//
//class IncreaseTranslateX : public InputCommand
//{
//public:
//	void execute(GameObject& model) override {
//		if (model.getComponent<TransformComponent>())
//			model.getComponent<TransformComponent>()->OnMessage("increaseTranslateX");
//	}
//};
//
//class DecreaseTranslateX : public InputCommand
//{
//public:
//	void execute(GameObject& model) override {
//		if (model.getComponent<TransformComponent>())
//			model.getComponent<TransformComponent>()->OnMessage("decreaseTranslateX");
//	}
//};
//
//class IncreaseTranslateY : public InputCommand
//{
//public:
//	void execute(GameObject& model) override {
//		if (model.getComponent<TransformComponent>())
//			model.getComponent<TransformComponent>()->OnMessage("increaseTranslateY");
//	}
//};
//
//class DecreaseTranslateY : public InputCommand
//{
//public:
//	void execute(GameObject& model) override {
//		if (model.getComponent<TransformComponent>())
//			model.getComponent<TransformComponent>()->OnMessage("decreaseTranslateY");
//	}
//};
//
//class IncreaseTranslateZ : public InputCommand
//{
//public:
//	void execute(GameObject& model) override {
//		if (model.getComponent<TransformComponent>())
//			model.getComponent<TransformComponent>()->OnMessage("increaseTranslateZ");
//	}
//};
//
//class DecreaseTranslateZ : public InputCommand
//{
//public:
//	void execute(GameObject& model) override {
//		if (model.getComponent<TransformComponent>())
//			model.getComponent<TransformComponent>()->OnMessage("decreaseTranslateZ");
//	}
//};
//
//class IncreaseScaleX : public InputCommand
//{
//public:
//	void execute(GameObject& model) override {
//		if (model.getComponent<TransformComponent>())
//			model.getComponent<TransformComponent>()->OnMessage("increaseScaleX");
//	}
//};
//
//class DecreaseScaleX : public InputCommand
//{
//public:
//	void execute(GameObject& model) override {
//		if (model.getComponent<TransformComponent>())
//			model.getComponent<TransformComponent>()->OnMessage("decreaseScaleX");
//	}
//};
//
//class IncreaseScaleY : public InputCommand
//{
//public:
//	void execute(GameObject& model) override {
//		if (model.getComponent<TransformComponent>())
//			model.getComponent<TransformComponent>()->OnMessage("increaseScaleY");
//	}
//};
//
//class DecreaseScaleY : public InputCommand
//{
//public:
//	void execute(GameObject& model) override {
//		if (model.getComponent<TransformComponent>())
//			model.getComponent<TransformComponent>()->OnMessage("decreaseScaleY");
//	}
//};
//
//class IncreaseScaleZ : public InputCommand
//{
//public:
//	void execute(GameObject& model) override {
//		if (model.getComponent<TransformComponent>())
//			model.getComponent<TransformComponent>()->OnMessage("increaseScaleZ");
//	}
//};
//
//class DecreaseScaleZ : public InputCommand
//{
//public:
//	void execute(GameObject& model) override {
//		if (model.getComponent<TransformComponent>())
//			model.getComponent<TransformComponent>()->OnMessage("decreaseScaleZ");
//	}
//};

struct InputHandler
{
private:
	GameObject* m_playerCube;

	std::map<int, InputCommand*> m_controlMapping;
public:
	InputHandler()
	{
		// the idea will be to map the keys directly from a config file that can be loaded in
		// and changed on the fly
		m_controlMapping[(int)'W'] = new KeyInputEvent("PlayerTransform", "decreaseTranslateZ");
		m_controlMapping[(int)'S'] = new KeyInputEvent("PlayerTransform", "increaseTranslateZ");
		m_controlMapping[(int)'A'] = new KeyInputEvent("PlayerTransform", "increaseRotateY");
		m_controlMapping[(int)'D'] = new KeyInputEvent("PlayerTransform", "decreaseRotateY");
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
};
