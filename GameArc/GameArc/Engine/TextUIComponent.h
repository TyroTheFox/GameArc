#pragma once

#include "GameObject.h"
#include "Component.h"
#include "TextWriter.h"
/**
\file TextUIComponent.h
\author Kieran Clare
\brief Draws text to the screen

Draws 2D text to the screen when attached to an object
*/
class TextUIComponent : public Component
{
private:
	///2D text writing object
	TextWriter* textWriter;
	///Position on screen
	glm::vec2 position;
	///Text to display
	std::string text;
	///Font size
	float scale;
public:
	///Constructor
	TextUIComponent() : scale(1.0f), text("") { textWriter = new TextWriter(); }
	///Called when attached to object, sets up Debug functions
	void OnSetUp() override {	
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
	///Sets drawn text
	void SetText(std::string t) { text = t;	}
	///Returns drawn text
	std::string GetText() { return text; }
	///Set by variables
	void SetPosition(float x, float y) { position = glm::vec2(x, y); }
	///Set by vector
	void SetPosition(glm::vec2 xy) { position = xy; }
	///Return vector
	glm::vec2 GetPosition() { return position; }
	///Set text size
	void SetScale(float s) { scale = s;	}
	///Get text size
	float GetScale() { return scale; }
	///Called on update tick, doesn't do anything
	void OnUpdate(float dt) override {}
	///Called on render call, draws text to screen
	void OnRender(IEngineCore* m_engineInterfacePtr) override {
		textWriter->SetEnginePtr(m_engineInterfacePtr);
		if (textWriter->m_engineInterfacePtr == nullptr) {
			textWriter->SetEnginePtr(m_engineInterfacePtr);
		}
		textWriter->DrawNormalText(text, position.x, position.y, scale); 
	}
	///Does largely nothing currently
	void OnMessage(const std::string m) override {}
	///Builds component from JSON values
	void BuildFromJson(const Json::Value& componentJSON) override {	
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