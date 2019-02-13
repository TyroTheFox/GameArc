#include "../Engine/LightHandler.h"

LightHandler::LightHandler()
{
}

void LightHandler::setUpLight(Light * l)
{
	if (l->lType() == Light::DIRECTIONAL) {
		l->ID = directionalLightCount;
		directionalLightCount++;
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

Light* LightHandler::createNewLight(LightColour lC)
{
	if (spotLightCount > maxDirectional) { return nullptr; }
	Light* l = new Light(lC);
	setUpLight(l);
	return l;
}

Light* LightHandler::createNewLight(LightColour lC, glm::vec3 o)
{
	if (spotLightCount > maxDirectional) { return nullptr; }
	Light* l = new Light(lC, o);
	setUpLight(l);
	return l;
}

Light* LightHandler::createNewLight(LightColour lC, glm::vec3 p, glm::vec3 o)
{
	if (spotLightCount > maxDirectional) { return nullptr; }
	Light* l = new Light(lC, p, o);
	setUpLight(l);
	return l;
}

Light* LightHandler::createNewLight(PointLightData pLD, LightColour lC)
{
	if (pointLightCount > lightMax) { return nullptr; }
	Light* l = new Light(pLD, lC);
	setUpLight(l);
	return l;
}

Light* LightHandler::createNewLight(PointLightData pLD, LightColour lC, glm::vec3 p)
{
	if (pointLightCount > lightMax) { return nullptr; }
	Light* l = new Light(pLD, lC, p);
	setUpLight(l);
	return l;
}

Light* LightHandler::createNewLight(SpotLightData sLD, LightColour lC)
{
	if (spotLightCount > lightMax) { return nullptr; }
	Light* l = new Light(sLD, lC);
	setUpLight(l);
	return l;
}

Light* LightHandler::createNewLight(SpotLightData sLD, LightColour lC, glm::vec3 p)
{
	if (spotLightCount > lightMax) { return nullptr; }
	Light* l = new Light(sLD, lC, p);
	setUpLight(l);
	return l;
}

Light* LightHandler::createNewLight(SpotLightData sLD, LightColour lC, glm::vec3 p, glm::vec3 o)
{
	if (spotLightCount > lightMax) { return nullptr; }
	Light* l = new Light(sLD, lC, p, o);
	setUpLight(l);
	return l;
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

const int LightHandler::getDirectionalLightCount()
{
	return directionalLightCount;
}

const int LightHandler::getPointLightCount()
{
	return pointLightCount;
}

const int LightHandler::getSpotLightCount()
{
	return spotLightCount;
}

