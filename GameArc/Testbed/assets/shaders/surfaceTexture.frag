#version 430 core
out vec4 FragColour;

in vec3 Normal;  
in vec3 FragPos;  
in vec2 textureCoords;

uniform sampler2D diffusetexture;
  
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColour;

void main()
{
	//Texture
	vec4 texel = texture (diffusetexture, textureCoords);
	// we assume the diffuse and ambient terms are based on the texel
	
	vec3 surface_ambient = texel.rgb;
	vec3 surface_diffuse = texel.rgb;	

    // ambient
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColour;
	ambient = ambient * surface_ambient;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColour;
	diffuse = diffuse * surface_diffuse;
    
    // specular
    float specularStrength = 0.8;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColour;  
        
    vec3 result = (ambient + diffuse + specular);
    FragColour = vec4(result, texel.a);
} 

