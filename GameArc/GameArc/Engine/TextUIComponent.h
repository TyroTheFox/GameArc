#pragma once
/**
* \class Text UI Component
* \file TextUIComponent.h
* \author Kieran Clare
* \brief Draws text to the screen
*
* Draws 2D text to the screen when attached to an object
*/
#include "GameObject.h"
#include "Component.h"
#include "TextWriter.h"

class TextUIComponent : public Component
{
private:
	TextWriter* textWriter;///2D text writing object
	glm::vec2 position;///Position on screen
	std::string text;///Text to display
	float scale;///Font size
public:
	TextUIComponent() : scale(1.0f), text("") { textWriter = new TextWriter(); }///Constructor
	void OnSetUp() override {	///Called when attached to object, sets up Debug functions
		debug->AddConsoleCommand("changetext", TextParser::InterpFunc([this](std::vector<std::string> s) {
			if (s.size() <= 0) { this->debug->WriteToConsole("Missing Values"); return; }
			if (this->parent->name == s.at(0)) {
				if (s.size() > 1) {
					this->SetText(s.at(1));
					this->debug->WriteToConsole("Set Text " + s.at(0) + " changed to " + s.at(1));
				}
				else {
					if (s.size() <= 0) { this->debug->WriteToConsole("Missing Text"); }
				}
			}
		}));
	}
	void SetText(std::string t) { text = t;	}///Sets drawn text
	std::string GetText() { return text; }///Returns drawn text
	void SetPosition(float x, float y) { position = glm::vec2(x, y); }///Set by variables
	void SetPosition(glm::vec2 xy) { position = xy; }///Set by vector
	glm::vec2 GetPosition() { return position; }///Return vector
	void SetScale(float s) { scale = s;	}///Set text size
	float GetScale() { return scale; }///Get text size
	void OnUpdate(float dt) override {}///Called on update tick, doesn't do anything
	void OnRender(IEngineCore* m_engineInterfacePtr) override {///Called on render call, draws text to screen
		if (textWriter->m_engineInterfacePtr == nullptr) {
			textWriter->SetEnginePtr(m_engineInterfacePtr);
		}
		textWriter->DrawNormalText(text, position.x, position.y, scale); 
	}
	void OnMessage(const std::string m) override {}///Does largely nothing currently
	void BuildFromJson(const Json::Value& componentJSON) override {	///Builds component from JSON values
		try {
			if (componentJSON.isMember("position")) {
				const Json::Value& p = componentJSON["position"];
				position.x = p[0].asFloat();
				position.y = p[1].asFloat();
			}
			else {
				position.x = 0;
				position.y = 0;
			}
			if (componentJSON.isMember("text")) {
				const Json::Value& t = componentJSON["text"];
				text = t.asString();
			}
			if (componentJSON.isMember("scale")) {
				const Json::Value& s = componentJSON["scale"];
				scale = s.asFloat();
			}
		}
		catch (...) {
			std::cout << "Exception thrown in parsing BuildFromJson in PlayerComponent." << std::endl;
			throw;
		}
	}
};