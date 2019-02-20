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
	vec3 viewVertex;
    vec4 DirectionalFragPosLightSpace;
	vec4 SpotFragPosLightSpace[NR_SPOT_LIGHTS];
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int noOfDirectionalLights;
uniform mat4 DirectionalLightMatrix;

uniform int noOfSpotLights;
uniform mat4 SpotLightMatrix[NR_SPOT_LIGHTS];

void main()
{
	//Texturing
	vs_out.TexCoords = aUV;
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
	vs_out.viewVertex = normalize(-vec3(view * vec4(vs_out.FragPos, 1.0)));

	//Shadows
	if(noOfDirectionalLights > 0){
		vs_out.DirectionalFragPosLightSpace = DirectionalLightMatrix * vec4(vs_out.FragPos, 1.0f);
	}

	uint nLights = min(noOfSpotLights, NR_SPOT_LIGHTS);
    for (uint i = 0u; i < nLights; ++i) {
        vs_out.SpotFragPosLightSpace[i] = SpotLightMatrix[i] * vec4(vs_out.FragPos, 1.0f);
    }

	//Final Position
	gl_Position = projection * view * vec4(vs_out.FragPos, 1.0f);
}