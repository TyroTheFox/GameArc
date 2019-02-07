#pragma once

#include "Light.h"
#include <vector>

class LightHandler {
private:
	int lightCount;
	int directionalLightCount;
	int pointLightCount;
	int spotLightCount;
	int lightMax = 16;
	std::vector<Light*> lights;
	void setUpLight(Light* l);
public:
	///Creates Directional Light
	LightHandler();
	///Creates Directional Light
	void createNewLight(LightColour lC);
	///Creates Directional Light
	void createNewLight(LightColour lC, glm::vec3 o);
	///Creates Directional Light
	void createNewLight(LightColour lC, glm::vec3 p, glm::vec3 o);

	///Creates Point Light
	void createNewLight(PointLightData pLD, LightColour lC);
	///Creates Point Light
	void createNewLight(PointLightData pLD, LightColour lC, glm::vec3 p);

	///Creates Spot Light
	void createNewLight(SpotLightData sLD, LightColour lC);
	///Creates Spot Light
	void createNewLight(SpotLightData sLD, LightColour lC, glm::vec3 p);
	///Creates Spot Light
	void createNewLight(SpotLightData sLD, LightColour lC, glm::vec3 p, glm::vec3 o);

	void addLight(Light& light);
	vector<Light*> getLights();
	const int getDirectionalLightCount();
	const int getPointLightCount();
	const int getSpotLightCount();
};