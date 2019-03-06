#version 430 core

#define NR_POINT_LIGHTS 8
#define NR_SPOT_LIGHTS 8

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
	vec3 viewVertex;
	vec3 tanViewVertex;
    vec4 DirectionalFragPosLightSpace;
	vec4 SpotFragPosLightSpace[NR_SPOT_LIGHTS];
	mat3 TBN;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int noOfDirectionalLights;
uniform mat4 DirectionalLightMatrix;

uniform int noOfSpotLights;
uniform mat4 SpotLightMatrix[NR_SPOT_LIGHTS];

uniform vec3 viewPos;

void main()
{
	//Texturing
	vs_out.TexCoords = aUV;
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
	vs_out.viewVertex = normalize(-vec3(view * vec4(vs_out.FragPos, 1.0)));

	//TBN Matrix
	//mat3 normalMatrix = transpose(inverse(mat3(model)));
	//vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
	//vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
   // T = normalize(T - dot(T, N) * N);
	//vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
	vec3 T = normalize(mat3(model) * aTangent);
    vec3 B = normalize(mat3(model) * aBitangent);
    vec3 N = normalize(mat3(model) * aNormal);
	vs_out.TBN = transpose(mat3(T, B, N));  

	vs_out.tanViewVertex = normalize(-viewPos * (vs_out.TBN * vs_out.FragPos));

	//Shadows
	if(noOfDirectionalLights > 0){
		vs_out.DirectionalFragPosLightSpace = DirectionalLightMatrix * vec4(vs_out.FragPos, 1.0f);
		  
		//vs_out.TangentLightPos = TBN * DirectionalLightMatrix;
	}

	uint nLights = min(noOfSpotLights, NR_SPOT_LIGHTS);
    for (uint i = 0u; i < nLights; ++i) {
        vs_out.SpotFragPosLightSpace[i] = SpotLightMatrix[i] * vec4(vs_out.FragPos, 1.0f);
		//vs_out.TangentLightPos = TBN * SpotLightMatrix[i];
    }

	//Final Position
	gl_Position = projection * view * vec4(vs_out.FragPos, 1.0f);
}