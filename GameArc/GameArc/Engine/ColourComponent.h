#ifndef _COLOURCOMPONENT_H_
#define _COLOURCOMPONENT_H_
#pragma once
#include "Component.h"
#define COLOUR_ADJUSTMENT_VALUE 0.001f

class ColourComponent : public Component
{
public:
	ColourComponent() : m_colourValue(0) {};

	void OnUpdate(float dt) override
	{

	}

	void OnMessage(const std::string m) override
	{
		if (m == "increase")
		{
			m_colourValue += COLOUR_ADJUSTMENT_VALUE;
		}
		else if (m == "decrease")
		{
			m_colourValue -= COLOUR_ADJUSTMENT_VALUE;
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