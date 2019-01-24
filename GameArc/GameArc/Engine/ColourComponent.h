#pragma once

#include "Component.h"
///Default colour adjustment rate
#define COLOUR_ADJUSTMENT_VALUE 0.001f
/**
\file ColourComponent.h
\author Kieran Clare
\brief Used to store and alter an RGB value

Used to store and allow for changes in RBG values
*/
class ColourComponent : public Component
{
public:
	///Constructor
	ColourComponent() : m_red(0), m_blue(0), m_green(0) {};
	///Called when added to object, sets up debug function
	void OnSetUp() override {
		debug->AddConsoleCommand("setColour", TextParser::InterpFunc([this](std::vector<std::string> s) {
			if (s.size() <= 0) { this->debug->WriteErrorToConsole("Missing value"); return; }
			std::string returnMessage = "Colour " + s.at(0);
			if (this->parent->name == s.at(0)) {
				if (s.size() > 1) { this->m_red = std::stof(s.at(1)); returnMessage.append(" set to " + s.at(1)); }
				if (s.size() > 2) { this->m_green = std::stof(s.at(2)); returnMessage.append(", " + s.at(2)); }
				if (s.size() > 3) { this->m_blue = std::stof(s.at(3)); returnMessage.append(", " + s.at(3)); }
				if (s.size() > 4) { this->m_alpha = std::stof(s.at(4)); returnMessage.append(", " + s.at(4)); }
				this->debug->WriteToConsole(returnMessage);
			}
		}));
	}
	///Called on update tick
	void OnUpdate(float dt) override
	{

	}
	///Called on render tick
	void OnRender(IEngineCore* m_engineInterfacePtr) override {}
	///Called on event call
	void OnMessage(const std::string m) override
	{
		if (m == "increaseRed")
		{
			m_red += COLOUR_ADJUSTMENT_VALUE;
		}
		else if (m == "decreaseRed")
		{
			m_red -= COLOUR_ADJUSTMENT_VALUE;
		}

		if (m == "increaseBlue")
		{
			m_blue += COLOUR_ADJUSTMENT_VALUE;
		}
		else if (m == "decreaseBlue")
		{
			m_blue -= COLOUR_ADJUSTMENT_VALUE;
		}

		if (m == "increaseGreen")
		{
			m_green += COLOUR_ADJUSTMENT_VALUE;
		}
		else if (m == "decreaseGreen")
		{
			m_green -= COLOUR_ADJUSTMENT_VALUE;
		}

		if (m == "increaseAlpha")
		{
			m_green += COLOUR_ADJUSTMENT_VALUE;
		}
		else if (m == "decreaseAlpha")
		{
			m_green -= COLOUR_ADJUSTMENT_VALUE;
		}
	}
	///Builds component from JSON values
	void BuildFromJson(const Json::Value& componentJSON) override {
		try {
			if (componentJSON.isMember("rgba")) {
				const Json::Value& colour = componentJSON["rgba"];
				m_red = colour[0].asFloat();
				m_blue = colour[1].asFloat();
				m_green = colour[2].asFloat();
				m_alpha = colour[3].asFloat();
			}
			else {
				m_alpha = 1;
				m_red = 0;
				m_blue = 0;
				m_green = 0;
			}
		}
		catch (...) {
			std::cout << "Exception thrown in parsing BuildFromJson in TransformComponent." << std::endl;
			throw;
		}
	};
	///amount of red colour
	float m_red;
	///amount of blue colour
	float m_blue;
	///amount of green colour
	float m_green;
	///amount of alpha
	float m_alpha;
};