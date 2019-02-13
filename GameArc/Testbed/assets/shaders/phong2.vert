#version 430 core

#define NR_POINT_LIGHTS 8
#define NR_SPOT_LIGHTS 8

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 DirectionalFragPosLightSpace;
	vec4 PointFragPosLightSpace[NR_POINT_LIGHTS];
	vec4 SpotFragPosLightSpace[NR_SPOT_LIGHTS];
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 DirectionalLightMatrix;

uniform int noOfPointLights;
uniform mat4 PointLightMatrix[NR_POINT_LIGHTS];

uniform int noOfSpotLights;
uniform mat4 SpotLightMatrix[NR_SPOT_LIGHTS];

void main()
{
	//Texturing
	vs_out.TexCoords = aUV;
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;

	//Shadows
    vs_out.DirectionalFragPosLightSpace = DirectionalLightMatrix * vec4(aPos, 1.0f);

	uint nLights = min(noOfPointLights, NR_POINT_LIGHTS);
    for (uint i = 0u; i < nLights; ++i) {
        vs_out.PointFragPosLightSpace[i] = PointLightMatrix[i] * vec4(aPos, 1.0f);
    }

	nLights = min(noOfSpotLights, NR_SPOT_LIGHTS);
    for (uint i = 0u; i < nLights; ++i) {
        vs_out.SpotFragPosLightSpace[i] = SpotLightMatrix[i] * vec4(aPos, 1.0f);
    }

	//Final Position
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}