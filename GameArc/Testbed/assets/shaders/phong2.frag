#version 430 core
out vec4 FragColour;

#define NR_POINT_LIGHTS 8
#define NR_SPOT_LIGHTS 8

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
	vec3 viewVertex;
    vec4 DirectionalFragPosLightSpace;
	vec4 SpotFragPosLightSpace[NR_SPOT_LIGHTS];
} fs_in;

uniform vec3 viewPos;

uniform float far_plane;

uniform sampler2D texture_diffuse1;

uniform bool blinn;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};  
uniform Material material;

//Directional Light Data
struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

uniform int noOfDirectionalLights;
uniform DirLight dirLight;
uniform sampler2D shadowMapDIR;

//Point Light Data
struct PointLight { 
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

uniform int noOfPointLights;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform samplerCube shadowMapPOINT[NR_POINT_LIGHTS];

//Spot Light Data
struct SpotLight { 
    vec3 position;
	vec3 direction;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float cutOff;
	float outerCutOff;
};  

uniform int noOfSpotLights;
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform sampler2D shadowMapSPOT[NR_SPOT_LIGHTS];

vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, sampler2D shadowMap, vec4 lightSpaceMatrix);  
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, samplerCube shadowMap);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, sampler2D shadowMap, vec4 lightSpaceMatrix);

float ShadowCalculation(vec4 fragPosLightSpace, sampler2D shadowMap, vec3 lightDir);
float PointShadowCalculation(vec3 fragPos, vec3 lightPos, samplerCube shadowMap);

//Debugging Value
float d = 1.0;

void main()
{  	
    // diffuse 
    vec3 norm = normalize(fs_in.Normal);

    vec3 result = vec3(0.0);
	//1: Directional Lights
	if(noOfDirectionalLights > 0){
		result += CalcDirLight(dirLight, norm, fs_in.viewVertex, shadowMapDIR, fs_in.DirectionalFragPosLightSpace);
	}

	//2: Point lights
    for(int j = 0; j < NR_POINT_LIGHTS; j++){
		if(j < noOfPointLights){
			result += CalcPointLight(pointLights[j], norm, fs_in.FragPos, fs_in.viewVertex, shadowMapPOINT[j]);   
		}
	}
		
    //3: Spot light
	for(int k = 0; k < NR_SPOT_LIGHTS; k++){
		if(k < noOfSpotLights){
			result += CalcSpotLight(spotLights[k], norm, fs_in.FragPos, fs_in.viewVertex, shadowMapSPOT[k], fs_in.SpotFragPosLightSpace[k]);  
		}
	}  
	FragColour = vec4(result, 1.0);
} 

//DIRECTIONAL LIGHT
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, sampler2D shadowMap, vec4 lightSpaceMatrix)
{
	vec3 diffTextColour = vec3(texture(texture_diffuse1, fs_in.TexCoords));
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
	float spec = 0.0;
    if(blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }
    // combine results
    vec3 ambient  = light.ambient  * diffTextColour * material.diffuse;
    vec3 diffuse  = light.diffuse  * diff * diffTextColour * material.diffuse;
    vec3 specular = light.specular * spec * diffTextColour * material.specular;

	float shadow = ShadowCalculation(lightSpaceMatrix, shadowMap, lightDir);       
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * diffTextColour;

    return lighting;
} 

//POINT LIGHT
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, samplerCube shadowMap)
{
	vec3 diffTextColour = vec3(texture(texture_diffuse1, fs_in.TexCoords));
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    float spec = 0.0;
    if(blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * diffTextColour * material.diffuse;
    vec3 diffuse  = light.diffuse  * diff * diffTextColour * material.diffuse;
    vec3 specular = light.specular * spec * diffTextColour * material.specular;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

	float shadow = PointShadowCalculation(fragPos, light.position, shadowMap);     
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * diffTextColour; 

	return lighting;
} 

//SPOT LIGHT
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, sampler2D shadowMap, vec4 lightSpaceMatrix)
{
	vec3 diffTextColour = vec3(texture(texture_diffuse1, fs_in.TexCoords));
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
	float spec = 0.0;
    if(blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient  = light.ambient  * diffTextColour * material.diffuse;
    vec3 diffuse  = light.diffuse  * diff * diffTextColour * material.diffuse;
    vec3 specular = light.specular * spec * diffTextColour * material.specular;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    	
	float shadow = ShadowCalculation(lightSpaceMatrix, shadowMap, lightDir);       
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * diffTextColour;
	
	return lighting;
}

float ShadowCalculation(vec4 fragPosLightSpace, sampler2D shadowMap, vec3 lightDir)
{	
   // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    float bias = max(0.05 * (1.0 - dot(normal, -lightDir)), 0.005);
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

float PointShadowCalculation(vec3 fragPos, vec3 lightPos, samplerCube shadowMap){
	// get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;

    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // test for shadows
    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
		vec3 uv = fragToLight + gridSamplingDisk[i] * diskRadius;
		uv.x = 1.0 - uv.x;
        float closestDepth = texture(shadowMap, uv).r;
        closestDepth *= far_plane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth){
            shadow += 1.0;
		}
    }
    shadow /= float(samples);
                
    return shadow;
}