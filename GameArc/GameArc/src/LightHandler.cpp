#include "../Engine/LightHandler.h"

LightHandler::LightHandler()
{
}

void LightHandler::setUpLight(Light * l)
{
	if (l->lType() == Light::DIRECTIONAL) {
		l->ID = 10;
	}
	if (l->lType() == Light::POINT) {
		l->ID = pointLightCount;
		pointLightCount++;
	}
	if (l->lType() == Light::SPOT) {
		l->ID = spotLightCount;
		spotLightCount++;
	}
	lights.push_back(l);
	lightCount++;
}

void LightHandler::createNewLight(Light::LightType lT, LightColour lC)
{
	if (pointLightCount > lightMax && lT == Light::POINT) { return; }
	if (spotLightCount > lightMax && lT == Light::SPOT) { return; }
	Light* l = new Light(lT, lC);
	setUpLight(l);
}

void LightHandler::createNewLight(Light::LightType lT, LightColour lC, glm::vec3 p)
{
	if (pointLightCount > lightMax && lT == Light::POINT) { return; }
	if (spotLightCount > lightMax && lT == Light::SPOT) { return; }
	Light* l = new Light(lT, lC, p);
	setUpLight(l);
}

void LightHandler::createNewLight(Light::LightType lT, LightColour lC, glm::vec3 p, glm::vec3 o)
{
	if (pointLightCount > lightMax && lT == Light::POINT) { return; }
	if (spotLightCount > lightMax && lT == Light::SPOT) { return; }
	Light* l = new Light(lT, lC, p, o);
	setUpLight(l);
}

void LightHandler::createNewLight(PointLightData pLD, LightColour lC)
{
	if (pointLightCount > lightMax) { return; }
	Light* l = new Light(pLD, lC);
	setUpLight(l);
}

void LightHandler::createNewLight(PointLightData pLD, LightColour lC, glm::vec3 p)
{
	if (pointLightCount > lightMax) { return; }
	Light* l = new Light(pLD, lC, p);
	setUpLight(l);
}

void LightHandler::createNewLight(PointLightData pLD, LightColour lC, glm::vec3 p, glm::vec3 o)
{
	if (pointLightCount > lightMax) { return; }
	Light* l = new Light(pLD, lC, p, o);
	setUpLight(l);
}

void LightHandler::createNewLight(SpotLightData sLD, LightColour lC)
{
	if (spotLightCount > lightMax) { return; }
	Light* l = new Light(sLD, lC);
	setUpLight(l);
}

void LightHandler::createNewLight(SpotLightData sLD, LightColour lC, glm::vec3 p)
{
	if (spotLightCount > lightMax) { return; }
	Light* l = new Light(sLD, lC, p);
	setUpLight(l);
}

void LightHandler::createNewLight(SpotLightData sLD, LightColour lC, glm::vec3 p, glm::vec3 o)
{
	if (spotLightCount > lightMax) { return; }
	Light* l = new Light(sLD, lC, p, o);
	setUpLight(l);
}

void LightHandler::addLight(Light& light)
{
	if (pointLightCount > lightMax && light.lType() == Light::POINT) { return; }
	if (spotLightCount > lightMax && light.lType() == Light::SPOT) { return; }
	setUpLight(&light);
}

vector<Light*> LightHandler::getLights()
{
	return lights;
}

const int LightHandler::getPointLightCount()
{
	return pointLightCount;
}

const int LightHandler::getSpotLightCount()
{
	return spotLightCount;
}

