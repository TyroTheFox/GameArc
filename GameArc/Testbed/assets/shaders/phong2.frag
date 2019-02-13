#version 430 core
out vec4 FragColour;

#define NR_POINT_LIGHTS 8
#define NR_SPOT_LIGHTS 8

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 DirectionalFragPosLightSpace;
	vec4 PointFragPosLightSpace[NR_POINT_LIGHTS];
	vec4 SpotFragPosLightSpace[NR_SPOT_LIGHTS];
} fs_in;
  
uniform vec3 lightPos;
uniform vec3 viewPos; 

uniform sampler2D texture_diffuse0;

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
uniform sampler2D shadowMapPOINT[NR_POINT_LIGHTS];

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

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, sampler2D shadowMap, vec4 lightSpaceMatrix);  
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, sampler2D shadowMap, vec4 lightSpaceMatrix);
float ShadowCalculation(vec4 fragPosLightSpace, sampler2D shadowMap, vec3 lightDir);

//Debugging Value
float d = 1.0;

void main()
{  	
    // diffuse 
    vec3 norm = normalize(fs_in.Normal);
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);

    vec3 result = vec3(0.0);
	//1: Directional Lights
	result += CalcDirLight(dirLight, norm, viewDir, shadowMapDIR, fs_in.DirectionalFragPosLightSpace);

	//2: Point lights
    for(int j = 0; j < NR_POINT_LIGHTS; j++){
		if(j < noOfPointLights){
			result += CalcPointLight(pointLights[j], norm, fs_in.FragPos, viewDir);   
		}
	}
		
    //3: Spot light
	for(int k = 0; k < NR_SPOT_LIGHTS; k++){
		if(k < noOfSpotLights){
			result += CalcSpotLight(spotLights[k], norm, fs_in.FragPos, viewDir, shadowMapSPOT[k], fs_in.SpotFragPosLightSpace[k]);  
		}
	}  
	FragColour = vec4(result, 1.0);
} 

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, sampler2D shadowMap, vec4 lightSpaceMatrix)
{
	vec3 diffTextColour = vec3(texture(texture_diffuse0, fs_in.TexCoords));
    vec3 lightDir = normalize(light.direction);
    // diffuse shading
    float diff = max(dot(lightDir, normal), 0.0);
    // specular shading
	float spec = 0.0;
    if(blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    }
    else
    {
        vec3 reflectDir = reflect(lightDir, normal);
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

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = -normalize(light.position - fragPos);
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
        vec3 reflectDir = reflect(lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(texture_diffuse0, fs_in.TexCoords)) * material.diffuse;
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse0, fs_in.TexCoords)) * material.diffuse;
    vec3 specular = light.specular * spec * vec3(texture(texture_diffuse0, fs_in.TexCoords)) * material.specular;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, sampler2D shadowMap, vec4 lightSpaceMatrix)
{
	vec3 diffTextColour = vec3(texture(texture_diffuse0, fs_in.TexCoords));
    vec3 lightDir = -normalize(light.position - fragPos);
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
        vec3 reflectDir = reflect(lightDir, normal);
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
    //vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, -lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
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