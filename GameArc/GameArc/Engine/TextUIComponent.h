#pragma once
#include "GameObject.h"
#include "Component.h"
#include "TextWriter.h"

class TextUIComponent : public Component
{
private:
	TextWriter* textWriter;
	glm::vec2 position;
	std::string text;
	float scale;
public:
	TextUIComponent() : scale(1.0f), text("") { textWriter = new TextWriter(); }
	void OnSetUp() override {	}
	void SetText(std::string t) { text = t;	}
	std::string GetText() { return text; }
	void SetPosition(float x, float y) { position = glm::vec2(x, y); }
	void SetPosition(glm::vec2 xy) { position = xy; }
	glm::vec2 GetPosition() { return position; }
	void SetScale(float s) { scale = s;	}
	float GetScale() { return scale; }
	void OnUpdate(float dt) override {}
	void OnRender(IEngineCore* m_engineInterfacePtr) override {
		if (textWriter->m_engineInterfacePtr == nullptr) {
			textWriter->SetEnginePtr(m_engineInterfacePtr);
		}
		textWriter->DrawNormalText(text, position.x, position.y, scale); 
	}
	void OnMessage(const std::string m) override {}
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
	void BuildToJson(Json::Value& componentJSON) override {}
};