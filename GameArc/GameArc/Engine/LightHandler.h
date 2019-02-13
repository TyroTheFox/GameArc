#pragma once

#include "Light.h"
#include <vector>

class LightHandler {
private:
	int lightCount;
	int directionalLightCount;
	int pointLightCount;
	int spotLightCount;
	int lightMax = 8;
	int maxDirectional = 1;
	std::vector<Light*> lights;
	void setUpLight(Light* l);
public:
	///Creates Directional Light
	LightHandler();
	///Creates Directional Light
	Light* createNewLight(LightColour lC);
	///Creates Directional Light
	Light* createNewLight(LightColour lC, glm::vec3 o);
	///Creates Directional Light
	Light* createNewLight(LightColour lC, glm::vec3 p, glm::vec3 o);

	///Creates Point Light
	Light* createNewLight(PointLightData pLD, LightColour lC);
	///Creates Point Light
	Light* createNewLight(PointLightData pLD, LightColour lC, glm::vec3 p);

	///Creates Spot Light
	Light* createNewLight(SpotLightData sLD, LightColour lC);
	///Creates Spot Light
	Light* createNewLight(SpotLightData sLD, LightColour lC, glm::vec3 p);
	///Creates Spot Light
	Light* createNewLight(SpotLightData sLD, LightColour lC, glm::vec3 p, glm::vec3 o);

	void addLight(Light& light);
	vector<Light*> getLights();
	const int getDirectionalLightCount();
	const int getPointLightCount();
	const int getSpotLightCount();
};