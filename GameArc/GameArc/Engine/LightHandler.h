#pragma once

#include "Light.h"
#include <vector>

class LightHandler {
private:
	int lightCount;
	int pointLightCount;
	int spotLightCount;
	int lightMax = 8;
	std::vector<Light*> lights;
	void setUpLight(Light* l);
public:
	LightHandler();
	void createNewLight(Light::LightType lT, LightColour lC);
	void createNewLight(Light::LightType lT, LightColour lC, glm::vec3 p);
	void createNewLight(Light::LightType lT, LightColour lC, glm::vec3 p, glm::vec3 o);
	void createNewLight(PointLightData pLD, LightColour lC);
	void createNewLight(PointLightData pLD, LightColour lC, glm::vec3 p);
	void createNewLight(PointLightData pLD, LightColour lC, glm::vec3 p, glm::vec3 o);
	void createNewLight(SpotLightData sLD, LightColour lC);
	void createNewLight(SpotLightData sLD, LightColour lC, glm::vec3 p);
	void createNewLight(SpotLightData sLD, LightColour lC, glm::vec3 p, glm::vec3 o);
	void addLight(Light& light);
	vector<Light*> getLights();
	const int getPointLightCount();
	const int getSpotLightCount();
};