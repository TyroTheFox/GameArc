#version 430 core
out vec4 FragColour;

in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColour;

uniform sampler2D diffusetexture;
in vec2 TexCoords;

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
#define NR_POINT_LIGHTS 8
uniform int noOfPointLights;
uniform PointLight pointLights[NR_POINT_LIGHTS];

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
#define NR_SPOT_LIGHTS 8
uniform int noOfSpotLights;
uniform SpotLight spotLights[NR_SPOT_LIGHTS];

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);  
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{  	
    // diffuse 
    vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
        
	//1: Directional Lights
    vec3 result = CalcDirLight(dirLight, norm, viewDir);

	//2: Point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++){
		if(i < noOfPointLights){
			result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);   
		}
	}
		
    //3: Spot light
	for(int j = 0; j < NR_SPOT_LIGHTS; j++){
		if(j < noOfSpotLights){
			result += CalcSpotLight(spotLights[j], norm, FragPos, viewDir);  
		}
	}
	
    FragColour = vec4(result, 1.0);
} 

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(diffusetexture, TexCoords)) * material.diffuse;
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(diffusetexture, TexCoords)) * material.diffuse;
    vec3 specular = light.specular * spec * vec3(texture(diffusetexture, TexCoords)) * material.specular;
    return (ambient + diffuse + specular);
} 

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(diffusetexture, TexCoords)) * material.diffuse;
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(diffusetexture, TexCoords)) * material.diffuse;
    vec3 specular = light.specular * spec * vec3(texture(diffusetexture, TexCoords)) * material.specular;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(diffusetexture, TexCoords)) * material.diffuse;
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(diffusetexture, TexCoords)) * material.diffuse;
    vec3 specular = light.specular * spec * vec3(texture(diffusetexture, TexCoords)) * material.specular;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}