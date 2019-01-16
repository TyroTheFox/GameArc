#ifndef _COLOURCOMPONENT_H_
#define _COLOURCOMPONENT_H_
#pragma once
#include "Component.h"
#define COLOUR_ADJUSTMENT_VALUE 0.001f

class ColourComponent : public Component
{
public:
	ColourComponent() : m_colourValue(0) {};
	void OnSetUp() override {	}
	void OnUpdate(float dt) override
	{

	}
	void OnRender(IEngineCore* m_engineInterfacePtr) override {}
	void OnMessage(const std::string m) override
	{
		try {
			if (m == "increase")
			{
				m_colourValue += COLOUR_ADJUSTMENT_VALUE;
			}
			else if (m == "decrease")
			{
				m_colourValue -= COLOUR_ADJUSTMENT_VALUE;
			}
		}
		catch (...) {
			std::cout << "Exception thrown in parsing BuildFromJson in ColourComponent." << std::endl;
			throw;
		}
	}

	void BuildToJson(Json::Value& componentJSON) override {};
	void BuildFromJson(const Json::Value& componentJSON) override {};

	float m_colourValue;
};

class RedComponent : public ColourComponent {};
class GreenComponent : public ColourComponent {};
class BlueComponent : public ColourComponent {};

#endif